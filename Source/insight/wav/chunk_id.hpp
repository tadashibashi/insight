//
//  chunk_id.hpp
//  insight
//
//  Represents the four letters that identify a chunk
//
//  Created by Aaron Ishibashi on 4/18/22.
//
#ifndef chunk_id_hpp
#define chunk_id_hpp
#include <cstdlib>
#include <insight/buffer.hpp>
#include <iosfwd>

namespace wave
{
    class chunk_id {
        friend std::ostream &operator<<(std::ostream &, const wave::chunk_id &);
    public:
        chunk_id();
        explicit chunk_id(const char *);
        explicit chunk_id(const chunk_id &);
        
        size_t read(buffer &buf);
        std::string to_string() const;
        chunk_id &operator=(const char *);
        void clear();
        
        /// @abstract Comparison with another chunk_id
        bool operator==(chunk_id &) const;
        bool operator!=(chunk_id &) const;
        /// @abstract Comparison with a c-string
        bool operator==(const char *) const;
        bool operator!=(const char *) const;
        
    private:
        char m_id[4];
    };
}

#endif /* chunk_id_hpp */
