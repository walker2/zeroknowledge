#include "Prover.h"
#include "Verifier.h"

void Prover::GenerateH()

{
    hs.SetLength(t);
    for (long i = 0; i < t; i++)
    {
        hs[i] = PowerMod(a, rs[i], p);
    }

    std::cout << "Vector of h:" << hs << "\n";

    SendMessage(STEPS::SENDHS, hs);
}

void Prover::GenerateR()
{
    rs.SetLength(t);
    for (long i = 0; i < t; i++)
    {
        rs[i] = RandomBnd(p - 1);
    }

    std::cout << "Prover generate vector of r:" << rs << "\n";
}

void Prover::SendMessage(STEPS step, const Vec<ZZ> &data)
{
    verifier->ReceiveMessage(step, data);
}

void Prover::ReceiveMessage(STEPS step, const Vec<ZZ> &data)
{
    switch (step)
    {
        case STEPS::COINN:
        {
            ReceiveNandX(data);
        }   break;
        case STEPS::COINX:
        {
            CheckYourGuess(data);
        }   break;
        case STEPS::COINOVER:
        {
            SendR();
        }   break;
        case STEPS::RREC:
        {
            SendMessage(STEPS::SENDS, sent_s);
        }   break;
        case STEPS::BITVER:
        {
            CalculateAndSendZ();
        }   break;
        case STEPS::OKAY:
        {
            return;
        }
        case STEPS::ERROR:
        {
            return;
        }
        default:
            std::cout << "Error parsing message" << std::endl;
    }
}

void Prover::SendR()
{
    sent_r.SetLength(t);
    sent_s.SetLength(t);

    for (long i = 0; i < t; i++)
    {
        if (bits[i] == 0)
        {
            sent_r[i] = rs[i];
            sent_s[i] = 0;
        }
        else
        {
            sent_r[i] = 0;
            sent_s[i] = (rs[i] - rs[min_idx]) % (p - 1);
        }
    }
    std::cout << "Vector of sent_r:" << sent_r << "\n";
    std::cout << "Vector of sent_s:" << sent_s << "\n";
    SendMessage(STEPS::SENDR, sent_r);
}

void Prover::CalculateAndSendZ()
{
    ZZ Z;

    Z = SubMod(x, rs[min_idx], p - 1);

    std::cout << "Z: " << Z << "\n";

    Vec<ZZ> send_vec;
    send_vec.SetLength(1);
    send_vec[0] = Z;

    SendMessage(STEPS::SENDZ, send_vec);
}

void Prover::CheckYourGuess(const Vec<ZZ> &data)
{
    Vec<ZZ> coin_vec;
    coin_vec.SetLength(3);
    coin_vec = static_cast<Vec<ZZ>>(data);

    ZZ coin_x, coin_p, coin_q;
    coin_x = coin_vec[0];
    coin_p = coin_vec[1];
    coin_q = coin_vec[2];
    ZZ n;
    n = coin_p * coin_q;

    ZZ x0;
    x0 = PowerMod(coin_x, 2, n);

    long is_x0_odd = IsOdd(x0);

    if (even_or_odd == is_x0_odd)
    {
        bits[coin_flips++] = 1;
    } else
    {
        bits[coin_flips++] = 0;
    }

    if (min_idx == -1 && bits[coin_flips - 1] == 1)
    {
        min_idx = coin_flips - 1;
    }
}

void Prover::ReceiveNandX(const Vec<ZZ> &data)
{
    Vec<ZZ> vec;
    vec.SetLength(2);
    vec = static_cast<Vec<ZZ>>(data);
    even_or_odd = RandomBnd(2);

    Vec<ZZ> send;
    send.SetLength(1);
    send[0] = even_or_odd;

    SendMessage(STEPS::COINRNG, send);
}

void Prover::Prove()
{
    GenerateR();
    GenerateH();
}
