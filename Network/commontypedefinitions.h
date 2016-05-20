#ifndef COMMONTYPEDEFINITIONS
#define COMMONTYPEDEFINITIONS

#include <QSharedPointer>
#include <QSet>
#include <QPair>
#include <QTcpSocket>
#include <QTcpServer>

typedef QSharedPointer < QTcpSocket > ClientSocketType;
typedef QSharedPointer < QTcpServer > ServerSocketType;
typedef QPair <QString, quint16> ConnectionCriteriaType;
typedef quint64 SocketIdentifierType;
typedef QSet<SocketIdentifierType> SocketIdentifierSetType;
typedef QMap <SocketIdentifierType, ClientSocketType> ClientSocketsMapType;
typedef QMap <SocketIdentifierType, ServerSocketType> ServerSocketsMapType;

#endif // COMMONTYPEDEFINITIONS

