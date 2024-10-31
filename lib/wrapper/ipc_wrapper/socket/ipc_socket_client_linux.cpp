// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>
#include <csignal>


#include "ipc_socket_client_linux.h"
#include "../../../include/dxrt/common.h"

using namespace dxrt;

const char* IPCSocketClientLinux::SOCKET_NAME = "/tmp/dxrt_service_ipc";

const int IPCSocketClientLinux::BUFFER_SIZE = 1024;
const int IPCSocketClientLinux::MAX_EVENTS = 10;

//#define SOCKET_NAME "/tmp/dxrt_service_ipc"

/*static // Non-blocking Setting
int setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}*/

IPCSocketClientLinux::IPCSocketClientLinux()
: _socketFd(-1), _epollFd(-1), _usrData(nullptr)
{
    LOG_DXRT_DBG << "IPCClientLinux::Constructor" << std::endl;
   
}

IPCSocketClientLinux::~IPCSocketClientLinux()
{
    LOG_DXRT_DBG << "IPCSocketClientLinux::Destructor" << std::endl;
    
    Close();
}


// Intitialize IPC
int32_t IPCSocketClientLinux::Initialize()
{
    LOG_DXRT_DBG << "IPCSocketClientLinux::Initialize" << std::endl;

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
        strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path)-1);

        /*if (setNonBlocking(_socketFd) == -1) {
            std::cerr << "Failed to set socket to non-blocking mode" << std::endl;
            close(_socketFd);
            return -1;
        }*/

        if (connect(_socketFd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            std::cerr << "Connection failed" << std::endl;
            return -1;
        }

        // create epoll instance
        _epollFd = epoll_create1(0);
        if (_epollFd == -1) {
            std::cerr << "Failed to create epoll instance" << std::endl;
            close(_socketFd);
            return -1;
        }

        // register client socket to epoll
        struct epoll_event event; // events[MAX_EVENTS];
        event.events = EPOLLOUT | EPOLLIN | EPOLLET;  // read/write/edge triger mode
        event.data.fd = _socketFd;
        if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _socketFd, &event) == -1) {
            std::cerr << "Failed to add socket to epoll" << std::endl;
            close(_socketFd);
            return -1;
        }

        //RegisterReceiveCB(dxrt::ipc_callBack);
        //_thread = std::thread(&IPCAsyncClientLinux::threadFunc, this);

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

int32_t IPCSocketClientLinux::SendToServer(IPCServerMessage& outServerMessage, IPCClientMessage& clientMessage)
{
    

    (void)outServerMessage;
    (void)clientMessage;

    return 0;
}

//static pid_t pid_cache = -1;
// Send to server
// return -1: error, 0: no data sent
int32_t IPCSocketClientLinux::SendToServer(IPCClientMessage& clientMessage)
{
    //std::cout << "IPCClientLinux::SendToServer" << std::endl;

    /*clientMessage.pid = getpid();
    DXRT_ASSERT(getpid() == clientMessage.pid, "GETPID ERROR");
    if (pid_cache == -1)
    {
        pid_cache = clientMessage.pid;
    }
    else DXRT_ASSERT(pid_cache == clientMessage.pid, "diff getpid");
    */

   int resultWriteSize = 0;

    if ( _socketFd >= 0 )
    {
        try
        {
            resultWriteSize = write(_socketFd, &clientMessage, sizeof(clientMessage));
            //std::cout << "write org-size=" << sizeof(clientMessage) << " size=" << size << std::endl;
        }
        catch(std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            resultWriteSize = -1;
        }
        catch(...)
        {
            std::cerr << "Error on socket write" << std::endl;
            resultWriteSize = -1;
        }

    }

    return resultWriteSize;
}

int32_t IPCSocketClientLinux::ReceiveFromServer(IPCServerMessage& serverMessage)
{
    int32_t resultReadSize = -1;

    try
    {
        // return -1: no data, 0: no connection
        int size = read(_socketFd, &serverMessage, sizeof(serverMessage));
        //std::cout << "Received message size=" << size << std::endl;
        
        if ( size == 0 ) resultReadSize = -1;
        else if ( size == -1 ) resultReadSize = 0;
        else resultReadSize = size;
        //std::cout << serverMessage << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        resultReadSize = -1;
    }
    catch(...)
    {
        std::cerr << "Error on read from server" << std::endl;
        resultReadSize = -1;
    }
   

    return resultReadSize;
}

// register receive message callback function
int32_t IPCSocketClientLinux::RegisterReceiveCB(std::function<int32_t(IPCServerMessage&,void*)> receiveCB, void* usrData)
{

    if ( _socketFd >= 0 ) 
    {
    
        if ( _threadRunning )
        {
            _threadRunning = false;
            if ( _thread.joinable() )
            {
                _thread.join();
            }

            _receiveCB = nullptr;
        }


        _receiveCB = receiveCB;
        _usrData = usrData;

        if ( _receiveCB != nullptr ) 
        {
            _threadRunning = true;
            _thread = std::thread(IPCSocketClientLinux::ThreadFunc, this);
        }
    } // socketFd
    
    return 0;
}

// close the connection
int32_t IPCSocketClientLinux::Close()
{
    //std::cout << "IPCClientLinux::Close" << std::endl;
    if ( _socketFd >= 0 ) 
    {
        RegisterReceiveCB(nullptr, nullptr);

        close(_socketFd);
        _socketFd = -1;
    }


    return 0;
}


void IPCSocketClientLinux::ThreadFunc(IPCSocketClientLinux* socketClient)
{

    struct epoll_event events[IPCSocketClientLinux::MAX_EVENTS];

    while(socketClient->_threadRunning)
    {
        int num_fds = epoll_wait(socketClient->_epollFd, events, IPCSocketClientLinux::MAX_EVENTS, -1);
        for (int i = 0; i < num_fds; ++i) {
            if (events[i].events & EPOLLIN) {
                
                if ( socketClient->_receiveCB != nullptr )
                {
                    IPCServerMessage serverMessage;
                    socketClient->ReceiveFromServer(serverMessage);
                    socketClient->_receiveCB(serverMessage, socketClient->_usrData);
                }
            }
            else 
            {
                //std::cout << "IPCAsyncClientLinux::Thread event=" << events[i].events << std::endl;
            }

            /*
            if (events[i].events & EPOLLOUT) {
                // 서버에 데이터 전송
                const char* message = "Hello, Server!";
                ssize_t sent_bytes = send(events[i].data.fd, message, strlen(message), 0);
                if (sent_bytes == -1) {
                    std::cerr << "Failed to send data" << std::endl;
                } else {
                    std::cout << "Sent to server: " << message << std::endl;
                }

                // EPOLLOUT 이벤트는 처음 전송 후 제거
                event.events = EPOLLIN | EPOLLET;  // 읽기만 감시하도록 변경
                epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_socket, &event);
            }
            */
        }

        //std::cout << "IPCAsyncClientLinux::Thread Running" << std::endl;
    }

    std::cout << "IPCSocketClientLinux::Thread Finished" << std::endl;
}

