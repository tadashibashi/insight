/*!
 * @file chunk_id.hpp -- insight
 * @author Aaron Ishibashi
 *
 * @class insight::wave::chunk_id
 * @abstract Represents the four letters that identify a chunk
 *
 */
#pragma once
#ifndef insight_wave_chunk_id_hpp
#define insight_wave_chunk_id_hpp

#include <cstdlib>
#include <insight/buffer.hpp>
#include <iosfwd>

namespace insight::wave
{
    class chunk_id {
        friend std::ostream &operator<<(std::ostream &, const wave::chunk_id &);
    public:
        chunk_id();
        explicit chunk_id(const char *);

        // Copying
        chunk_id(const chunk_id &);
        chunk_id &operator=(const char *);

        /// Read the four header bytes into this object.
        size_t read(buffer &buf);

        // Convert the internal bytes to a 4-lettered string
        [[nodiscard]] std::string to_string() const;

        // Clear the internals
        void clear();

        // chunk_id == chunk_id
        bool operator==(chunk_id &) const;
        bool operator!=(chunk_id &) const;
        // chunk_id == const char *
        bool operator==(const char *) const;
        bool operator!=(const char *) const;
        
    private:
        char m_id[4];
    };
}

#endif /* insight_wave_chunk_id_hpp */
