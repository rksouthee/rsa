#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include "rks/base64.h"
#include "rsa.h"
#include "rsa_key.h"

using namespace boost::multiprecision;
using namespace boost::random;
typedef boost::multiprecision::number<cpp_int_backend<>, et_off> int_type;

static
void write_key(const char* filename, const int_type& key, const int_type& n);

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "Usage: rsa-keygen public_key private_key\n";
        return EXIT_FAILURE;
    }

    mt11213b prime_base_generator(clock());
    independent_bits_engine<mt11213b, RSA_KEY_SIZE_BITS, cpp_int> prime_generator;

    mt19937 candidate_base_generator(clock());
    independent_bits_engine<mt19937, RSA_KEY_SIZE_BITS, cpp_int> candidate_generator;

    auto keys = rsa_keys<int_type>(prime_generator, candidate_generator);
    const auto& pub = std::get<0>(keys);
    const auto& n   = std::get<1>(keys);
    const auto& prv = std::get<2>(keys);

    write_key(argv[1], pub, n);
    write_key(argv[2], prv, n);
}

static
void write_key(const char* filename, const int_type& key, const int_type& n)
{
    std::ofstream key_file(filename);
    if (!key_file) {
        std::cerr << "rsa-keygen: error: ";
        std::cerr << filename << ": ";
        std::cerr << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
    }
    std::vector<unsigned char> data;
    export_bits(key, std::back_inserter(data), 8);
    export_bits(n, std::back_inserter(data), 8);
    rks::base64_encode(data.begin(), data.end(),
                       std::ostream_iterator<char>(key_file));
}
