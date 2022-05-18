project(insight_tests)
add_subdirectory(Catch2)

add_executable(insight_tests wav_test.cpp buffer_test.cpp insight_tests.cpp timecode_tests.cpp chunk_id_tests.cpp default_chunk_tests.cpp insight_tests.hpp)
target_link_libraries(insight_tests insight Catch2)
target_include_directories(insight_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/Source
        Catch2/src)
