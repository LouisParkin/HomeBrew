#include "network.h"

struct NetworkImpl {
  NetworkImpl(Network* this_)
  {
    _this = this_;
  }
  ~NetworkImpl() {}

  Network::SocketIdentifierType createSocket(eSocketTypes type = eClientSocket);
  quint64 allocateNextId(Network::SocketIdentifierSetType& socketsIds);
  quint64 findMaxId(const Network::SocketIdentifierSetType& socketIds) const;

  bool serverStartListening(Network::SocketIdentifierType serverSocketId, quint16 port);
  bool clientConnectToHost(Network::SocketIdentifierType clientSocketId, QString ip, quint16 port);

private:
  Network* _this;

  Network::ServerSocketsMapType _serverSockets;
  Network::ClientSocketsMapType _clientSockets;
  Network::SocketIdentifierSetType _activeServerSockets;
  Network::SocketIdentifierSetType _activeClientSockets;
};


Network::Network(QObject* parent):
  QObject(parent),
  _pimpl(new NetworkImpl(this))
{
}

quint64 NetworkImpl::findMaxId(const Network::SocketIdentifierSetType& socketIds) const
{
  Network::SocketIdentifierSetType::const_iterator i = socketIds.begin();
  quint64 maxId = 1;
  while (i != socketIds.end()) {
    quint64 currentId = *i;
    maxId = currentId > maxId ? currentId : maxId;
    ++i;
  }
  return maxId;
}

bool NetworkImpl::serverStartListening(Network::SocketIdentifierType serverSocketId, quint16 port)
{
  if (_serverSockets.contains(serverSocketId)) {
    QTcpServer* server = _serverSockets.value(serverSocketId).data();
    if (!server->isListening()) {
      return server->listen(QHostAddress::AnyIPv4, port);
    }
    else {
      return true;
    }
  }
  else {
    return false;
  }
}

bool NetworkImpl::clientConnectToHost(Network::SocketIdentifierType clientSocketId, QString ip, quint16 port)
{
  if (_clientSockets.contains(clientSocketId)) {
    QTcpSocket* client = _clientSockets.value(clientSocketId).data();
    if (client->isOpen()) {
      client->close();
    }

    client->connectToHost(ip, port, QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);
    return client->isOpen();
  }
  else {
    return false;
  }
}

quint64 NetworkImpl::allocateNextId(Network::SocketIdentifierSetType& socketIds)
{
  quint64 minId = 1;
  if (socketIds.isEmpty()) {
    return minId;
  }

  quint64 maxId = findMaxId(socketIds);
  quint64 size = socketIds.size();

  if (maxId != size) {
    for (quint64 i = 1; i <= size; ++i) {
      if (!socketIds.contains(i)) {
        socketIds.insert(i);
        return i;
      }
    }
  }
  else {
    socketIds.insert(size + 1);
    return size + 1;
  }
  return 0;
}

Network::~Network()
{

}

Network::SocketIdentifierType NetworkImpl::createSocket(eSocketTypes type)
{
  QSharedPointer<QTcpSocket> clSock;
  QSharedPointer<QTcpServer> srSock;
  switch (type) {
  case eClientSocket: {
    clSock.create();
    _clientSockets.insert(allocateNextId(_activeClientSockets), clSock);
    return _clientSockets.key(clSock);
  }
  case eServerSocket: {
    srSock.create();
    _serverSockets.insert(allocateNextId(_activeServerSockets), srSock);
    return _serverSockets.key(srSock);
  }
  }
  return quint64(0);
}

Network::SocketIdentifierType Network::createClientSocket()
{
  return _pimpl->createSocket(eClientSocket);
}

Network::SocketIdentifierType Network::createServerSocket()
{
  return _pimpl->createSocket(eServerSocket);
}

bool Network::serverStartListening(Network::SocketIdentifierType serverSocketId, quint16 port)
{
  return _pimpl->serverStartListening(serverSocketId, port);
}

bool Network::clientConnectToHost(Network::SocketIdentifierType clientSocketId, QString ip, quint16 port)
{
  return _pimpl->clientConnectToHost(clientSocketId, ip, port);
}
