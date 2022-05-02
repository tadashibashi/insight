//
//  default_chunk.cpp
//  insight
//
//  Created by Aaron Ishibashi on 4/18/22.
//

#include "default_chunk.hpp"

void
wave::default_chunk::read_impl(buffer &buf, size_t chunk_size)
{
    std::vector<unsigned char> data(chunk_size);
    buf.read(reinterpret_cast<unsigned char *>(data.data()), chunk_size);
    m_data.swap(data);
}

void
wave::default_chunk::clear_impl()
{
    m_data.clear();
}
