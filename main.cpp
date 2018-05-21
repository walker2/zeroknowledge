#include <iostream>

#include <NTL/ZZ.h>
#include "Verifier.h"
#include "Prover.h"

using namespace std;
using namespace NTL;

ZZ power_mod(ZZ a, ZZ b, ZZ n)
{
    ZZ res;
    res = 1;

    while (b != 0)
    {
        if (b % 2 == 0)
        {
            b /= 2;
            a *= a;
            a %= n;
        } else {
            b--;
            res *= a;
            res %= n;
        }
    }

    return res % n;
}

bool miller_rabin(ZZ &num, long l, long err = 80)
{
    if (num % 2 == 0)
        return false;

    ZZ a;
    ZZ s;
    for (long i = 0; i < l; i++)
    {
        if (bit(num, i) == 1)
            s = i;
    }

    ZZ t;
    ZZ two;
    two = 2^s;

    t = divide(num - 1, t);
    std::cout << s << ":" << t << std::endl;

    for (int i = 0; i < err; i++)
    {
        do {
            a = RandomBits_ZZ(l);
            a += 2;
        } while (a == num);

        a = power_mod(a, t, num);

        if ( a != 1 && a != num - 1 )
        {
            for (int j = 0; j < s; j++)
            {
                a = power_mod(a, ZZ(2), num);

                if (a == num - 1)
                    break;

                if (a == 1 || j == s - 1)
                    return false;
            }
        }
    }

    return true;
}

ZZ gen_prime(long l, long err = 80)
{
    ZZ res;
    res = RandomBits_ZZ(l);
    res = 45;
    while (!miller_rabin(res, l, err)) { res = RandomBits_ZZ(l); }

    return res;
}

int main()
{
    ZZ seed;
    seed = time(nullptr);
    SetSeed(seed);

    long t = 64;
    ZZ a, x, b, p;

    a = RandomBits_ZZ(7);
    x = RandomBits_ZZ(7);
    p = gen_prime(8);
    b = power_mod(a, x, p);

    cout << "a: " << a << "\n";
    cout << "x: " << x << "\n";
    cout << "p: " << p << "\n";
    cout << "b: " << b << "\n";

    ZZ con;
    con = 10;

    auto *prover   = new Prover(a, b, x, p, t);
    auto *verifier = new Verifier(a, b, p, t);

    prover->SetVerifier(verifier);
    verifier->SetProver(prover);

    prover->Prove();
}

