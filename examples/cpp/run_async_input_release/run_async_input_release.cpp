/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * User Input Early Release Example
 * 
 * This example demonstrates how to use the User Input Early Release callback
 * to immediately release input buffers after NFH encoding completes, allowing
 * efficient buffer reuse for high-throughput inference pipelines.
 * 
 * Architecture:
 * - Producer Thread: Continuously submits RunAsync() jobs when buffers available
 * - Consumer Thread: Calls Wait() to retrieve results and release output buffers
 * - Input Release Callback: Releases input buffers after NFH encoding
 */

#include <iostream>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <thread>
#include <queue>

#include "dxrt/dxrt_api.h"

// Resource handle to track input/output buffers per job
struct ResourceHandle {
    int input_buffer_id;     // ID of the input buffer being used
    int output_buffer_id;    // ID of the output buffer being used
    std::chrono::steady_clock::time_point submit_time;  // When the job was submitted
    
    ResourceHandle(int in_id, int out_id) 
        : input_buffer_id(in_id), output_buffer_id(out_id), 
          submit_time(std::chrono::steady_clock::now()) {}
};

class AsyncInputReleaseExample {
public:
    AsyncInputReleaseExample(const std::string& modelPath, int numInputBuffers = 6, int numOutputBuffers = 8, bool useORT = false)
        : model_path_(modelPath),
          num_input_buffers_(numInputBuffers),
          num_output_buffers_(numOutputBuffers),
          stop_threads_(false),
          total_jobs_submitted_(0),
          total_jobs_completed_(0),
          total_inputs_released_(0)
    {
        // Initialize inference engine
        dxrt::InferenceOption options;
        options.boundOption = 0;  // Use all NPUs
        options.useORT = useORT;
        
        inference_engine_ = std::make_unique<dxrt::InferenceEngine>(modelPath, options);
        
        size_t input_size = inference_engine_->GetInputSize();
        size_t output_size = inference_engine_->GetOutputSize();
        std::cout << "[INIT] Model loaded successfully" << std::endl;
        std::cout << "[INIT] Input size:  " << input_size << " bytes" << std::endl;
        std::cout << "[INIT] Output size: " << output_size << " bytes" << std::endl;
        
        // Create pools of input and output buffers
        input_buffers_.reserve(num_input_buffers_);
        output_buffers_.reserve(num_output_buffers_);
        input_buffer_available_.resize(num_input_buffers_, true);
        output_buffer_available_.resize(num_output_buffers_, true);
        
        for (int i = 0; i < num_input_buffers_; i++) {
            input_buffers_.emplace_back(input_size, 0);
            std::cout << "[INIT] Allocated input buffer " << i << " (" << input_size << " bytes)" << std::endl;
        }
        
        for (int i = 0; i < num_output_buffers_; i++) {
            output_buffers_.emplace_back(output_size, 0);
            std::cout << "[INIT] Allocated output buffer " << i << " (" << output_size << " bytes)" << std::endl;
        }
        
        // Register the User Input Early Release callback
        inference_engine_->RegisterUserInputReleaseCallback(
            [this](void* userArg, int job_id) {
                this->onInputRelease(userArg, job_id);
            }
        );
        
        std::cout << "[INIT] User Input Early Release callback registered" << std::endl;
        std::cout << "[INIT] Ready to run inference with " << num_input_buffers_ << " input buffers and " 
                  << num_output_buffers_ << " output buffers" << std::endl;
        std::cout << std::string(80, '=') << std::endl;
    }
    
    ~AsyncInputReleaseExample() {
        stop_threads_ = true;
        
        // Wake up any waiting threads
        job_queue_cv_.notify_all();
        
        // Wait for threads to finish
        if (producer_thread_.joinable()) {
            producer_thread_.join();
        }
        if (consumer_thread_.joinable()) {
            consumer_thread_.join();
        }
        
        printStatistics();
    }
    
