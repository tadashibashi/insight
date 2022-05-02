//
//  browser_mac.hpp
//  audio_file_insight
//
//  Created by Aaron Ishibashi on 3/12/22.
//

#ifndef browser_mac_hpp
#define browser_mac_hpp
#include <string>
#include <vector>

/// @function get_filename_from_path
/// @brief Gets a list of filepaths selected by the user in Finder.
void get_selected_browser_paths(std::vector<std::string> &paths);

/// @function get_filename_from_path
/// @brief Mac-specific filename extraction from a full filepath.
std::string get_filename_from_path(const std::string &path);

/// @function get_filename_from_path
/// @brief Mac-specific file extension extraction from a full filepath.
/// @returns The file extension (without the '.') or an empty string if there was no file extension.
std::string get_file_ext_from_path(const std::string &path);

#endif /* browser_mac_hpp */
