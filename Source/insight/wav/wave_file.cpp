//
//  wav_file.cpp
//  audio_file_insight
//
//  Created by Aaron Ishibashi on 3/9/22.
//
#include "wave_file.hpp"

#include <insight/buffer.hpp>
#include <insight/platform.hpp>
#include <insight/wav/bext_chunk.hpp>
#include <insight/wav/cue_chunk.hpp>
#include <insight/wav/list_chunk.hpp>
#include <insight/wav/fmt_chunk.hpp>
#include <insight/wav/default_chunk.hpp>

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <map>

using namespace std;

namespace wave
{


wave_file::wave_file(const string &filepath) : m_seconds{0}, m_valid{false}
{
    open(filepath);
}

void
wave_file::close()
{
    m_filepath.clear();
    m_valid = false;
    m_chunks.close();
}

void
wave_file::parse_chunk(buffer &buf, size_t chunk_size, const chunk_id &id)
{
    if (id == "fmt ")
        m_chunks.store_chunk(new fmt_chunk).read(buf, chunk_size);
    else if (id == "data") // TODO: Create data chunk
        m_chunks.store_chunk(new default_chunk(id)).read(buf, chunk_size);
    else if (id == "bext")
        m_chunks.store_chunk(new bext_chunk).read(buf, chunk_size);
    else if (id == "cue ")
        m_chunks.store_chunk(new cue_chunk).read(buf, chunk_size);
    else if (id == "LIST")
        m_chunks.store_chunk(new list_chunk).read(buf, chunk_size);
    else
        m_chunks.store_chunk(new default_chunk(id)).read(buf, chunk_size);
}

bool
wave_file::read_chunks(buffer &buf)
{
    chunk_id id;
    uint32_t chunksize;
    id.read(buf);
    buf.read(chunksize);
    if (id != "RIFF")
    {
        std::cerr << "Error: This is not a valid WAV file. Missing RIFF header...\n";
        return false;
    }
    
    
    id.read(buf);
    if (id != "WAVE")
    {
        std::cerr << "This is not a valid WAVE file!\n";
        return false;
    }

    while(buf.tellg() < buf.size())
    {
        id.read(buf);
        buf.read(chunksize);
        
        // If JUNK chunk size is not even, add one. According to: http://www.daubnet.com/en/file-format-riff
        // Same behavior for bext, by observation, and I assume may be for every header.
        if (chunksize % 2 != 0)
            ++chunksize;
        
        parse_chunk(buf, chunksize, id);
    }
    
    auto *fmt = get_chunk<fmt_chunk>();
    auto *defaults = get_chunks<default_chunk>();
    auto *lists = get_chunks<list_chunk>();
    
    list_chunk *labels = nullptr;
    default_chunk *data = nullptr;
    
    if (lists)
    {
        for (list_chunk *list : *lists)
        {
            if (list->sub_id() == "adtl")
            {
                labels = list;
                break;
            }
        }
        
    }

    if (defaults)
    {
        for (auto *def : *defaults)
        {
            if (def->id() == "data")
            {
                data = def;
                break;
            }
        }
    }

    
    
    auto *cues = get_chunk<cue_chunk>();
    
    // Post-fix tasks
    // Calculate the seconds
    if (fmt && data)
    {
        m_seconds = ((float)data->size() / (float)fmt->byte_rate());
    }
    
    // Connect labels to corresponding cue markers if they both exist in this wav.
    if (cues && labels)
    {
        // visit each label
        for (auto &[index, label] : labels->data.adtl->labels)
        {
            // search the cues cache and sets the label to the corresponding index identifier
            cues->set_name(index, label);
        }
    }
    
    return true;
}

bool
wave_file::open(const string &filepath)
{
    // Safely open the file
    ifstream file;
    file.open(filepath.c_str(), ios::binary);
    if (!file.is_open())
    {
        cerr << "Problem opening file: \"" << filepath.c_str() << "\"\n";
        perror(nullptr);
        return -1;
    }
    
    // Get the file size -> allocate memory in buffer
    file.seekg(0, ios::end);
    size_t filesize = file.tellg();
    file.seekg(0, ios::beg);
    
    vector<unsigned char> data(filesize);
    
    // Read the file data
    file.read(reinterpret_cast<char *>(data.data()), filesize);
    if (file.fail() || file.eof())
    {
        perror("Error: Failure while reading data from file");
        return false;
    }
    file.close();
    
    m_filepath = filepath;
    
    // Interpret the wave file data
    buffer buf{data.data(), filesize};
    
    if (!read_chunks(buf))
    {
        close();
        return false;
    }
    
    m_valid = true;
    return true;
}

}


