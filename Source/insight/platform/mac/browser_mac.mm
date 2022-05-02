//
//  browser_mac.cpp
//  audio_file_insight
//
//  Created by Aaron Ishibashi on 3/12/22.
//
#include "browser_mac.hpp"
#include "Finder.h"

void get_selected_browser_paths(std::vector<std::string> &paths)
{
    FinderApplication *finder = [SBApplication applicationWithBundleIdentifier:@"com.apple.finder"];
    SBElementArray *selection = [[finder selection] get];
    NSArray *items = [selection arrayByApplyingSelector:@selector(URL)];
    
    std::vector<std::string> temp;
    for (NSString *item in items)
    {
        NSURL *url = [NSURL URLWithString:item];
        temp.emplace_back((char *)[[url path] UTF8String]);
    }
    
    std::swap(temp, paths);
}

std::string get_filename_from_path(const std::string &path)
{
    std::string filename;
    
    auto pos = path.find_last_of('/');
    if (pos != std::string::npos)
    {
        filename = path.substr(pos + 1);
    }
    else
    {
        filename = path;
    }
    
    return filename;
}

std::string get_file_ext_from_path(const std::string &path)
{
    std::string file_extension;
    
    auto pos = path.find_last_of('.');
    if (pos != std::string::npos)
        file_extension = path.substr(pos + 1);
    
    return file_extension;
}
