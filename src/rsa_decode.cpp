#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include "base64.h"
#include "rsa.h"
#include "rsa_code.h"

using namespace boost::multiprecision;
typedef number<cpp_int_backend<>, et_off> int_type;

int main(int argc, char** argv)
{
    if (argc != 4) {
        std::cerr << "Usage: rsa-decode private_key input output\n";
        return EXIT_FAILURE;
    }

    const char* private_key_filename = argv[1];
    std::ifstream private_key_file(private_key_filename);
    if (!private_key_file) {
        std::cerr << "rsa-decode: error: ";
        std::cerr << private_key_filename << ": ";
        std::cerr << strerror(errno) << '\n';
        return EXIT_FAILURE;
    }

    std::vector<char> private_key_base64(
        (std::istream_iterator<char>(private_key_file)),
         std::istream_iterator<char>());
    size_t expected_size = rks::base64_size_encoded(
        RSA_PRIVATE_KEY_SIZE + RSA_MODULUS_KEY_SIZE);
    if (private_key_base64.size() != expected_size) {
        std::cerr << private_key_filename << ": error: ";
        std::cerr << "bad private key size\n";
        return EXIT_FAILURE;
    }

    std::vector<unsigned char> data;
    auto t = rks::base64_decode(private_key_base64.begin(),
                                private_key_base64.size(),
                                std::back_inserter(data));
    if (std::get<1>(t) != 0) {
        std::cerr << private_key_filename << ": error: ";
        std::cerr << "bad base64 format\n";
        return EXIT_FAILURE;
    }

    int_type prv, n;
    import_bits(prv, data.begin(), data.begin() + RSA_PRIVATE_KEY_SIZE);
    import_bits(n, data.begin() + RSA_PRIVATE_KEY_SIZE, data.end());

    const char* input_filename = argv[2];
    std::ifstream input_file(input_filename);
    if (!input_file) {
        std::cerr << "rsa-decode: error: ";
        std::cerr << input_filename << ": ";
        std::cerr << strerror(errno) << '\n';
        return EXIT_FAILURE;
    }

    std::vector<char> input_base64(
        (std::istream_iterator<char>(input_file)),
         std::istream_iterator<char>());
    data.clear();
    t = rks::base64_decode(input_base64.begin(),
                           input_base64.size(),
                           std::back_inserter(data));
    if (std::get<1>(t) != 0) {
        std::cerr << input_filename << ": error: ";
        std::cerr << "bad base64 format\n";
        return EXIT_FAILURE;
    }

    std::vector<unsigned char> decoded;
    encode(data.begin(), data.size(), RSA_DECODE_BLOCK_SIZE,
           std::back_inserter(decoded), prv, n);

    const char* output_filename = argv[3];
    std::ofstream output_file(output_filename);
    if (!output_file) {
        std::cerr << "rsa-decode: error: ";
        std::cerr << output_filename << ": ";
        std::cerr << strerror(errno) << '\n';
        return EXIT_FAILURE;
    }

    std::copy(decoded.begin(), decoded.end(),
              std::ostream_iterator<char>(output_file));
}

