#ifndef NTPLIBCLI_H
#define NTPLIBCLI_H

#include <QObject>
#include <QTimer>
#include <QHostAddress>
#include <sysinfoapi.h>
#include "config.h"
#include "options.h"
#include "client.h"
#include "reply.h"

class NTPLIBCLI_EXPORT NtpLibCli: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Default constructor
     *
     * @param parent                    Parent of this object
     */
    explicit NtpLibCli(QObject *parent = 0);

    /**
     * @brief Destructor
     */
    ~NtpLibCli();    

    /**
     * @brief Config the NTP server connection
     *
     * @param address                   NTP server address
     * @param port                      NTP server port
     * @param maxver                    NTP server version (true: 4, faalse: 3)
     */
    void setNTPServer(QString address, quint16 port, bool maxver);

    /**
     * @brief Get NTP server address
     *
     * @returns                         The configured NTP server address
     */
    QString getAddress(){ return address; }

    /**
     * @brief Get NTP server port
     *
     * @returns                         The configured NTP server port
     */
    quint16 getPort(){ return port; }

    /**
     * @brief Get NTP server max version
     *
     * @returns                         The configured NTP server max version
     */
    bool getVersion(){ return maxver; }

    /**
     * @brief Set number of connections attempts
     *
     * @param tryamount                 Connection attempts amount
     */
    void setTryAmount(quint8 tryamount);

    /**
     * @brief Get number of connections attempts
     *
     * @returns                         Connection attempts amount
     */
    quint8 getTryAmount(){ return tryamount; }

    /**
     * @brief Set the interval in seconds for client sync request
     *
     * @param synctime                  Number of seconds for sync
     */
    void setSyncInterval(quint16 synctime);

    /**
     * @brief Get the interval in seconds for client sync request
     *
     * @returns                         Number of seconds for sync
     */
    quint16 getSyncInterval(){ return synctime; }

public Q_SLOTS:
    /**
     * @brief Sync process initial method
     */
    void beginSync();    

Q_SIGNALS:
    void connecting(QString address);

    void addrResolved(QString address);

    void waitingResponse();

    void destroying();

    void responseFetched();

    void timeAdjusted();

    void waitingSync(quint16 synctime);

    void packetLost();

    void delayedReply();

    void makingTheUniverse();

    void spawningTheLight();

    /**
     * @brief Connection fail,
     * when address can't be resolved
     */
    void connectionFailed();    

    /**
     * @brief Connection attempt is emitted,
     * when a new sync process begin
     */
    void connectionAttempt();

private Q_SLOTS:
    /**
     * @brief Client Signal(replyReceived) --> NtpLibCli Slot(onReplyReceived)
     * @param host                      NTP server address
     * @param port                      NTP server port
     * @param reply                     NTP server reply
     */
    void onReplyReceived(QHostAddress host, quint16 port, Reply reply);    

    /**
     * @brief Singleshot timer is started on "Resync" step reached,
     * for an amount of `synctime` seconds,
     * once over: resetSync() is called.
     * On execution set the `syncstep` to "NoSync" and calls beginSync()
     */
    void resetSync();

    /**
     * @brief Singleshot timer is started on client sendRequest() false response,
     * once over: reconnectAttempt() is called.
     * On execution check tht `tryamount` and calls beginSync()
     */
    void reconnectAttempt();

    /**
     * @brief It destroys all the known universe and it makes a new one
     */
    void bigCrunch();

private:
    QTimer * response;                  // Timer for waiting a reply packet
    QTimer * reconnect;                 // Timer for waiting on reconnection attempt
    SYSTEMTIME st;                      // Windows time structure
    Client * m_client;                  // NTP client pointer
    QString address = "pool.ntp.org";   // NTP server address
    quint16 port = 123;                 // NTP server port
    bool maxver = true;                 // NTP packet send and request version
    unsigned char syncstep = NoSync;    // Local clock sync step
    quint16 synctime = 16;              // Resync interval in seconds
    quint8 tryamount = 5;               // Number of connections attempts
    quint8 trycompare = 0;              // Compare variable against `tryamount`
    quint8 tryinterval = 5;             // Interval between connection attempts in seconds    
};

#endif // NTPLIBCLI_H
