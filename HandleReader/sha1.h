#ifndef SHA1_H
#define SHA1_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdint> /* fixed width integer types */
#include <cstring> /* memcpy */
#include <cassert>

namespace sha1 {
    /* Consts */
    const size_t INTS_PER_BLOCK = 16;
    const size_t BYTES_PER_BLOCK = 4*INTS_PER_BLOCK;

    /* Helper Functions */
    template<typename T>
    std::string int_to_hex(T value);
    
    std::string hash_to_hex(uint32_t hash[]);

    std::string append_int(std::string message, uint64_t n);

    uint32_t rotate_left(uint32_t value, unsigned int n);

    void buffer_to_block(std::string buffer, uint32_t block[INTS_PER_BLOCK]);

    void process_block(uint32_t block[INTS_PER_BLOCK], uint32_t hash[]);

    /* API */
    std::string sha1(std::string message);
}

#endif // SHA1_H