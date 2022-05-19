//
// Created by Aaron Ishibashi on 5/3/22.
//
#pragma once
#ifndef insight_timecode_hpp
#define insight_timecode_hpp
#include <string>

namespace insight
{
    class timecode
    {
    public:
        timecode();
        timecode(long double seconds, long double framerate, int subframe_base);

        [[nodiscard]] const std::string &str() const;
        [[nodiscard]] long double framerate() const { return m_framerate; }
        timecode &framerate(long double rate) { m_framerate = rate; return *this; }
        [[nodiscard]] int subframe_base() const { return m_subframe_base; }
        timecode &subframe_base(int base) { m_subframe_base = base; return *this; }
        [[nodiscard]] long double timecode_seconds() const { return m_tcseconds; }
    private:
        long double m_tcseconds; // timecode seconds
        long double m_framerate; // framerate
        int m_subframe_base; // frames divided into this number of subframes
        std::string m_str; // cached
    };
}

#endif /* insight_timecode_hpp */
