// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "gtest/gtest.h"
#include "dxrt/driver.h"
#include "dxrt/device.h"
#include "dxrt_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>

using namespace std;
using namespace dxrt;
#define DEVICE_FILENAME "/dev/dxrt"
// #define NUM_DEVICES 4
#define NUM_DEVICES 1

int openDevice(const char *file)
{
    int fd;
    fd = open(file, O_RDWR);
    if(fd<0)
    {
        cout << "Error: Can't open " << file << endl;
    }
    return fd;
}

TEST(driver, open)
{
    int fd[NUM_DEVICES];
    for(int dev=0; dev<NUM_DEVICES; dev++)
    {
        string devName = DEVICE_FILENAME + to_string(dev);
        fd[dev] = openDevice(devName.c_str());
        ASSERT_GT(fd[dev], 0);
        close(fd[dev]);        
    }    
}
TEST(driver, read)
{
    int fd[NUM_DEVICES];
    for(int dev=0; dev<NUM_DEVICES; dev++)
    {
        string devName = DEVICE_FILENAME + to_string(dev);
        fd[dev] = openDevice(devName.c_str());
        ASSERT_GT(fd[dev], 0);
    }
    for(int dev=0; dev<NUM_DEVICES; dev++)
    {
        int response = 0xffaa;
        ssize_t bytes_read = read(fd[dev], &response, sizeof(int));
        // EXPECT_EQ(bytes_read, sizeof(response));
        EXPECT_GT(bytes_read, -1);
        LOG_VALUE(bytes_read);
        if(bytes_read==sizeof(int))
        {
            cout << "Response with id: " << response << endl;
        }
    }
    for(int dev=0; dev<NUM_DEVICES; dev++)
    {
        close(fd[dev]);
    }    
}
TEST(driver, write)
{
    int fd[NUM_DEVICES];
    for(int dev=0; dev<NUM_DEVICES; dev++)
    {
        string devName = DEVICE_FILENAME + to_string(dev);
        fd[dev] = openDevice(devName.c_str());
        EXPECT_GT(fd[dev], 0);
    }
    for(int dev=0; dev<NUM_DEVICES; dev++)
    {
        dxrt_request_t inference;
        fillStructIncreasingValues(inference);
        EXPECT_EQ(*(reinterpret_cast<uint64_t*>(&inference)), 0x706050403020100 );
        cout << inference << endl;
        EXPECT_EQ(write(fd[dev], &inference, sizeof(inference)), sizeof(inference));
    }
    for(int dev=0; dev<NUM_DEVICES; dev++)
    {
        close(fd[dev]);
    }
}
TEST(driver, mmap)
{
    auto devices = dxrt::CheckDevices();
    int numDevices = devices.size();
    LOG_VALUE(numDevices);
    EXPECT_GT(numDevices, 0);
    int size = 16*1024*1024;
    vector<int> fd;
    cout << "mmap size: " << size << endl;
    for(int dev=0; dev<numDevices; dev++)
    {
        string devName = DEVICE_FILENAME + to_string(dev);
        fd.emplace_back(openDevice(devName.c_str()));
        EXPECT_GT(fd.back(), 0);
    }
    for(int dev=0; dev<numDevices; dev++)
    {
        void *buf;
        buf = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd[dev], 0);        
        if(devices[dev]->info().type==1)
        {
            EXPECT_NE(reinterpret_cast<int64_t>(buf), -1);
            LOG_VALUE_HEX(buf);
        }
    }
    for(int dev=0; dev<NUM_DEVICES; dev++)
    {
        close(fd[dev]);
    }
}
TEST(driver, poll)
{
    int numDevices = 1; // devices.size();
    LOG_VALUE(numDevices);
    EXPECT_GT(numDevices, 0);
    vector<int> fd;
    for(int dev=0; dev<numDevices; dev++)
    {
        string devName = DEVICE_FILENAME + to_string(dev);
        fd.emplace_back(openDevice(devName.c_str()));
        EXPECT_GT(fd.back(), 0);
    }
    for(int dev=0; dev<numDevices; dev++)
    {
        struct pollfd pollFd;
        pollFd.fd = fd[dev];
        pollFd.events = POLLIN|POLLHUP;
        pollFd.revents = 0;
        
        int ret = poll(&pollFd, 1, -1);
        EXPECT_GE(ret, 0);
        if(ret<0)
        {
            cout << "poll fail" << endl;
        }
    }
    for(int dev=0; dev<NUM_DEVICES; dev++)
    {
        close(fd[dev]);
    }
}
TEST(driver, ioctl)
{    
    auto devices = dxrt::CheckDevices();
    int numDevices = devices.size();
    LOG_VALUE(numDevices);
    EXPECT_GT(numDevices, 0);
    vector<int> fd;
    for(int dev=0; dev<numDevices; dev++)
    {
        string devName = DEVICE_FILENAME + to_string(dev);
        fd.emplace_back(openDevice(devName.c_str()));
        EXPECT_GT(fd.back(), 0);
    }
    for(int dev=0; dev<numDevices; dev++)
    {
        for(auto &pair:ioctlTable)
        {
            for(int cmd=0; cmd<DXRT_CMD_MAX; cmd++)
            {
                int ret;
                vector<dxrt_message_t> messages(1);
                messages[0].cmd = cmd;
                messages[0].data = nullptr;
                messages[0].size = 0;
                for(auto &msg:messages)
                {
                    cout << pair.second << ", " << cmd << endl;
                    ret = ioctl(fd[dev], pair.first, &msg);
                    EXPECT_EQ(ret, 0);
                }
            }
        }
    }
    for(int dev=0; dev<NUM_DEVICES; dev++)
    {
        close(fd[dev]);
    }
}
TEST(driver, ioctl_soc_custom)
{
    int numReq = 1;
    
    auto devices = dxrt::CheckDevices();
    int numDevices = devices.size();
    LOG_VALUE(numDevices);
    EXPECT_GT(numDevices, 0);
    vector<int> fd;
    for(int dev=0; dev<numDevices; dev++)
    {
        string devName = DEVICE_FILENAME + to_string(dev);
        fd.emplace_back(openDevice(devName.c_str()));
        EXPECT_GT(fd.back(), 0);
    }
    for(int dev=0; dev<numDevices; dev++)
    {
        for(int i=0; i<numReq; i++)
        {
            int ret;
            dxrt_message_t message;
            message.cmd = dxrt::dxrt_cmd_t::DXRT_CMD_SOC_CUSTOM;
            message.data = nullptr;
            message.size = 0; 
            ret = ioctl(fd[dev], dxrt::dxrt_ioctl_t::DXRT_IOCTL_MESSAGE, &message);
            EXPECT_EQ(ret, 0);
        }
    }
    for(int dev=0; dev<NUM_DEVICES; dev++)
    {
        close(fd[dev]);
    }
}
TEST(driver, struct)
{
    dxrt_meminfo_t meminfo;
    dxrt_request_t inference;
    dxrt_request_acc_t inferenceAcc;
    dxrt_model_t model;
    fillStructIncreasingValues(meminfo);
    fillStructIncreasingValues(inference);
    fillStructIncreasingValues(inferenceAcc);
    fillStructIncreasingValues(model);
    EXPECT_EQ(*(reinterpret_cast<uint64_t*>(&meminfo)), 0x706050403020100 );
    EXPECT_EQ(*(reinterpret_cast<uint64_t*>(&inference)), 0x706050403020100 );
    EXPECT_EQ(*(reinterpret_cast<uint64_t*>(&inferenceAcc)), 0x706050403020100 );
    EXPECT_EQ(*(reinterpret_cast<uint64_t*>(&model)), 0x706050403020100 );
    cout << meminfo << endl;
    cout << inference << endl;
    cout << inferenceAcc << endl;
    cout << model << endl;
}
