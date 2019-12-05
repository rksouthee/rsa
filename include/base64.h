#pragma once

#include <iterator>
#include <algorithm>
#include <tuple>

namespace rks {

namespace {
const char* base64_table =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char base64_padding = '=';
}

template <typename N>
// requires Integer(N)
inline
N base64_size_encoded(const N& n)
{
    return ((n + N(2)) / N(3)) * N(4);
}

// TODO
template <typename N>
// requires Integer(N)
inline
N base64_size_decoded(const N& n)
{
    return (n / N(4)) * N(3);
}

template <typename O>
// requires Writable(O) && Iterator(O) && ValueType(O) == char
O base64_encode3(unsigned char a, unsigned char b, unsigned char c, O f_o)
{
    *f_o++ = base64_table[( a & 0xFC) >> 2];
    *f_o++ = base64_table[((a & 0x03) << 4) | ((b & 0xF0) >> 4)];
    *f_o++ = base64_table[((b & 0x0F) << 2) | ((c & 0xC0) >> 6)];
    *f_o++ = base64_table[  c & 0x3F];
    return f_o;
}

template <typename O>
// requires Writable(O) && Iterator(O) && ValueType(O) == char
O base64_encode2(unsigned char a, unsigned char b, O f_o)
{
    *f_o++ = base64_table[( a & 0xFC) >> 2];
    *f_o++ = base64_table[((a & 0x03) << 4) | ((b & 0xF0) >> 4)];
    *f_o++ = base64_table[( b & 0x0F) << 2];
    *f_o++ = base64_padding;
    return f_o;
}

template <typename O>
// requires Writable(O) && Iterator(O) && ValueType(O) == char
O base64_encode1(unsigned char a, O f_o)
{
    *f_o++ = base64_table[(a & 0xFC) >> 2];
    *f_o++ = base64_table[(a & 0x03) << 4];
    *f_o++ = base64_padding;
    *f_o++ = base64_padding;
    return f_o;
}

template <typename I, typename O, typename N>
// requires Readable(I) && Iterator(I) && ValueType(I) == unsigned char &&
//      Writable(O) && Iterator(O) && ValueType(O) == char
std::pair<I, O> base64_encode(I f_i, N n_i, O f_o)
{
     N steps = n_i / N(3);
     while (steps--) {
         unsigned char a = *f_i++;
         unsigned char b = *f_i++;
         unsigned char c = *f_i++;
         f_o = base64_encode3(a, b, c, f_o);
     }

     steps = n_i % N(3);
     if (steps == 2) {
         unsigned char a = *f_i++;
         unsigned char b = *f_i++;
         f_o = base64_encode2(a, b, f_o);
     } else if (steps == 1) {
         unsigned char a = *f_i++;
         f_o = base64_encode1(a, f_o);
     }
     return std::make_pair(f_i, f_o);
}

template <typename I, typename O>
// requires Readable(I) && Iterator(I) && ValueType(I) == unsigned char &&
//      Writable(O) && Iterator(O) && ValueType(O) == char
inline
O base64_encode(I f_i, I l_i, O f_o)
{
    return base64_encode(f_i, std::distance(f_i, l_i), f_o).second;
}

template <typename I, typename O, typename N>
// requires Readable(I) && Iterator(I) && ValueType(I) == char &&
//      Writable(O) && Iterator(O) && ValueType(O) == unsigned char &&
//      Integer(N)
std::tuple<I, N, O> base64_decode(I f_i, N n_i, O f_o)
{
    unsigned char values[4];
    N read = 0;
    while (n_i >= N(4) - read) {
        const char* tmp = std::find(base64_table, base64_table + 64, *f_i);
        if (tmp == base64_table + 64) {
            if (*f_i != base64_padding) break;
            switch (read) {
            case 0:
            case 1:
                break;
            case 2:
                --n_i;
                ++f_i;
                if (*f_i != base64_padding) break;
                --n_i;
                ++f_i;
                *f_o++ = (values[0] << 2) | (values[1] >> 4);
                break;
            case 3:
                --n_i;
                ++f_i;
                *f_o++ = (values[0] << 2) | (values[1] >> 4);
                *f_o++ = ((values[1] & 0x0F) << 4) | (values[2] >> 2);
                break;
            }
            return std::make_tuple(f_i, n_i, f_o);
        }
        --n_i;
        ++f_i;
        values[read++] = static_cast<unsigned char>(tmp - base64_table);
        if (read == 4) {
            *f_o++ = (values[0] << 2) | (values[1] >> 4);
            *f_o++ = ((values[1] & 0x0F) << 4) | (values[2] >> 2);
            *f_o++ = ((values[2] & 0x03) << 6) | values[3];
            read = 0;
        }
    }
    return std::make_tuple(f_i, n_i, f_o);
}

} // namespace rks
