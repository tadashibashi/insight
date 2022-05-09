//
//  platform.hpp
//  insight
//
//  Created by Aaron Ishibashi on 4/11/22.
//
#pragma once
#ifndef insight_platform_hpp
#define insight_platform_hpp

#include <vector>
#include <string>

// OS-specific functions included here
#if defined(__APPLE__)
    #include <insight/platform/mac/browser_mac.hpp>
#else
    #error Platform not supported!
#endif

namespace insight
{
    void collect_files(const std::string &in_path, std::vector<std::string> &out_paths);
}


#endif /* platform_common_hpp */
