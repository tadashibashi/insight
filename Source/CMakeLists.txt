add_subdirectory(insight)
add_subdirectory(tests)
project(ai_wavtime)

add_executable(ai_wavtime
    ai_wavtime.cpp ai_wavtime.hpp tests/wav_test.cpp)
target_link_libraries(ai_wavtime PRIVATE insight)
