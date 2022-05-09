add_subdirectory(insight)

project(ai_wavtime)

add_executable(ai_wavtime
    ai_wavtime.cpp ai_wavtime.hpp)
target_link_libraries(ai_wavtime PRIVATE insight)
