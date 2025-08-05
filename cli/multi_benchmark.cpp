#include <string>
#include <memory>
#include <thread>

#include "dxrt/dxrt_api.h"
//dxapp_api.hpp"

// Inference runner
// accel_id id of the accelerator to be used
void runner(int accel_id)
{
    const std::string modelPath1 = "yolov8n_relu6_age--256x256_quant_deepx_m1a_1.dxnn";
    const std::string modelPath2 = "yolov8n_relu6_coco--640x640_quant_deepx_m1a_1.dxnn";

    int iterations = 8; // number of model reloads
    int inferences = 16;  // number of inferences for each loaded model 

    int model_id = 0;
    for (int i = 0; i < iterations; ++i)
    {
        // Set the model for this iteration
        model_id = model_id % 2;
        std::string modelPath = model_id % 2 ? modelPath1 : modelPath2;
        model_id++;

        // Specify on which acclerator to load the model
        dxrt::InferenceOption op;
        //op.devices = {accel_id};
        op.devices = {0};

        std::cout << "------- Accelerator " << accel_id << " Loading model " << model_id << std::endl;
        std::shared_ptr<dxrt::InferenceEngine> inferenceEngine = std::make_shared<dxrt::InferenceEngine>(modelPath, op);
        std::cout << "------- Accelerator " << accel_id << " Model " << model_id << " loaded" << std::endl;

        uint64_t inputSize = inferenceEngine->GetInputSize();

        // CS
        std::mutex cs_mutex;
        std::condition_variable cs_cv;
        int cs_count = 0;
        // CS

        // Callback function
        std::function<int(std::vector<std::shared_ptr<dxrt::Tensor> >, void *)> dx_callback =
            [&i, &cs_count, &cs_mutex, &cs_cv, inferences](std::vector<std::shared_ptr<dxrt::Tensor> > dx_outputs, void *args_)
        {
            (void)dx_outputs;
            int *args = reinterpret_cast<int*>(args_);
            std::cout << "Callback " << args[0] << " Iteration: " << args[1] << " Inference: " << args[2] << std::endl;
            delete args;

            // CS
            std::unique_lock<std::mutex> lock(cs_mutex);
            cs_count++;
            if ( cs_count >= inferences ) cs_cv.notify_one();
            // CS

            return 0;
        };

        inferenceEngine->RegisterCallback(dx_callback);

        // input tensor
        uint8_t *input_tensor = new uint8_t[inputSize];
        memset(input_tensor, 0, inputSize * sizeof(uint8_t));

        // run inferences
        int last_job_id = 0;
        for (int j = 0; j < inferences; ++j)
        {
            std::cout << "Accelerator " << accel_id << " Iteration: " << i <<" Inference: " << j << std::endl;
            int * args_ptr = new int[3];
            args_ptr[0] = accel_id;
            args_ptr[1] = i;  // iteration id
            args_ptr[2] = j;  // inference id
            last_job_id = inferenceEngine->RunAsync(input_tensor, reinterpret_cast<void *>(args_ptr));
        }

        // Wait for all all jobs to be finished
        std::cout << "Start Wait " << accel_id << " Iteration " << i << std::endl;

        // CS
        // inferenceEngine->Wait(last_job_id);
        std::unique_lock<std::mutex> lock(cs_mutex);
        cs_cv.wait(lock, [&cs_count, inferences]{ return cs_count >= inferences;});
        // CS

        std::cout << "End Wait in " << accel_id << " Iteration " << i <<std::endl;
        std::ignore = last_job_id;
        delete[] input_tensor;
    }
}

int main()
{
    std::thread worker1(runner, 0);
    std::thread worker2(runner, 1);

    worker1.join();
    worker2.join();

    return 0;
}
