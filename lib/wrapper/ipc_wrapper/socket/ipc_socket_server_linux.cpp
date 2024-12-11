
#include "ipc_socket_server_linux.h"
#include "ipc_socket_client_linux.h"

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


using namespace dxrt;

static int gServerFdCloser = -1;
static void cleanup(int signal) {
    std::ignore = signal;
    if (gServerFdCloser != -1) {
        close(gServerFdCloser);
        gServerFdCloser = -1;
    }
    unlink(IPCSocketClientLinux::SOCKET_NAME);
    //LOG_DXRT_DBG << "cleanup\n";
    std::cout << "cleanup" << std::endl;
    std::exit(0);  // normal exit
}

static void setup_signal_handlers() {
    std::signal(SIGINT, cleanup);
    std::signal(SIGTERM, cleanup);
    std::signal(SIGKILL, cleanup);
    std::signal(SIGABRT, cleanup);
    std::signal(SIGPIPE, SIG_IGN);

}

/*static // Non-blocking Setting
int setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}*/

class socket_closer
{
 public:
    explicit socket_closer(int sock_fd)
    : _sock_fd(sock_fd)
    {
    }
    ~socket_closer()
    {
        close(_sock_fd);
        unlink(IPCSocketClientLinux::SOCKET_NAME);
    }
 private:
    int _sock_fd;
};

IPCSocketServerLinux::IPCSocketServerLinux()
: _socketFd(-1), _epollFd(-1), _usrData(nullptr)
{
    std::cout << "IPCSocketServerLinux::Constructor" << std::endl;
}

IPCSocketServerLinux::IPCSocketServerLinux(uint64_t fd)
: _epollFd(-1), _usrData(nullptr)
{
    _socketFd = static_cast<int>(fd);
}

IPCSocketServerLinux::~IPCSocketServerLinux()
{
    std::cout << "IPCSocketServerLinux::Destructor" << std::endl;

    Close();
}

// Intitialize IPC Server
// return error code
int32_t IPCSocketServerLinux::Initialize()
{
    std::cout << "IPCSocketServerLinux::Initialize fd=" << _socketFd << std::endl;

    int32_t result = 0;
    if ( _socketFd < 0 ) 
    {

        try {

            if ((_socketFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) 
            {
                std::cerr << "Socket creation failed" << std::endl;
                return -1;
            }

            struct sockaddr_un addr;
            memset(&addr, 0, sizeof(addr));

            addr.sun_family = AF_UNIX;
            //snprintf(addr.sun_path, sizeof(addr.sun_path)-1, IPCClientLinux::SOCKET_NAME);
            strncpy(addr.sun_path, IPCSocketClientLinux::SOCKET_NAME, sizeof(addr.sun_path)-1);
            unlink(addr.sun_path);

            int err = bind(_socketFd, (struct sockaddr*)&addr, sizeof(addr));
            if (err < 0)
            {
                std::cout << "bind ERROR" << errno << std::endl;
                close(_socketFd);
                _socketFd = -1;
                return -1;
            }

            err = listen(_socketFd, 0);
            if (err < 0)
            {
                std::cout << "listen ERROR" << errno << std::endl;
                close(_socketFd);
                _socketFd = -1;
                return -1;
            }

            std::cout << "socket listen fd=" << _socketFd << std::endl;

            setup_signal_handlers();


            //socket_closer socCloser(_socketFd);
            //gServerFdCloser = _socketFd;

            //fd_set allFds, readFds;

            FD_ZERO(&_readFds);
            FD_SET(_socketFd, &_readFds);

            _max_sock = _socketFd;
        }
        catch (const std::exception& e) 
        {
            std::cerr << e.what() << std::endl;
            result = -1;
        }
        catch (...)
        {
            std::cerr << "Error on IPC socket" << std::endl;
            result = -1;
        }

    #if 0
        while(true)
        {
            _allFds = _readFds;
            int fd_num = select(_max_sock+1, &_allFds, nullptr, nullptr, nullptr);
            if (FD_ISSET(_socketFd, &_allFds))
            {
                struct sockaddr client_addr;
                socklen_t sock_size = sizeof(client_addr);
                int connect_socket_fd = accept(_socketFd, (struct sockaddr*)&client_addr, &sock_size);
                FD_SET(connect_socket_fd, &_readFds);
                if (_max_sock < connect_socket_fd) _max_sock = connect_socket_fd;
                std::cout << "SOCKET ACCEPTED connect-socket-fd=" << connect_socket_fd << std::endl;
            }

            usleep(1000); // 1ms

        }
    #endif // 

    } // socket < 0
    else // available connected client fd
    {
        try {

            // create epoll instance
            _epollFd = epoll_create1(0);
            if (_epollFd == -1) {
                std::cerr << "Failed to create epoll instance" << std::endl;
                close(_socketFd);
                //return -1;
            }

            // register client socket to epoll
            struct epoll_event event; //, events[IPCSocketClientLinux::MAX_EVENTS];
            event.events = EPOLLOUT | EPOLLIN | EPOLLET;  // read/write/edge triger mode
            event.data.fd = _socketFd;
            if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _socketFd, &event) == -1) {
                std::cerr << "Failed to add socket to epoll" << std::endl;
                close(_socketFd);
                //return -1;
            }

        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            //result = -1;
        }
        catch(...)
        {
            std::cerr << "Error on IPC socket (client-fd)" << std::endl;
            //result = -1;
        }
    }

    return result;
}

