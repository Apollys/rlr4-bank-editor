/**
  *  Starcode v1.4 - C++ port by Apollys / Ashen / Entity
  *  Original Galaxy code by s3rius
  *
  **/

#ifndef STARCODE_H
#define STARCODE_H
  
#include <string>
#include <algorithm> /* std::reverse */
#include <unordered_map>

class Starcode {
    private:
    /* Private class variables */
    std::string alphabet;
    std::string current_code;
    std::unordered_map<char, unsigned int> alphabet_index;
    
    public:
    /* Constructor */
    Starcode();
    
    private:
    
    std::string reversed(const std::string s);
    
    /*
     * Prepends '0' to the given string until it reaches the given new_length
     * If input string is longer than new_length, simply returns input string
     */
    std::string fill(std::string s, int new_length);
    
    /*
     * Strips leading zeros from the input string
     * If the input string is all zeros, returns "0"
     */
    std::string cut(std::string s);
    
    /*
     * Performs the base-10 addition operation and returns the result as a string
     * The length of the result will be either the length of the longer input or
     * the length of the longer input plus one, depending upon whether or not
     * there is a carry in the most significant digit.
     */
    std::string bigint_add(std::string a, std::string b);
    
    /*
     * Performs the base-10 subtraction: a-b
     * If result would be negative, the carry from the most significant
     * digit is ignored, effectively adding 10^(a.length()) to the result
     */
    std::string bigint_subtract(std::string a, std::string b);
    
    /*
     * Performs multiplication of the two input values and returns the result as a string.
     * As seen in the original code, the input string values are converted directly to
     * integer values, so it is assumed they are not too big.
     * The first argument is then multiplied by each digit of the second argument
     * to acquire the product.
     * We must reproduce this algorithm exactly to achieve the same results as the original.
     */
    std::string bigint_multiply(std::string a, std::string b);
    
    /*
     * Performs division of the first operand by the second operand
     * Following the algorithm in the original code to ensure same results
     */
    std::string bigint_divide(std::string a, std::string b);
    
    /*
     * Modulo operation - same code as above, but discard "result" and return carry
     */
    std::string bigint_modulo(std::string a, std::string b);
    
    /*
     * Power operator - simply performs repeated multiplies
     * (could be optimized but need to mimic original functionality,
     * optimization may cause different behaviour in some cases)
     */
    std::string bigint_power(std::string a, int power);
    
    /*
     * Given a string s containing an integer, encode the given value into the string,
     * utilizing the given max_value as an upper limit.
     * The formula is simply:
     *   result = input*max_value + value
     * The beauty is that this can be decoded by taking the modulus of the result,
     * and the previous string can be recovered by dividing by the max value.
     * This process can be repeatedly applied to encode values, and is completely reversible.
     */
    std::string encode(std::string s, int value, int max_value);
    
    /*
     * Extracts the value that was saved to the string using the given max_value
     */
    int decode_value(std::string s, int max_value);
    
    /*
     * Recovers the previous string, given the max_value used to encode
     */
    std::string decode_string(std::string s, int max_value);
    
    char shift_forward(char c, char d);
    
    char shift_backward(char c, char d);
    
    /* Helper method to convert a single character to a string */
    std::string ctos(char c);
    
    /* String equality check */
    int strings_equal(std::string first, std::string second);
    
    std::string base_10_to_n(std::string s, int n);
    
    std::string base_n_to_10(std::string s, int n);
    
    int int_pow(int a, int b);
    
    std::string hash(std::string s, int key_length);
    
    
    public:
    
    /* API Starts Here */
    
    void start_code();
    
    void set_code(std::string s);
    
    std::string get_code();
    
    void store_int(int value, int max_value);
    
    int get_int(int max_value);
    
    /*
     * Compress: represent base 10 number using the whole alphabet
     */
    std::string compress(std::string s);
    
    /*
     * Decompress: extract base 10 number from string using full alphabet
     */
    std::string decompress(std::string s);
    
    /*
     * Encrypt: walk through the string and the key (circularly),
     * shifting forward the character in the string by the corresponding
     * character in the key
     */
    std::string encrypt(std::string s, std::string key);
    
    /*
     * Decrypt: same as encrypt, but shift backward instead of foward
     */
    std::string decrypt(std::string s, std::string key);
    
    /* Some stuff about hashing omitted, unused in this application */
};

#endif // STARCODE_H
