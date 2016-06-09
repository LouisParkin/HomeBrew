#ifndef NETWORK_H
#define NETWORK_H

#include "socket_types.h"
#include "connection_states.h"
#include "network_typedefinitions.h"
#include "network_defs.h"

#include <QObject>

class NetworkImpl;

class Network : public QObject
{

  Q_OBJECT

public:
  static Network& getInstance()
  {
    static Network instance;
    return instance;
  }

  ~Network();

  SocketIdentifierType createClientSocket();
  SocketIdentifierType createServerSocket();

  bool serverStartListening(SocketIdentifierType serverSocketId, quint16 port);
  bool clientConnectToHost(SocketIdentifierType clientSocketId, QString url, quint16 port);

  void closeServerListening(SocketIdentifierType serverSocketId);
  void closeClientConnection(SocketIdentifierType clientSocketId);

  void deleteServerSocket(SocketIdentifierType serverSocketId);
  void deleteClientSocket(SocketIdentifierType clientSocketId);

  //  TODO:
  //  Network should provide send functionality, and keep callback function pointers to call when data is received.

  // IN PROGRESS:
  //  Network should open, close and handle all events on sockets like sending, receiving, and accepting incoming connections.

  // DONE
  //  Network Class should manage an infinite number of sockets.
  //  Network Class should manufacture the sockets and return a shared pointer to a socket (or connection handle).

  private slots:
  void newIncomingConnection();

private:

  explicit Network(QObject* parent = 0);
  Network(const Network&);
  Network& operator =(const Network&);

  QScopedPointer<NetworkImpl> _pimpl;
};

#endif // NETWORK_H
