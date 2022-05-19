//
// Created by Aaron Ishibashi on 5/3/22.
//
#include "timecode.hpp"
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

/// Rounds total timecode seconds to desired subframe base value.
/// @param tc_seconds        The time in Timecode seconds
/// @param base_rate          The number of frames in one timecode second (NOTE: this can differ from the actual frame rate.
///                      e.g. 29.97fps has a base_rate of 30 frames per timecode second.)
/// @param subframe_base The number of subframes to divide a frame with. Usually it is 80 (Logic Pro) or 100.
static long double
round_timecode_to_subframe(long double tc_seconds, long double base_rate, long double subframe_base)
{
    const long double secs_per_frame = 1.0/base_rate;
    return round(tc_seconds / secs_per_frame * subframe_base) / subframe_base * secs_per_frame;
}

static long double
seconds_to_tcseconds(long double seconds, long double base_framerate, long double framerate, long double subframe_base)
{
      // Convert seconds -> frames -> timecode seconds
    long double total_frames = seconds * framerate;
    //std::cout << "total frames: " << total_frames << '\n';
    return round_timecode_to_subframe(total_frames / base_framerate, base_framerate, subframe_base);
}

//static std::string seconds_to_timecode_str(long double seconds, long double base_rate, long double frame_rate, long double subframe_base)
//{
//
//    // Convert seconds -> frames -> timecode seconds
//    long double total_frames = seconds * frame_rate;
//    //std::cout << "total frames: " << total_frames << '\n';
//    long double tc_seconds = round_timecode_to_subframe(total_frames / base_rate, base_rate, subframe_base);
//
//
//    //std::cout << "timecode seconds: " << tc_seconds << '\n';
//    long double frames = fmod(tc_seconds, 1.0) / (1.0 / base_rate);
//    //std::cout << "timecode frames : " << frames << '\n';
//
//    long double subframes = round(fmod(frames, 1.0) * subframe_base * subframe_base) / subframe_base;
//    //std::cout << "subframes nonrounded: " << fmod(frames, 1.0) * subframe_base << '\n';
//
//    std::stringstream stream;
//    stream << std::setprecision(4);
//    stream << std::setfill('0') << std::setw(2) << (int)(tc_seconds / 3600.0) << ":" // hours
//           << std::setfill('0') << std::setw(2) << (int)(tc_seconds / 60.0) % 60 << ":" // minutes
//           << std::setfill('0') << std::setw(2) << ((int)tc_seconds % 60) << ":" // seconds
//           << std::setfill('0') << std::setw(2) << (int)frames % (int)base_rate << '.'
//           << std::setfill('0') << std::setw(2) << (int)subframes % (int)subframe_base;
//
//    return stream.str();
//}

static std::string
tcseconds_to_display_str(long double tc_seconds, long double base_rate, long double frame_rate, long double subframe_base)
{
    //std::cout << "timecode seconds: " << tc_seconds << '\n';
    long double frames = fmod(tc_seconds, 1.0) / (1.0 / base_rate);
    //std::cout << "timecode frames : " << frames << '\n';

    long double subframes = round(fmod(frames, 1.0) * subframe_base * subframe_base) / subframe_base;
    //std::cout << "subframes nonrounded: " << fmod(frames, 1.0) * subframe_base << '\n';

    std::stringstream stream;
    stream << std::setprecision(4);
    stream << std::setfill('0') << std::setw(2) << (int)(tc_seconds / 3600.0) << ":" // hours
           << std::setfill('0') << std::setw(2) << (int)(tc_seconds / 60.0) % 60 << ":" // minutes
           << std::setfill('0') << std::setw(2) << ((int)tc_seconds % 60) << ":" // seconds
           << std::setfill('0') << std::setw(2) << (int)frames % (int)base_rate << '.'
           << std::setfill('0') << std::setw(2) << (int)subframes % (int)subframe_base;

    return stream.str();
}

namespace insight
{
    timecode::timecode() : m_framerate(30), m_subframe_base(80), m_tcseconds(0), m_str()
    {

    }

    timecode::timecode(long double seconds, long double framerate, int subframe_base) :
        m_framerate(framerate), m_subframe_base(subframe_base),
        m_tcseconds(seconds_to_tcseconds(seconds, round(framerate), framerate, subframe_base)), m_str()
    {
        m_str = tcseconds_to_display_str(m_tcseconds, round(m_framerate), m_framerate, m_subframe_base);
    }

    const std::string &
    timecode::str() const
    {
        return m_str;
    }
}