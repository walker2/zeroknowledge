#include <iostream>

#include <NTL/ZZ.h>
#include "Verifier.h"
#include "Prover.h"

using namespace std;
using namespace NTL;

int main()
{
    ZZ seed;
    seed = time(nullptr);
    SetSeed(seed);

    long t = 64;
    ZZ a, x, b, p;

    a = RandomBits_ZZ(4);
    x = RandomBits_ZZ(4);
    p = GenPrime_ZZ(5);
    b = PowerMod(a, x, p);

    cout << "a: " << a << "\n";
    cout << "x: " << x << "\n";
    cout << "p: " << p << "\n";
    cout << "b: " << b << "\n";

    auto *prover   = new Prover(a, b, x, p, t);
    auto *verifier = new Verifier(a, b, x, p, t);

    prover->SetVerifier(verifier);
    verifier->SetProver(prover);

    prover->Prove();
}

