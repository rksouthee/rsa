#ifndef RSA_KEY_H
#define RSA_KEY_H

#include <tuple>

template <typename I>
// requires Integer(I)
bool even(const I& i)
{
    return (i & 0x1) == I(0);
}

template <typename I>
// requires Integer(I)
bool odd(const I& i)
{
    return (i & 0x1) != I(0);
}

template <typename I>
// requires Integer(I)
I half(const I& i)
{
    return i >> 1;
}

template <typename A, typename N, typename Op>
// requires Regular(A) && Integer(N) && SemigroupOperation(Op)
A power_accumulate_semigroup(A r, A a, N n, Op op)
{
    // precondition: n >= 0
    if (n == N(0)) return r;
    while (true) {
        if (odd(n)) {
            r = op(r, a);
            if (n == N(1)) return r;
        }
        n = half(n);
        a = op(a, a);
    }
}

template <typename A, typename N, typename Op>
// requires Regular(A) && Integer(N) && SemigroupOperation(Op)
A power_semigroup(A a, N n, Op op)
{
    // precondition: n > 0
    while (!odd(n)) {
        a = op(a, a);
        n = half(n);
    }
    if (n == N(1)) return a;
    return power_accumulate_semigroup(a, op(a, a), half(n - N(1)), op);
}

template <typename I>
// requires Integer(I)
struct modulo_multiply
{
    I modulus;

    explicit modulo_multiply(const I& m) :
        modulus(m) { }

    I operator()(const I& x, const I& y) const
    {
        return (x * y) % modulus;
    }
};

template <typename I>
I gcd(I a, I b)
{
    while (b != I(0)) {
        a = a % b;
        std::swap(a, b);
    }
    return a;
}

template <typename I>
// requires Integer(I)
bool miller_rabin_test(I n, I q, I k, I w)
{
    // precondition: n > 1 && n - 1 = 2^k*q && odd(q)
    modulo_multiply<I> mmult(n);
    I x = power_semigroup(w, q, mmult);
    if (x == I(1) || x == n - I(1)) return true;
    for (I i(1); i < k; ++i) {
        // invariant: x = w^{2^{i-1}*q}
        x = mmult(x, x);
        if (x == n - I(1)) return true;
        if (x == I(1)) return false;
    }
    return false;
}

template <typename I, typename G>
// requires Integer(I)
bool miller_rabin_trials(I n, int trials, G& gen)
{
    // precondition: odd(n)
    I q = n - 1;
    I k = 0;
    while (even(q)) {
        q = half(q);
        k = k + I(1);
    }

    for (int i = 0; i < trials; ++i) {
        if (!miller_rabin_test(n, q, k, I(gen()))) return false;
    }
    return true;
}

template <typename I>
std::pair<I, I> quotient_remainder(I a, I b)
{
    I q, r;
    divide_qr(a, b, q, r);
    return std::make_pair(q, r);
}

template <typename E>
std::pair<E, E> extended_gcd(E a, E b)
{
    E x0(1);
    E x1(0);
    while (b != E(0)) {
        // compute new r and x
        std::pair<E, E> qr = quotient_remainder(a, b);
        E x2 = x0 - qr.first * x1;
        // shift r and x
        x0 = x1;
        x1 = x2;
        a = b;
        b = qr.second;
    }
    return std::make_pair(x0, a);
}

template <typename I>
// requires Integer(I)
I multiplicative_inverse(I a, I n)
{
    std::pair<I, I> p = extended_gcd(a, n);
    if (p.second != I(1)) return I(0);
    if (p.first < I(0)) return p.first + n;
    return p.first;
}

template <typename I, typename P, typename C>
I generate_prime(P& g_p, C& g_c)
{
    while (true) {
        I n = g_p();
        if (even(n)) n = n - 1;
        if (miller_rabin_trials(n, 100, g_c)) return n;
    }
}

template <typename I, typename G>
I public_key(const I& phi, G& g)
{
    while (true) {
        I n = g();
        if (gcd(n, phi) == 1) return n;
    }
}

template <typename I, typename P, typename C>
std::tuple<I, I, I> rsa_keys(P& g_p, C& g_c)
{
    I p1 = generate_prime<I>(g_p, g_c);
    I p2 = generate_prime<I>(g_p, g_c);
    I phi = (p1 - 1) * (p2 - 1);
    I pub = public_key(phi, g_p);
    I prv = multiplicative_inverse(pub, phi);
    I n = p1 * p2;
    return std::make_tuple(pub, n, prv);
}

#endif
