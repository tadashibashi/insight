//
//  default_chunk.hpp
//  insight
//
//  Used to generically store data in a chunk that does not have support yet.
//
//  Created by Aaron Ishibashi on 4/18/22.
//

#ifndef default_chunk_hpp
#define default_chunk_hpp
#include <insight/wav/chunk.hpp>
#include <vector>

namespace wave
{

class default_chunk : public chunk {
public:
    explicit default_chunk(const chunk_id &id) : chunk{id}, m_data{}
    {
        
    }
    
private:
    void read_impl(buffer &buf, size_t chunk_size);
    void clear_impl();
    std::vector<unsigned char> m_data;
};

}

#endif /* default_chunk_hpp */
