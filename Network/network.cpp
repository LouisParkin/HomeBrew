#include "network.h"

struct NetworkImpl {
  NetworkImpl(Network* this_)
  {
    _this = this_;
  }
  ~NetworkImpl() {}

  SocketIdentifierType createSocket(eSocketTypes type = eClientSocket);
  void closeSocketConnection(SocketIdentifierType socket, eSocketTypes type = eClientSocket);
  void deleteSocket(SocketIdentifierType socket, eSocketTypes type = eClientSocket);

  quint64 allocateNextId(SocketIdentifierSetType& socketsIds);
  quint64 findMaxId(const SocketIdentifierSetType& socketIds) const;

  bool serverStartListening(SocketIdentifierType serverSocketId, quint16 port);
  bool clientConnectToHost(SocketIdentifierType clientSocketId, QString url, quint16 port);


private:
  Network* _this;

  ServerSocketsMapType _serverSockets;
  ClientSocketsMapType _clientSockets;
  SocketIdentifierSetType _activeServerSockets;
  SocketIdentifierSetType _activeClientSockets;
};


Network::Network(QObject* parent):
  QObject(parent),
  _pimpl(new NetworkImpl(this))
{
}

quint64 NetworkImpl::findMaxId(const SocketIdentifierSetType& socketIds) const
{
  SocketIdentifierSetType::const_iterator i = socketIds.begin();
  quint64 maxId = LOWEST_SOCKET_ID;
  while (i != socketIds.end()) {
    quint64 currentId = *i;
    maxId = currentId > maxId ? currentId : maxId;
    ++i;
  }
  return maxId;
}

bool NetworkImpl::serverStartListening(SocketIdentifierType serverSocketId, quint16 port)
{
  // Check for valid socket.
  if (_serverSockets.contains(serverSocketId)) {
    // Get a raw pointer to the socket.
    QTcpServer* server = _serverSockets.value(serverSocketId).data();
    // If open, close
    if (server->isListening()) {
      server->close();
    }

    // Return bool that indicates listen action success or failure.
    return server->listen(QHostAddress::AnyIPv4, port);
  }
  else {
    // Not a valid socket, cannot be opened.
    return false;
  }
}

bool NetworkImpl::clientConnectToHost(SocketIdentifierType clientSocketId, QString url, quint16 port)
{
  // Check for valid socket.
  if (_clientSockets.contains(clientSocketId)) {
    // Get a raw pointer to the socket.
    QTcpSocket* client = _clientSockets.value(clientSocketId).data();
    if (client->isOpen()) {
      // If open, close it.
      client->close();
    }

    // Attempt a connection.
    client->connectToHost(url, port, QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);

    // Return whether or not connection is open.
    return client->isOpen();
  }
  else {
    // Not a valid socket, cannot be opened.
    return false;
  }
}

quint64 NetworkImpl::allocateNextId(SocketIdentifierSetType& socketIds)
{
  // There are no active sockets, return LOWEST_SOCKET_ID as next id
  if (socketIds.isEmpty()) {
    return LOWEST_SOCKET_ID;
  }

  // This simply finds the highest number allocated as an identifier.
  quint64 maxId = findMaxId(socketIds);

  // This stores the number of active sockets.
  quint64 size = socketIds.size();

  // If the highest id is not equal to the size, there is a hole somewhere, giving a chance for reuse.
  if (maxId != size) {
    for (quint64 i = LOWEST_SOCKET_ID; i <= size; ++i) {
      // Find the hole
      if (!socketIds.contains(i)) {
        // Plug the hole.
        socketIds.insert(i);
        return i;
      }
    }
  }
  else {
    // Allocate size + 1 as the new id and return it.
    socketIds.insert(size + LOWEST_SOCKET_ID);
    return size + LOWEST_SOCKET_ID;
  }
  // impossible.
  return 0;
}

Network::~Network()
{

}

