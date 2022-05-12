//
//  list_chunk.hpp
//  insight
//
//  Created by Aaron Ishibashi on 4/16/22.
//
#pragma once
#ifndef insight_list_chunk_hpp
#define insight_list_chunk_hpp

#include <insight/wav/chunk.hpp>
#include "list/adtl_list_data.hpp"
#include <cstdlib>

namespace insight::wave
{

class list_chunk : public chunk {
public:
    list_chunk() : chunk("LIST"), m_list_id{}, data{}
    {}
    
    [[nodiscard]] const chunk_id &sub_id() const { return m_list_id; }
    union {
        const char *null;
        adtl_list_data *adtl;
    } data;
private:
    void read_impl(buffer &buf, size_t chunk_size) override;
    void clear_impl() override;
    
    chunk_id m_list_id;
    


};

}

#endif /* insight_list_chunk_hpp */
