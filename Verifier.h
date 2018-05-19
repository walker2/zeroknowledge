#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
#ifndef ZEROKNOWLEDGE_VIKTOR_H
#define ZEROKNOWLEDGE_VIKTOR_H

#include "ITransportClient.h"

#include <NTL/ZZ.h>

using namespace NTL;

class Prover;

class Verifier : ITransportClient
{
public:
    Verifier(const ZZ &_a, const ZZ &_b, const ZZ &_x, const ZZ &_p, long _t) : a(_a), b(_b), x(_x), p(_p), t(_t)
    {
        bits.SetLength(t);
    }

    void SetProver(Prover *_prover) { prover = _prover; }

public:
    void SendMessage(STEPS step, const Vec<ZZ>& data) override;
    void ReceiveMessage(STEPS step, const Vec<ZZ>& data) override;

private:
    Prover* prover;

    void CoinFlipProtocol();

    void Approved(bool isApproved)
    {
        if (isApproved)
        {
            std::cout << COLOR_GREEN << "OKAY" << COLOR_DEFAULT << std::endl;
            std::cout << "Prover knows x, probability of her cheating is " << 1 / (std::pow(2, t)) << std::endl;
            SendMessage(STEPS::OKAY, {});
        } else
        {
            std::cout << COLOR_RED << "DENIED" << COLOR_DEFAULT << std::endl;
            std::cout << "Prover is lying" << std::endl;
            SendMessage(STEPS::ERROR, {});
        }



    }

private:
    long coin_flips = 0;
    long t;
    long min_idx = -1;
    long is_x0_odd;
    ZZ a, b, x, p;
    Vec<ZZ> hs;
    Vec<ZZ> bits;
    ZZ coin_x, coin_p, coin_q;
    Vec<ZZ> sent_r, sent_s;
};


#endif

#pragma clang diagnostic pop