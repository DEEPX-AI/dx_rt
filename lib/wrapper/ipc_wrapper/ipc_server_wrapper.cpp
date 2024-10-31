


#include "../../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"
#include "socket_sync/ipc_server_linux.h"
#include "socket/ipc_socket_server_linux.h"
#include "message_queue/ipc_mq_server_linux.h"


using namespace dxrt;


IPCServerWrapper::IPCServerWrapper(IPC_TYPE type)
{
    if ( type == IPC_TYPE::SOCKET_CB ) 
    {
        _ipcServer = std::make_shared<IPCSocketServerLinux>();
    }
    else if ( type == IPC_TYPE::MESSAE_QUEUE )
    {
        _ipcServer = std::make_shared<IPCMessageQueueServerLinux>();
    }
    else if ( type == IPC_TYPE::SOCKET_SYNC )
    {
        _ipcServer = std::make_shared<IPCServerLinux>();
    }
    else 
    {
        LOG_DXRT_I_ERR("[ERROR] IPCClientWrapper No implementation");
    }
    
    
    //_ipcServer = std::make_shared<IPCServerWindows>();
}


IPCServerWrapper::IPCServerWrapper(uint64_t fd, IPC_TYPE type)
{
    // implementation for linux
    if ( type == IPC_TYPE::SOCKET_CB )
    {
        _ipcServer = std::make_shared<IPCSocketServerLinux>(fd);
    }
    else if ( type == IPC_TYPE::SOCKET_SYNC )
    {
        _ipcServer = std::make_shared<IPCServerLinux>(fd);
        //_ipcClient = std::make_shared<IPCClientWindows>(fd);
    }

}

IPCServerWrapper::~IPCServerWrapper()
{

}

// Intitialize IPC Server
// return error code
int32_t IPCServerWrapper::Initialize()
{

    return _ipcServer->Initialize();
}

// listen
int32_t IPCServerWrapper::Listen()
{

    return _ipcServer->Listen();
}

int32_t IPCServerWrapper::Select(int64_t& connectedFd)
{
    return _ipcServer->Select(connectedFd);
}

// ReciveFromClient
int32_t IPCServerWrapper::ReceiveFromClient(IPCClientMessage& clientMessage)
{

    return _ipcServer->ReceiveFromClient(clientMessage);
}

// SendToClient
int32_t IPCServerWrapper::SendToClient(IPCServerMessage& serverMessage)
{

    return _ipcServer->SendToClient(serverMessage);
}

// register receive message callback function
int32_t IPCServerWrapper::RegisterReceiveCB(std::function<int32_t(IPCClientMessage&,void*,int32_t)> receiveCB, void* usrData)
{
    return _ipcServer->RegisterReceiveCB(receiveCB, usrData);
}

int32_t IPCServerWrapper::RemoveClient(long msgType) // Only for Message Queue (POSIX)
{

    return _ipcServer->RemoveClient(msgType);
}

// Close
int32_t IPCServerWrapper::Close()
{

    return _ipcServer->Close();
}