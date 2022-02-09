#include <cmath>
#include "reply.h"
#include "replydata.h"


#include <QDebug>

Reply::Reply(): pd(new ReplyData())
{
    memset(&pd->fpacket, 0, sizeof (pd->fpacket));
}

Reply::Reply(ReplyData * rd): pd(rd)
{
    Q_ASSERT(rd != NULL);
}

Reply::Reply(const Reply &alt): pd(alt.pd) {}

Reply::~Reply() {}

Reply &Reply::operator=(const Reply &alt)
{
    pd = alt.pd;
    return *this;
}

LeapIndicator Reply::leapIndicator() const
{
    return static_cast<LeapIndicator>(pd->fpacket.basic.packetFlags.leap);
}

quint8 Reply::versionNumber() const
{
    return pd->fpacket.basic.packetFlags.version;
}

Mode Reply::mode() const
{
    return static_cast<Mode>(pd->fpacket.basic.packetFlags.mode);
}

quint8 Reply::stratum() const
{
    return pd->fpacket.basic.stratum;
}

qreal Reply::pollInterval() const
{
    return std::pow(static_cast<qreal>(2), static_cast<qreal>(pd->fpacket.basic.poll));
}

qreal Reply::precision() const
{
    return std::pow(static_cast<qreal>(2), static_cast<qreal>(pd->fpacket.basic.precision));
}

qreal Reply::rootDelay() const
{    
    quint16 seconds = pd->fpacket.basic.rootDelay.seconds_upper;
    seconds = (seconds << 0x8) | pd->fpacket.basic.rootDelay.seconds_lower;
    quint16 fraction = pd->fpacket.basic.rootDelay.fraction_upper;
    fraction = (fraction << 0x8) | pd->fpacket.basic.rootDelay.fraction_lower;
    quint32 result = seconds;
    result = (result << 0x10) | fraction;
    return qreal(result) / 65536;
}

qreal Reply::rootDispersion() const
{
    quint16 seconds = pd->fpacket.basic.rootDispersion.seconds_upper;
    seconds = (seconds << 0x8) | pd->fpacket.basic.rootDispersion.seconds_lower;
    quint16 fraction = pd->fpacket.basic.rootDispersion.fraction_upper;
    fraction = (fraction << 0x8) | pd->fpacket.basic.rootDispersion.fraction_lower;
    quint32 result = seconds;
    result = (result << 0x10) | fraction;
    return qreal(result) / 65536;
}

QDateTime Reply::referenceTime() const
{
    return timestamp::toDateTime(pd->fpacket.basic.referenceTimestamp);
}

QDateTime Reply::originTime() const
{
    return timestamp::toDateTime(pd->fpacket.basic.originateTimestamp);
}

QDateTime Reply::receiveTime() const
{
    return timestamp::toDateTime(pd->fpacket.basic.receiveTimestamp);
}

QDateTime Reply::transmitTime() const
{
    return timestamp::toDateTime(pd->fpacket.basic.transmitTimestamp);
}

QDateTime Reply::destinationTime() const
{
    return pd->destinationTime;
}

qint64 Reply::roundTripDelay() const
{
    return originTime().msecsTo(destinationTime()) - receiveTime().msecsTo(transmitTime());
}

qint64 Reply::localClockOffset() const
{
    return (originTime().msecsTo(receiveTime()) + destinationTime().msecsTo(transmitTime())) / 2;
}

bool Reply::isNull() const
{
    return pd->destinationTime.isNull();
}
