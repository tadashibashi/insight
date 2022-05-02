//
//  bext_chunk.cpp
//  ai_wavtime
//
//  Created by Aaron Ishibashi on 4/12/22.
//

#include "bext_chunk.hpp"
#include <iostream>

using namespace std;

const int SMPTE_UMID_LENGTH = 64;
const int BEXT_DESCRIPTION_LENGTH = 256;
const int BEXT_ORIGINATOR_LENGTH = 32;
const int BEXT_ORIGINATOR_REF_LENGTH = 32;
const int BEXT_ORIGINATION_DATE_LENGTH = 10;
const int BEXT_ORIGINATION_TIME_LENGTH = 8;
const int BEXT_RESERVED_LENGTH = 180;

namespace wave
{

bext_chunk::bext_chunk() : chunk{"bext"}
{
    clear();
}

void
bext_chunk::log()
{
    cout << "=== Bext Chunk Info ===\n" <<
    "description:      " << m_description << '\n' <<
    "oritinator:       " << m_originator << '\n' <<
    "originator ref:   " << m_originator_ref << '\n' <<
    "origination date: " << m_origination_date << '\n' <<
    "origination time: " << m_origination_time << '\n' <<
    "time ref low:     " << m_time_ref_low << '\n' <<
    "time ref high:    " << m_time_ref_high << '\n' <<
    "bwf version std:  " << m_version << '\n' <<
    "SMPTE Umid:       " << m_umid << '\n' <<
    "loudness value:   " << m_loudness_value << '\n' <<
    "loudness range:   " << m_loudness_range << '\n' <<
    "max truepeak lv:  " << m_max_truepeak_level << '\n' <<
    "max moment loudn: " << m_max_momentary_loudness << '\n' <<
    "max shortt loudn: " << m_max_shortterm_loudness << '\n' <<
    "reserved:         " << m_reserved << '\n' <<
    "coding history:   " << m_coding_history << '\n';
}

void
bext_chunk::read_impl(buffer &buf, size_t chunk_size)
{
    size_t bytes_read = 0;
    
    std::string t_description, t_originator, t_originator_ref, t_origination_date, t_origination_time, t_reserved, t_coding_history;
    uint32_t t_time_ref_low, t_time_ref_high;
    uint16_t t_version, t_loudness_value, t_loudness_range,
        t_max_truepeak_level, t_max_momentary_loudness, t_max_shortterm_loudness;
    char t_umid[64];

    
    bytes_read += read_string(buf, t_description, BEXT_DESCRIPTION_LENGTH);
    bytes_read += read_string(buf, t_originator, BEXT_ORIGINATOR_LENGTH);
    bytes_read += read_string(buf, t_originator_ref, BEXT_ORIGINATOR_REF_LENGTH);
    bytes_read += read_string(buf, t_origination_date, BEXT_ORIGINATION_DATE_LENGTH);
    bytes_read += read_string(buf, t_origination_time, BEXT_ORIGINATION_TIME_LENGTH);
    bytes_read += buf.read(t_time_ref_low);
    bytes_read += buf.read(t_time_ref_high);
    bytes_read += buf.read(t_version);
    bytes_read += buf.read((unsigned char *)t_umid, SMPTE_UMID_LENGTH);
    bytes_read += buf.read(t_loudness_value);
    bytes_read += buf.read(t_loudness_range);
    bytes_read += buf.read(t_max_truepeak_level);
    bytes_read += buf.read(t_max_momentary_loudness);
    bytes_read += buf.read(t_max_shortterm_loudness);
    bytes_read += read_string(buf, t_reserved, BEXT_RESERVED_LENGTH);
    bytes_read += read_string(buf, t_coding_history, chunk_size - bytes_read);
    
    m_description.swap(t_description);
    m_originator.swap(t_originator);
    m_originator_ref.swap(t_originator_ref);
    m_origination_date.swap(t_origination_date);
    m_origination_time.swap(t_origination_time);
    m_time_ref_low = t_time_ref_low;
    m_time_ref_high = t_time_ref_high;
    m_version = t_version;
    memcpy(m_umid, t_umid, SMPTE_UMID_LENGTH);
    m_loudness_value = t_loudness_value;
    m_loudness_range = t_loudness_range;
    m_max_truepeak_level = t_max_truepeak_level;
    m_max_momentary_loudness = t_max_momentary_loudness;
    m_max_shortterm_loudness = t_max_shortterm_loudness;
    m_reserved.swap(t_reserved);
    m_coding_history.swap(t_coding_history);
}

size_t
bext_chunk::read_string(buffer &buf, std::string &out_str, size_t length)
{
    char *chars = (char *)malloc(length);
    buf.read((unsigned char *)chars, length);

    std::string result;
    result.reserve(length);
    
    for (char *i = chars; i < chars + length; ++i)
    {
        if (result.empty())
        {
            if (isalnum(*i))
                result += *i;
        }
        else
        {
            if ((*i) == '\0') break;
            
            result += *i;
        }
    }
    
    free(chars);
    out_str.swap(result);
    return length;
}

uint64_t
bext_chunk::time_ref() const
{
    return ((uint64_t)m_time_ref_high << 32ull) + (uint64_t)m_time_ref_low;
}

void
bext_chunk::clear_impl()
{
    m_description.clear();
    m_originator.clear();
    m_originator_ref.clear();
    m_origination_date.clear();
    m_origination_time.clear();
    m_time_ref_low = 0;
    m_time_ref_high = 0;
    m_version = 0;
    memset(m_umid, 0, 64);
    m_loudness_value = 0;
    m_loudness_range = 0;
    m_max_truepeak_level = 0;
    m_max_momentary_loudness = 0;
    m_max_shortterm_loudness = 0;
    m_reserved.clear();
    m_coding_history.clear();
}

}