// listen
int32_t IPCSocketServerLinux::Listen()
{
    std::cout << "IPCSocketServerLinux::Listen" << std::endl;

    return 0;
}

int32_t IPCSocketServerLinux::Select(int64_t& connectedFd)
{
    int32_t result = 0;
    int connect_socket_fd = -1;
    try
    {
        _allFds = _readFds;
        int fd_num = select(_max_sock+1, &_allFds, nullptr, nullptr, nullptr);
        if (fd_num > 0 && FD_ISSET(_socketFd, &_allFds))
        {
            struct sockaddr client_addr;
            socklen_t sock_size = sizeof(client_addr);
            connect_socket_fd = accept(_socketFd, (struct sockaddr*)&client_addr, &sock_size);
            //setNonBlocking(connect_socket_fd);
            FD_SET(connect_socket_fd, &_readFds);


            

            //if (_max_sock < connect_socket_fd) _max_sock = connect_socket_fd;
            //std::cout << "SOCKET ACCEPTED connect-socket-fd=" << connect_socket_fd << std::endl;

            // temp
            /*
            IPCClientMessage clientMessage;
            int size = read(connect_socket_fd, &clientMessage, sizeof(clientMessage));
            std::cout << "Received message size=" << size << std::endl;
            std::cout << clientMessage << std::endl;

            IPCServerMessage serverMessage;
            serverMessage.code = RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION_INPUT_AND_OUTPUT;
            size = write(connect_socket_fd, &serverMessage, sizeof(serverMessage));
            std::cout << "Send message size=" << size << std::endl;
            std::cout << serverMessage << std::endl;
            */
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        result = -1;
    }
    catch(...)
    {
        std::cerr << "Error on IPC socket select" << std::endl;
        result = -1;
    }

    if ( connect_socket_fd >= 0 )
    {
        connectedFd = static_cast<int64_t>(connect_socket_fd);
    }
    
    return result;
}


// ReceiveFromClient
// return 0: no data, -1: no connection
int32_t IPCSocketServerLinux::ReceiveFromClient(IPCClientMessage& clientMessage)
{
    //std::cout << "IPCSocketServerLinux::ReceiveFromClient" << std::endl;
    int32_t result = -1;

    try
    {
        // return -1: no data, 0: no connection
        int size = read(_socketFd, &clientMessage, sizeof(clientMessage));
        //std::cout << "Received message size=" << size << std::endl;

        if ( size == 0 ) result = -1;
        else if ( size == -1 ) result = 0;
        else result = size;
        //std::cout << clientMessage << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        result = -1;
    }
    catch(...)
    {
        std::cerr << "Error on read from client" << std::endl;
        result = -1;
    }
   

    return result;
}

// SendToClient
int32_t IPCSocketServerLinux::SendToClient(IPCServerMessage& serverMessage)
{
    //std::cout << "IPCServerLinux::SendToClient" << std::endl;

    int32_t result = 0;
    try
    {
        int size = write(_socketFd, &serverMessage, sizeof(serverMessage));
        std::cout << "Send message size=" << size << std::endl;
        if ( size <= 0 ) 
        {
            result = -1;
        }
        //std::cout << serverMessage << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        result = -1;
    }
    catch(...)
    {
        std::cerr << "Error on read from client" << std::endl;
        result = -1;
    }

    return result;
}

int32_t IPCSocketServerLinux::RegisterReceiveCB(std::function<int32_t(IPCClientMessage&,void*,int32_t)> receiveCB, void* usrData)
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
            _thread = std::thread(IPCSocketServerLinux::ThreadFunc, this);
        }
    } // socketFd
    
    return 0;
}

// Close
int32_t IPCSocketServerLinux::Close()
{
    std::cout << "IPCSocketServerLinux::Close" << std::endl;

    if ( _socketFd >= 0 )
    {
        RegisterReceiveCB(nullptr, nullptr);

        close(_socketFd);
        _socketFd = -1;
    }
    return 0;
}


void IPCSocketServerLinux::ThreadFunc(IPCSocketServerLinux* asyncServer)
{

    struct epoll_event events[IPCSocketClientLinux::MAX_EVENTS];

    while(asyncServer->_threadRunning)
    {
        int num_fds = epoll_wait(asyncServer->_epollFd, events, IPCSocketClientLinux::MAX_EVENTS, -1);
        for (int i = 0; i < num_fds; ++i) {
            if (events[i].events & EPOLLIN) {
                
                if ( asyncServer->_receiveCB != nullptr )
                {
                    IPCClientMessage clientMessage;
                    int32_t result = asyncServer->ReceiveFromClient(clientMessage);
                    asyncServer->_receiveCB(clientMessage, asyncServer->_usrData, result);
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

        std::cout << "IPCAsyncClientLinux::Thread Running" << std::endl;
    }

    std::cout << "IPCSocketServerLinux::Thread Finished" << std::endl;
}