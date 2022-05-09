//
//  list_chunk.cpp
//  insight
//
//  Created by Aaron Ishibashi on 4/16/22.
//
#include "list_chunk.hpp"
#include <iostream>

namespace insight::wave
{

void
list_chunk::clear_impl()
{
    if (m_list_id == "adtl") delete data.adtl;
}

void
list_chunk::read_impl(buffer &buf, size_t chunk_size)
{
    m_list_id.read(buf);
    
    size_t data_size = chunk_size - 4;
    if (m_list_id == "adtl")
    {
        data.adtl = new adtl_list_data;
        
        for (int i = 0; i < data_size;)
        {
            chunk_id subchunkid;
            i += subchunkid.read(buf);
            uint32_t subchunksize;
            i += buf.read(subchunksize);
            
            if (subchunkid == "labl" || subchunkid == "note")
            {
                uint32_t cue_point_id;
                i += buf.read(cue_point_id);
                
                size_t label_buf_size = subchunksize - sizeof(cue_point_id) + 1;
                char *label_name = (char *)malloc(label_buf_size);
                i += buf.read(reinterpret_cast<unsigned char *>(label_name), label_buf_size - 1);
                label_name[label_buf_size - 1] = '\0';
                
                data.adtl->labels[cue_point_id] = label_name;
                
                free(label_name);
            }
            else
            {
                i += subchunksize;
            }
        }
            
        
    }
    else
    {
        data.null = 0;
    }
}


}
