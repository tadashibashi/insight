//
//  list_adtl_subchunk.hpp
//  insight
//
//  Created by Aaron Ishibashi on 4/18/22.
//
#pragma once
#ifndef insight_wave_list_adtl_subchunk_hpp
#define insight_wave_list_adtl_subchunk_hpp

#include <insight/wav/chunk.hpp>
#include <map>
#include <string>

namespace insight::wave
{
    struct adtl_list_data {
        adtl_list_data() : labels{} {}
        std::map<uint32_t, std::string> labels;
    };
}

#endif /* insight_wave_list_adtl_subchunk_hpp */