    // Run benchmark with producer/consumer threads
    void runBenchmark(int loops = 100, int duration_seconds = 0) {
        std::cout << "[BENCH] Starting benchmark..." << std::endl;
        
        if (duration_seconds > 0) {
            std::cout << "[BENCH] Running for " << duration_seconds << " seconds" << std::endl;
        } else {
            std::cout << "[BENCH] Running " << loops << " loops" << std::endl;
        }
        
        auto start_time = std::chrono::steady_clock::now();
        
        // Start consumer thread (Wait thread)
        consumer_thread_ = std::thread([this, start_time]() {
            this->consumerThread(start_time);
        });
        
        // Start producer thread (RunAsync thread)
        producer_thread_ = std::thread([this, loops, duration_seconds, start_time]() {
            this->producerThread(loops, duration_seconds, start_time);
        });
        
        // Wait for producer to finish
        producer_thread_.join();
        
        // Signal consumer to stop and wake it up
        stop_threads_ = true;
        job_queue_cv_.notify_all();
        
        // Wait for consumer to finish
        consumer_thread_.join();
        
        auto end_time = std::chrono::steady_clock::now();
        auto total_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time).count();
        
        std::cout << "[BENCH] Benchmark complete!" << std::endl;
        std::cout << "[BENCH] Total time: " << total_time_ms << " ms" << std::endl;
        std::cout << "[BENCH] Total jobs: " << total_jobs_submitted_.load() << std::endl;
        std::cout << "[BENCH] Throughput: " << std::fixed << std::setprecision(2)
                  << (total_jobs_submitted_.load() * 1000.0 / total_time_ms) << " jobs/sec" << std::endl;
    }

