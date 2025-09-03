/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */
 
#ifdef _WIN32 // all or nothing

#include "ipc_pipe_windows.h"
#include "dxrt/common.h"
#include "dxrt/ipc_wrapper/ipc_message.h"

using namespace dxrt;

#include <strsafe.h>

// using namespace std;

#define BUFSIZE 4096

const char* IPCPipeWindows::PIPE_NAME = "\\\\.\\pipe\\dxrt_service_ipc" ;

// /////////////////////////////////////////////////////////////////////////////
IPCPipeWindows::IPCPipeWindows(HANDLE hPipe)
	: _hPipe(hPipe)
{
	overlappedSend = {};
	overlappedSend.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	overlappedRecv = {};
	overlappedRecv.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
}
IPCPipeWindows::~IPCPipeWindows()
{
	Close();
}

int32_t IPCPipeWindows::SendOL(LPCVOID message, int32_t messageLength, LPDWORD byteWritten)
{
	if (!IsAvailable())   return 0;
	// OVERLAPPED overlappedSend1 = {}; overlappedSend1.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	while (true) {
		BOOL fSuccess = WriteFile(_hPipe, message, messageLength, byteWritten, &overlappedSend);
		// if (!fSuccess || messageLength != *byteWritten) {
		if (!fSuccess) {
			if (GetLastError() == ERROR_IO_PENDING) {
				auto start = std::chrono::high_resolution_clock::now();
				WaitForSingleObject(overlappedSend.hEvent, INFINITE);
//				durationPrint(start, "IPCPipeWindows::SendOL :");
				GetOverlappedResult(_hPipe, &overlappedSend, byteWritten, FALSE);
				fSuccess = TRUE ;
			}
			else {
				LOG_DXRT_I_ERR("WriteFile to pipe failed. GLE=" << GetLastError() << ", handle=" << reinterpret_cast<uint64_t>(_hPipe)); // return -1;
				if (GetLastError() == ERROR_NO_DATA)return -1;	// pipe is closing;
				if (GetLastError() == ERROR_INVALID_HANDLE) return -1;  // pipe is closing

				if (_hPipe == INVALID_HANDLE_VALUE)
				{
					LOG_DXRT_I_ERR("pipe is invalid value " << GetLastError());
					return -1; //closed pipe
				}

			}
		}
		if (fSuccess)	break;
		// break;
	}
	// printf("\nMessage sent to server(%d len)\n", *byteWritten);
	return *byteWritten;
}
int32_t IPCPipeWindows::ReceiveOL(LPVOID buffer, int32_t bytesToRead, LPDWORD byteRead)
{
	// OVERLAPPED overlappedRecv1 = {};overlappedRecv1.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	BOOL fSuccess = FALSE;
	auto start = std::chrono::high_resolution_clock::now();
	do {
#if 1
		DWORD bytesAvail = 0;
		if (!PeekNamedPipe(_hPipe, NULL, 0, NULL, &bytesAvail, NULL)) {
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			continue; 
		}
#endif
		// durationPrint(start, "IPCPipeWindows::PeekNamedPipe :");
#if 0
		// no overlapped
		fSuccess = ReadFile(_hPipe, buffer, bytesToRead, byteRead, NULL);
		if (!fSuccess && GetLastError() == ERROR_NO_DATA) continue;
		if (!fSuccess && GetLastError() != ERROR_MORE_DATA) break;
#endif
#if 1
		// overlapped
		fSuccess = ReadFile(_hPipe, buffer, bytesToRead, byteRead, &overlappedRecv);
		if (!fSuccess) {
			if (GetLastError() == ERROR_IO_PENDING) {
				DWORD e = WaitForSingleObject(overlappedRecv.hEvent, INFINITE);
				GetOverlappedResult(_hPipe, &overlappedRecv, byteRead, FALSE);
				// if (e == WAIT_OBJECT_0)	break;
				// if (e == WAIT_TIMEOUT) continue;
				fSuccess = true;
			}
			else {
				LOG_DXRT_I_ERR("ReadFile from pipe failed. GLE=" << GetLastError() << ", handle " << reinterpret_cast<uint64_t>(_hPipe));
				if (GetLastError() == ERROR_NO_DATA)return -1;	// pipe is closing;
			}
		}
		// if ( fSuccess ) break;
		// if (!fSuccess && GetLastError() == ERROR_NO_DATA) continue;
		// if (!fSuccess && GetLastError() != ERROR_MORE_DATA) break;
#endif
	} while (!fSuccess);  // repeat loop if ERROR_MORE_DATA
	// if (!fSuccess) { LOG_DXRT_I_ERR("ReadFile from pipe failed. GLE=" << GetLastError()); return -1; }
	return *byteRead;
}


