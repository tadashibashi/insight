//
//  platform_common.cpp
//  insight
//
//  Created by Aaron Ishibashi on 4/11/22.
//
#include "platform.hpp"
#include <filesystem>

using namespace std;

void
insight::collect_files(const string &in_path, vector<string> &out_paths)
{
    // Avoid all system files that begin with '.', and definitely avoid the root directory from being selected due to any sort of bug that would pass an empty path.
    string base_filename = in_path.substr(in_path.find_last_of('/') + 1);
    if (in_path.empty() || base_filename[0] == '.')
        return;
    
    // Add file if it is a regular file
    if (filesystem::is_regular_file(in_path))
    {
        out_paths.emplace_back(in_path);
    }
    else if (filesystem::is_directory(in_path)) // check directory entries
    {
        for (auto &dir : filesystem::directory_iterator(in_path))
        {
            base_filename = dir.path().filename().string();
            if (base_filename.empty() || base_filename[0] == '.') continue;
            
            if (dir.is_regular_file())    // add regular files
                out_paths.emplace_back(dir.path());
            else if (dir.is_directory())  // recurse other directories
                collect_files(dir.path(), out_paths);
        }
    }
}
