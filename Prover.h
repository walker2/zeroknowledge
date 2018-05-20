#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
#ifndef ZEROKNOWLEDGE_PEGGY_H
#define ZEROKNOWLEDGE_PEGGY_H


#include "ITransportClient.h"
#include <NTL/ZZ.h>

using namespace NTL;

class Verifier;

class Prover : ITransportClient
{

public:
    Prover(const ZZ &_a, const ZZ &_b, const ZZ &_x, const ZZ &_p, long _t) : a(_a), b(_b), x(_x), p(_p), t(_t)
    {
        bits.SetLength(t);
    }

    void SetVerifier(Verifier *_verifier) { verifier = _verifier; }

    void Prove();

public:
    void SendMessage(STEPS step, const Vec<ZZ>& data) override;
    void ReceiveMessage(STEPS step, const Vec<ZZ>& data) override;

private:
    Verifier* verifier;
    void GenerateR();
    void GenerateH();

private:
    long coin_flips = 0;
    long t;
    long min_idx = -1;
    long even_or_odd = 0;
    ZZ a, b, x, p;
    Vec<ZZ> rs;
    Vec<ZZ> hs;
    Vec<ZZ> bits;
    Vec<ZZ> sent_r;
    Vec<ZZ> sent_s;

    void ReceiveNandX(const Vec<ZZ> &data);

    void CheckYourGuess(const Vec<ZZ> &data);

    void CalculateAndSendZ();

    void SendR();
};

#endif

#pragma clang diagnostic pop