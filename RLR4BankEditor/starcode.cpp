#include "starcode.h"

    /* Constructor */
Starcode::Starcode() {
    alphabet = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!$%/()=?,.-;:_^#+* @{[]}|~`";
    current_code = "0";
    /* precompute alphabet index map */
    for (unsigned int i = 0; i < alphabet.length(); i++) {
        char ch = alphabet[i];
        alphabet_index[ch] = i;
    }
}

std::string Starcode::reversed(const std::string s) {
    std::string rev(s);
    std::reverse(rev.begin(), rev.end());
    return rev;
}

/*
 * Prepends '0' to the given string until it reaches the given new_length
 * If input string is longer than new_length, simply returns input string
 */
std::string Starcode::fill(std::string s, int new_length) {
    if (s.length() >= new_length) {
        return s;
    }
    int num_zeros = new_length - s.length();
    return std::string(num_zeros, '0') + s;
}

/*
 * Strips leading zeros from the input string
 * If the input string is all zeros, returns "0"
 */
std::string Starcode::cut(std::string s) {
    int start = 0;
    while( (start+1 < s.length()) && (s[start] == '0') ) {
        start++;
    }
    int len = s.length() - start;
    return s.substr(start, len);
}

/*
 * Performs the base-10 addition operation and returns the result as a string
 * The length of the result will be either the length of the longer input or
 * the length of the longer input plus one, depending upon whether or not
 * there is a carry in the most significant digit.
 */
std::string Starcode::bigint_add(std::string a, std::string b) {
    int len_a = a.length();
    int len_b = b.length();
    /* prepend zeros to match lengths */
    if (len_a < len_b) {
        a = fill(a, len_b);
        len_a = len_b;
    }
    else if (len_b < len_a) {
        b = fill(b, len_a);
        len_b = len_a;
    }
    /* perform the addition */
    int carry = 0;
    std::string result(len_a, '0');
    for(int i = len_a - 1; i >= 0; i--) {
        int digit_a = a[i] - '0';
        int digit_b = b[i] - '0';
        int sum = digit_a + digit_b + carry;
        int digit_result = sum%10;
        carry = sum/10;
        result[i] = '0' + digit_result;
    }
    if (carry) {
        result.insert(0, 1, '0' + carry); /* string.insert(pos, count, char) */
    }
    return result;
}

/*
 * Performs the base-10 subtraction: a-b
 * If result would be negative, the carry from the most significant
 * digit is ignored, effectively adding 10^(a.length()) to the result
 */
std::string Starcode::bigint_subtract(std::string a, std::string b) {
    int len_a = a.length();
    int len_b = b.length();
    /* prepend zeros to match lengths */
    if (len_a < len_b) {
        a = fill(a, len_b);
        len_a = len_b;
    }
    else if (len_b < len_a) {
        b = fill(b, len_a);
        len_b = len_a;
    }
    /* perform the subtraction */
    int carry = 0;  /* carry will take on negative values to indicate a borrow */
    std::string result(len_a, '0');
    for(int i = len_a - 1; i >= 0; i--) {
        int digit_a = a[i] - '0';
        int digit_b = b[i] - '0';
        int sum = digit_a - digit_b + carry; /* carry is negative or zero */
        if (sum < 0) {
            carry = -1;
            sum += 10;
        }
        else {
            carry = 0;
        }
        int digit_result = sum;
        result[i] = '0' + digit_result;
    }
    /* if (carry) -> we have a negative result, simply ignore carry */
    return cut(result); /* strip leading zeros before returning */
}

/*
 * Performs multiplication of the two input values and returns the result as a string.
 * As seen in the original code, the input string values are converted directly to
 * integer values, so it is assumed they are not too big.
 * The first argument is then multiplied by each digit of the second argument
 * to acquire the product.
 * We must reproduce this algorithm exactly to achieve the same results as the original.
 */
