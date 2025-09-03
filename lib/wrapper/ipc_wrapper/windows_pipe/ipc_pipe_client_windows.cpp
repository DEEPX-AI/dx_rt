/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#ifdef _WIN32 // all or nothing

#include <windows.h>
#include "ipc_pipe_client_windows.h"

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

// #include <fcntl.h>
// #include <errno.h>
// #include <csignal>

#include "../../../include/dxrt/common.h"

// for debug
// #define LOG_DXRT_DBG std::cout

namespace dxrt
{

IPCPipeClientWindows::IPCPipeClientWindows(long msgType)
	: _usrData(nullptr), _msgType(msgType), _receiveCB(nullptr)
{
    LOG_DXRT_DBG << "IPCPipeClientWindows::Constructor" << std::endl;
}

IPCPipeClientWindows::~IPCPipeClientWindows()
{
    LOG_DXRT_DBG << "IPCPipeClientWindows::Destructor" << std::endl;
	// _queCv.notify_all();
    Close();
}

#if 0
// /////////////////////////////////////////////////////////////////////////////
void IPCPipeClientWindows::enQue(IPCServerMessage& m)
{
	unique_lock<mutex> lk(_queMt);
	_que.push(m);
	_queCv.notify_all();
}
int32_t IPCPipeClientWindows::deQue(IPCServerMessage& m)
{
	LOG_DXRT_DBG << "IPCPipeClientWindows::ReceiveFromClient:deQue start\n" ;
	unique_lock<mutex> lk(_queMt);
	_queCv.wait(
		lk, [this] {
			return _que.size() || _stop.load();
		}
	);
	// LOG_DXRT_DBG << threadName << " : wake up. (" << _que.size() << ") " << endl;
	if (_stop.load()) {
		// LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
		return -1; //
	}
	auto m1 = _que.front();
	_que.pop();
	lk.unlock();
	memcpy(&m, &m1, sizeof(m));
	LOG_DXRT_DBG << "IPCPipeClientWindows::ReceiveFromClient:deQue end\n" ;
	return 0;
}
#endif

// /////////////////////////////////////////////////////////////////////////////
int32_t IPCPipeClientWindows::Initialize()
{
    LOG_DXRT_DBG << "IPCPipeClientWindows::Initialize" << std::endl;
    _pipe.InitClient();
    return 0;
}

int32_t IPCPipeClientWindows::SendToServer(IPCServerMessage& outServerMessage, IPCClientMessage& clientMessage)
{
    if ( _receiveCB == nullptr )	{
        clientMessage.seqId = 0; //seq_increment++; // review
        SendToServer(clientMessage);
        ReceiveFromServer(outServerMessage);
    }
    else
    {
       return -1;
    }
    return 0;
}

// return -1: error, 0: no data sent
int32_t IPCPipeClientWindows::SendToServer(IPCClientMessage& clientMessage)
{
	LOG_DXRT_DBG << "IPCPipeClientWindows::SendToServer start\n" ;
	int resultWriteSize = 0;
    if (!_pipe.IsAvailable()) return resultWriteSize;
    clientMessage.msgType = _msgType;
    DWORD cbWritten=0;
	_pipe.Send(&clientMessage, sizeof(clientMessage), &cbWritten);
    resultWriteSize = cbWritten;
    LOG_DXRT_DBG << "IPCPipeClientWindows::SendToServer end\n" ;
    return resultWriteSize;
}

int32_t IPCPipeClientWindows::ReceiveFromServer(IPCServerMessage& serverMessage)
{
	LOG_DXRT_DBG << "IPCPipeClientWindows::ReceiveFromServer start\n" ;
    int32_t resultReadSize = -1;
    if (!_pipe.IsAvailable()) return resultReadSize;
    try
    {
        DWORD  cbRead=0;
        _pipe.Receive(&serverMessage, sizeof(serverMessage), &cbRead);
        // return -1: no data, 0: no connection
        if (cbRead == 0)    return -1;
        resultReadSize = cbRead;
    }
    catch(const std::exception& e)
    {
        LOG_DXRT_ERR(e.what());
        resultReadSize = -1;
    }
    catch(...)
    {
        LOG_DXRT_ERR("Error on read from server");
        resultReadSize = -1;
    }
	LOG_DXRT_DBG << "IPCPipeClientWindows::ReceiveFromServer end\n" ;
    return resultReadSize;
}

// close the connection
int32_t IPCPipeClientWindows::Close()
{
    if ( _threadRunning.load() )	{
        RegisterReceiveCB(nullptr, nullptr);
    }
	_pipe.Close();
    return 0;
}

// register receive message callback function
int32_t IPCPipeClientWindows::RegisterReceiveCB(std::function<int32_t(IPCServerMessage&,void*)> receiveCB, void* usrData)
{
    if ( _threadRunning.load() )
    {
        _threadRunning.store(false);
        /*if ( _thread.joinable() )
        {
            _thread.join();
        }*/
        _thread.detach();
        _receiveCB = nullptr;
        LOG_DXRT_I_DBG << "IPCPipeClientWindows: Detached Callback Thread" << std::endl;
    }

    if ( _pipe.IsAvailable() )
    {
        _receiveCB = receiveCB;
        _usrData = usrData;
        if ( _receiveCB != nullptr )	{
            _threadRunning.store(true);
            _thread = std::thread(IPCPipeClientWindows::ThreadFunc, this);
            LOG_DXRT_I_DBG << "IPCPipeClientWindows: Created Callback Thread" << std::endl;
        }
    }
    return 0;
}

void IPCPipeClientWindows::ThreadFunc(IPCPipeClientWindows* _pipe)
{
    while(true)// mqClient->_threadRunning.load())
    {
        IPCServerMessage serverMessage;
        serverMessage.msgType = getpid();
        if (_pipe->ReceiveFromServer(serverMessage) != -1) {
            LOG_DXRT_I_DBG << "Thread Running by " << serverMessage.code << std::endl;
            if ( _pipe->_receiveCB != nullptr )
            {
            	_pipe->_receiveCB(serverMessage, _pipe->_usrData);
            }
        } else {
            LOG_DXRT_I_ERR("ReceiveFromServer fail");
        }
    }
    LOG_DXRT_I_DBG << "IPCPipeClientWindows::Thread Finished" << std::endl;
}

}  // namespace dxrt

#endif // _WIN32
