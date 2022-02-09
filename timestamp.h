#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <QtEndian>
#include <QDateTime>
#include "config.h"

// Number of seconds passed
// since: January 1st 1900 (NTP Era 0)
//   to: January 1st 1970 (1st Day Unix)
namespace epoch{ namespace{ const qint64 january_1st_1970_unix = -2208988800000ll; }}

#pragma pack(push, 1)
struct timestamp
{
    quint32 seconds;    // 32 bits: Seconds passed since Jan 1 1900, big-endian
    quint32 fraction;   // 32 bits: Fraction of second, big-endian

   /**
    *    @param dateTime        Qt datetime
    *    @returns               Ntp time for given Qt datetime
    */
    static inline timestamp fromDateTime(const QDateTime &dateTime);

   /**
    *    @param ntpTime         Ntp timestamp
    *    @returns               Qt datetime for given Ntp timestamp
    */
    static inline QDateTime toDateTime(const timestamp &ntpTime);
};
#pragma pack(pop)

timestamp timestamp::fromDateTime(const QDateTime &dateTime)
{
    // Convert given time to number of milliseconds passed since Jan 1 1900
    qint64 ntpMSecs = dateTime.toMSecsSinceEpoch() - epoch::january_1st_1970_unix;

    // Note that NTP epoch information is lost here. Assume 1900-2036 NTP epoch
    quint32 seconds = ntpMSecs / 1000;
    quint32 fraction = 0x100000000ll * (ntpMSecs % 1000) / 1000;

    // Convert to big-endian
    timestamp result;
    result.seconds = qToBigEndian(seconds);
    result.fraction = qToBigEndian(fraction);
    return result;
}

QDateTime timestamp::toDateTime(const timestamp &ntpTime)
{
    // Convert to local-endian
    quint32 seconds = qFromBigEndian(ntpTime.seconds);
    quint32 fraction = qFromBigEndian(ntpTime.fraction);

    // Convert NTP timestamp to number of milliseconds passed since Jan 1 1900
    qint64 ntpMSecs = seconds * 1000ll + fraction * 1000ll / 0x100000000ll;

    // Construct Qt date time
    return QDateTime::fromMSecsSinceEpoch(ntpMSecs + epoch::january_1st_1970_unix);
}

#endif // TIMESTAMP_H
