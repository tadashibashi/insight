//
//  default_chunk.hpp
//  insight
//
//  Used to generically store data in a chunk that does not have support yet.
//
//  Created by Aaron Ishibashi on 4/18/22.
//
#pragma once
#ifndef insight_wave_default_chunk_hpp
#define insight_wave_default_chunk_hpp
#include <insight/wav/chunk.hpp>
#include <vector>

namespace insight::wave
{

class default_chunk : public chunk {
public:
    explicit default_chunk(const chunk_id &id) : chunk{id}, m_data{}
    {
        
    }
    
private:
    void read_impl(buffer &buf, size_t chunk_size) override;
    void clear_impl() override;
    std::vector<unsigned char> m_data;
};

}

#endif /* insight_wave_default_chunk_hpp */
