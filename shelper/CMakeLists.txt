# Add executable called "helloDemo" that is built from the source files
# "demo.cxx" and "demo_b.cxx". The extensions are automatically found.

set (SOURCES 
		src/main.cpp
		src/media_center/media_center.cpp
		src/media_center/adapters/kodi_adapter.cpp
		src/interop_mgr.cpp
		src/subtitles/subtitles.cpp
	)

set (HEADERS 
		src/json.hpp
		src/ctpl_stl.h
		src/media_center/media_center.h
		src/media_center/adapters/kodi_adapter.h
		src/interop_mgr.h
		src/defs.h
		src/output/output.h
		src/output/adapters/console_output.h
		src/time_routines.h
		src/subtitles/subtitles.h
	)

include_directories(src 
					src/media_center 
					src/media_center/adapters
					src/output
					src/output/adapters
					../common_libs/curlpp-0.8.1/include)

message(STATUS "System: ${CMAKE_SYSTEM_NAME}")

add_executable(shelper ${SOURCES} ${HEADERS})


set (STATIC_LIBRARIES ${LIB_DIR}/libcurlpp.a
					  ${LIB_DIR}/libcurl.a
					  ${LIB_DIR}/libzlib.a)

target_link_libraries(shelper ${STATIC_LIBRARIES})



