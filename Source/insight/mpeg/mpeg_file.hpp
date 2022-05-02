//
//  mp3_file.hpp
//  audio_file_insight
//
//  Created by Aaron Ishibashi on 3/11/22.
//
#ifndef mpeg_file_hpp
#define mpeg_file_hpp

#include <string>
#include <vector>
#include "../audio_file.hpp"
#include "mpeg_header.hpp"

/// @class mpeg_file
/// @brief Supports opening MPEG v1-2, layers 1-3.
class mpeg_file : public audio_file {
public:
    mpeg_file(const std::string &filepath);
    mpeg_file();
    ~mpeg_file() override;
    
    /// @function open
    /// @brief Opens the mp3 file, and reads/parses its contents.
    /// All file resources are automatically freed, and there is no object state changes on exceptions or errors.
    /// @returns True on successful read, False if there was an issue either opening or parsing the contents.
    bool open(const std::string &filepath) override;
    
    /// @function close
    /// @brief Cleans up the file. Automatically called during destructor.
    void close() override;
    
    /// @function get_length
    /// @brief Gets the length of the file in seconds.
    double seconds() const override;
    
    std::string filepath() const override;
    
    /// @function is_valid
    /// @returns True if this is a valid MPEG file or False if there is an issue with it.
    bool is_valid() const override;
    
    /// @function total_frames
    /// @returns The total count of MPEG file frames.
    size_t total_frames() const;
private: // data
    std::string m_filepath;
    std::vector<mpeg_header> m_headers;
    
    double m_seconds;
    unsigned char *m_data;
    unsigned char *m_data_end;
    mutable bool m_valid;
private: // helpers
    bool init_headers();
    
    /// @function copy
    /// @brief Functions as a wrapper around memcpy. Checks for buffer overflow.
    void *copy(void *target, unsigned byte_pos, unsigned size) const;
    
    /// @function next_header
    /// @brief Finds the next header, or returns nullptr if there was none found.
    /// @param header          A header assumed to have the frame sync bits set.
    /// @param out_header Header will be swapped if one is found at the next location
    /// @param ptr                 The data ptr where the first header param was found.
    /// @returns Pointer to the next header, or nullptr if none was found.
    unsigned char *next_header(const mpeg_header &header, mpeg_header &out_header, unsigned char *ptr) const;
    
    /// @function find_first_valid_header
    /// @brief Searches for and returns the first header that has a following valid header. This tries to best ensure that you begin reading from a valid header. You can also use this function to test whether or not this is a valid MP3 file.
    /// @param header_out The header to receive.
    /// @param current        The pointer to start reading from
    /// @returns Pointer to the data of the first valid header, or nullptr on failure to find a valid header in the file.
    unsigned char *find_first_valid_header(mpeg_header &header_out, unsigned char *current) const;
    
    /// @function is_pointer_inbounds
    /// @brief Finds whether or not the pointer is within the mpeg file's buffer range
    bool is_pointer_inbounds(unsigned char *ptr) const;
    
    /// @function is_lame
    /// @brief Detects whether or not the file has a LAME header. In this case, the first
    /// header is ignored in counting the length. This functionality can be expanded to parse
    /// the LAME Info or Xing headers for CBR, VBR and ABR files, other info, etc.
    bool is_lame() const;
    
};

inline bool mpeg_file::is_valid() const
{
    return m_valid;
}

inline std::string mpeg_file::filepath() const
{
    return m_filepath;
}

#endif /* mp3_file_hpp */
