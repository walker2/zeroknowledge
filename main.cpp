#include <iostream>

#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

int main()
{
    ZZ seed;
    seed = time(nullptr);
    SetSeed(seed);

    long t = 64;
    ZZ a, x, b, p;

    a = RandomBits_ZZ(127);
    x = RandomBits_ZZ(127);
    p = GenPrime_ZZ(128);
    b = PowerMod(a, x, p);

    cout << "a: " << a << "\n";
    cout << "x: " << x << "\n";
    cout << "p: " << p << "\n";
    cout << "b: " << b << "\n";

    /* First step. Peggy. Generate t random values r lesser than p - 1 */

    Vec<ZZ> rs;
    rs.SetLength(t);
    for (long i = 0; i < t; i++)
    {
        rs[i] = RandomBnd(p - 1);
    }

    std::cout << "Vector of r:" << rs << "\n";

    /* Second step. Peggy. Calculate h = A^r mod p for i and send them to Victor*/

    Vec<ZZ> hs;
    hs.SetLength(t);
    for (long i = 0; i < t; i++)
    {
        hs[i] = PowerMod(a, rs[i], p);//(a ^ rs[i]) % p;
    }

    std::cout << "Vector of h:" << hs << "\n";

    /* Third step. Using coin protocol Peggy and Victor generate t bits */

    // Should be done as protocol but for now it's okay
    long min_idx = -1;

    Vec<long> bits;
    bits.SetLength(t);
    for (long i = 0; i < t; i++)
    {
        bits[i] = RandomBnd(2);
        if (min_idx == -1 && bits[i] == 1)
        {
            min_idx = i;
        }

    }

    std::cout << "Vector of bits:" << bits << "\n";
    std::cout << "Min index " << min_idx << "\n";

    /* Fourth step. Peggy. For every bit: if 0 then send Victor r[i]; else send s = (r[i] - r[j]) where j min index of b = 1 */
    Vec<ZZ> sent_r;
    Vec<ZZ> sent_s;
    sent_r.SetLength(t);
    sent_s.SetLength(t);

    for (long i = 0; i < t; i++)
    {
        if (bits[i] == 0)
        {
            // Send r
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
    /* Fifth step. Victor. For every bit: if 0 then a^r = h mod p; else a^s = h[i]h[j]^-1 mod p */

    for (long i = 0; i < t; i++)
    {
        ZZ res;
        if (bits[i] == 0)
        {
            res = PowerMod(a, sent_r[i], p);
            if (res != hs[i])
            {
                std::cout << "Error 1 \n";
            }
        }
        else
        {
            ZZ right;
            res = PowerMod(a, sent_s[i], p);
            right = MulMod(hs[i], InvMod(hs[min_idx], p), p);
            if (res != right)
            {
                std::cout << "Error 2 \n";
            }
        }
    }

    /* Sixth step. Peggy. Z = (x - r[j]) mod (p - 1) */
    ZZ Z;

    Z = SubMod(x, rs[min_idx], p - 1);

    std::cout << "Z: " << Z << "\n";

    /* Seventh step. Victor. A^z == B*inv(h[j]) mod p */

    ZZ left, right;

    left = PowerMod(a, Z, p);
    right = MulMod(b, InvMod(hs[min_idx], p), p);

    std::cout << "left: " << left << "\n";
    std::cout << "right: "<< right << "\n";
    if (left == right)
    {
        std::cout << "Peggy knows x, probability of her cheating is " << 1 / (std::pow(2, t)) << std::endl;
    } else
    {
        std::cout << "Peggy is lying" << std::endl;
    }

}

