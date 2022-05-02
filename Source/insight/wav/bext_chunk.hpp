#ifndef bext_hpp
#define bext_hpp

#include <insight/wav/chunk.hpp>
#include <string>

namespace wave
{
    class bext_chunk : public chunk {
    public:
        
        void log();
        
        bext_chunk();
        
        const std::string &description() const { return m_description; }
        const std::string &originator() const { return m_originator; }
        const std::string &originator_ref() const { return m_originator_ref; }
        const std::string &origination_date() const { return m_origination_date; }
        const std::string &origination_time() const { return m_origination_time; }
        uint64_t time_ref() const;
        uint32_t time_ref_high() const { return m_time_ref_high; }
        uint32_t time_ref_low() const { return m_time_ref_low; }
        uint16_t version() const { return m_version; }
        
        /// @returns SMPTE UMID bytes, which is 64 bytes in length. This is not a null-terminated c-string.
        const char *umid() const { return m_umid; }
        uint16_t loudness_value() const  { return m_loudness_value; }
        uint16_t max_truepeak_level() const { return m_max_truepeak_level; }
        uint16_t max_momentary_loudness() const { return m_max_momentary_loudness; }
        uint16_t max_shortterm_loudness() const { return m_max_shortterm_loudness; }
        const std::string &reserved() const { return m_reserved; }
        const std::string &coding_history() const { return m_coding_history; }
        
    private:
        void read_impl(buffer &buf, size_t length) override;
        void clear_impl() override;
        std::string m_description;      // description of the sound sequence
        std::string m_originator;       // name of the originator
        std::string m_originator_ref;   // reference of the originator
        std::string m_origination_date; // yyyy:mm:dd
        std::string m_origination_time; // hh:mm:ss
        uint32_t m_time_ref_low;     // first sample count since midnight, low word
        uint32_t m_time_ref_high;    // first sample count since midnight, high word
        uint16_t m_version;          // version of the bwf standard
        char m_umid[64];             // binary bytes 0 of SMPTE UMID
        uint16_t m_loudness_value;   // Integrated loudness value of the file in LUFS
                                   // multiplied by 100
        uint16_t m_loudness_range;   // Integrated range of file in LU (mult by 100)
        uint16_t m_max_truepeak_level; // Max true peak of file (dBTP) (mult by 100)
        uint16_t m_max_momentary_loudness; // Highest value of momentary loudness level of file in LUFS (mult by 100)
        uint16_t m_max_shortterm_loudness; // Highest value of the short-term loudness level of the file in LUFS (mult by 100)
        std::string m_reserved;
        std::string m_coding_history;
        // Helper to read strings from the buffer. Clips out white space.
        size_t read_string(buffer &buf, std::string &out_str, size_t length);
    };
}


#endif