int32_t IPCPipeWindows::Send(LPCVOID message, int32_t messageLength, LPDWORD byteWritten)
{
	if (!IsAvailable())   return 0;
	return SendOL(message, messageLength, byteWritten);

	BOOL fSuccess = WriteFile(_hPipe, message, messageLength, byteWritten, NULL);
	// if (!fSuccess || messageLength != *byteWritten) {
	if (!fSuccess) {
		LOG_DXRT_I_ERR("WriteFile to pipe failed. GLE=" << GetLastError()); return -1;
	}
	LOG_DXRT_I_DBG << "Message sent to server(" << *byteWritten << " len), receiving reply as follows:" << std::endl;
	return *byteWritten;
}
int32_t IPCPipeWindows::Receive(LPVOID buffer, int32_t bytesToRead, LPDWORD byteRead)
{
	return ReceiveOL(buffer, bytesToRead, byteRead);
	BOOL fSuccess = FALSE;
	do {
		DWORD bytesAvail = 0;
		// if (!PeekNamedPipe(_hPipe, NULL, 0, NULL, &bytesAvail, NULL)) {	continue; }
		fSuccess = ReadFile(_hPipe, buffer, bytesToRead, byteRead, NULL);
		if (!fSuccess && GetLastError() == ERROR_NO_DATA) continue;
		if (!fSuccess && GetLastError() != ERROR_MORE_DATA) break;
	} while (!fSuccess);  // repeat loop if ERROR_MORE_DATA
	if (!fSuccess) {
		printf("ReadFile from pipe failed. GLE=%d\n", GetLastError()); return -1;
	}
	return *byteRead;
}
void IPCPipeWindows::Close()
{
	if (_hPipe == INVALID_HANDLE_VALUE)    return;
	if (isServerSide) CloseServerSide();
	else              CloseHandle(_hPipe);
	_hPipe = INVALID_HANDLE_VALUE;
}
void IPCPipeWindows::CloseServerSide()
{
	if (_hPipe == INVALID_HANDLE_VALUE)    return;
	// Flush the pipe to allow the client to read the pipe's contents
	// before disconnecting. Then disconnect the pipe, and close the
	// handle to this pipe instance.
	FlushFileBuffers(_hPipe);
	DisconnectNamedPipe(_hPipe);
	CloseHandle(_hPipe);
	_hPipe = INVALID_HANDLE_VALUE;
}
void IPCPipeWindows::InitClient()
{
	if (IsAvailable())	return;
	while (1) {
		LOG_DXRT_I_DBG << "Pipe Client : IPCPipeWindows::InitClient at RT, PipeName=" << PIPE_NAME << std::endl;
		_hPipe = CreateFile(
			PIPE_NAME,   // pipe name
			GENERIC_READ |  GENERIC_WRITE,
			0,              // no sharing
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe
			FILE_FLAG_OVERLAPPED,
			// 0,              // default attributes
			NULL);          // no template file
		// Break if the pipe handle is valid.
		if (_hPipe != INVALID_HANDLE_VALUE) break;
		// Exit if an error other than ERROR_PIPE_BUSY occurs.
		if (GetLastError() != ERROR_PIPE_BUSY) {
			LOG_DXRT_I_ERR("Could not open pipe. GLE=" << GetLastError());
			return;
		}
		// All pipe instances are busy, so wait for 5 seconds.
		if (!WaitNamedPipe(PIPE_NAME, 5000)) {
			LOG_DXRT_I_ERR("Could not open pipe: 20 second wait timed out.");
			return;
		}
	}
	LOG_DXRT_I_DBG << "IPCPipeWindows::InitClient at RT : Success" << std::endl;
	// The pipe connected; change to message-read mode.
	DWORD dwMode = PIPE_READMODE_MESSAGE;
	BOOL   fSuccess = SetNamedPipeHandleState(
		_hPipe,    // pipe handle
		&dwMode,  // new pipe mode
		NULL,     // don't set maximum bytes
		NULL);    // don't set maximum time
	if (!fSuccess) {
		LOG_DXRT_I_ERR("SetNamedPipeHandleState failed. GLE=" << GetLastError());
	}
}
void IPCPipeWindows::InitServer()
{
	LOG_DXRT_I_DBG << "Pipe Server: before CreateNamedPipe on " << PIPE_NAME << std::endl;
	constexpr int szBuf = 4096;
	_hPipe = CreateNamedPipe(
		PIPE_NAME,             		// pipe name
		FILE_FLAG_OVERLAPPED |
		PIPE_ACCESS_DUPLEX,       	// read/write access
		PIPE_TYPE_MESSAGE |       	// message type pipe
		PIPE_READMODE_MESSAGE |   	// message-read mode
		PIPE_WAIT,                	// blocking mode
//		PIPE_NOWAIT,                	// blocking mode
		PIPE_UNLIMITED_INSTANCES, 	// max. instances
		szBuf,                  	// output buffer size
		szBuf,                  	// input buffer size
		0,                        	// client time-out
		NULL);                    	// default security attribute
	if (_hPipe == INVALID_HANDLE_VALUE) {
		LOG_DXRT_I_ERR("CreateNamedPipe failed, GLE=" << GetLastError() << "."); return;
	}
	// Wait for the client to connect; if it succeeds, the function returns a nonzero value.
	// If the function returns zero, GetLastError returns ERROR_PIPE_CONNECTED.
	LOG_DXRT_I_DBG << "Pipe Server: waiting client connection on " << PIPE_NAME << std::endl;
	BOOL fConnected = ConnectNamedPipe(_hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
	if (!fConnected) { CloseHandle(_hPipe); _hPipe = INVALID_HANDLE_VALUE; return; }
	LOG_DXRT_I_DBG << "Pipe Server: connected client connection on " << PIPE_NAME << std::endl;
#if 0
	while (true) {
		BOOL fConnected = ConnectNamedPipe(_hPipe, NULL);
		if (fConnected)	break;
		if (!fConnected && GetLastError() == ERROR_PIPE_CONNECTED) {
			_hPipe = INVALID_HANDLE_VALUE; return;
		}
		if (!fConnected && GetLastError() == ERROR_PIPE_LISTENING ) { continue; }	// PIPE_NOWAIT
	}
#endif
	isServerSide = true;
}

// /////////////////////////////////////////////////////////////////////////////
#endif
