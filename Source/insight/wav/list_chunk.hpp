//
//  list_chunk.hpp
//  insight
//
//  Created by Aaron Ishibashi on 4/16/22.
//
#ifndef list_chunk_hpp
#define list_chunk_hpp
#include <insight/wav/chunk.hpp>
#include <insight/wav/list/adtl_list_data.hpp>
#include <cstdlib>

namespace wave
{

class list_chunk : public chunk {
public:
    list_chunk() : chunk("LIST"), m_list_id{}, data{}
    {}
    ~list_chunk() = default;
    
    const chunk_id &sub_id() const { return m_list_id; }
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

#endif /* list_chunk_hpp */
