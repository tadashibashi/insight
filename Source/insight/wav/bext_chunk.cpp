/*!
 * @file bext_chunk.cpp – insight
 * @abstract class bext_chunk implementation file
 *
 */
#include "bext_chunk.hpp"
#include <iostream>

using namespace std;

static const int SMPTE_UMID_LENGTH = 64;
static const int BEXT_DESCRIPTION_LENGTH = 256;
static const int BEXT_ORIGINATOR_LENGTH = 32;
static const int BEXT_ORIGINATOR_REF_LENGTH = 32;
static const int BEXT_ORIGINATION_DATE_LENGTH = 10;
static const int BEXT_ORIGINATION_TIME_LENGTH = 8;
static const int BEXT_RESERVED_LENGTH = 180;

namespace insight::wave
{

bext_chunk::bext_chunk() : chunk{"bext"},
    m_description(), m_time_ref_high(), m_time_ref_low(), m_loudness_range(),
    m_loudness_value(), m_max_momentary_loudness(), m_max_shortterm_loudness(),
    m_max_truepeak_level(), m_umid{0}, m_version()
{
    clear();
}

void
bext_chunk::log(std::ostream *os) const
{
    std::ostream &output = (os) ? *os : std::cout;
    output << *this;
}

void
bext_chunk::read_impl(buffer &buf, size_t chunk_size)
{
    // Read each element into temporary variables first
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

    // There were no exceptions, commit changes
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

std::ostream &operator<<(ostream &os, const insight::wave::bext_chunk &bext)
{
    os <<
       "description:      " << bext.description() << '\n' <<
       "oritinator:       " << bext.originator() << '\n' <<
       "originator ref:   " << bext.originator_ref() << '\n' <<
       "origination date: " << bext.origination_date() << '\n' <<
       "origination time: " << bext.origination_time() << '\n' <<
       "time ref low:     " << bext.time_ref_low() << '\n' <<
       "time ref high:    " << bext.time_ref_high() << '\n' <<
       "bwf version std:  " << bext.version() << '\n' <<
       "SMPTE Umid:       " << bext.umid() << '\n' <<
       "loudness value:   " << bext.loudness_value() << '\n' <<
       "loudness range:   " << bext.loudness_range() << '\n' <<
       "max truepeak lv:  " << bext.max_truepeak_level() << '\n' <<
       "max moment loudn: " << bext.max_momentary_loudness() << '\n' <<
       "max shortt loudn: " << bext.max_shortterm_loudness() << '\n' <<
       "reserved:         " << bext.reserved() << '\n' <<
       "coding history:   " << bext.coding_history() << '\n';
    return os;
}
