
#include "Network.h"

DirectSocket::DirectSocket(std::string address, unsigned short port, unsigned int inputCapacity, unsigned int outputCapacity) : m_address(address), m_port(port), currentSendAmount(0),
    m_initialized(false), m_dataWritten(false), m_connected(false), m_inputSize(inputCapacity), m_outputSize(outputCapacity), outputBuffer(), inputBuffer()
{
    ZeroMemory(&m_connInfo, sizeof(sockaddr_in));
    m_fd = INVALID_SOCKET;
}

DirectSocket::~DirectSocket()
{
    closesocket(m_fd);
}

bool DirectSocket::Update()
{
    if(!IsConnected())
        return false;

    WriteOutputBuffer();
    UpdateInputBuffer();
    return IsConnected();
}

void DirectSocket::Disconnect()
{
    m_connected = false;
    if(m_fd == INVALID_SOCKET)
        return;

    OnDisconnect();
    memset(&m_connInfo, 0, sizeof(sockaddr_in));
    CancelIo((HANDLE)m_fd);
    shutdown(m_fd, SD_BOTH);
    closesocket(m_fd);
    m_fd = INVALID_SOCKET;
}

bool DirectSocket::Initialize()
{
    if(m_initialized && IsConnected())
        return true;
    else if(m_initialized == false)
    {
        m_initialized = true;
        outputBuffer.Reallocate(m_inputSize);
        inputBuffer.Reallocate(m_outputSize);
    }
    return Connect();
}

void DirectSocket::AppendData(char *data, size_t len)
{
    assert(len);
    outputBuffer.Write(data, len);
    m_dataWritten = true;
}

bool DirectSocket::Connect()
{
    if(m_fd != INVALID_SOCKET)
        return false;

    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char portaddr[5];
    sprintf_s(portaddr, 5, "%u", m_port);

    PADDRINFOA addrInfo;
    if(getaddrinfo(m_address.c_str(), portaddr, &hints, &addrInfo))
        return false;

    bool result = false;
    struct addrinfo *ptr = NULL;
    for(struct addrinfo *ptr = addrInfo; ptr != NULL; ptr = ptr->ai_next)
    {
        if(ptr->ai_family != AF_INET)
            continue;
        memcpy(&m_connInfo, ((struct sockaddr_in *)ptr->ai_addr), sizeof(sockaddr_in));
        result = true;
        break;
    }

    freeaddrinfo(addrInfo);
    if(result == false)
        return false;

    /* open socket */
    m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_fd == INVALID_SOCKET)
        return false;

    /* try to connect */
    if(connect(m_fd, (const sockaddr*)&m_connInfo, sizeof(sockaddr_in)) != 0)
        return false;

    /* set to non blocking mode */
    u_long arg = 1;
    ioctlsocket(m_fd, FIONBIO, &arg);

    /// Call our onconnect logic
    OnConnect();
    return (m_connected = true);
}

void DirectSocket::UpdateInputBuffer()
{
    if(!IsConnected())
        return;

    int numRecv = recv(m_fd, inputBuffer.GetBuffer(), inputBuffer.GetSpace(), 0);
    if(numRecv == SOCKET_ERROR)
    {
        switch(GetLastError())
        {
        case EAGAIN:
        case EWOULDBLOCK:
            break;
        default:
            {
                int32 wsaError = WSAGetLastError();
                switch(wsaError)
                {
                case WSA_IO_PENDING:
                case WSAEWOULDBLOCK:
                    break;
                default:
                    OnError(wsaError);
                    break;
                }
            }break;
        }
        return;
    }
    else if(numRecv == 0)
    {
        Disconnect();
        return;
    }

    inputBuffer.Write(numRecv);
    OnRecvData();
}

void DirectSocket::OnError(int error)
{
    Disconnect();
}

void DirectSocket::WriteOutputBuffer()
{
    if(!IsConnected() || !m_dataWritten || outputBuffer.GetSize() == 0)
        return;
    m_dataWritten = false;

    if(currentSendAmount == 0)
    {
        currentSendAmount = outputBuffer.GetSize();
        if(send(m_fd, (const char*)&currentSendAmount, sizeof(uint32), 0) != sizeof(uint32))
        {
            // Try again later
            currentSendAmount = 0;
            return;
        }
    }

    int numSent = send(m_fd, outputBuffer.GetBuffer(), currentSendAmount, 0);
    if(numSent == SOCKET_ERROR)
    {
        switch(GetLastError())
        {
        case EAGAIN:
        case EWOULDBLOCK:
            break;
        default:
            {
                int32 wsaError = WSAGetLastError();
                switch(wsaError)
                {
                case WSA_IO_PENDING:
                case WSAEWOULDBLOCK:
                    break;
                default:
                    OnError(wsaError);
                    return;
                }
            }break;
        }
        return;
    }

    currentSendAmount -= numSent;
    outputBuffer.Remove(numSent);
}
