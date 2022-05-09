//
//  browser_mac.cpp
//  audio_file_insight
//
//  Created by Aaron Ishibashi on 3/12/22.
//
#include "browser_mac.hpp"
#include "Finder.h"

using std::string, std::vector, std::swap;

void
insight::get_selected_browser_paths(vector<string> &paths)
{
    FinderApplication *finder = [SBApplication applicationWithBundleIdentifier:@"com.apple.finder"];
    SBElementArray *selection = [[finder selection] get];
    NSArray *items = [selection arrayByApplyingSelector:@selector(URL)];
    
    vector<string> temp;
    for (NSString *item in items)
    {
        NSURL *url = [NSURL URLWithString:item];
        temp.emplace_back((char *)[[url path] UTF8String]);
    }
    
    swap(temp, paths);
}

string
insight::get_filename_from_path(const string &path)
{
    string filename;
    
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

string
insight::get_file_ext_from_path(const string &path)
{
    string file_extension;
    
    auto pos = path.find_last_of('.');
    if (pos != string::npos)
        file_extension = path.substr(pos + 1);
    
    return file_extension;
}
