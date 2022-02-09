#ifndef TDIST_H
#define TDIST_H

#include "config.h"

struct tdist
{
    quint8 seconds_upper;
    quint8 seconds_lower;
    quint8 fraction_upper;
    quint8 fraction_lower;
};

#endif // TDIST_H
