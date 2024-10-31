
#include "ipc_server_linux.h"
#include "ipc_client_linux.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
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
    unlink(IPCClientLinux::SOCKET_NAME);
    //LOG_DXRT_DBG << "cleanup\n";
    LOG_DXRT_DBG << "cleanup" << std::endl;
    exit(0);  // 정상 종료
}

static void setup_signal_handlers() {
    std::signal(SIGINT, cleanup);
    std::signal(SIGTERM, cleanup);
    std::signal(SIGKILL, cleanup);
    std::signal(SIGABRT, cleanup);
    std::signal(SIGPIPE, SIG_IGN);

}

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
        unlink(IPCClientLinux::SOCKET_NAME);
    }
 private:
    int _sock_fd;
};

IPCServerLinux::IPCServerLinux()
: _socketFd(-1)
{
    LOG_DXRT_DBG << "IPCServerLinux::Constructor" << std::endl;
}

IPCServerLinux::IPCServerLinux(uint64_t fd)
{
    _socketFd = static_cast<int>(fd);
}

IPCServerLinux::~IPCServerLinux()
{
    LOG_DXRT_DBG << "IPCServerLinux::Destructor" << std::endl;
}

// Intitialize IPC Server
// return error code
int32_t IPCServerLinux::Initialize()
{
    LOG_DXRT_DBG << "IPCServerLinux::Initialize fd=" << _socketFd << std::endl;

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
            strcpy(addr.sun_path, IPCClientLinux::SOCKET_NAME);
            unlink(addr.sun_path);

            int err = bind(_socketFd, (struct sockaddr*)&addr, sizeof(addr));
            if (err < 0)
            {
                LOG_DXRT_DBG << "bind ERROR" << errno << std::endl;
                close(_socketFd);
                _socketFd = -1;
                return -1;
            }

            err = listen(_socketFd, 0);
            if (err < 0)
            {
                LOG_DXRT_DBG << "listen ERROR" << errno << std::endl;
                close(_socketFd);
                _socketFd = -1;
                return -1;
            }

            LOG_DXRT_DBG << "socket listen fd=" << _socketFd << std::endl;

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
                LOG_DXRT_DBG << "SOCKET ACCEPTED connect-socket-fd=" << connect_socket_fd << std::endl;
            }

            usleep(1000); // 1ms

        }
    #endif // 

    } // socket < 0

    return result;
}

// listen
int32_t IPCServerLinux::Listen()
{
    LOG_DXRT_DBG << "IPCServerLinux::Listen" << std::endl;

    return 0;
}

int32_t IPCServerLinux::Select(int64_t& connectedFd)
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
            FD_SET(connect_socket_fd, &_readFds);
            //if (_max_sock < connect_socket_fd) _max_sock = connect_socket_fd;
            //LOG_DXRT_DBG << "SOCKET ACCEPTED connect-socket-fd=" << connect_socket_fd << std::endl;

            // temp
            /*
            IPCClientMessage clientMessage;
            int size = read(connect_socket_fd, &clientMessage, sizeof(clientMessage));
            LOG_DXRT_DBG << "Received message size=" << size << std::endl;
            LOG_DXRT_DBG << clientMessage << std::endl;

            IPCServerMessage serverMessage;
            serverMessage.code = RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION_INPUT_AND_OUTPUT;
            size = write(connect_socket_fd, &serverMessage, sizeof(serverMessage));
            LOG_DXRT_DBG << "Send message size=" << size << std::endl;
            LOG_DXRT_DBG << serverMessage << std::endl;
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
int32_t IPCServerLinux::ReceiveFromClient(IPCClientMessage& clientMessage)
{
    LOG_DXRT_DBG << "IPCServerLinux::ReceiveFromClient" << std::endl;
    int32_t result = 0;

    try
    {
        int size = read(_socketFd, &clientMessage, sizeof(clientMessage));
        LOG_DXRT_DBG << "Received message size=" << size << std::endl;
        if ( size <= 0 ) 
        {
            result = -1;
        }
        //LOG_DXRT_DBG << clientMessage << std::endl;
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
int32_t IPCServerLinux::SendToClient(IPCServerMessage& serverMessage)
{
    LOG_DXRT_DBG << "IPCServerLinux::SendToClient" << std::endl;

    int32_t result = 0;
    try
    {
        int size = write(_socketFd, &serverMessage, sizeof(serverMessage));
        LOG_DXRT_DBG << "Send message size=" << size << std::endl;
        if ( size <= 0 ) 
        {
            result = -1;
        }
        //LOG_DXRT_DBG << serverMessage << std::endl;
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

int32_t IPCServerLinux::RegisterReceiveCB(std::function<int32_t(IPCClientMessage&,void*,int32_t)> receiveCB, void* usrData)
{

    (void)receiveCB;
    (void)usrData;
    
    return 0;
}

// Close
int32_t IPCServerLinux::Close()
{
    LOG_DXRT_DBG << "IPCServerLinux::Close" << std::endl;

    if ( _socketFd >= 0 )
    {
        close(_socketFd);
        _socketFd = -1;
    }
    return 0;
}