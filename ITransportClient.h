#ifndef ZEROKNOWLEDGE_ITRANSPORTCLIENT_H
#define ZEROKNOWLEDGE_ITRANSPORTCLIENT_H

#include <NTL/ZZ.h>
#include "Utils.h"

using namespace NTL;

class ITransportClient
{
public:
    virtual void SendMessage(STEPS step, const Vec<ZZ>& data) = 0;

    virtual void ReceiveMessage(STEPS step, const Vec<ZZ>& data) = 0;
};

#endif
