#ifndef REPLYDATA_H
#define REPLYDATA_H

#include <QSharedData>
#include "config.h"
#include "packet.h"

class ReplyData: public QSharedData
{
public:
    full_packet fpacket;
    QDateTime destinationTime;
};

#endif // REPLYDATA_H
