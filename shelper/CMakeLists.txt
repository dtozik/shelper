
set (SOURCES_GROUP_FILES
		src/media_center/adapters/kodi_adapter.cpp
		src/interop_mgr.cpp
		src/subtitles/subtitles.cpp
		src/translator/adapters/lingualeo.cpp
	)

set (HEADERS_GROUP_FILES
		src/json.hpp
		src/ctpl_stl.h
		src/media_center/media_center.h
		src/media_center/adapters/kodi_adapter.h
		src/interop_mgr.h
		src/defs.h
		src/output.h
		src/time_routines.h
		src/subtitles/subtitles.h
		src/view.h
		src/translator/translator.h
	)

set(SOURCES ${SOURCES} ${SOURCES_GROUP_FILES})
set(HEADERS ${HEADERS} ${HEADERS_GROUP_FILES})	


if (TEST_SUB)
	message(STATUS "Test subtitles file: ${TEST_SUB}")
	set (RESOURCES ${RESOURCES} 
		assets/${TEST_SUB})
	add_definitions(-DTEST_SUB="${TEST_SUB}")
endif()

include_directories(src 
					src/media_center 
					src/media_center/adapters
					src/output
					src/output/adapters
					src/translator
					src/translator/adapters
					../common_libs/curlpp-0.8.1/include
					../common_libs/curl/curl-7.64.1/include)

if(TARGET_IOS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")

	set (SOURCES_GROUP_FILES
		src/platform/ios/AppDelegate.mm
		src/platform/ios/main.mm
		src/platform/ios/ViewController.mm
		src/platform/ios/AppDelegate.h
		src/platform/ios/ViewController.h
		src/platform/ios/SceneDelegate.h
		src/platform/ios/SceneDelegate.mm
	)	


	set(SOURCES ${SOURCES} ${SOURCES_GROUP_FILES})
	source_group(platform FILES ${SOURCES_GROUP_FILES})

	set (RESOURCES ${RESOURCES} 
		src/platform/ios/Assets.xcassets
		src/platform/ios/Base.lproj/Main.storyboard
		src/platform/ios/Base.lproj/LaunchScreen.storyboard)

	set_source_files_properties(
	 	${RESOURCES}
	 	PROPERTIES
	 	MACOSX_PACKAGE_LOCATION Resources)

	set(CMAKE_EXE_LINKER_FLAGS "-framework UIKit
								-framework CoreGraphics")
	add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${HEADERS} ${SOURCES} ${RESOURCES})

	set_target_properties(${PROJECT_NAME} PROPERTIES
	  MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/ios/Info.plist
	  RESOURCE "${RESOURCES}"
	  XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES 
	  XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
	  XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER NO
	  XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS NO
	)

elseif(TARGET_OSX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")

	set (SOURCES_GROUP_FILES
		src/platform/osx/AppDelegate.mm
		src/platform/osx/main.mm
		src/platform/osx/ViewController.mm
		src/platform/osx/AppDelegate.h
		src/platform/osx/ViewController.h
		src/platform/osx/SettingsViewController.h
		src/platform/osx/SettingsViewController.mm
	)	


	set(SOURCES ${SOURCES} ${SOURCES_GROUP_FILES})
	source_group(platform FILES ${SOURCES_GROUP_FILES})

	set (RESOURCES ${RESOURCES} 
		src/platform/osx/Assets.xcassets
		src/platform/osx/Base.lproj/Main.storyboard
		src/platform/osx/shelper.entitlements)

	set_source_files_properties(
	 	${RESOURCES}
	 	PROPERTIES
	 	MACOSX_PACKAGE_LOCATION Resources)

	set(CMAKE_EXE_LINKER_FLAGS "-framework AppKit")
	add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${HEADERS} ${SOURCES} ${RESOURCES})

	set_target_properties(${PROJECT_NAME} PROPERTIES
	  #MACOSX_BUNDLE_GUI_IDENTIFIER ${PACKAGE_NAME}
	  MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/osx/Info.plist
	  RESOURCE "${RESOURCES}"
	  XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES 
	  XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
	  XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER NO
	  XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS NO
	)

else()
	set(SOURCES ${SOURCES} src/main.cpp)

	add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})
endif()


message(STATUS "System: ${CMAKE_SYSTEM_NAME}")

set (STATIC_LIBRARIES ${LIB_DIR}/libcurlpp.a
					  ${LIB_DIR}/libcurl.a
					  ${LIB_DIR}/libzlib.a
					  ${LIB_DIR}/libcrypto.a
					  ${LIB_DIR}/libssl.a
					  )

target_link_libraries(${PROJECT_NAME} ${STATIC_LIBRARIES})



