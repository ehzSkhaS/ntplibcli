#include <QUdpSocket>
#include <QHostAddress>
#include "client.h"
#include "reply.h"
#include "replydata.h"

Client::Client(QObject *parent): QObject(parent)
{
    init(QHostAddress::Any, 0);
}

Client::Client(const QHostAddress &bindAddress, quint16 bindPort, QObject *parent): QObject(parent)
{
    init(bindAddress, bindPort);
}

void Client::init(const QHostAddress &bindAddress, quint16 bindPort)
{
    mSocket = new QUdpSocket(this);
    mSocket->bind(bindAddress, bindPort);

    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

Client::~Client(){ return; }

bool Client::sendRequest(const QHostAddress &address, quint16 port, bool maxver)
{
    // Is Client Connected?
    if(mSocket->state() != QAbstractSocket::BoundState)
        return false;

    // Initialize the NTP transmit packet
    simple_packet packet;
    memset(&packet, 0, sizeof (packet));
    packet.packetFlags.mode = ClientMode;
    packet.packetFlags.leap = UnsynchronizedWarning;
    if(maxver)
        packet.packetFlags.version = 4;
    else
        packet.packetFlags.version = 3;

    // This will be the packet signature for discard delayed ones
    packet.transmitTimestamp = timestamp::fromDateTime(QDateTime::currentDateTimeUtc());

    // Adds the value to a validity queue for later compare
    validityStamps.enqueue(packet.transmitTimestamp);

    // Ensure queue size ramains lesser or equal 10
    if(validityStamps.size() > 10)
        validityStamps.dequeue();

    // Send it
    qDebug() << mSocket->writeDatagram(reinterpret_cast<const char *>(&packet), sizeof(packet), address, port) << "bytes sent";

    return true;
}

void Client::readPendingDatagrams()
{
    while (mSocket->hasPendingDatagrams())
    {
        // Initialize the NTP receive packet
        full_packet packet;
        memset(&packet, 0, sizeof(packet));

        QHostAddress address;
        quint16 port;

        if(mSocket->readDatagram(reinterpret_cast<char *>(&packet), sizeof(packet), &address, &port) < sizeof(simple_packet))
          continue;

        QDateTime now = QDateTime::currentDateTime();

        // Prepare reply
        ReplyData *replyPrivate = new ReplyData();
        replyPrivate->fpacket = packet;
        replyPrivate->destinationTime = now;
        Reply reply(replyPrivate);

        // Notify
        Q_EMIT replyReceived(address, port, reply);
    }
}

