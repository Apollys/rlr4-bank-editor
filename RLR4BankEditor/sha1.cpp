#include "sha1.h"

template<typename T>
std::string sha1::int_to_hex(T value) {
  std::stringstream stream;
  stream << std::setfill ('0') << std::setw(sizeof(T)*2) << std::hex << value;
  return stream.str();
}

std::string sha1::hash_to_hex(uint32_t hash[]) {
    std::string s;
    for(unsigned int i = 0; i < 5; i++) {
        s += int_to_hex(hash[i]);
    }
    return s;
}

std::string sha1::append_int(std::string message, uint64_t n) {
    for(int i = 0; i < 8; i++) {
        int shift = 56-8*i;
        message += (char) ((n>>shift) & 0xFF);
    }
    return message;
}

uint32_t sha1::rotate_left(uint32_t value, unsigned int n) {
    return (value << n) | (value >> (32-n));
}

void sha1::buffer_to_block(std::string buffer, uint32_t block[INTS_PER_BLOCK]) {
    /* convert every 4 bytes from buffer to a single 4-byte int in block */
    for (unsigned int i = 0; i < INTS_PER_BLOCK; i++) {
        block[i] = (buffer[4*i] & 0xFF) << 3*8;
        block[i] |= (buffer[4*i + 1] & 0xFF) << 2*8;
        block[i] |= (buffer[4*i + 2] & 0xFF) << 1*8;
        block[i] |= (buffer[4*i + 3] & 0xFF);
    }
}

/* one block is 512 bits = 16 x 32-bit ints */
void sha1::process_block(uint32_t block[INTS_PER_BLOCK], uint32_t hash[]) {
    uint32_t w[80];
    memcpy(w, block, 4*INTS_PER_BLOCK);
    for (unsigned int i = 16; i < 80; i++) {
        w[i] = rotate_left(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
    }
    /* a-e represent this block's hash */
    uint32_t a = hash[0];
    uint32_t b = hash[1];
    uint32_t c = hash[2];
    uint32_t d = hash[3];
    uint32_t e = hash[4];
    /* temp variables */
    uint32_t f, k, r;
    for (unsigned int i = 0; i < 80; i++) {
        if (i < 20) {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        }
        else if (i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        }
        else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        }
        else { // i < 80
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }
        r = rotate_left(a, 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = rotate_left(b, 30);
        b = a;
        a = r;
    }
    /* add this block's hash to final hash */
    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
    hash[4] += e;
}

std::string sha1::sha1(std::string message) {
    uint32_t hash[5];
    hash[0] = 0x67452301;
    hash[1] = 0xEFCDAB89;
    hash[2] = 0x98BADCFE;
    hash[3] = 0x10325476;
    hash[4] = 0xC3D2E1F0;
    
    uint64_t ml = message.length();
    ml *= 8; /* convert to length in bits */
    message += (char)0x80;
    /* append zeros until we reach -64 mod 512 (bits) = -8 mod 64 (bytes) */
    uint32_t ml_bytes = message.length();
    while ((ml_bytes%64) != (64-8)) {
        message += (char)0x00;
        ml_bytes++;
    }
    /* append original ml in bytes, now total message length is multiple of block size */
    message = append_int(message, ml);
    assert((message.length()%64) == 0);
    
    /* create a string stream to read one block of bytes at a time */
    std::istringstream is(message);
    std::string buffer;
    while (true) {
        char stream_buffer[BYTES_PER_BLOCK];
        is.read(stream_buffer, BYTES_PER_BLOCK);
        buffer.append(stream_buffer, is.gcount());
        if (buffer.size() == 0) {
            break;
        }
        else if (buffer.size() != BYTES_PER_BLOCK) {
            std::cout << "Error: buffer contains " << buffer.size() << " bytes" << std::endl;
            break;
        }
        uint32_t block[INTS_PER_BLOCK];
        buffer_to_block(buffer, block);
        buffer.clear();
        process_block(block, hash);
    }
    
    return hash_to_hex(hash);
}