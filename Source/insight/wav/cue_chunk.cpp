//
//  cue_chunk.cpp
//  insight
//
//  Created by Aaron Ishibashi on 4/12/22.
//
#include "cue_chunk.hpp"
#include <iostream>

namespace wave
{

void
cue::log() const
{
    std::cout << "cue " << id << " info:\n"
        "name:        " << name << '\n' <<
        "position:    " << position << '\n' <<
        "chunk type:  " << chunk_type << '\n' <<
        "chunk start: " << chunk_start << '\n' <<
        "block start: " << block_start << '\n' <<
        "sampl start: " << sample_start << '\n';
}

cue_chunk::cue_chunk() : chunk("cue "), m_cues{}
{
    
}

void
cue_chunk::log() const
{
    for (const cue &c : m_cues)
    {
        c.log();
    }
}

void
cue_chunk::set_name(uint32_t id, std::string name)
{
    for (cue &c : m_cues)
    {
        if (c.id == id)
        {
            c.name = std::move(name);
            break;
        }
    }
}

void
cue_chunk::read_impl(buffer &buf, size_t chunk_size)
{
    std::vector<cue> cues;
    
    uint32_t cue_count;
    buf.read(cue_count);
    cues.reserve(cue_count);
    
    for (int i = 0; i < cue_count; ++i)
    {
        cue c;
        
        // unique id number used by other chunks to identify cue
        buf.read(c.id);
        
        // if no playlist chunk, this is 0, otherwise contains sample position that the cue point occurs
        buf.read(c.position);
        
        // either "data" or "slnt" whether cue occurs in a data or silent chunk
        buf.read(reinterpret_cast<unsigned char *>(c.chunk_type), 4);
        c.chunk_type[4] = '\0';
        
        // position of the start of the data chunk that contains the cue point. if there is a wave list chunk, this value is the byte position of the chunk that contains the cue. if there is no wave list chunk (wavl), there is only one data chunk and this value will be zero.
        buf.read(c.chunk_start);
        
        // byte position of the cue in the data or sInt chunk. In uncompressed PCM files the count will be from the beginning of the chunk's data. In a compressed file, you'll have to calculate the decompression.
        buf.read(c.block_start);
        
        // The position of the cue in number of bytes from the start of the block.
        // how is this different from block start??
        buf.read(c.sample_start);
        
        cues.emplace_back(c);
    }
    
    m_cues.swap(cues);
}

}