///
/// \brief NetworkImpl::createSocket
/// \param type is server or client
/// \return an id to the socket.
///
SocketIdentifierType NetworkImpl::createSocket(eSocketTypes type)
{
  QSharedPointer<QTcpSocket> clSock;
  QSharedPointer<QTcpServer> srSock;
  switch (type) {
  case eClientSocket: {
    // Creates an instance within the shared pointer of type QTcpSocket
    clSock.create();

    // Adds it to the internal client socket pool with a unique ID allocated to find it again.
    _clientSockets.insert(allocateNextId(_activeClientSockets), clSock);

    // Returns the value of the unique identifier
    return _clientSockets.key(clSock);
  }
  case eServerSocket: {
    // Creates an instance within the shared pointer of type QTcpServer
    srSock.create();

    // Adds it to the internal server socket pool with a unique ID allocated to find it again.
    _serverSockets.insert(allocateNextId(_activeServerSockets), srSock);

    // Returns the value of the unique identifier
    return _serverSockets.key(srSock);
  }
  }
  // If type-checking of parameter 'type' fails, we return a very much invalid value.
  return quint64(0);
}

void NetworkImpl::closeSocketConnection(SocketIdentifierType socket, eSocketTypes type)
{
  switch (type) {
  case eClientSocket:
    if (_clientSockets.contains(socket)) {
      _clientSockets.value(socket)->close();
    }
    break;
  case eServerSocket:
    if (_serverSockets.contains(socket)) {
      _serverSockets.value(socket)->close();
    }
    break;
  }
}

void NetworkImpl::deleteSocket(SocketIdentifierType socket, eSocketTypes type)
{
  switch (type) {
  case eClientSocket:
    if (_clientSockets.contains(socket)) {
      ClientSocketType sock = _clientSockets.take(socket);
      if(sock->isOpen())
      {
        sock->close();
      }
      sock.clear();
    }
    break;
  case eServerSocket:
    if (_serverSockets.contains(socket)) {
      ServerSocketType sock = _serverSockets.take(socket);
      if(sock->isListening())
      {
        sock->close();
      }
      sock.clear();
    }
    break;
  }
}

///
/// \brief Network::createClientSocket
/// Creates a socket, adds it to the list of active sockets
/// \return an identifier to enable
/// callers to further use that socket.
///
SocketIdentifierType Network::createClientSocket()
{
  return _pimpl->createSocket(eClientSocket);
}

///
/// \brief Network::createServerSocket
/// Creates a socket, adds it to the list of active sockets
/// \return an identifier to enable
/// callers to further use that socket.
///
SocketIdentifierType Network::createServerSocket()
{
  return _pimpl->createSocket(eServerSocket);
}

void Network::deleteServerSocket(SocketIdentifierType serverSocketId)
{
  _pimpl->deleteSocket(serverSocketId, eServerSocket);
}

void Network::deleteClientSocket(SocketIdentifierType clientSocketId)
{
  _pimpl->deleteSocket(clientSocketId);
}

///
/// \brief Network::serverStartListening
/// Instructs a socket to start listening on a given port
/// \param serverSocketId
/// \param port
/// \return bool to indicate success or failure.
///
bool Network::serverStartListening(SocketIdentifierType serverSocketId, quint16 port)
{
  return _pimpl->serverStartListening(serverSocketId, port);
}

void Network::closeServerListening(SocketIdentifierType serverSocketId)
{
  _pimpl->closeSocketConnection(serverSocketId, eServerSocket);
}

///
/// \brief Network::clientConnectToHost
/// Instructs a socket to connect to a host url/port
/// \param clientSocketId
/// \param url
/// \param port
/// \return bool to indicate success or failure.
///
bool Network::clientConnectToHost(SocketIdentifierType clientSocketId, QString url, quint16 port)
{
  return _pimpl->clientConnectToHost(clientSocketId, url, port);
}

void Network::closeClientConnection(SocketIdentifierType clientSocketId)
{
  _pimpl->closeSocketConnection(clientSocketId);
}
