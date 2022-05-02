//
//  audio_file.hpp
//  audio_file_insight
//
//  Created by Aaron Ishibashi on 3/9/22.
//
#ifndef audio_file_hpp
#define audio_file_hpp
#include <string>

class audio_file {
public:
    audio_file() {};
    virtual ~audio_file() {}
    virtual bool open(const std::string &filepath) = 0;
    virtual void close() = 0;
    virtual double seconds() const = 0;
    virtual std::string filepath() const { return std::string(); }
    virtual std::string filename() const
    {
        auto pos = filepath().find_last_of('/');
        return (pos == std::string::npos) ?
            filepath()                    :
            filepath().substr(pos + 1);
    }
    
    virtual bool is_valid() const = 0;
};

#endif /* audio_file_hpp */
