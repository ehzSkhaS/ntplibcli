#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QQueue>
#include "config.h"
#include "options.h"
#include "packet.h"

class QUdpSocket;
class QHostAddress;

class Reply;

class NTPLIBCLI_EXPORT Client: public QObject
{
    Q_OBJECT;
public:
    /**
     * @brief Default constructor
     *
     * @param parent                    Parent of this object
     */
    Client(QObject *parent = NULL);

    /**
     * @brief Constructor
     *
     * @param bindAddress               Address to bind udp socket to
     * @param bindPort                  Port to bind udp socket to
     * @param parent                    Parent of this object
     */
    Client(const QHostAddress &bindAddress, quint16 bindPort, QObject *parent = NULL);

    /**
     * @brief Virtual destructor
     */
    virtual ~Client();

    /**
     * @brief Sends NTP request
     *
     * @param address                   NTP server address
     * @param port                      NTP server port
     * @param maximize version          NTP version (True: 4, False: 3)
     * @returns                         Whether the NTP request was successfully sent
     */
    bool sendRequest(const QHostAddress &address, quint16 port, bool maxver = true);

    bool waitingPackets() { return !validityStamps.isEmpty(); }

    timestamp getHeadPacket() { return validityStamps.head(); }

    timestamp delHeadPacket() { return validityStamps.dequeue(); }

Q_SIGNALS:
    /**
    * @brief This signal is emitted whenever NTP reply is received
    *
    * @param address                    Address of the server that sent this reply
    * @param port                       Port of the server that sent this reply
    * @param reply                      NTP reply
    */
    void replyReceived(const QHostAddress &address, quint16 port, const Reply &reply);

private Q_SLOTS:
    void readPendingDatagrams();

private:
    QUdpSocket * mSocket;
    QQueue <timestamp> validityStamps;

    void init(const QHostAddress &bindAddress, quint16 bindPort);    
};

#endif // CLIENT_H
