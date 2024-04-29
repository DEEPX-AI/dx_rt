// #include "gtest/gtest.h"
// #include "dxrt/inference_engine.h"
// #include "dxrt/task.h"
// // #include "dxrt/inference_option.h"
// #include "dxrt/request.h"
// #include "dxrt_test.h"

// using namespace std;
// using namespace dxrt;

// #define TEST_RMAP_SIZE 2*1024*1024
// #define TEST_WEIGHT_SIZE 10*1024*1024
/* #define COMMON_CODE() \
//     vector<rmapinfo> infos; \ 
//     rmapinfo info; \ 
//     info.mutable_version()->set_npu("dummy npu"); \ 
//     info.set_model("dummy model"); \ 
//     info.set_size(14029); \ 
//     info.mutable_counts()->set_cmd(0x128); \ 
//     auto input = info.mutable_input(); \
//     input->set_type(deepx_rmapinfo::DataType::UINT8); \
//     auto shapes = input->mutable_shapes(); \
//     shapes->add_shape(1); \
//     shapes->add_shape(640); \
//     shapes->add_shape(640); \
//     shapes->add_shape(3); \
//     auto outputlists = info.mutable_outputs()->mutable_outputlist(); \
//     auto output0 = outputlists->add_output(); \
//     auto output1 = outputlists->add_output(); \
//     auto output2 = outputlists->add_output(); \
//     { \
//         output0->set_name("789");\
//         auto shapes = output0->mutable_shapes();\
//         shapes->add_shape(1); shapes->add_shape(80); shapes->add_shape(80); shapes->add_shape(255); \
//         output0->set_type(deepx_rmapinfo::DataType::FLOAT32); \
//         auto memory = output0->mutable_memory(); memory->set_offset(0); \
//     } \
//     { \
//         output1->set_name("123");\
//         auto shapes = output1->mutable_shapes();\
//         shapes->add_shape(3); shapes->add_shape(4); shapes->add_shape(5); shapes->add_shape(8); \
//         output1->set_type(deepx_rmapinfo::DataType::FLOAT32); \
//         auto memory = output1->mutable_memory(); memory->set_offset(0x1000); \
//     } \
//     { \
//         output2->set_name("456");\
//         auto shapes = output2->mutable_shapes();\
//         shapes->add_shape(8); shapes->add_shape(10); shapes->add_shape(61);\
//         output2->set_type(deepx_rmapinfo::DataType::FLOAT32); \
//         auto memory = output2->mutable_memory(); memory->set_offset(0x2000); \
//     } \
//     deepx_rmapinfo::Memorys *memorys = info.mutable_memorys(); \ 
//     deepx_rmapinfo::Memory *rmap_memory = memorys->add_memory(); \ 
//     deepx_rmapinfo::Memory *weight_memory = memorys->add_memory(); \ 
//     deepx_rmapinfo::Memory *temp_memory = memorys->add_memory(); \ 
//     rmap_memory->set_name("RMAP"); \ 
//     rmap_memory->set_size(TEST_RMAP_SIZE); \ 
//     weight_memory->set_name("WEIGHT"); \ 
//     weight_memory->set_size(TEST_WEIGHT_SIZE); \ 
//     temp_memory->set_name("TEMP"); \ 
//     temp_memory->set_offset(0x78901234); \ 
//     infos.emplace_back(info); \ 
//     vector<vector<uint8_t>> data; \ 
//     data.emplace_back( move(vector<uint8_t>(TEST_RMAP_SIZE, 0)) ); \ 
//     data.emplace_back( move(vector<uint8_t>(TEST_WEIGHT_SIZE, 0)) ); \ 
//     for(int i=0; i<data[0].size(); i++) \ 
//     { \ 
//         data[0][i] = i+1; \ 
//     } \ 
//     for(int i=0; i<data[1].size(); i++) \ 
//     { \ 
//         data[1][i] = i+1; \ 
//     } \ 
//     auto npuTask = Task( infos, move(data) ); \
//     cout << npuTask << endl;

// TEST(task, basic)
// {
//     COMMON_CODE();
// }
// // TEST(task, setup)
// // {
// //     COMMON_CODE();
// //     npuTask.Setup();
// //     auto check = npuTask.Check();
// //     EXPECT_EQ(check, 0);
// // }
// TEST(task, tensors)
// {
//     COMMON_CODE();
//     auto srcInputTensors = npuTask.inputs((void*)0x1234);
//     auto srcOutputTensors = npuTask.outputs((void*)0x5678);
//     Tensors destInputTensors;
//     Tensors destOutputTensors;
//     for(auto &tensor:srcInputTensors)
//     {
//         destInputTensors.emplace_back( move(Tensor(tensor, (void*)0xaabb)) );
//         EXPECT_EQ(destInputTensors.back().data(), (void*)0xaabb);
//     }
//     for(auto &tensor:srcOutputTensors)
//     {
//         destOutputTensors.emplace_back( move(Tensor(tensor, (void*)0xcafe)) );
//         EXPECT_EQ(destOutputTensors.back().data(), (void*)0xcafe);
//     }
//     destInputTensors = npuTask.inputs((void*)0xabcd);
//     destOutputTensors = npuTask.outputs((void*)0xfedc);
//     for(auto &tensor:srcInputTensors)
//     {
//         cout << tensor << endl;
//     }
//     for(auto &tensor:srcOutputTensors)
//     {
//         cout << tensor << endl;
//     }
//     for(auto &tensor:destInputTensors)
//     {
//         EXPECT_EQ(tensor.data(), (void*)0xabcd);
//         cout << tensor << endl;
//     }
//     for(auto &tensor:destOutputTensors)
//     {
//         EXPECT_GE(tensor.data(), (void*)0xfedc);
//         cout << tensor << endl;
//     }
// }
// TEST(task, request)
// {
//     COMMON_CODE();
//     auto inputs = npuTask.inputs();
//     auto outputs = npuTask.outputs();
//     auto req = Request(&npuTask, inputs, outputs);
//     cout << req << endl;
// } */