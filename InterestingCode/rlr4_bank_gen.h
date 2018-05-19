#ifndef RLR4_BANK_GEN_H
#define RLR4_BANK_GEN_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cctype> // toupper, isspace

#include "starcode.h"

namespace bankgen {
    const static std::string OUTPUT_FILENAME = "RunlingRun004.SC2Bank";
    static Starcode starcode;
    
    std::string vtos(std::vector<int> &arr);
    std::string svtos(std::vector<std::string> &arr);
    int strings_equal(std::string first, std::string second);
    int string_startswith(std::string s, std::string prefix);
    std::string str_toupper(std::string s);

    int calc_points_used(int regen, int energy, int speed, int first_skill, int second_skill);
    int calc_points_used(std::vector<int> data);
    std::string encode_account_data(std::vector<int> &data);
    std::string encode_runling_data(std::vector<int> &data);
    std::string encode_slots_data(std::vector<int> &data);
    std::string encode_checksums(int account_checksum, int character_checksum);
    int compute_account_checksum(std::vector<int> &account_data);
    int compute_character_checksum(std::vector<std::vector<int>> &all_character_data, int handle = 0);
    std::string compute_signature(int player_handle, int server, std::vector<std::vector<std::vector<std::string>>> bank_tags);
    std::vector<std::string> make_key_vec(std::string name, std::string type, std::string value);
    std::string generate_bank(std::vector<int> account_data, std::vector<std::vector<int>> runling_data,
                        std::vector<int> slot_numbers, int handle, int server);
    void write_to_file(std::string filename, std::string s);
}

#endif // RLR4_BANK_GEN_H
