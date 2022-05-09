//
//  chunk.hpp
//  insight
//
//  Created by Aaron Ishibashi on 4/18/22.
//
#ifndef insight_wave_chunk_hpp
#define insight_wave_chunk_hpp

#include <insight/buffer.hpp>
#include <insight/wav/chunk_id.hpp>
#include <cstdint>

namespace insight::wave
{

class chunk {
public:
    explicit chunk(const char *chunk_id)
        : m_was_read{}, m_size{}, m_id{chunk_id} {}
    explicit chunk(const chunk_id &cid) : m_was_read{}, m_size{}, m_id{cid} {}
    chunk() = delete;
    virtual ~chunk() { clear(); }
    
    size_t read(buffer &buf, size_t chunk_size);
    
    /// @abstract Cleans the chunk of any internal memory usage.
    /// Automatically called during destruction, or you may optionally call this before reading new
    /// information into the chunk.
    void clear();
    
    [[nodiscard]] bool was_read() const { return m_was_read; }
    [[nodiscard]] const chunk_id &id() const { return m_id; }
    
    /// @returns the size of the chunk's data. Add 8 bytes to include the identifying header and chunk size itself.
    [[nodiscard]] size_t size() const { return m_size; }
    
private:

    
    /// @abstract The chunk's Implementation of data reading.
    /// Abstract, must be overridden.
    virtual void read_impl(buffer &buf, size_t chunk_size) = 0;
    
    /// @abstract Please override to free any dynamic memory or internal data.
    virtual void clear_impl() {}
    
    chunk_id m_id;
    bool m_was_read;
    size_t m_size;
};

}

#endif /* insight_wave_chunk_hpp */
