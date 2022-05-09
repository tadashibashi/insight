//
//  audio_file.hpp
//  audio_file_insight
//
//  Created by Aaron Ishibashi on 3/9/22.
//
#ifndef audio_file_hpp
#define audio_file_hpp
#include <string>

namespace insight
{
    class audio_file {
    public:
        audio_file() = default;
        virtual ~audio_file() = default;
        virtual bool open(const std::string &filepath) = 0;
        virtual void close() = 0;
        [[nodiscard]] virtual double seconds() const = 0;
        [[nodiscard]] virtual std::string filepath() const { return {}; }
        [[nodiscard]] virtual std::string filename() const
        {
            auto pos = filepath().find_last_of('/');
            return (pos == std::string::npos) ?
                   filepath()                    :
                   filepath().substr(pos + 1);
        }

        [[nodiscard]] virtual bool is_valid() const = 0;
    };
}

#endif /* audio_file_hpp */