std::string Starcode::bigint_multiply(std::string a, std::string b) {
    int len_a = a.length();
    /* convert b directly to an int, as in original algorithm */
    int int_b = std::stoi(b);
    /* check for zero multiplicand */
    if (int_b == 0) {
        return std::string("0");
    }
    int carry = 0;
    std::string result;
    for (int i = len_a - 1; i >= 0; i--) {
        int digit_a = a[i] - '0';
        int tmp = digit_a * int_b + carry;
        char char_res = '0' + (tmp % 10);
        /* note: appending the results gives us the reversed solution */
        result.append(1, char_res); /* string.append(count, char) */
        carry = tmp/10;
    }
    /* reverse our currently reversed solution */
    result = reversed(result);
    /* prepend the carry value if it exists */
    if (carry) {
        result.insert(0, std::to_string(carry)); /* string.insert(position, string) */
    }
    return result;
}

/*
 * Performs division of the first operand by the second operand
 * Following the algorithm in the original code to ensure same results
 */
std::string Starcode::bigint_divide(std::string a, std::string b) {
    int len_a = a.length();
    /* convert b directly to an int, as in original algorithm */
    int int_b = std::stoi(b);
    if (int_b == 0) return std::string("0"); /* otherwise we'll get a core dump */
    int carry = 0;
    std::string result;
    for (int i = 0; i < len_a; i++) {
        int digit_a = a[i] - '0';
        int dividend = digit_a + 10*carry;
        int quotient = dividend/int_b;
        result.append(std::to_string(quotient)); /* string.append(string) */
        carry = dividend % int_b;
    }
    /* simply ignore any carry left over at this point */
    return cut(result);
}

/*
 * Modulo operation - same code as above, but discard "result" and return carry
 */
std::string Starcode::bigint_modulo(std::string a, std::string b) {
    int len_a = a.length();
    /* convert b directly to an int, as in original algorithm */
    int int_b = std::stoi(b);
    if (int_b == 0) return std::string("0"); /* otherwise we'll get a core dump */
    int carry = 0;
    for (int i = 0; i < len_a; i++) {
        int digit_a = a[i] - '0';
        int dividend = digit_a + 10*carry;
        carry = dividend % int_b;
    }
    return std::to_string(carry);
}

/*
 * Power operator - simply performs repeated multiplies
 * (could be optimized but need to mimic original functionality,
 * optimization may cause different behaviour in some cases)
 */
std::string Starcode::bigint_power(std::string a, int power) {
    if (power == 0) {
        return std::string("1");
    }
    /* "standard" coding practice would initialize the result to 1:
    std::string result("1");
    while (power > 0) {
        result = bigint_multiply(result, a);
        power--;
    } */
    /* we follow the original code, using power=1 as base case */
    std::string result = a;
    while (power > 1) {
        result = bigint_multiply(result, a);
        power--;
    }
    return result;
}

/*
 * Given a string s containing an integer, encode the given value into the string,
 * utilizing the given max_value as an upper limit.
 * The formula is simply:
 *   result = input*max_value + value
 * The beauty is that this can be decoded by taking the modulus of the result,
 * and the previous string can be recovered by dividing by the max value.
 * This process can be repeatedly applied to encode values, and is completely reversible.
 */
std::string Starcode::encode(std::string s, int value, int max_value) {
    std::string product = bigint_multiply(s, std::to_string(max_value));
    std::string result = bigint_add(product, std::to_string(value));
    return result;
}

/*
 * Extracts the value that was saved to the string using the given max_value
 */
int Starcode::decode_value(std::string s, int max_value) {
    return std::stoi(bigint_modulo(s, std::to_string(max_value)));
}

/*
 * Recovers the previous string, given the max_value used to encode
 */
std::string Starcode::decode_string(std::string s, int max_value) {
    return bigint_divide(s, std::to_string(max_value));
}

char Starcode::shift_forward(char c, char d) {
    int c_index = alphabet_index[c];
    int d_index = alphabet_index[d];
    int result_index = (c_index + d_index) % alphabet.length();
    return alphabet[result_index];
}

