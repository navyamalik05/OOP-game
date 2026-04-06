/**
* @file ids.h
 * @author Navya Malik
 * @brief Defines unique IDs for menu items.
 */

#ifndef PROJECT1_IDS_H
#define PROJECT1_IDS_H

/**
 * @enum IDs
 * @brief Unique identifiers for game menu items
 */
enum IDs {
    // Level Menu
    IDM_LEVEL_0 = wxID_HIGHEST + 1, ///< ID for Level 0
    IDM_LEVEL_1,                     ///< ID for Level 1
    IDM_LEVEL_2,                     ///< ID for Level 2
    IDM_LEVEL_3,                     ///< ID for Level 3
};

const int IDM_HELP_ABOUT = wxID_HIGHEST + 5; // Just giving it a unique ID

#endif //PROJECT1_IDS_H
