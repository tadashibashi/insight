set (Insight_Sources
	audio_file.hpp
	buffer.hpp
	mpeg/mpeg_file.cpp mpeg/mpeg_file.hpp
	mpeg/mpeg_header.cpp mpeg/mpeg_header.hpp
	platform.cpp platform.hpp
	swap_endian.hpp
	wav/bext_chunk.cpp wav/bext_chunk.hpp
	wav/chunk_cache.hpp
	wav/chunk_id.cpp wav/chunk_id.hpp
	wav/chunk.cpp wav/chunk.hpp
	wav/cue_chunk.cpp wav/cue_chunk.hpp
	wav/default_chunk.cpp wav/default_chunk.hpp
	wav/fmt_chunk.cpp wav/fmt_chunk.hpp
	wav/list/adtl_list_data.cpp wav/list/adtl_list_data.hpp
	wav/list_chunk.cpp wav/list_chunk.hpp
	wav/wave_file.cpp wav/wave_file.hpp
	timecode.cpp timecode.hpp hash.hpp hash.cpp
	)

if (APPLE)
	set(Insight_Platform_Sources
		platform/mac/browser_mac.hpp
		platform/mac/browser_mac.mm
		platform/mac/Finder.h)
endif()

add_library(insight STATIC ${Insight_Sources} ${Insight_Platform_Sources})
target_link_libraries(insight PUBLIC "-framework Cocoa" "-framework ScriptingBridge" "-framework AppKit")
target_include_directories(insight PUBLIC ${CMAKE_SOURCE_DIR}/Source)