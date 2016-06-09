#ifndef NETWORK_TYPEDEFINITIONS
#define NETWORK_TYPEDEFINITIONS

#include <QSharedPointer>
#include <QSet>
#include <QPair>
#include <QTcpSocket>
#include <QTcpServer>

// Type definitions
typedef QSharedPointer < QTcpSocket > SocketType;
typedef QSharedPointer < QTcpServer > TcpServerType;
typedef QPair <QString, quint16> ConnectionCriteriaType;
typedef quint64 SocketIdentifierType;
typedef QSet<SocketIdentifierType> SocketIdentifierSetType;
typedef QMap <SocketIdentifierType, SocketType> SocketsMapType;
typedef QMap <SocketIdentifierType, TcpServerType> TcpServerMapType;

#endif // NETWORK_TYPEDEFINITIONS

