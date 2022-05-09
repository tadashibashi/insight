//
//  chunk_cache.h
//  insight
//
//  Created by Aaron Ishibashi on 4/18/22.
//
#pragma once
#ifndef insight_wave_chunk_cache_h
#define insight_wave_chunk_cache_h

#include <insight/wav/chunk.hpp>
#include <map>
#include <vector>
#include <typeindex>
#include <functional>

namespace insight::wave
{

    class chunk_cache {
    public:
        
        /// @abstract gets the number of chunks in a particular type list
        /// @returns returns the pointer to the vector of the type of chunk that was checked in this function, or nullptr if none exists.
        template <typename T>
        const std::vector<T *> *get_chunks()
        {
            static_assert(std::is_base_of_v<chunk, T>, "typename T must be a child of class wave::chunk.");
            auto it = m_chunks.find(std::type_index(typeid(T)));
            if (it != m_chunks.end())
            {
                return (std::vector<T *> *)(&it->second);
            }
            else
            {
                return nullptr;
            }
        }
        
        /// @abstract Gets the first chunk of the type of chunk passed into the function, or nullptr if there are none
        /// @returns a ptr to the first chunk of type T stored in this container or nullptr if none exist.
        template <typename T>
        T *get_chunk()
        {
            static_assert(std::is_base_of_v<chunk, T>, "typename T must be a child of class wave::chunk.");
            
            auto it = m_chunks.find(std::type_index(typeid(T)));
            if (it != m_chunks.end() && !it->second.empty())
            {
                return dynamic_cast<T *>(it->second[0]);
                
            }
            else
            {
                return nullptr;
            }
        }
        
        template <typename T>
        T &store_chunk(T *newChunk)
        {
            static_assert(std::is_base_of_v<chunk, T>, "typename T must be a child of class wave::chunk.");
            
            m_chunks[std::type_index(typeid(T))].emplace_back((chunk *)newChunk);
            
            return *newChunk;
        }
        
        void close()
        {
            for (auto &[k, v] : m_chunks)
            {
                for (chunk *c : v)
                    delete c;
                
            }
            m_chunks.clear();
        }
        
    private:
        std::map<std::type_index, std::vector<chunk *> > m_chunks;
    };

}

#endif /* insight_wave_chunk_cache_h */
