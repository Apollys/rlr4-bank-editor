#ifndef RLR4_H
#define RLR4_H

#include <string>
#include <vector>

/* Constant definitions */
namespace rlr4 {
    // Account data
    const std::vector<std::string> account_data_labels{
        "Normal Games", "Normal Wins", "Hard Games", "Hard Wins", "Total Saves",
        "Total Score", "Total Deaths", "Bot 2000 Kills", "Odin Kills", "Diablo Kills",
        "Insane Games", "Insane Wins", "Blank 3 (placeholder)",
        "Time Games", "Time Wins", "Minigame High Score", "Time High Score",
        "Camera Distance", "Camera Rotation", "Camera Angle", "Camera Follow",
        "Hide Tips", "Hide HUD", "Hide Minimap", "Hide Energy Bar", "Hide Experience Bar",
        "Hide Menu", "WASD Movement", "Increase Distance Skill", "Decrease Distance Skill",
        "Rotate Right Skill", "Rotate Left Skill", "Follow Runling Skill"};
    const std::vector<int> account_data_maxvals{
        190000, 100000, 110000, 120000, 90300000, 94000000, 96000000, 150000,
        160000, 170000, 180000, 190000, 200000, 210000, 220000, 230000, 240000,
        1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 10, 11, 12, 13, 14};
    const std::vector<int> default_account_data{
        0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 90, 84, 0, 1, 1,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
    // Runling data
    const std::vector<std::string> runling_data_labels{
        "Class", "Experience", "Energy Regeneration", "Maximum Energy", "Speed",
        "Skill 1 Level", "Skill 2 Level", "Runling Level", "Remaining Points"};
    const std::vector<int> runling_data_maxvals{
        300000, 8100000, 320000, 330000, 340000, 350000, 360000, 370000, 380000};
    const std::vector<int> default_runling_data{
        0, 72671, 0, 0, 100, 5, 5, 75, 160};
    // Encryption key
    const std::string key = "WalkerKey";
    // Map author handle
    const int author_handle_NA = 417073;
    const int author_handle_EU = 3564862;
    const std::string handle_prefix_NA = "1-S2-1-";
    const std::string handle_prefix_EU = "2-S2-1-";
    const int REGION_NA = 1;
    const int REGION_EU = 2;
    // Bank filename (used for computing signature)
    const std::string bank_filename = "RunlingRun004";
}

#endif // RLR4_H
