//
//  mp3_header.cpp
//  audio_file_insight
//
//  Created by Aaron Ishibashi on 3/11/22.
//
#include "mpeg_header.hpp"
#include <iostream>
#include "swap_endian.hpp"

const int MPEG_L1_SAMPLES_PER_SECOND = 384;
const int MPEG_L2AND3_SAMPLES_PER_SECOND = 1152;

mpeg_header::mpeg_header(unsigned four_bytes) : m_frame_sync{0}
{
   set(four_bytes);
}

mpeg_header::mpeg_header() : m_frame_sync{0}
{
    
}

void
mpeg_header::set(unsigned four_bytes)
{
    if (!is_big_endian())
        four_bytes = swap_endian(four_bytes);
    
    m_frame_sync   = four_bytes >> 21 & 0b11111111111;
    m_version      = four_bytes >> 19 & 0b11;
    m_layer        = four_bytes >> 17 & 0b11;
    m_protection   = four_bytes >> 16 & 0b1;
    m_bitrate      = four_bytes >> 12 & 0b1111;
    m_sample_rate  = four_bytes >> 10 & 0b11;
    m_padding      = four_bytes >> 9 & 0b1;
    m_private_bit  = four_bytes >> 8 & 0b1;
    m_channel_mode = four_bytes >> 6 & 0b11;
    m_mode_ext     = four_bytes >> 4 & 0b11;
    m_copyrighted  = four_bytes >> 3 & 0b1;
    m_original     = four_bytes >> 2 & 0b1;
    m_emphasis     = four_bytes & 0b11;
}

int
mpeg_header::samples_per_second() const
{
   int ret;
   switch (layer())
   {
       case MPEG_LAYER_1:
           ret = MPEG_L1_SAMPLES_PER_SECOND;
           break;
      
       case MPEG_LAYER_2:
       case MPEG_LAYER_3:
           ret = MPEG_L2AND3_SAMPLES_PER_SECOND;
           break;
           
       default:
           ret = -1; // faulty layer value
           break;
   }
   
   return ret;
}

void
mpeg_header::log_bits() const
{
    std::cout
    << std::bitset<11>{m_frame_sync} << ": frame sync\n"
    << std::bitset<2>{m_version}     << ": version\n"
    << std::bitset<2>{m_layer}       << ": layer\n"
    << std::bitset<1>{m_protection}  << ": protection\n"
    << std::bitset<4>{m_bitrate}     << ": bitrate\n"
    << std::bitset<2>{m_sample_rate} << ": sample rate\n"
    << std::bitset<1>{m_padding}     << ": padding\n"
    << std::bitset<1>{m_private_bit} << ": private bit\n"
    << std::bitset<2>{m_channel_mode}<< ": channel mode\n"
    << std::bitset<2>{m_mode_ext}    << ": mode extension\n"
    << std::bitset<1>{m_copyrighted} << ": copyrighted\n"
    << std::bitset<1>{m_original}    << ": original\n"
    << std::bitset<2>{m_emphasis}    << ": emphasis\n";
}

int
mpeg_header::sample_rate() const
{
    int temp;
    switch(m_version)
    {
        case MPEG_VERSION_1:
            switch(m_sample_rate)
            {
                case 0: temp = 44100; break;
                case 1: temp = 48000; break;
                case 2: temp = 32000; break;
                default: temp = -1; break;
            }
            break;
        case MPEG_VERSION_2:
            switch(m_sample_rate)
            {
                case 0: temp = 22050; break;
                case 1: temp = 24000; break;
                case 2: temp = 16000; break;
                default: temp = -1; break;
            }
            break;
        case MPEG_VERSION_2_5:
            switch(m_sample_rate)
            {
                case 0: temp = 11025; break;
                case 1: temp = 12000; break;
                case 2: temp = 8000; break;
                default: temp = -1; break;
            }
            break;
        default:
            temp = -1;
            break;
    }
    
    return temp;
}

