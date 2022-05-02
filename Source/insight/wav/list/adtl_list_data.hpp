//
//  list_adtl_subchunk.hpp
//  insight
//
//  Created by Aaron Ishibashi on 4/18/22.
//

#ifndef list_adtl_subchunk_hpp
#define list_adtl_subchunk_hpp
#include <insight/wav/chunk.hpp>
#include <map>
#include <string>

namespace wave
{
    struct adtl_list_data {
        adtl_list_data() : labels{} {}
        ~adtl_list_data() {}
        std::map<uint32_t, std::string> labels;
    };
}

#endif /* list_adtl_subchunk_hpp */
