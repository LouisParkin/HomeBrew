#ifndef NETWORK_TYPEDEFINITIONS
#define NETWORK_TYPEDEFINITIONS

#include <QSharedPointer>
#include <QSet>
#include <QPair>
#include <QTcpSocket>
#include <QTcpServer>

// Type definitions
typedef QSharedPointer < QTcpSocket > ClientSocketType;
typedef QSharedPointer < QTcpServer > ServerSocketType;
typedef QPair <QString, quint16> ConnectionCriteriaType;
typedef quint64 SocketIdentifierType;
typedef QSet<SocketIdentifierType> SocketIdentifierSetType;
typedef QMap <SocketIdentifierType, ClientSocketType> ClientSocketsMapType;
typedef QMap <SocketIdentifierType, ServerSocketType> ServerSocketsMapType;

#endif // NETWORK_TYPEDEFINITIONS