private:
    // Producer thread: continuously submits RunAsync jobs
    void producerThread(int loops, int duration_seconds, 
                       std::chrono::steady_clock::time_point start_time) {
        std::cout << "[PRODUCER] Thread started" << std::endl;
        
        while (!stop_threads_) {
            // Check termination conditions
            if (duration_seconds > 0) {
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::steady_clock::now() - start_time).count();
                if (elapsed >= duration_seconds) {
                    break;
                }
            } else {
                if (total_jobs_submitted_ >= loops) {
                    break;
                }
            }
            
            // Get available input and output buffers
            int input_id = getAvailableInputBuffer();
            if (input_id < 0) {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
                continue;
            }
            
            int output_id = getAvailableOutputBuffer();
            if (output_id < 0) {
                // Return input buffer and retry
                releaseInputBuffer(input_id);
                std::this_thread::sleep_for(std::chrono::microseconds(100));
                continue;
            }
            
            // Submit inference
            int job_id = submitInference(input_id, output_id);
            if (job_id >= 0) {
                // Add to job queue for consumer
                std::lock_guard<std::mutex> lock(job_queue_mutex_);
                job_queue_.push(job_id);
                job_queue_cv_.notify_one();
            }
        }
        
        std::cout << "[PRODUCER] Thread finished (submitted " << total_jobs_submitted_.load() << " jobs)" << std::endl;
    }
    
    // Consumer thread: calls Wait() to get results
    void consumerThread(std::chrono::steady_clock::time_point start_time) {
        (void)start_time;  // Unused parameter
        std::cout << "[CONSUMER] Thread started" << std::endl;
        
        while (!stop_threads_ || !job_queue_.empty()) {
            int job_id = -1;
            
            // Get next job from queue
            {
                std::unique_lock<std::mutex> lock(job_queue_mutex_);
                job_queue_cv_.wait(lock, [this] {
                    return !job_queue_.empty() || stop_threads_;
                });
                
                if (job_queue_.empty() && stop_threads_) {
                    break;
                }
                
                if (!job_queue_.empty()) {
                    job_id = job_queue_.front();
                    job_queue_.pop();
                }
            }
            
            if (job_id < 0) {
                continue;
            }
            
            // Wait for this job to complete
            dxrt::TensorPtrs outputs = inference_engine_->Wait(job_id);
            
            // Get resource handle to release output buffer
            int output_id = -1;
            {
                std::lock_guard<std::mutex> lock(job_resource_mutex_);
                auto it = job_resources_.find(job_id);
                if (it != job_resources_.end()) {
                    output_id = it->second;
                    job_resources_.erase(it);
                }
            }
            
            // Release output buffer
            if (output_id >= 0) {
                releaseOutputBuffer(output_id);
            }
            
            total_jobs_completed_++;
            
            if (total_jobs_completed_ % 10000 == 0) {
                std::cout << "[CONSUMER] Completed " << total_jobs_completed_ << " jobs" << std::endl;
            }
        }
        
        std::cout << "[CONSUMER] Thread finished (completed " << total_jobs_completed_.load() << " jobs)" << std::endl;
    }
    
    // Get an available input buffer from the pool
    int getAvailableInputBuffer() {
        std::lock_guard<std::mutex> lock(input_buffer_mutex_);
        for (int i = 0; i < num_input_buffers_; i++) {
            if (input_buffer_available_[i]) {
                input_buffer_available_[i] = false;
                return i;
            }
        }
        return -1;
    }
    
    // Get an available output buffer from the pool
    int getAvailableOutputBuffer() {
        std::lock_guard<std::mutex> lock(output_buffer_mutex_);
        for (int i = 0; i < num_output_buffers_; i++) {
            if (output_buffer_available_[i]) {
                output_buffer_available_[i] = false;
                return i;
            }
        }
        return -1;
    }
    
    // Release input buffer back to pool
    void releaseInputBuffer(int buffer_id) {
        std::lock_guard<std::mutex> lock(input_buffer_mutex_);
        input_buffer_available_[buffer_id] = true;
    }
    
    // Release output buffer back to pool
    void releaseOutputBuffer(int buffer_id) {
        std::lock_guard<std::mutex> lock(output_buffer_mutex_);
        output_buffer_available_[buffer_id] = true;
    }
    
    // Submit inference job with given input/output buffers
    int submitInference(int input_id, int output_id) {
        // Create resource handle for this job
        auto* resource = new ResourceHandle(input_id, output_id);
        
        // Submit async inference with output buffer
        int job_id = inference_engine_->RunAsync(
            input_buffers_[input_id].data(), 
            resource,
            output_buffers_[output_id].data());
        
        if (job_id >= 0) {
            // Track output buffer for this job
            {
                std::lock_guard<std::mutex> lock(job_resource_mutex_);
                job_resources_[job_id] = output_id;
            }
            
            total_jobs_submitted_++;
            
            if (total_jobs_submitted_ % 10000 == 0) {
                std::cout << "[PRODUCER] Submitted " << total_jobs_submitted_ 
                          << " jobs (input=" << input_id << ", output=" << output_id << ")" << std::endl;
            }
            
            return job_id;
        } else {
            std::cerr << "[ERROR] Failed to submit inference (input=" << input_id 
                      << ", output=" << output_id << ")" << std::endl;
            // Return buffers to pool
            releaseInputBuffer(input_id);
            releaseOutputBuffer(output_id);
            delete resource;
            return -1;
        }
    }
    
    // Callback when input is released (after NFH encoding)
    void onInputRelease(void* userArg, int job_id) {
        if (userArg == nullptr) {
            std::cerr << "[ERROR] Input release callback: null userArg for jobId=" << job_id << std::endl;
            return;
        }
        
        auto* resource = static_cast<ResourceHandle*>(userArg);
        int input_id = resource->input_buffer_id;
        
        // Calculate time from submission to input release
        auto release_time = std::chrono::steady_clock::now();
        auto time_to_release_us = std::chrono::duration_cast<std::chrono::microseconds>(
            release_time - resource->submit_time).count();
        
        // Atomically increment and get previous value (thread-safe check)
        int release_count = total_inputs_released_.fetch_add(1);
        if (release_count < 10) {
            std::cout << "[INPUT_RELEASE] Job " << job_id 
                      << " - Input buffer " << input_id 
                      << " released after " << time_to_release_us << " us (NFH encoding complete)" << std::endl;
        }
        
        // Return input buffer to pool (can be reused immediately!)
        releaseInputBuffer(input_id);
        
        // Cleanup resource handle
        delete resource;
    }

    // Print final statistics
    void printStatistics() {    
        std::cout << std::string(80, '=') << std::endl;
        std::cout << "[STATISTICS] Final Results:" << std::endl;
        std::cout << "  - Total jobs submitted:  " << total_jobs_submitted_.load() << std::endl;
        std::cout << "  - Total jobs completed:  " << total_jobs_completed_.load() << std::endl;
        std::cout << "  - Total inputs released: " << total_inputs_released_.load() << std::endl;
        std::cout << "  - Input buffers used:    " << num_input_buffers_ << std::endl;
        std::cout << "  - Output buffers used:   " << num_output_buffers_ << std::endl;
        
        // Check for consistency
        if (total_inputs_released_ == total_jobs_submitted_) {
            std::cout << "  [✓] All inputs were released correctly" << std::endl;
        } else {
            std::cout << "  [✗] Input release count mismatch!" << std::endl;
        }
        
        if (total_jobs_completed_ == total_jobs_submitted_) {
            std::cout << "  [✓] All jobs completed successfully" << std::endl;
        } else {
            std::cout << "  [✗] Job completion count mismatch!" << std::endl;
        }
        
        std::cout << std::string(80, '=') << std::endl;
    }

