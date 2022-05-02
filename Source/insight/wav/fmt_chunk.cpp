//
//  fmt_chunk.cpp
//  insight
//
//  Created by Aaron Ishibashi on 4/18/22.
//

#include "fmt_chunk.hpp"
#include <iostream>

void
wave::fmt_chunk::read_impl(buffer &buf, size_t chunk_size)
{
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    buf.read(audio_format);
    buf.read(num_channels);
    buf.read(sample_rate);
    buf.read(byte_rate);
    buf.read(block_align);
    buf.read(bits_per_sample);
    
    m_audio_format = audio_format;
    m_num_channels = num_channels;
    m_sample_rate = sample_rate;
    m_byte_rate = byte_rate;
    m_block_align = block_align;
    m_bits_per_sample = bits_per_sample;
}
