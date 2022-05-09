//
//  cue_chunk.hpp
//  insight
//
//  Created by Aaron Ishibashi on 4/12/22.
//
#pragma once
#ifndef insight_wave_cue_chunk_hpp
#define insight_wave_cue_chunk_hpp

#include <insight/wav/chunk.hpp>
#include <string>
#include <vector>

namespace insight::wave
{
    struct cue {
        std::string name;
        uint32_t id;
        uint32_t position;
        char chunk_type[5];
        uint32_t chunk_start;
        uint32_t block_start;
        uint32_t sample_start;
        
        void log() const;
    };

    class cue_chunk : public chunk {
    public:
        cue_chunk();
        
        void log() const;
        void set_name(uint32_t id, std::string name);
        
        cue *begin() { return &(*m_cues.begin()); }
        cue *end() { return &(*m_cues.end()); }

    private:
        void read_impl(buffer &buf, size_t chunk_size) override;
        
        std::vector<cue> m_cues;
    };
}

#endif /* insight_wave_cue_chunk_hpp */
