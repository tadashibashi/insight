/*!
 * @file bext_chunk.hpp -- insight
 * @author Aaron Ishibashi
 *
 * @class insight::wave::bext_chunk
 * @abstract Represents a broadcast wave format chunk "bext"
 *
 */
#pragma once
#ifndef insight_wave_bext_hpp
#define insight_wave_bext_hpp

#include <insight/wav/chunk.hpp>
#include <string>
#include <iosfwd>

namespace insight::wave
{
    class bext_chunk : public chunk {
    public:
        bext_chunk();

        /// Logs the data in this bext_chunk to an output.
        /// @param os if specified, information will be output to it.
        /// Otherwise, if it is null, it will be output to std::cout.
        void log(std::ostream *os = nullptr) const;

        // ========== Getters ==========

        /// Description of the sound sequence
        [[nodiscard]] const std::string &description() const { return m_description; }
        /// Name of the originator; often the name of the program that exported the wave file.
        [[nodiscard]] const std::string &originator() const { return m_originator; }
        /// Reference of the originator
        [[nodiscard]] const std::string &originator_ref() const { return m_originator_ref; }
        /// The date this wave file was created, in the format: yyyy:mm:dd
        [[nodiscard]] const std::string &origination_date() const { return m_origination_date; }
        /// The time this wave file was created, in the format: hh:mm:ss
        [[nodiscard]] const std::string &origination_time() const { return m_origination_time; }
        /// First sample count since midnight. Helper to retrieve the full number.
        [[nodiscard]] uint64_t time_ref() const;
        /// First sample count since midnight, high word
        [[nodiscard]] uint32_t time_ref_high() const { return m_time_ref_high; }
        /// First sample count since midnight, low word
        [[nodiscard]] uint32_t time_ref_low() const { return m_time_ref_low; }
        /// Version of the Broadcast Wave Format (BWF) standard
        [[nodiscard]] uint16_t version() const { return m_version; }
        /// SMPTE UMID bytes, which is 64 bytes in length. This is not a null-terminated c-string.
        [[nodiscard]] const char *umid() const { return m_umid; }
        /// Integrated loudness value of the file in LUFS multiplied by 100.
        [[nodiscard]] uint16_t loudness_value() const  { return m_loudness_value; }
        /// Integrated range of file in LU, multiplied by 100.
        [[nodiscard]] uint16_t loudness_range() const { return m_loudness_range; }
        /// Max true peak of file (dBTP), multiplied by 100.
        [[nodiscard]] uint16_t max_truepeak_level() const { return m_max_truepeak_level; }
        /// Highest value of momentary loudness level of file in LUFS, multiplied by 100.
        [[nodiscard]] uint16_t max_momentary_loudness() const { return m_max_momentary_loudness; }
        /// Highest value of the short-term loudness level of the file in LUFS, multiplied by 100.
        [[nodiscard]] uint16_t max_shortterm_loudness() const { return m_max_shortterm_loudness; }
        [[nodiscard]] const std::string &reserved() const { return m_reserved; }
        [[nodiscard]] const std::string &coding_history() const { return m_coding_history; }
        
    private:
        // ========== Class data ==========
        std::string m_description, m_originator, m_originator_ref, m_origination_date, m_origination_time;
        uint32_t m_time_ref_low, m_time_ref_high;
        uint16_t m_version;
        char m_umid[64];
        uint16_t m_loudness_value, m_loudness_range, m_max_truepeak_level, m_max_momentary_loudness, m_max_shortterm_loudness;
        std::string m_reserved, m_coding_history;

        // ========== Overrides ==========
        void read_impl(buffer &buf, size_t length) override;
        void clear_impl() override;

        // Helper to read strings from the buffer. Trims white space.
        static size_t read_string(buffer &buf, std::string &out_str, size_t length);
    };
}

std::ostream &operator << (std::ostream &os, const insight::wave::bext_chunk &bext);

#endif
