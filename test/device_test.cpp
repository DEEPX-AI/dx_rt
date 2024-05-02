#include "gtest/gtest.h"
#include "dxrt/device.h"
#include "dxrt_test.h"

using namespace std;
using namespace dxrt;

TEST(device, basic)
{
    auto devices = dxrt::CheckDevices();
    int numDevices = devices.size();
    LOG_VALUE(numDevices);
    EXPECT_GT(numDevices, 0);
    for(auto &device:devices)
    {
        cout << *device << endl;
    }
}
TEST(device, memory)
{
    auto devices = dxrt::CheckDevices();
    int numDevices = devices.size();
    LOG_VALUE(numDevices);
    EXPECT_GT(numDevices, 0);
    for(auto &device:devices)
    {
        EXPECT_NE(device->Allocate(0x100), -1);
        EXPECT_NE(device->Allocate(0x200), -1);
        EXPECT_NE(device->Allocate(0x300), -1);
        EXPECT_NE(device->Allocate(0xa00), -1);
        cout << *device << endl;
        device->Deallocate(0x300);
        cout << *device << endl;
        device->Allocate(0x128);
        cout << *device << endl;
    }
}
TEST(device, process)
{
    auto devices = dxrt::CheckDevices();
    int numDevices = devices.size();
    LOG_VALUE(numDevices);
    EXPECT_GT(numDevices, 0);

    for(auto &device:devices)
    {
        cout << *device << endl;
        for(int i=dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE; i<dxrt::dxrt_cmd_t::DXRT_CMD_MAX-2; i++)
        {
            int ret = -1, expect_ret = 0;
            dxrt_cmd_t cmd = (dxrt_cmd_t)i;
            void *data = NULL;
            dxrt_device_info_t devInfo;
            dxrt_meminfo_t meminfo;
            switch(cmd)
            {
                case dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE:
                    data = static_cast<void*>(&devInfo);
                    break;
                case dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_MEM:
                case dxrt::dxrt_cmd_t::DXRT_CMD_READ_MEM:
                    data = static_cast<void*>(&meminfo);
                    fillStructIncreasingValues(meminfo);
                    expect_ret = -1;
                    break;
                case dxrt::dxrt_cmd_t::DXRT_CMD_SOC_CUSTOM:
                default:
                    break;
            }
            ret = device->Process(static_cast<dxrt::dxrt_cmd_t>(i), data);
            cout << cmd << ": " << ret << ", " << expect_ret << endl;
            EXPECT_EQ(ret, expect_ret);
        }
    }
}
TEST(device, write_read)
{
    auto devices = dxrt::CheckDevices();
    int numDevices = devices.size();
    LOG_VALUE(numDevices);
    EXPECT_GT(numDevices, 0);

    unsigned int len = 4096;
    vector<uint8_t> writeData(len, 0);
    vector<uint8_t> readData(len, 0);
    for(unsigned int i=0;i<len;i++)
    {
        writeData[i] = static_cast<uint8_t>(i);
    }
    dxrt_meminfo_t writeInfo;
    writeInfo.data = reinterpret_cast<uint64_t>(writeData.data());
    writeInfo.base = 0;
    writeInfo.offset = 0x1000;
    writeInfo.size = len;
    dxrt_meminfo_t readInfo;
    readInfo.data = reinterpret_cast<uint64_t>(readData.data());
    readInfo.base = 0;
    readInfo.offset = 0x1000;
    readInfo.size = len;

    for(auto &device:devices)
    {
        cout << *device << endl;
        writeInfo.base = device->info().mem_addr;
        readInfo.base = device->info().mem_addr;
        DXRT_ASSERT(device->Write(writeInfo)==0, "fail to write");
        DXRT_ASSERT(device->Read(readInfo)==0, "fail to read");
        // for(int i=0;i<len;i++) cout << (uint32_t)readData[i] << ", " << endl;
        EXPECT_EQ(0, memcmp(writeData.data(), readData.data(), len));
    }
}