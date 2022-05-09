//
//  fmt_chunk.hpp
//  insight
//
//  Created by Aaron Ishibashi on 4/18/22.
//
#pragma once
#ifndef insight_fmt_chunk_hpp
#define insight_fmt_chunk_hpp

#include <insight/wav/chunk.hpp>

namespace insight::wave
{

    class fmt_chunk : public chunk {
    public:
        fmt_chunk() : chunk("fmt "),
            m_audio_format{}, m_num_channels{}, m_sample_rate{},
            m_byte_rate{}, m_block_align{}, m_bits_per_sample{}
        {}
        
        [[nodiscard]] uint16_t audio_format() const { return m_audio_format; }
        [[nodiscard]] uint16_t num_channels() const { return m_num_channels; }
        [[nodiscard]] uint32_t sample_rate() const { return m_sample_rate; }
        [[nodiscard]] uint32_t byte_rate() const { return m_byte_rate; }
        [[nodiscard]] uint16_t block_align() const { return m_block_align; }
        [[nodiscard]] uint16_t bits_per_sample() const { return m_bits_per_sample; }
    private:
        void read_impl(buffer &buf, size_t chunk_size) override;
        uint16_t m_audio_format;
        uint16_t m_num_channels;
        uint32_t m_sample_rate;
        uint32_t m_byte_rate;
        uint16_t m_block_align;
        uint16_t m_bits_per_sample;
    };

}

#endif /* insight_fmt_chunk_hpp */
