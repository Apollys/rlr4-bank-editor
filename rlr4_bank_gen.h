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
#include "sha1.h"

/* Constant definitions */
namespace rlr4 {
    // Account data
    std::vector<std::string> account_data_labels{
        "Normal Games", "Normal Wins", "Hard Games", "Hard Wins", "Total Saves",
        "Total Score", "Total Deaths", "Bot 2000 Kills", "Odin Kills", "Diablo Kills",
        "Insane Games", "Insane Wins", "Blank 3 (placeholder)", 
        "Time Games", "Time Wins", "Minigame High Score", "Time High Score",
        "Camera Distance", "Camera Rotation", "Camera Angle", "Camera Follow",
        "Hide Tips", "Hide HUD", "Hide Minimap", "Hide Energy Bar", "Hide Experience Bar",
        "Hide Menu", "WASD Movement", "Increase Distance Skill", "Decrease Distance Skill",
        "Rotate Right Skill", "Rotate Left Skill", "Follow Runling Skill"};
    std::vector<int> account_data_maxvals{
        190000, 100000, 110000, 120000, 90300000, 94000000, 96000000, 150000,
        160000, 170000, 180000, 190000, 200000, 210000, 220000, 230000, 240000,
        1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 10, 11, 12, 13, 14};
    std::vector<int> default_account_data{
        0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 90, 84, 0, 1, 1,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
    // Runling data
    std::vector<std::string> runling_data_labels{
        "Class", "Experience", "Energy Regeneration", "Maximum Energy", "Speed", 
        "Skill 1 Level", "Skill 2 Level", "Runling Level", "Remaining Points"};
    std::vector<int> runling_data_maxvals{
        300000, 8100000, 320000, 330000, 340000, 350000, 360000, 370000, 380000};
    // Encryption key
    std::string key = "WalkerKey";
    // Map author handle
    int author_handle = 417073;
    // Bank filename (used for computing signature)
    std::string bank_filename = "RunlingRun004";
}

namespace bankgen {
    Starcode starcode;
    
    std::string vtos(std::vector<int> &arr);
    std::string svtos(std::vector<std::string> &arr);
    int strings_equal(std::string first, std::string second);
    int string_startswith(std::string s, std::string prefix);
    std::string str_toupper(std::string s);
    
    int calc_points_used(int regen, int energy, int speed, int first_skill, int second_skill);
    std::string encode_account_data(std::vector<int> &data);
    std::string encode_runling_data(std::vector<int> &data);
    std::string encode_slots_data(std::vector<int> &data);
    std::string encode_checksums(int account_checksum, int character_checksum);
    int compute_account_checksum(std::vector<int> &account_data);
    int compute_character_checksum(std::vector<std::vector<int>> &all_character_data, int handle = 0);
    std::string compute_signature(int player_handle, std::vector<std::vector<std::vector<std::string>>> bank_tags);
    std::vector<std::string> make_key_vec(std::string name, std::string type, std::string value);
    std::string generate_bank(std::vector<int> account_data, std::vector<std::vector<int>> runling_data,
                        std::vector<int> slot_numbers, int handle);
    void write_to_file(std::string filename, std::string s);
}

#endif // RLR4_BANK_GEN_H