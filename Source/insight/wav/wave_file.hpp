//
//  wav_file.hpp
//  audio_file_insight
//
//  Created by Aaron Ishibashi on 3/9/22.
//
#ifndef wave_file_hpp
#define wave_file_hpp

#include <insight/wav/chunk_cache.hpp>
#include <insight/audio_file.hpp>

#include <string>
#include <vector>
#include <map>
#include <typeindex>

class buffer;

namespace wave
{

/// @class wave_file
/// @brief Reads a file in .wav format
class wave_file : public audio_file {
public:
    wave_file(const std::string &filepath);
    wave_file();
    ~wave_file();
    bool open(const std::string &filepath) override;
    void close() override;
    double seconds() const override;
    std::string filepath() const override;
    bool is_valid() const override;
    
    template <typename T>
    const std::vector<T *> *get_chunks()
    {
        return m_chunks.get_chunks<T>();
    }
    
    template <typename T>
    T *get_chunk()
    {
        return m_chunks.get_chunk<T>();
    }
    
private: // data
    double m_seconds;
    bool m_valid;
    chunk_cache m_chunks;
    std::string m_filepath;
    
private: // helpers
    bool read_chunks(buffer &buf);
    void parse_chunk(buffer &buf, size_t chunk_size, const chunk_id &id);
};

inline
wave_file::wave_file() : m_seconds{0}, m_valid{false}, m_filepath{} {}

inline
wave_file::~wave_file()
{
    close();
}

inline double
wave_file::seconds() const
{
    return m_seconds;
}

inline std::string
wave_file::filepath() const
{
    return m_filepath;
}

inline bool
wave_file::is_valid() const
{
    return m_valid;
}


}

#endif /* wav_reader_hpp */
