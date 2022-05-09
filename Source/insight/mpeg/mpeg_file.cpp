//
//  mp3_file.cpp
//  audio_file_insight
//
//  Created by Aaron Ishibashi on 3/11/22.
//
#include "mpeg_file.hpp"
#include <fstream>
#include <iostream>
#include <insight/swap_endian.hpp>

insight::mpeg_file::mpeg_file(const std::string &filepath) : m_filepath{filepath}, m_data{}, m_data_end{}, m_seconds{-1}, m_headers{}, m_valid{false}
{
    open(filepath);
}

insight::mpeg_file::mpeg_file() : m_filepath{}, m_data{}, m_data_end{}, m_seconds{-1}, m_headers{}, m_valid{false}
{
    
}

insight::mpeg_file::~mpeg_file()
{
    close();
}

void
insight::mpeg_file::close()
{
    if (m_data)
    {
        free(m_data);
        m_data = nullptr;
    }
    
    m_seconds = -1;
    m_headers.clear();
    m_filepath.clear();
    
    m_data_end = nullptr;
    m_valid = false;
}

bool
insight::mpeg_file::open(const std::string &filepath)
{
    std::ifstream file {filepath};
    if (!file.is_open())
    {
        perror("mp3_file::open");
        return false;
    }
    
    file.seekg(0, std::ios::end);
    size_t filesize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    auto *temp = (unsigned char *)malloc(filesize);
    
    try
    {
        file.read((char *)temp, filesize);
    }
    catch(const std::exception &e)
    {
        free (temp);
        throw e;
    }
    
    m_data = temp;
    m_data_end = temp + filesize;
    m_filepath = filepath;
    return init_headers();
}

double
insight::mpeg_file::seconds() const
{
    return m_seconds;
}

size_t
insight::mpeg_file::total_frames() const
{
    return m_headers.size();
}

unsigned char *
insight::mpeg_file::find_first_valid_header(mpeg_header &header, unsigned char *current) const
{
    while(current < m_data_end)
    {
        if (*current == 255u) // we found a correct first byte
        {
            if (is_pointer_inbounds(current + sizeof(unsigned)))
            {
                // Get the header and check if it is valid
                unsigned current_data;
                memcpy(&current_data, current, sizeof(unsigned));
                mpeg_header current_header{current_data};
                
                if (!current_header.is_valid())
                {
                    ++current;
                    continue;
                }
                
                // Check if there's a valid next header
                mpeg_header current_header2;
                if (next_header(current_header, current_header2, current))
                {
                    header = current_header;
                    return current;
                }
                else
                {
                    ++current;
                    continue;
                }
            }
            else
            {
                return nullptr;
            }
            
        }
        
        ++current;
    }
    
    return nullptr;
}

bool
insight::mpeg_file::is_pointer_inbounds(unsigned char *ptr) const
{
    return ptr < m_data_end && ptr >= m_data;
}

unsigned char *
insight::mpeg_file::next_header(const mpeg_header &header, mpeg_header &out_header, unsigned char *ptr) const
{
    // Check CRC protection bits
    if (header.is_crc_protected())
    {
        // Check that target destination fits within memory
        if (!is_pointer_inbounds(ptr + sizeof(unsigned) + sizeof(uint16_t)))
        {
            std::cout << "Could not check crc bits, crc was out of memory range!\n";
            return nullptr;
        }
        
        // It does. Now read it, and check against frame bytes.
        uint16_t crc;
        memcpy(&crc, ptr + sizeof(unsigned) + sizeof(uint16_t), sizeof(uint16_t));
        if (!is_big_endian())
            crc = swap_endian(crc);
        
        if (header.frame_byte_length() != crc)
        {
            //std::cout << "CRC and frame byte calculation did not match!\n";
            return nullptr;
        }
    }
    
    // Check next location for next frame
    mpeg_header header2{};
    int frame_length = header.frame_byte_length();
    if (is_pointer_inbounds(ptr + frame_length + sizeof(unsigned)))
    {
        unsigned header2_uint;
        memcpy(&header2_uint, ptr + frame_length, sizeof(unsigned));
        header2.set(header2_uint);
        
        
        if (!header2.is_valid())
        {
            //std::cout << "Mpeg header found at location was invalid\n";
            //header2.log_bits();
            return nullptr;
        }
        
        if(!header.check(header2))
        {
            //std::cout << "Mpeg header found at location did not match\n";
            //header.log_bits();
            //header2.log_bits();
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
    
    std::swap(out_header, header2);
    return ptr + frame_length;
}

bool
insight::mpeg_file::init_headers()
{
    if (!m_headers.empty())
        m_headers.clear();
    m_seconds = 0;
    
    unsigned char *current = m_data;
    /* First, grab the first two headers that match. */
    mpeg_header header;
    
    current = find_first_valid_header(header, current);
    if (!current)
    {
        std::cerr << "Could not find a matching pair of headers! Failed to count length and headers in mpeg file: \"" <<
        m_filepath << "\"\n";
        return false;
    }
    
    const double seconds_per_frame = (double)header.samples_per_second() / (double)header.sample_rate();
    
    if (!is_lame())
    {
        m_seconds += seconds_per_frame;
        m_headers.push_back(header);
        //header.log_bits();
    }
    else
    {
        //std::cout << "This is so lame...\n";
    }
    
    mpeg_header header2;
    while( (current = next_header(header, header2, current)) )
    {
        m_seconds += seconds_per_frame;
        m_headers.push_back(header2);
        //header2.log_bits();
        
        std::swap(header, header2);
    }
    
    m_valid = true;
    return true;
}

void *
insight::mpeg_file::copy(void *target, unsigned byte_pos, unsigned size) const
{
    if (byte_pos + size < (uintptr_t)m_data_end)
    {
        return memcpy(target, m_data + byte_pos, size);
    }
    else
    {
        std::cerr << "Critical error: Attempted read from mpeg_file::copy would cause overflow exception. Returned a nullptr instead.\n";
        return nullptr;
    }
}

// more info: http://gabriel.mp3-tech.org/mp3infotag.html#replaygain
bool
insight::mpeg_file::is_lame() const
{
    char lame[5], header[5];
    copy(header, 36, 4);
    copy(lame, 156, 4);
    lame[4] = '\0';
    header[4] = '\0';
    
    return std::strcmp(lame, "LAME") == 0 && (std::strcmp(header, "Info") == 0 || std::strcmp(header, "Xing"));
}

