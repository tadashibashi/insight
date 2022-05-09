//
//  chunk.cpp
//  insight
//
//  Created by Aaron Ishibashi on 4/18/22.
//
#include "chunk.hpp"
#include <iostream>

size_t
insight::wave::chunk::read(buffer &buf, size_t chunk_size)
{
    if (was_read())
    {
        std::cout << "Warning: Blocked attempt to re-read into a \"" << m_id.to_string() << "\" chunk that was already read.\n";
        return 0;
    }
    
    size_t bytes_read, start = buf.tellg();
    read_impl(buf, chunk_size);
    bytes_read = buf.tellg() - start;
    
    if (bytes_read != chunk_size) // correct any misreads, output dev warning.
    {
        std::cout << "Warning: bytes read by \"" << m_id.to_string() << "\" chunk does not match chunk size\n";
        buf.move(buf.tellg() + chunk_size - bytes_read);
    }
    
    m_size = chunk_size;
    m_was_read = true;
    return bytes_read;
}

void
insight::wave::chunk::clear()
{
    clear_impl();
    m_was_read = false;
}
