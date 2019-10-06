#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include "rks/base64.h"
#include "rsa.h"
#include "rsa_code.h"

using namespace boost::multiprecision;
typedef number<cpp_int_backend<>, et_off> int_type;

int main(int argc, char** argv)
{
    if (argc != 4) {
        std::cerr << "Usage: rsa-encode public_key input output\n";
        return EXIT_FAILURE;
    }

    const char* public_key_filename = argv[1];
    std::ifstream public_key_file(public_key_filename);
    if (!public_key_file) {
        std::cerr << "rsa-encode: error: ";
        std::cerr << public_key_filename << ": ";
        std::cerr << strerror(errno) << '\n';
        return EXIT_FAILURE;
    }

    std::vector<char> public_key_base64(
        (std::istream_iterator<char>(public_key_file)),
         std::istream_iterator<char>());
    size_t expected_size = rks::base64_size_encoded(
        RSA_PUBLIC_KEY_SIZE + RSA_MODULUS_KEY_SIZE);
    if (public_key_base64.size() != expected_size) {
        std::cerr << public_key_filename << ": error: ";
        std::cerr << "bad public key size\n";
        return EXIT_FAILURE;
    }

    std::vector<unsigned char> data;
    auto t = rks::base64_decode(public_key_base64.begin(),
                                public_key_base64.size(),
                                std::back_inserter(data));
    if (std::get<1>(t) != 0) {
        std::cerr << public_key_filename << ": error: ";
        std::cerr << "bad base64 format\n";
        return EXIT_FAILURE;
    }

    int_type pub, n;
    import_bits(pub, data.begin(), data.begin() + RSA_PUBLIC_KEY_SIZE);
    import_bits(n, data.begin() + RSA_PUBLIC_KEY_SIZE, data.end());

    const char* input_filename = argv[2];
    std::ifstream input_file(input_filename);
    if (!input_file) {
        std::cerr << "rsa-encode: error: ";
        std::cerr << input_filename << ": ";
        std::cerr << strerror(errno) << '\n';
        return EXIT_FAILURE;
    }

    std::vector<unsigned char> input(
        (std::istreambuf_iterator<char>(input_file)),
        std::istreambuf_iterator<char>());
    data.clear();
    encode(input.begin(), input.size(), RSA_ENCODE_BLOCK_SIZE,
           std::back_inserter(data), pub, n);

    const char* output_filename = argv[3];
    std::ofstream output_file(output_filename);
    if (!output_file) {
        std::cerr << "rsa-encode: error: ";
        std::cerr << output_filename << ": ";
        std::cerr << strerror(errno) << '\n';
        return EXIT_FAILURE;
    }

    rks::base64_encode(data.begin(), data.end(),
                       std::ostream_iterator<char>(output_file));
}