char Starcode::shift_backward(char c, char d) {
    int c_index = alphabet_index[c];
    int d_index = alphabet_index[d];
    /* we add an extra alphabet.length because in C++ (-3)%10 evaluates to -3 */
    int result_index = (c_index - d_index + alphabet.length()) % alphabet.length();
    return alphabet[result_index];
}

/* Helper method to convert a single character to a string */
std::string Starcode::ctos(char c) {
    return std::string(1, c);
}
/* String equality check */
int Starcode::strings_equal(std::string first, std::string second) {
    return first.compare(second) == 0;
}

std::string Starcode::base_10_to_n(std::string s, int n) {
    std::string base = std::to_string(n);
    std::string remainder;
    std::string result;
    while (!strings_equal(s, std::string("0"))) {
        remainder = bigint_modulo(s, base);
        result.append(1, alphabet[std::stoi(remainder)]); /* string.append(count, char) */
        s = bigint_divide(s, base);
    }
    /* again, we were appending instead of prepending values for efficiency, */
    /* so we obtained the reversed solution */
    return reversed(result);
}

std::string Starcode::base_n_to_10(std::string s, int n) {
    std::string base = std::to_string(n);
    std::string power("1");
    std::string result("0");
    for (int i = s.length()-1; i >= 0; i--) {
        int index = alphabet_index[s[i]];
        std::string digit_value = bigint_multiply(power, std::to_string(index));
        result = bigint_add(result, digit_value);
        power = bigint_multiply(power, base);
    }
    return result;
}

int Starcode::int_pow(int a, int b) {
    int accum = 1;
    int pow = b;
    while (pow) {
        accum *= a;
        pow--;
    }
    return accum;
}

std::string Starcode::hash(std::string s, int key_length) {
    std::string result("0");
    for (int i = s.length()-1; i >= 0; i--) {
        int index = alphabet_index[s[i]];
        result = bigint_add(result, std::to_string(index*i));
    }
    std::string modulus = std::to_string(int_pow(alphabet.length(), key_length));
    std::string modded = bigint_modulo(result, modulus);
    std::string encoded = base_10_to_n(modded, alphabet.length());
    std::string filled = fill(encoded, key_length);
    return filled;
}



/* API Starts Here */

void Starcode::start_code() {
    current_code = std::string("0");
}

void Starcode::set_code(std::string s) {
    current_code = s;
}

std::string Starcode::get_code() {
    return current_code;
}

void Starcode::store_int(int value, int max_value) {
    if (value <= max_value) {
        current_code = encode(current_code, value, max_value + 1);
    }
}

int Starcode::get_int(int max_value) {
    int value = decode_value(current_code, max_value + 1);
    current_code = decode_string(current_code, max_value + 1);
    return value;
}

/*
 * Compress: represent base 10 number using the whole alphabet
 */
std::string Starcode::compress(std::string s) {
    return base_10_to_n(s, alphabet.length());
}

/*
 * Decompress: extract base 10 number from string using full alphabet
 */
std::string Starcode::decompress(std::string s) {
    return base_n_to_10(s, alphabet.length());
}

/*
 * Encrypt: walk through the string and the key (circularly),
 * shifting forward the character in the string by the corresponding
 * character in the key
 */
std::string Starcode::encrypt(std::string s, std::string key) {
    std::string result;
    for (int i = 0; i < s.length(); i++) {
        int key_index = i%key.length();
        result.append(1, shift_forward(s[i], key[key_index])); // string.append(count, char)
    }
    return result;
}

/*
 * Decrypt: same as encrypt, but shift backward instead of foward
 */
std::string Starcode::decrypt(std::string s, std::string key) {
    std::string result;
    for (int i = 0; i < s.length(); i++) {
        int key_index = i%key.length();
        result.append(1, shift_backward(s[i], key[key_index])); // string.append(count, char)
    }
    return result;
}
