#include "Verifier.h"
#include "Prover.h"

void Verifier::SendMessage(STEPS step, const Vec<ZZ> &data)
{
    if (!prover)
    {
        return;
    }

    prover->ReceiveMessage(step, data);
}

void Verifier::ReceiveMessage(STEPS step, const Vec<ZZ> &data)
{
    switch (step)
    {
        case STEPS::SENDHS:
        {
            hs.SetLength(t);
            hs = static_cast<Vec<ZZ>> (data);
            std::cout << "Verifier received hs: " << hs << std::endl;
            std::cout << "Starting coin flip protocol!" << std::endl;
            CoinFlipProtocol();
        }   break;
        case STEPS::COINRNG:
        {
            auto vector = static_cast<Vec<ZZ>>(data);
            ZZ even_or_odd = vector[0];
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

            Vec<ZZ> vec;
            vec.SetLength(3);
            vec[0] = coin_x;
            vec[1] = coin_p;
            vec[2] = coin_q;
            SendMessage(STEPS::COINX, vec);

            if (coin_flips < t)
            {
                CoinFlipProtocol();
            } else
            {
                std::cout << "generated bits: " << bits << std::endl;
                std::cout << "Min index " << min_idx << std::endl;

                SendMessage(STEPS::COINOVER, {});
            }
        }   break;
        case STEPS::SENDR:
        {
            sent_r = static_cast<Vec<ZZ>>(data);
            SendMessage(STEPS::RREC, {});
        }
            break;
        case STEPS::SENDS:
        {
            sent_s = static_cast<Vec<ZZ>>(data);

            std::cout << "sent_r" << sent_r << std::endl;
            std::cout << "sent_s" << sent_s << std::endl;

            for (long i = 0; i < t; i++)
            {
                ZZ res;
                if (bits[i] == 0)
                {
                    res = PowerMod(a, sent_r[i], p);
                    if (res != hs[i])
                    {
                        std::cout << COLOR_RED <<  "a^r = h mod p is not true" << COLOR_DEFAULT << std::endl;
                        Approved(false);
                        return;
                    }
                }
                else
                {
                    ZZ right;
                    res = PowerMod(a, sent_s[i], p);
                    right = MulMod(hs[i], InvMod(hs[min_idx], p), p);
                    if (res != right)
                    {
                        std::cout << COLOR_RED << "a^s = h[i]h[j]^-1 mod p is not true" << COLOR_DEFAULT << std::endl;
                        Approved(false);
                        return;
                    }
                }
            }

            SendMessage(STEPS::BITVER, {});
        }
            break;
        case STEPS::SENDZ:
        {
            auto Z = data[0];

            ZZ left, right;

            left = PowerMod(a, Z, p);
            right = MulMod(b, InvMod(hs[min_idx], p), p);

            std::cout << "left: " << left << "\n";
            std::cout << "right: "<< right << "\n";
            Approved(left == right != 0);
        }
            break;
        default:
            std::cout << "Error parsing message" << std::endl;
    }
}

void Verifier::CoinFlipProtocol()

{
    coin_p = GenPrime_ZZ(64);
    while (coin_p % 4 != 3)
    {
        coin_p = GenPrime_ZZ(64);
    }

    coin_q = GenPrime_ZZ(64);
    while (coin_p % 4 != 3)
    {
        coin_q = GenPrime_ZZ(64);
    }

    ZZ n;
    n = coin_p * coin_q;

    ZZ x, x0, x1;
    x = GenPrime_ZZ(64);
    while (GCD(x, n) != 1)
    {
        x = GenPrime_ZZ(64);
    }

    x0 = PowerMod(x, 2, n);
    x1 = PowerMod(x0, 2, n);

    is_x0_odd = IsOdd(x0);

    Vec<ZZ> coin_vec;
    coin_vec.SetLength(2);
    coin_vec[0] = n;
    coin_vec[1] = x1;


    coin_x = x;
    SendMessage(STEPS::COINN, coin_vec);
}