int
mpeg_header::bit_rate_v1() const
{
    int temp;
    switch(m_layer)
    {
        case MPEG_LAYER_1:
            switch(m_bitrate)
            {
                case 0b0001: temp = 32; break;
                case 0b0010: temp = 64; break;
                case 0b0011: temp = 96; break;
                case 0b0100: temp = 128; break;
                case 0b0101: temp = 160; break;
                case 0b0110: temp = 192; break;
                case 0b0111: temp = 224; break;
                case 0b1000: temp = 256; break;
                case 0b1001: temp = 288; break;
                case 0b1010: temp = 320; break;
                case 0b1011: temp = 352; break;
                case 0b1100: temp = 384; break;
                case 0b1101: temp = 416; break;
                case 0b1110: temp = 448; break;
                default: temp = -1;
            }
            break;
        case MPEG_LAYER_2:
            switch(m_bitrate)
            {
                case 0b0001: temp = 32; break;
                case 0b0010: temp = 48; break;
                case 0b0011: temp = 56; break;
                case 0b0100: temp = 64; break;
                case 0b0101: temp = 80; break;
                case 0b0110: temp = 96; break;
                case 0b0111: temp = 112; break;
                case 0b1000: temp = 128; break;
                case 0b1001: temp = 160; break;
                case 0b1010: temp = 192; break;
                case 0b1011: temp = 224; break;
                case 0b1100: temp = 256; break;
                case 0b1101: temp = 320; break;
                case 0b1110: temp = 384; break;
                default: temp = -1;
            }
            break;
        case MPEG_LAYER_3:
            switch(m_bitrate)
            {
                case 0b0001: temp = 32; break;
                case 0b0010: temp = 40; break;
                case 0b0011: temp = 48; break;
                case 0b0100: temp = 56; break;
                case 0b0101: temp = 64; break;
                case 0b0110: temp = 80; break;
                case 0b0111: temp = 96; break;
                case 0b1000: temp = 112; break;
                case 0b1001: temp = 128; break;
                case 0b1010: temp = 160; break;
                case 0b1011: temp = 192; break;
                case 0b1100: temp = 224; break;
                case 0b1101: temp = 256; break;
                case 0b1110: temp = 320; break;
                default: temp = -1;
            }
            break;
        default:
            temp = -1;
            break;
    }
    
    return temp;
}

int
mpeg_header::bit_rate_v2() const
{
    int temp;
    switch(m_layer)
    {
        case MPEG_LAYER_1:
            switch(m_bitrate)
            {
                case 0b0001: temp = 32; break;
                case 0b0010: temp = 48; break;
                case 0b0011: temp = 56; break;
                case 0b0100: temp = 64; break;
                case 0b0101: temp = 80; break;
                case 0b0110: temp = 96; break;
                case 0b0111: temp = 112; break;
                case 0b1000: temp = 128; break;
                case 0b1001: temp = 144; break;
                case 0b1010: temp = 160; break;
                case 0b1011: temp = 176; break;
                case 0b1100: temp = 192; break;
                case 0b1101: temp = 224; break;
                case 0b1110: temp = 256; break;
                default: temp = -1;
            }
            break;
        case MPEG_LAYER_2:
        case MPEG_LAYER_3:
            switch(m_bitrate)
            {
                case 0b0001: temp = 8; break;
                case 0b0010: temp = 16; break;
                case 0b0011: temp = 24; break;
                case 0b0100: temp = 32; break;
                case 0b0101: temp = 40; break;
                case 0b0110: temp = 48; break;
                case 0b0111: temp = 56; break;
                case 0b1000: temp = 64; break;
                case 0b1001: temp = 80; break;
                case 0b1010: temp = 96; break;
                case 0b1011: temp = 112; break;
                case 0b1100: temp = 128; break;
                case 0b1101: temp = 144; break;
                case 0b1110: temp = 160; break;
                default: temp = -1;
            }
            break;
        default:
            temp = -1;
            break;
    }
    
    return temp;
}

int
mpeg_header::bit_rate() const
{
    int temp;
    switch(m_version)
    {
        case MPEG_VERSION_1:
            temp = bit_rate_v1();
            break;
        case MPEG_VERSION_2:
        case MPEG_VERSION_2_5:
            temp = bit_rate_v2();
            break;
        default:
            temp = -1;
            break;
    }
    
    return temp;
}

bool
mpeg_header::check(mpeg_header &other) const
{
    return (is_valid() && other.is_valid() &&
            m_version == other.m_version &&
            m_layer == other.m_layer &&
            m_protection == other.m_protection &&
            m_sample_rate == other.m_sample_rate &&
            m_channel_mode == other.m_channel_mode &&
            //m_mode_ext == other.m_mode_ext &&
            m_copyrighted == other.m_copyrighted &&
            m_original == other.m_original);
}

bool
mpeg_header::is_valid_v2() const
{
    // Check that bitrates for mpeg version 2 match appropriate channel mode
    int bitrate = bit_rate_v2();
    if (bitrate == 32 || bitrate == 48 || bitrate == 56 || bitrate == 80)
    {
       if (m_channel_mode != MPEG_CHANNEL_MODE_SINGLE_CHANNEL)
           return false;
    }

    if (bitrate == 224 || bitrate == 256 || bitrate == 320 || bitrate == 384)
    {
       if (m_channel_mode == MPEG_CHANNEL_MODE_SINGLE_CHANNEL)
           return false;
    }

    return true;
}

int
mpeg_header::frame_byte_length() const
{
    int ret;
    if (m_layer == MPEG_LAYER_1)
    {
        ret = (int)(12.f * (float)bit_rate() * 1000.f / (float)sample_rate() + (int)is_padded()) * 4;
    }
    else if (m_layer == MPEG_LAYER_2 || m_layer == MPEG_LAYER_3)
    {
        ret = (int)(144.f * (float)bit_rate() * 1000.f / (float)sample_rate() + (int)is_padded());
    }
    else
    {
        ret = -1;
    }
    
    return ret;
}
