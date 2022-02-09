#ifndef REPLY_H
#define REPLY_H

#include <QDateTime>
#include <QSharedDataPointer>
#include "config.h"
#include "options.h"

class ReplyData;

class NTPLIBCLI_EXPORT Reply
{
public:
    Reply();

    Reply(const Reply &alt);

    ~Reply();

    Reply &operator=(const Reply &alt);

    /**
     * @returns                         Leap indicator
     */
    LeapIndicator leapIndicator() const;

    /**
     * @returns                         NTP version number
     */
    quint8 versionNumber() const;

    /**
     * @returns                         Ntp mode
     */
    Mode mode() const;

    /**
     * @returns                         Stratum
     */
    quint8 stratum() const;

    /**
     * @returns                         Maximum interval between successive messages, in seconds
     */
    qreal pollInterval() const;

    /**
     * @returns                         Clock precision, in seconds
     */
    qreal precision() const;

    /**
     * @returns                         Total round trip delay to the primary reference source
     */
    qreal rootDelay() const;

    /**
     *  @returns                        Maximun amount of variance between the NTP server and its known time-source
     */
    qreal rootDispersion() const;

    /**
     * @returns                         Time when the system clock was last set or corrected
     */
    QDateTime referenceTime() const;

    /**
     * @returns                         Time at the client when the request departed for the server
     */
    QDateTime originTime() const;

    /**
     * @returns                         Time at the server when the request arrived from the client
     */
    QDateTime receiveTime() const;

    /**
     * @returns                         Time at the server when the response left for the client
     */
    QDateTime transmitTime() const;

    /**
     * @returns                         Time at the client when the reply arrived from the server
     */
    QDateTime destinationTime() const;

    /**
     * @returns                         Round trip delay, in milliseconds
     */
    qint64 roundTripDelay() const;

    /**
     * @returns                         Local clock offset, in milliseconds
     */
    qint64 localClockOffset() const;

    /**
     * @returns                         Whether this ntp reply is null
     */
    bool isNull() const;

protected:
    friend class Client;                // Calls protected constructor

    Reply(ReplyData *rd);

private:
    QSharedDataPointer<ReplyData> pd;

};

#endif // REPLY_H
