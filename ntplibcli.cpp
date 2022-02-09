#include <QtDebug>
#include <QHostInfo>
#include <QUdpSocket>
#include <QAbstractSocket>
#include "ntplibcli.h"

NtpLibCli::NtpLibCli(QObject * parent) : QObject(parent)
{
    m_client = new Client(this);
    connect(m_client, SIGNAL(replyReceived(QHostAddress,quint16,Reply)), this, SLOT(onReplyReceived(QHostAddress,quint16,Reply)));
    connect(m_client, SIGNAL(destroyed()), this, SLOT(bigCrunch()));    

    response = new QTimer(this);
    response->setInterval(1000);
    response->setSingleShot(true);
    connect(response, SIGNAL(timeout()), SLOT(beginSync()));

    reconnect = new QTimer(this);
    reconnect->setInterval(tryinterval*1000);
    reconnect->setSingleShot(true);
    connect(reconnect, SIGNAL(timeout()), SLOT(reconnectAttempt()));
}

/** Check this destructor validity */
NtpLibCli::~NtpLibCli(){ return; }

void NtpLibCli::beginSync()
{
    qDebug() << "Connecting...";
    emit connecting(address);

    QHostInfo info = QHostInfo::fromName(address);
    QHostAddress addr;

    if(!info.addresses().isEmpty())
    {
        addr = info.addresses().at(0);
        qDebug() << "Address Resolved:" << addr.toString();
        emit addrResolved(addr.toString());
        if(!m_client->sendRequest(addr, port, maxver))
            reconnect->start();
        else
        {
            trycompare = 0;
            response->start();
            qDebug() << "Waiting 1 second for response";
            emit waitingResponse();
        }
    }else
    {
        qDebug() << "Can't Resolve Address";
        emit connectionFailed();
    }
}

void NtpLibCli::setNTPServer(QString address, quint16 port, bool maxver)
{
    qDebug() << "Destroying the universe...";
    emit destroying();
    m_client->deleteLater();

    this->address = address;
    this->port = port;
    this->maxver = maxver;
}

void NtpLibCli::setTryAmount(quint8 tryamount)
{
    if(1 <= tryamount && tryamount <= 99)
        this->tryamount = tryamount;
}

void NtpLibCli::setSyncInterval(quint16 synctime)
{
    if(synctime <= 1024)
        this->synctime = synctime;
}

void NtpLibCli::onReplyReceived(QHostAddress host, quint16 port, Reply reply)
{    
    response->stop();
    qDebug() << "Response fetched";
    emit responseFetched();

    while(m_client->waitingPackets())
    {
        /* Get the difference in mseconds between
           the validity queue head transmit timestamp
           and the reply packet origin timestamp */
        qint64 diffStamps = timestamp::toDateTime(m_client->getHeadPacket()).msecsTo(reply.originTime());

        if(diffStamps == 0)             // 0 difference, then are the same as expecting
        {
            // Delete checked timestamp
            m_client->delHeadPacket();

            QDateTime rt = reply.receiveTime().toUTC();
            qint64 offset = reply.localClockOffset();
            qint64 delay = reply.roundTripDelay();

            rt = rt.addMSecs(delay);
            if(offset >= -500 && offset <= 500)
                rt = rt.addMSecs(offset/2);

            GetSystemTime(&st);

            st.wYear = rt.date().year();
            st.wMonth = rt.date().month();
            st.wDay = rt.date().day();
            st.wHour = rt.time().hour();
            st.wMinute = rt.time().minute();
            st.wSecond = rt.time().second();
            st.wMilliseconds = rt.time().msec();

            /** Check Error on Set Time */
            if(SetSystemTime(&st))
            {
                qDebug() << "Time Adjusted";
                emit timeAdjusted();
            }

            qDebug() << "Stratum:" << reply.stratum();
            qDebug() << "Delay:" << delay;
            qDebug() << "Offset:" << offset;
            qDebug() << "Receive Time:" << rt;

            if(syncstep == NoSync)
            {
                syncstep = Sync;
                beginSync();
                break;
            }else if(syncstep == Sync)
            {
                syncstep = Resync;
                qDebug() << "wait" << synctime << "secs";
                emit waitingSync(synctime);
                // Check this timer singleshoot
                QTimer::singleShot(synctime*1000,this,SLOT(resetSync()));
            }
        }else if(diffStamps > 0)        // there are lost packets in replys
        {
            m_client->delHeadPacket();
            qDebug() << "Lost packet";
            emit packetLost();
        }else                           // the replys are behind schedule
        {
            qDebug() << "Delayed reply";
            emit delayedReply();
            break;
        }
    }
}

void NtpLibCli::resetSync()
{
    syncstep = NoSync;
    beginSync();
}

void NtpLibCli::reconnectAttempt()
{    
    if(trycompare < tryamount)
    {
        qDebug() << "Reconnect attempt:" << ++trycompare;
        emit connectionAttempt();
        beginSync();
    }else
    {
        qDebug() << "Can't connect to NTP server";
        emit connectionFailed();
    }
}

void NtpLibCli::bigCrunch()
{
    qDebug() << "Making the universe...";
    emit makingTheUniverse();
    response->stop();
    response->setInterval(1000);
    reconnect->stop();
    reconnect->setInterval(tryinterval*1000);
    syncstep = NoSync;
    trycompare = 0;

    m_client = new Client(this);
    connect(m_client, SIGNAL(replyReceived(QHostAddress,quint16,Reply)), this, SLOT(onReplyReceived(QHostAddress,quint16,Reply)));
    connect(m_client, SIGNAL(destroyed()), this, SLOT(bigCrunch()));
    qDebug() << "Spawning the light";
    emit spawningTheLight();
    beginSync();
}