private:
    std::string model_path_;
    std::unique_ptr<dxrt::InferenceEngine> inference_engine_;
    
    // Buffer pools
    int num_input_buffers_;
    int num_output_buffers_;
    std::vector<std::vector<uint8_t>> input_buffers_;
    std::vector<std::vector<uint8_t>> output_buffers_;
    std::vector<bool> input_buffer_available_;
    std::vector<bool> output_buffer_available_;
    std::mutex input_buffer_mutex_;
    std::mutex output_buffer_mutex_;
    
    // Job queue for producer-consumer pattern
    std::queue<int> job_queue_;
    std::mutex job_queue_mutex_;
    std::condition_variable job_queue_cv_;
    
    // Job to output buffer mapping
    std::map<int, int> job_resources_;  // jobId -> output_buffer_id
    std::mutex job_resource_mutex_;
    
    // Threads
    std::thread producer_thread_;
    std::thread consumer_thread_;
    std::atomic<bool> stop_threads_;
    
    // Statistics
    std::atomic<int> total_jobs_submitted_;
    std::atomic<int> total_jobs_completed_;
    std::atomic<int> total_inputs_released_;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <model.dxnn> [OPTIONS]" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Required:" << std::endl;
        std::cerr << "  model.dxnn           : Path to the DXNN model file" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Options:" << std::endl;
        std::cerr << "  --loops <N>          : Number of inference loops to run (default: 100)" << std::endl;
        std::cerr << "  --duration <SEC>     : Run for N seconds instead of fixed loops (default: 0)" << std::endl;
        std::cerr << "  --input-buffers <N>  : Number of input buffers (default: 6)" << std::endl;
        std::cerr << "  --output-buffers <N> : Number of output buffers (default: 8)" << std::endl;
        std::cerr << "  --use-ort            : Use ONNX Runtime for CPU tasks" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Examples:" << std::endl;
        std::cerr << "  " << argv[0] << " model.dxnn --loops 1000" << std::endl;
        std::cerr << "  " << argv[0] << " model.dxnn --duration 30 --input-buffers 10 --output-buffers 16" << std::endl;
        std::cerr << "  " << argv[0] << " model.dxnn --loops 1000 --use-ort" << std::endl;
        return 1;
    }
    
    std::string model_path = argv[1];
    int loops = 100;
    int duration_sec = 0;
    int num_input_buffers = 6;
    int num_output_buffers = 8;
    bool use_ort = false;
    
    // Parse command line arguments
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--loops" && i + 1 < argc) {
            loops = std::atoi(argv[++i]);
        } else if (arg == "--duration" && i + 1 < argc) {
            duration_sec = std::atoi(argv[++i]);
        } else if (arg == "--input-buffers" && i + 1 < argc) {
            num_input_buffers = std::atoi(argv[++i]);
        } else if (arg == "--output-buffers" && i + 1 < argc) {
            num_output_buffers = std::atoi(argv[++i]);
        } else if (arg == "--use-ort") {
            use_ort = true;
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            return 1;
        }
    }
    
    std::cout << "========================================" << std::endl;
    std::cout << "  User Input Early Release Example" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Model: " << model_path << std::endl;
    std::cout << "Buffer pool: " << num_input_buffers << " input, " << num_output_buffers << " output" << std::endl;
    std::cout << "Use ORT: " << (use_ort ? "Yes" : "No") << std::endl;
    std::cout << std::endl;
    
    try {
        AsyncInputReleaseExample example(model_path, num_input_buffers, num_output_buffers, use_ort);
        example.runBenchmark(loops, duration_sec);
        
        std::cout << std::endl;
        std::cout << "[SUCCESS] Example completed successfully!" << std::endl;
        return 0;
        
    } catch (const dxrt::Exception& e) {
        std::cerr << "[ERROR] DXRT Exception: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Exception: " << e.what() << std::endl;
        return 1;
    }
}