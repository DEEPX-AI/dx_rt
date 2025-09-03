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
#include "ipc_pipe_server_windows.h"

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace std;
using namespace dxrt;

IPCPipeServerWindows::IPCPipeServerWindows()
{
}

IPCPipeServerWindows::IPCPipeServerWindows(uint64_t fd)
{
}

IPCPipeServerWindows::~IPCPipeServerWindows()
{
	_queCv.notify_all();
}

int32_t IPCPipeServerWindows::Initialize()
{
	LOG_DXRT_I_DBG << "IPCPipeServerWindows::Initialize " << std::endl;
    int result = 0;
	std::thread th(&IPCPipeServerWindows::ThreadAtServerMainForListen, this);
	th.detach();

    return result;
}

void IPCPipeServerWindows::ThreadAtServerMainForListen()
{
	// The main loop creates an instance of the named pipe and  then waits for a client to connect to it.
	// When the client connects, a thread is created to handle communications with that client,
	//   and this loop is free to wait for the next client connect request. It is an infinite loop.
    LOG_DXRT_I_DBG << "@@@ Thread Start : ThreadAtServerMainForListen" << std::endl ;
	for (;;) {
		_pipe.InitServer();
		if (!_pipe.IsAvailable())  continue;
		LOG_DXRT_I_DBG << "Client connected, creating a processing thread.\n" ;
		std::thread th(&IPCPipeServerWindows::ThreadAtServerByClient, this, _pipe.Detatch());
		th.detach();
	}
    LOG_DXRT_I_DBG << "@@@ Thread End : ThreadAtServerMainForListen" << std::endl ;
	LOG_DXRT_I_DBG << "ThreadAtServerMainForListen exiting.\n" ;
}
void IPCPipeServerWindows::ThreadAtServerByClient(HANDLE hPipe)
{
    LOG_DXRT_I_DBG << "@@@ Thread Start : ThreadAtServerByClient(enQue)" << std::endl ;
	IPCPipeWindows pipe(hPipe);
	IPCClientMessage clientMessage;
	DWORD cbReceived = 0;
	bool bFirst = true;
	while (1) {
		auto start = std::chrono::high_resolution_clock::now();
		pipe.Receive(&clientMessage, sizeof(clientMessage), &cbReceived);
		string s = "Server pipe.Receive : "; s += _s(clientMessage.code);	s += " : ";
		// durationPrint(start, s.c_str());
		if (sizeof(clientMessage) != cbReceived)   break;
		LOG_DXRT_I_DBG << "Received: client msgType:" << clientMessage.msgType << std::endl ;
		if (bFirst) {
			bFirst = false;
			_msgType2handle[clientMessage.msgType] = hPipe;
		}
		enQue(clientMessage);
		// std::this_thread::sleep_for(std::chrono::microseconds(1)); // important:if use, 30ms delay
	}
	pipe.CloseServerSide();
    LOG_DXRT_I_DBG << "@@@ Thread End : ThreadAtServerByClient(enQue)" << std::endl ;
	LOG_DXRT_I_DBG << "ThreadAtServerByClient exiting.\n" ;
}

// listen
int32_t IPCPipeServerWindows::Listen()
{
    LOG_DXRT_I_DBG << "IPCPipeServerWindows::Listen" << std::endl;
    return 0;
}

int32_t IPCPipeServerWindows::Select(int64_t& connectedFd)
{
    (void)connectedFd;
    return 0;
}

// ReceiveFromClient
// return 0: no data, -1: no connection
int32_t IPCPipeServerWindows::ReceiveFromClient(IPCClientMessage& clientMessage)
{
	return deQue(clientMessage);
}

int32_t IPCPipeServerWindows::SendToClient(IPCServerMessage& serverMessage)
{
	int resultWriteSize = 0;
	// if (_hPipe == INVALID_HANDLE_VALUE)	return resultWriteSize;
	try
	{
		auto it1 = _msgType2handle.find(serverMessage.msgType);
		if (it1 == _msgType2handle.end())    {
			LOG_DXRT_I_DBG << "IPCPipeServerWindows::SendToClient : Pipe Handle not found.\n" ;
			resultWriteSize = 0;
		}
		else {
			HANDLE hPipe = it1->second;
			IPCPipeWindows pipe(hPipe);
			DWORD cbWritten = 0;
			pipe.Send(&serverMessage, sizeof(serverMessage), &cbWritten);
			pipe.Detatch();
			if (sizeof(serverMessage) != cbWritten)  resultWriteSize = -1;
			else resultWriteSize = cbWritten;
		}
    }
    catch (std::exception& e)
    {
        LOG_DXRT_ERR(e.what());
        resultWriteSize = -1;
    }
    catch (...)
    {
        LOG_DXRT_ERR("Error on socket write");
        resultWriteSize = -1;
    }
	LOG_DXRT_I_DBG << "IPCPipeServerWindows::SendToClient end\n" ;
	return resultWriteSize;
}

int32_t IPCPipeServerWindows::RegisterReceiveCB(std::function<int32_t(IPCClientMessage&,void*,int32_t)> receiveCB, void* usrData)
{
	// unused
	return 0;
}

int32_t IPCPipeServerWindows::Close()
{
    LOG_DXRT_I_DBG << "IPCPipeServerWindows::Close" << std::endl;
	_pipe.Close();
    return 0;
}

// /////////////////////////////////////////////////////////////////////////////
void IPCPipeServerWindows::enQue(IPCClientMessage& m)
{
	unique_lock<mutex> lk(_queMt);
	_que.push(m);
	_queCv.notify_all();
}
int32_t IPCPipeServerWindows::deQue(IPCClientMessage& m)
{
	LOG_DXRT_I_DBG << "IPCPipeServerWindows::ReceiveFromClient:deQue start\n" ;
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
	LOG_DXRT_I_DBG << "IPCPipeServerWindows::ReceiveFromClient:deQue end\n" ;
	return 0;
}

#endif
