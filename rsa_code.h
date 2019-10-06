#include <iterator>
#include <utility>
#include "rsa_key.h"

template <typename I>
using DistanceType = typename std::iterator_traits<I>::difference_type;

template <typename I, typename O, typename N>
O encode_block(I f_i, I l_i, O f_o, const N& key, const N& modulus)
{
    N i;
    import_bits(i, f_i, l_i, 8);
    i = power_semigroup(i, key, modulo_multiply<N>(modulus));
    f_o = export_bits(i, f_o, 8);
    return f_o;
}

template <typename I, typename O, typename N>
std::pair<I, O> encode(I f_i, DistanceType<I> n_i, unsigned n_b, O f_o,
                       const N& key, const N& modulus)
{
    DistanceType<I> n = n_i / n_b;
    while (n) {
        I l_i = std::next(f_i, n_b);
        f_o = encode_block(f_i, l_i, f_o, key, modulus);
        f_i = l_i;
        --n;
    }
    I l_i = std::next(f_i, n_i % n_b);
    if (f_i != l_i) f_o = encode_block(f_i, l_i, f_o, key, modulus);
    return std::make_pair(l_i, f_o);
}

