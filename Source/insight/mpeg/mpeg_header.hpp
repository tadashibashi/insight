//
//  mpeg_header.hpp
//  insight
//
//  Created by Aaron Ishibashi on 3/11/22.
//
#pragma once
#ifndef insight_mpeg_header_h
#define insight_mpeg_header_h
#include <bitset>

// Constants

typedef int MPEG_VERSION_;
const int MPEG_VERSION_RESERVED = 1;
const int MPEG_VERSION_2_5 = 0;
const int MPEG_VERSION_2   = 2;
const int MPEG_VERSION_1   = 3;
const int MPEG_VERSION_ERROR = -1;
typedef int MPEG_LAYER_;
const int MPEG_LAYER_3      = 1;
const int MPEG_LAYER_2      = 2;
const int MPEG_LAYER_1      = 3;
const int MPEG_LAYER_RESERVED = 0;
const int MPEG_LAYER_ERROR  = -1;
typedef int MPEG_CHANNEL_MODE_;
const int MPEG_CHANNEL_MODE_STEREO = 0;         // STEREO
const int MPEG_CHANNEL_MODE_JOINT_STEREO = 1;   // STEREO
const int MPEG_CHANNEL_MODE_DUAL_CHANNEL = 2;   // STEREO
const int MPEG_CHANNEL_MODE_SINGLE_CHANNEL = 3; // MONO

namespace insight
{
    /// @class mp3_header
/// @brief Represents the header of an mp3 frame
    class mpeg_header {
    public:
        mpeg_header(unsigned byte);
        mpeg_header();
        bool is_valid() const;
        void set(unsigned byte);
        void log_bits() const;

        /// @function sample_rate
        /// @returns Sample rate in Hertz, or -1 on bad value.
        int sample_rate() const;

        /// @function mpeg_version
        /// @returns 0 -> v2.5 || 1 -> reserved || 2 -> v2 || 3-> v1
        int version() const;

        /// @function mpeg_layer
        /// @returns 0 -> reserved || 1 -> layer3 || 2 -> layer2 || 3 -> layer1
        int layer() const;

        /// @function crc_protected
        /// @returns True: crc protected, the first 16 bits after this header contain the length of the frame in bytes. False: not crc protected, no crc bits exist.
        bool is_crc_protected() const;
        /// @function bit_rate
        /// @returns bit rate in kbps
        int bit_rate() const;

        bool is_copyrighted() const;

        /// @function compare
        /// @brief Checks another mpeg header if it could originate from same file.
        /// @return True: same file, False: different files
        bool check(mpeg_header &other) const;

        int frame_byte_length() const;

        bool is_padded() const;

        int mode_extension() const;

        bool is_original() const;

        int emphasis() const;

        int samples_per_second() const;

    private:
        unsigned m_frame_sync  : 11;
        unsigned m_version     : 2;
        unsigned m_layer       : 2;
        unsigned m_protection  : 1;
        unsigned m_bitrate     : 4;
        unsigned m_sample_rate : 2;
        unsigned m_padding     : 1;
        unsigned m_private_bit : 1;
        unsigned m_channel_mode: 2;
        unsigned m_mode_ext    : 2;
        unsigned m_copyrighted : 1;
        unsigned m_original    : 1;
        unsigned m_emphasis    : 2;

        int bit_rate_v1() const; // V1 bitrates
        int bit_rate_v2() const; // V2 bitrates, including V2.5

        /// @function
        /// @brief Assumes that this has been checked to be a V2 header, and validates particular bitrate and mode combinations that this mpeg file could possibly be.
        bool is_valid_v2() const;
    };
}


// Inline member functions

inline bool
insight::mpeg_header::is_valid() const
{
    if (m_version == MPEG_VERSION_2)
        if (!is_valid_v2())
            return false;
    return (m_frame_sync == 0b11111111111);
}

inline int
insight::mpeg_header::version() const
{
    return m_version;
}

inline int
insight::mpeg_header::layer() const
{
    return m_layer;
}

inline bool
insight::mpeg_header::is_crc_protected() const
{
    return !m_protection;
}

inline bool
insight::mpeg_header::is_copyrighted() const
{
    return m_copyrighted;
}

inline bool
insight::mpeg_header::is_padded() const
{
    return m_padding;
}

inline int
insight::mpeg_header::mode_extension() const
{
    return m_mode_ext;
}

inline bool
insight::mpeg_header::is_original() const
{
    return m_original;
}

inline int
insight::mpeg_header::emphasis() const
{
    return m_emphasis;
}

#endif /* insight_mpeg_header_h */
