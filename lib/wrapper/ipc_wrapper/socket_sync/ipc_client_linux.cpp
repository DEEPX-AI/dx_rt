// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "ipc_client_linux.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>

using namespace dxrt;

const char* IPCClientLinux::SOCKET_NAME = "/tmp/dxrt_service_ipc";
//#define SOCKET_NAME "/tmp/dxrt_service_ipc"

IPCClientLinux::IPCClientLinux()
: _socketFd(-1)
{
    LOG_DXRT_DBG << "IPCClientLinux::Constructor" << std::endl;
    
}

IPCClientLinux::~IPCClientLinux()
{
    LOG_DXRT_DBG << "IPCClientLinux::Destructor" << std::endl;
    _stop = true;
    if (_thread.joinable())
        _thread.join();
}
namespace dxrt{
int ipc_callBack(IPCServerMessage& outResponseServerMessage, void* usrData);
}

// Intitialize IPC
int32_t IPCClientLinux::Initialize()
{
    LOG_DXRT_DBG << "IPCClientLinux::Initialize" << std::endl;

    try
    {
        if ((_socketFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
            std::cerr << "Socket creation failed" << std::endl;
            return -1;
        }

        struct sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        //snprintf(addr.sun_path, sizeof(addr.sun_path)-1, SOCKET_NAME);
        strcpy(addr.sun_path, SOCKET_NAME);


        if (connect(_socketFd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            std::cerr << "Connection failed" << std::endl;
            return -1;
        }
        RegisterReceiveCB(dxrt::ipc_callBack, nullptr);
        _thread = std::thread(&IPCClientLinux::threadFunc, this);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Error on socket initialize" << std::endl;
    }

    return 0;
}

static pid_t pid_cache = -1;
// Send to server
int32_t IPCClientLinux::SendToServer(IPCClientMessage& clientMessage)
{
    LOG_DXRT_DBG << "IPCClientLinux::SendToServer" << std::endl;

    clientMessage.pid = getpid();
    DXRT_ASSERT(getpid() == clientMessage.pid, "GETPID ERROR");
    if (pid_cache == -1)
    {
        pid_cache = clientMessage.pid;
    }
    else DXRT_ASSERT(pid_cache == clientMessage.pid, "diff getpid");

    if ( _socketFd >= 0 )
    {
        try
        {
            int size = write(_socketFd, &clientMessage, sizeof(clientMessage));
            LOG_DXRT_DBG << "write org-size=" << sizeof(clientMessage) << " size=" << size << std::endl;
        }
        catch(std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        catch(...)
        {
            std::cerr << "Error on socket write" << std::endl;
        }

    }

    return 0;
}

int32_t IPCClientLinux::SendToServer(IPCServerMessage& outResponseServerMessage, IPCClientMessage& inRequestClientMessage)
{

    std::cout << "IPCClientLinux::SendToServer (request and response)" << std::endl;
    if ( _socketFd >= 0 )
    {
        try 
        {
            int size = write(_socketFd, &inRequestClientMessage, sizeof(inRequestClientMessage));
            LOG_DXRT_DBG << "write org-size=" << sizeof(inRequestClientMessage) << " size=" << size << std::endl;
        }
        catch(std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        catch(...)
        {
            std::cerr << "Error on socket write" << std::endl;
        }

        // read
        _waitingCall = std::make_shared<std::promise<IPCServerMessage> >();
        auto future = _waitingCall->get_future();

        outResponseServerMessage = future.get();
    }

    return 0;

}



void IPCClientLinux::threadFunc()
{
    while (true)
    {
        if (_stop)
        {
            Close();
            break;
        }
        fd_set read_fds;
        struct timeval timeout;

        // 파일 디스크립터 설정
        FD_ZERO(&read_fds);
        FD_SET(_socketFd, &read_fds);

        // 타임아웃 설정
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000*500;

        // select() 호출
        int result = select(_socketFd+1, &read_fds, nullptr, nullptr, &timeout);

        if (result < 0) {
            perror("select error");
            return;
        } else if (result == 0) {
            //LOG_DXRT_DBG << "Timeout occurred! No data available." << std::endl;
            continue;
        }
        IPCServerMessage outResponseServerMessage;
        int size = read(_socketFd, &outResponseServerMessage, sizeof(outResponseServerMessage));
        if ( size > 0 )
        {
            LOG_DXRT_DBG << "read org-size=" << sizeof(outResponseServerMessage) << " size=" << size << std::endl;
        }
        if (_waitingCall)
        {
            _waitingCall->set_value(outResponseServerMessage);
            _waitingCall = nullptr;
        }
        _receiveCB(outResponseServerMessage, nullptr);
        usleep(1000);
    }
}


int32_t IPCClientLinux::ReceiveFromServer(IPCServerMessage& serverMessage)
{
    (void)serverMessage;

    return 0;
}

// register receive message callback function
int32_t IPCClientLinux::RegisterReceiveCB(std::function<int32_t(IPCServerMessage&,void*)> receiveCB, void* usrData)
{
    _receiveCB = receiveCB;
    (void)usrData;
    return 0;
}

// close the connection
int32_t IPCClientLinux::Close()
{
    LOG_DXRT_DBG << "IPCClientLinux::Close" << std::endl;
    if ( _socketFd >= 0 ) 
    {
        close(_socketFd);
        _socketFd = -1;
    }


    return 0;
}






