//
//  chunk_id.cpp
//  insight
//
//  Created by Aaron Ishibashi on 4/18/22.
//
#include "chunk_id.hpp"
#include <insight/hash.hpp>
#include <iostream>
#include <cstring>

using namespace std;

const int CHUNK_ID_LENGTH = 4;

insight::wave::chunk_id::chunk_id() : m_id{0, 0, 0, 0, 0}, m_hash()
{
}

size_t
insight::wave::chunk_id::read(buffer &buf)
{
    size_t bytesRead = buf.read(reinterpret_cast<unsigned char *>(m_id), CHUNK_ID_LENGTH);
    m_hash = get_hash(m_id);

    return bytesRead;
}

void
insight::wave::chunk_id::clear() noexcept
{
    for (char *c = m_id, *end = c + CHUNK_ID_LENGTH;
         c != end;
         ++c)
    {
        *c = 0;
    }
}

std::string
insight::wave::chunk_id::to_string() const
{
    std::string ret;
    ret.reserve(CHUNK_ID_LENGTH);
    
    for (const char *c = m_id, *end = c + CHUNK_ID_LENGTH;
         c != end;
         ++c)
    {
        // null-terminator reached?
        if (!*c)
            break;

        ret += *c;
    }
    
    return ret;
}


// ============= Assignment ========================================

insight::wave::chunk_id::chunk_id(const char *str) : m_id{0, 0, 0, 0, 0}, m_hash()
{
    *this = str;
}

insight::wave::chunk_id::chunk_id(const chunk_id &other) : m_id{0, 0, 0, 0, 0}
{
    // copy ids
    for (int i = 0; i < CHUNK_ID_LENGTH || other.m_id[i] != '\0'; ++i)
    {
        m_id[i] = other.m_id[i];
    }

    // copy cached hash
    m_hash = other.m_hash;
}

insight::wave::chunk_id &
insight::wave::chunk_id::operator=(const char *str)
{
    if (str)
    {
        for (int i = 0; i < CHUNK_ID_LENGTH; ++i)
        {
            m_id[i] = str[i];

            if (str[i] == '\0')
                break;
        }

        m_hash = insight::get_hash(str);
    }
    else
    {
        m_id[0] = '\0';
    }

    return *this;
}

// ============ Comparison Operators ================================
bool
insight::wave::chunk_id::operator==(chunk_id &other) const
{
    for (int i = 0; i < CHUNK_ID_LENGTH; ++i)
    {
        if (m_id[i] != other.m_id[i])
            return false;
    }
    
    return true;
}

bool
insight::wave::chunk_id::operator!=(chunk_id &other) const
{
    return !(*this == other);
}


bool
insight::wave::chunk_id::operator==(const char *str) const
{
    if (str) // c-string pointing somewhere
    {
        for (const char *i = m_id, *j = str, *end = m_id + CHUNK_ID_LENGTH;
             i != end;
             ++i, ++j)
        {
            if (*j == '\0' || *i != *j) return false;
        }

        return true;
    }
    else // nullptr was passed
    {
        return false;
    }
}


bool
insight::wave::chunk_id::operator!=(const char *str) const
{
    return !(*this == str);
}

std::ostream &operator<<(std::ostream &os, const insight::wave::chunk_id &id)
{
    os << id.to_string();
    return os;
}
