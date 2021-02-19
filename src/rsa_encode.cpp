#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
typedef number<cpp_int_backend<>, et_off> int_type;

template <typename I, typename O>
O encode(I f_i, I l_i, const int_type& key, const int_type& modulus, O f_o)
{
    while (f_i != l_i) {
        int_type value = *f_i++;
        value = powm(value, key, modulus);
        *f_o++ = value;
    }
    return f_o;
}

int main(int argc, char** argv)
{
    if (argc != 4) {
        std::cerr << "Usage: rsa-encode <public_key> <input> <output>\n";
        return EXIT_FAILURE;
    }

    const char* public_key_filename = argv[1];
    std::ifstream public_key_file(public_key_filename);
    if (!public_key_file) {
        std::cerr << "rsa-encode: error: " << public_key_filename << ": " << strerror(errno) << '\n';
        return EXIT_FAILURE;
    }

    int_type pub, n;
    public_key_file >> pub >> n;

    const char* input_filename = argv[2];
    std::ifstream input_file(input_filename);
    if (!input_file) {
        std::cerr << "rsa-encode: error: " << input_filename << ": " << strerror(errno) << '\n';
        return EXIT_FAILURE;
    }

    std::vector<char> input((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    std::vector<int_type> output;
    encode(input.begin(), input.end(), pub, n, std::back_inserter(output));

    const char* output_filename = argv[3];
    std::ofstream output_file(output_filename);
    if (!output_file) {
        std::cerr << "rsa-encode: error: " << output_filename << ": " << strerror(errno) << '\n';
        return EXIT_FAILURE;
    }

    std::copy(output.begin(), output.end(), std::ostream_iterator<int_type>(output_file, "\n"));
}
