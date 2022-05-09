//
//  buffer.hpp
//  insight
//
//  Created by Aaron Ishibashi on 3/10/22.
//
#pragma once
#ifndef insight_buffer_hpp
#define insight_buffer_hpp
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace insight
{
    /// @class buffer
    /// @abstract Handles reading (and eventually writing) from a buffer. This object does not own the data, but serves as a point of access into the data.
    class buffer {
    public:
        buffer(unsigned char *ptr, size_t size) : m_data{ptr}, m_current{ptr}, m_size{size} {}

        /// @function read
        /// @abstract Reads data from the buffer equal in length to the size of the object being passed.
        /// @returns The number of bytes read, sizeof(T)
        template <typename T>
        size_t read(T &obj);

        /// @function read
        /// @abstract Reads a number of bytes into a char buffer.
        /// @returns the number of bytes read, which is probably just the bytes parameter.
        size_t read(unsigned char *buf, size_t bytes);

        [[nodiscard]] size_t tellg() const { return (size_t)(m_current - m_data); }

        /// @function move
        /// @abstract Move the head to a literal position within the buffer. Use buffer::tellg or buffer::size to get relative positions
        void move(size_t pos);

        /// @function peek
        /// @brief Looks at the top char without
        [[nodiscard]] unsigned char peek() const { return *m_current; }

        /// @function free
        /// @brief While this object does not own the data in the buffer, this is a convenience function to delete the data, if necessary.
        void free();

        /// @function size
        /// @brief Gets the size of the buffer in bytes.
        [[nodiscard]] size_t size() const;

        unsigned char *current() { return m_current; }
        unsigned char *data() { return m_data; }
    private:
        unsigned char *m_data, *m_current;
        size_t m_size;
    };

}


// ======================================
// Inline member function implementation
// ======================================
inline void
insight::buffer::free()
{
    ::free(m_data);
    m_data = nullptr;
}

inline size_t
insight::buffer::size() const
{
    return m_size;
}

inline size_t
insight::buffer::read(unsigned char *buf, size_t bytes)
{
    if ((uintptr_t)(bytes + m_current - m_data) > (uintptr_t)m_size)
    {
        throw std::overflow_error("Attempted read would cause buffer to overflow.");
    }
    
    memcpy((void *)buf, m_current, bytes);
    m_current += bytes;
    return bytes;
}

template <typename T>
inline size_t
insight::buffer::read(T &obj)
{
    if ((uintptr_t)(sizeof(T) + m_current - m_data) > (uintptr_t)m_size)
    {
        throw std::overflow_error("Attempted read would cause buffer to overflow.");
    }
    
    memcpy((void *)&obj, m_current, sizeof(T));
    
    m_current += sizeof(T);
    return sizeof(T);
}


inline void
insight::buffer::move(size_t pos)
{
    if (pos > m_size)
    {
        throw std::overflow_error("Attempted move would cause buffer position to overflow.");
    }
    
    m_current = m_data + pos;
}

#endif /* insight_buffer_hpp */
