#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
typedef number<cpp_int_backend<>, et_off> int_type;

template <typename I, typename O>
O decode(I f_i, I l_i, const int_type& key, const int_type& modulus, O f_o)
{
    while (f_i != l_i) {
        int_type value = *f_i++;
        value = powm(value, key, modulus);
        *f_o++ = value.template convert_to<char>();
    }
    return f_o;
}

int main(int argc, char** argv)
{
    if (argc != 4) {
        std::cerr << "Usage: rsa-decode <private_key> <input> <output>\n";
        return EXIT_FAILURE;
    }

    const char* private_key_filename = argv[1];
    std::ifstream private_key_file(private_key_filename);
    if (!private_key_file) {
        std::cerr << "rsa-decode: error: " << private_key_filename << ": " << strerror(errno) << '\n';
        return EXIT_FAILURE;
    }

    int_type prv, n;
    private_key_file >> prv >> n;

    const char* input_filename = argv[2];
    std::ifstream input_file(input_filename);
    if (!input_file) {
        std::cerr << "rsa-decode: error: " << input_filename << ": " << strerror(errno) << '\n';
        return EXIT_FAILURE;
    }

    std::vector<int_type> input;
    std::copy(std::istream_iterator<int_type>(input_file), std::istream_iterator<int_type>(), std::back_inserter(input));

    std::string output;
    decode(input.begin(), input.end(), prv, n, std::back_inserter(output));

    const char* output_filename = argv[3];
    std::ofstream output_file(output_filename);
    if (!output_file) {
        std::cerr << "rsa-decode: error: " << output_filename << ": " << strerror(errno) << '\n';
        return EXIT_FAILURE;
    }

    output_file << output;
}

