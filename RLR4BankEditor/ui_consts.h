#ifndef UI_CONSTS_H
#define UI_CONSTS_H

#include <string>
#include <vector>

namespace ui {
    const std::vector<int> account_data_indices{
        0, 1, 2, 3, 10, 11, 13, 14, /* games & wins */
        7, 8, 9, /* boss kills */ 16, 15, /* high scores */
        4, 6, 5, /* saves, deaths, score */
        17, 18, 19, 20, /* camera */
        28, 29, 30, 31, 32, 27, 22, 23, 25, 24, 26, 21};
    const std::vector<int> account_data_ui_maxvals{
        190000, 100000, 110000, 120000, 90300000, 94000000, 96000000, 150000,
        160000, 170000, 180000, 190000, 200000, 210000, 220000, 230000, 240000,
        100, 360, 360, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const std::vector<int> runling_data_indices{
        0, 7, 1, 2, 3, 4, 5, 6, 8};
    const std::vector<int> runling_data_ui_maxvals{
        5, 8100000, 150, 150, 150, 5, 5, 75, 300};
}

#endif // UI_CONSTS_H
