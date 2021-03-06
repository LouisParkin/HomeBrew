#ifndef NETWORK_H
#define NETWORK_H

#include "socket_types.h"
#include "connection_states.h"

#include <QObject>
#include <QSharedPointer>
#include <QSet>
#include <QPair>
#include <QTcpSocket>
#include <QTcpServer>

class NetworkImpl;

class Network : public QObject
{

  Q_OBJECT

public:
  // Type definitions
  typedef QSharedPointer < QTcpSocket > ClientSocketType;
  typedef QSharedPointer < QTcpServer > ServerSocketType;
  typedef QPair <QString, quint16> ConnectionCriteriaType;
  typedef quint64 SocketIdentifierType;
  typedef QSet<SocketIdentifierType> SocketIdentifierSetType;
  typedef QMap <SocketIdentifierType, ClientSocketType> ClientSocketsMapType;
  typedef QMap <SocketIdentifierType, ServerSocketType> ServerSocketsMapType;


  ~Network();

  SocketIdentifierType createClientSocket();
  SocketIdentifierType createServerSocket();

  bool serverStartListening(SocketIdentifierType serverSocketId, quint16 port);
  bool clientConnectToHost(SocketIdentifierType clientSocketId, QString ip, quint16 port);

  // Network Class should manage an infinite number of sockets.
  // Network Class should manufacture the sockets and return a shared pointer to a socket (or connection handle).
  // Network should provide send functionality, and keep callback function pointers to call when data is received.
  // Network should open, close and handle all events on sockets.

private:

  explicit Network(QObject* parent = 0);
  Network(const Network&);
  Network& operator =(const Network&);

  QScopedPointer<NetworkImpl> _pimpl;
};

#endif // NETWORK_H
