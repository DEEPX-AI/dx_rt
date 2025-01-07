// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#ifdef _WIN32 // all or nothing

#pragma once

#include <stdint.h>
#include <cstdint>
#include <future>

#include <windows.h>

namespace dxrt 
{
	class IPCPipeWindows
	{
	public:
		static const char* PIPE_NAME; //  = "\\\\.\\pipe\\mynamedpipe";
	protected:
		HANDLE _hPipe = INVALID_HANDLE_VALUE;
		bool isServerSide = false;
		OVERLAPPED overlappedSend = {};
		OVERLAPPED overlappedRecv = {};

	public:
		IPCPipeWindows(HANDLE hPipe_ = INVALID_HANDLE_VALUE);
		virtual ~IPCPipeWindows();
		bool IsAvailable() { return _hPipe != INVALID_HANDLE_VALUE; }
		HANDLE Detatch()
		{
			HANDLE h = _hPipe;
			_hPipe = INVALID_HANDLE_VALUE;
			return h;
		}
		int32_t SendOL(LPCVOID message, int32_t messageLength, LPDWORD byteWritten);
		int32_t ReceiveOL(LPVOID buffer, int32_t bytesToRead, LPDWORD byteRead);
		int32_t Send(LPCVOID message, int32_t messageLength, LPDWORD byteWritten);
		int32_t Receive(LPVOID buffer, int32_t bytesToRead, LPDWORD byteRead);
		void Close();
		void CloseServerSide();
		// ////////////////////////////////////////////////////////////////////////
		void InitClient();
		void InitServer();
	};

}  // namespace dxrt

#endif
