project(CURL)

cmake_minimum_required(VERSION 2.8)


set(CURL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/curl-7.64.1)
set(CURL_LIBRARIES libcurl.a)

include_directories(${CURL_DIR}/include) #openssl headers
include_directories(${CURL_DIR}/include/curl) #curl_config
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../openssl/openssl-1.1.1b/include) #curl_config



	set(SOURCE_FILES ${SOURCE_FILES}
	${CURL_DIR}/lib/amigaos.c
	${CURL_DIR}/lib/asyn-ares.c
	${CURL_DIR}/lib/asyn-thread.c
	${CURL_DIR}/lib/base64.c
	${CURL_DIR}/lib/conncache.c
	${CURL_DIR}/lib/connect.c
	${CURL_DIR}/lib/content_encoding.c
	${CURL_DIR}/lib/cookie.c
	${CURL_DIR}/lib/curl_addrinfo.c
	${CURL_DIR}/lib/curl_ctype.c
	${CURL_DIR}/lib/curl_des.c
	${CURL_DIR}/lib/curl_endian.c
	${CURL_DIR}/lib/curl_fnmatch.c
	${CURL_DIR}/lib/curl_gethostname.c
	${CURL_DIR}/lib/curl_gssapi.c
	${CURL_DIR}/lib/curl_memrchr.c
	${CURL_DIR}/lib/curl_multibyte.c
	${CURL_DIR}/lib/curl_ntlm_core.c
	${CURL_DIR}/lib/curl_ntlm_wb.c
	${CURL_DIR}/lib/curl_range.c
	${CURL_DIR}/lib/curl_rtmp.c
	${CURL_DIR}/lib/curl_sspi.c
	${CURL_DIR}/lib/curl_sasl.c
	${CURL_DIR}/lib/curl_threads.c
	${CURL_DIR}/lib/dict.c
	${CURL_DIR}/lib/doh.c
	${CURL_DIR}/lib/dotdot.c
	${CURL_DIR}/lib/easy.c
	${CURL_DIR}/lib/escape.c
	${CURL_DIR}/lib/file.c
	${CURL_DIR}/lib/fileinfo.c
	${CURL_DIR}/lib/formdata.c
	${CURL_DIR}/lib/ftp.c
	${CURL_DIR}/lib/ftplistparser.c
	${CURL_DIR}/lib/getenv.c
	${CURL_DIR}/lib/getinfo.c
	${CURL_DIR}/lib/gopher.c
	${CURL_DIR}/lib/hash.c
	${CURL_DIR}/lib/hmac.c
	${CURL_DIR}/lib/hostasyn.c
	${CURL_DIR}/lib/hostcheck.c
	${CURL_DIR}/lib/hostip.c
	${CURL_DIR}/lib/hostip4.c
	${CURL_DIR}/lib/hostip6.c
	${CURL_DIR}/lib/hostsyn.c
	${CURL_DIR}/lib/http.c
	${CURL_DIR}/lib/http2.c
	${CURL_DIR}/lib/http_chunks.c
	${CURL_DIR}/lib/http_digest.c
	${CURL_DIR}/lib/http_ntlm.c
	${CURL_DIR}/lib/http_negotiate.c
	${CURL_DIR}/lib/http_proxy.c
	${CURL_DIR}/lib/idn_win32.c
	${CURL_DIR}/lib/if2ip.c
	${CURL_DIR}/lib/imap.c
	${CURL_DIR}/lib/inet_ntop.c
	${CURL_DIR}/lib/inet_pton.c
	${CURL_DIR}/lib/krb5.c
	${CURL_DIR}/lib/ldap.c
	${CURL_DIR}/lib/llist.c
	${CURL_DIR}/lib/md4.c
	${CURL_DIR}/lib/md5.c
	${CURL_DIR}/lib/memdebug.c
	${CURL_DIR}/lib/mime.c
	${CURL_DIR}/lib/mprintf.c
	${CURL_DIR}/lib/multi.c
	${CURL_DIR}/lib/netrc.c
	${CURL_DIR}/lib/non-ascii.c
	${CURL_DIR}/lib/nonblock.c
	${CURL_DIR}/lib/nwlib.c
	${CURL_DIR}/lib/nwos.c
	${CURL_DIR}/lib/openldap.c
	${CURL_DIR}/lib/parsedate.c
	${CURL_DIR}/lib/pingpong.c
	${CURL_DIR}/lib/pop3.c
	${CURL_DIR}/lib/progress.c
	${CURL_DIR}/lib/pipeline.c
	${CURL_DIR}/lib/rand.c
	${CURL_DIR}/lib/rtsp.c
	${CURL_DIR}/lib/security.c
	${CURL_DIR}/lib/select.c
	${CURL_DIR}/lib/sendf.c
	${CURL_DIR}/lib/setopt.c
	${CURL_DIR}/lib/share.c
	${CURL_DIR}/lib/sha256.c
	${CURL_DIR}/lib/slist.c
	${CURL_DIR}/lib/smb.c
	${CURL_DIR}/lib/smtp.c
	${CURL_DIR}/lib/socks.c
	${CURL_DIR}/lib/socks_gssapi.c
	${CURL_DIR}/lib/socks_sspi.c
	${CURL_DIR}/lib/speedcheck.c
	${CURL_DIR}/lib/splay.c
	${CURL_DIR}/lib/ssh.c
	${CURL_DIR}/lib/strcase.c
	${CURL_DIR}/lib/strdup.c
	${CURL_DIR}/lib/strerror.c
	${CURL_DIR}/lib/strtok.c
	${CURL_DIR}/lib/strtoofft.c
	${CURL_DIR}/lib/system_win32.c
	${CURL_DIR}/lib/telnet.c
	${CURL_DIR}/lib/tftp.c
	${CURL_DIR}/lib/timeval.c
	${CURL_DIR}/lib/transfer.c
	${CURL_DIR}/lib/url.c
	${CURL_DIR}/lib/urlapi.c
	${CURL_DIR}/lib/version.c
	${CURL_DIR}/lib/warnless.c
	${CURL_DIR}/lib/wildcard.c
	${CURL_DIR}/lib/x509asn1.c
	${CURL_DIR}/lib/vtls/vtls.c
	${CURL_DIR}/lib/vtls/openssl.c
	${CURL_DIR}/lib/vauth/digest.c
	${CURL_DIR}/lib/vauth/vauth.c
	${CURL_DIR}/lib/vauth/ntlm.c
	)

if(TRUE) #NOT WIN32 AND NOT SQ_BUILD_PLATFORM_WPHONE AND NOT SQ_BUILD_PLATFORM_WSTORE)
    INCLUDE (CheckTypeSize) 
	if (NOT IOS_PLATFORM)
    check_type_size(size_t  SIZEOF_SIZE_T)
    check_type_size(ssize_t  SIZEOF_SSIZE_T)
    check_type_size("long long"  SIZEOF_LONG_LONG)
    check_type_size("long"  SIZEOF_LONG)
    check_type_size("short"  SIZEOF_SHORT)
    check_type_size("int"  SIZEOF_INT)
    check_type_size("__int64"  SIZEOF___INT64)
    check_type_size("long double"  SIZEOF_LONG_DOUBLE)
    check_type_size("time_t"  SIZEOF_TIME_T)
	endif()

    if(NOT HAVE_SIZEOF_SSIZE_T)
        if(SIZEOF_LONG EQUAL SIZEOF_SIZE_T)
            set(ssize_t long)
		endif(SIZEOF_LONG EQUAL SIZEOF_SIZE_T)
	
        if(NOT ssize_t AND SIZEOF___INT64 EQUAL SIZEOF_SIZE_T)
            set(ssize_t __int64)
		endif(NOT ssize_t AND SIZEOF___INT64 EQUAL SIZEOF_SIZE_T)
	endif(NOT HAVE_SIZEOF_SSIZE_T)

# Different sizeofs, etc.

#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long long
#    define CURL_FORMAT_CURL_OFF_T  "lld"
#    define CURL_FORMAT_CURL_OFF_TU "llu"
#    define CURL_FORMAT_OFF_T       "%lld"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  LL
#    define CURL_SUFFIX_CURL_OFF_TU ULL

	if(NOT SIZEOF_LONG)
		set(SIZEOF_LONG 4)
		set(SIZEOF_LONG_LONG 8)
		set(SIZEOF_INT 4)
		message("curl: set SIZEOF_LONG manually to ${SIZEOF_LONG}")
        endif()

	set(CURL_SIZEOF_LONG ${SIZEOF_LONG})

	if(SIZEOF_LONG EQUAL 8)
		set(CURL_TYPEOF_CURL_OFF_T long)
		set(CURL_SIZEOF_CURL_OFF_T 8)
		set(CURL_FORMAT_CURL_OFF_T "ld")
		set(CURL_FORMAT_CURL_OFF_TU "lu")
		set(CURL_FORMAT_OFF_T "%ld")
		set(CURL_SUFFIX_CURL_OFF_T L)
		set(CURL_SUFFIX_CURL_OFF_TU UL)
	endif(SIZEOF_LONG EQUAL 8)

	if(SIZEOF_LONG_LONG EQUAL 8)
		set(CURL_TYPEOF_CURL_OFF_T "long long")
		set(CURL_SIZEOF_CURL_OFF_T 8)
		set(CURL_FORMAT_CURL_OFF_T "lld")
		set(CURL_FORMAT_CURL_OFF_TU "llu")
		set(CURL_FORMAT_OFF_T "%lld")
		set(CURL_SUFFIX_CURL_OFF_T LL)
		set(CURL_SUFFIX_CURL_OFF_TU ULL)
	endif(SIZEOF_LONG_LONG EQUAL 8)

	if(NOT CURL_TYPEOF_CURL_OFF_T)
		set(CURL_TYPEOF_CURL_OFF_T ${ssize_t})
		set(CURL_SIZEOF_CURL_OFF_T ${SIZEOF_SSIZE_T})
		# TODO: need adjustment here.
		set(CURL_FORMAT_CURL_OFF_T "ld")
		set(CURL_FORMAT_CURL_OFF_TU "lu")
		set(CURL_FORMAT_OFF_T "%ld")
		set(CURL_SUFFIX_CURL_OFF_T L)
		set(CURL_SUFFIX_CURL_OFF_TU LU)
	endif(NOT CURL_TYPEOF_CURL_OFF_T)

    if(HAVE_SIZEOF_LONG_LONG)
        set(HAVE_LONGLONG 1)
        set(HAVE_LL 1)
    endif(HAVE_SIZEOF_LONG_LONG)

  set(CURL_TYPEOF_CURL_SOCKLEN_T int)
  set(CURL_SIZEOF_CURL_SOCKLEN_T ${SIZEOF_INT})

    set(OS "\"${CMAKE_SYSTEM_NAME}\"")
    set(SIZEOF_CURL_OFF_T ${CURL_SIZEOF_CURL_OFF_T})

    message(STATUS "curl version=[${CURL_DIR}]")

    if(SQ_BUILD_PLATFORM_IPHONE)
        configure_file(curl_config.h.in ${SQ_ROOT_DIR}/build/lib/ios/common_libs-iphoneos/include/curl/curl_config.h)
        if(SQ_SKIP_SIMULATOR)
            message("iOS Simulator skipped. (curl)")
        else()
            configure_file(curl_config.h.in ${SQ_ROOT_DIR}/build/lib/ios/common_libs-iphonesimulator/include/curl/curl_config.h)
        endif()
    else(SQ_BUILD_PLATFORM_IPHONE)
        configure_file(curl_config.h.in ${CURL_DIR}/include/curl/curl_config.h)
    endif(SQ_BUILD_PLATFORM_IPHONE)


    #include(xCode)

endif()

include_directories(${CURL_DIR}/lib)

add_definitions(-DCURL_DISABLE_TFTP)
add_definitions(-DCURL_DISABLE_FTP)
add_definitions(-DCURL_DISABLE_LDAP)
add_definitions(-DCURL_DISABLE_TELNET)
add_definitions(-DCURL_DISABLE_DICT)
add_definitions(-DCURL_DISABLE_RTSP)
add_definitions(-DCURL_DISABLE_IMAP)
add_definitions(-DCURL_DISABLE_GOPHER)
add_definitions(-DCURL_DISABLE_POP3)
add_definitions(-DCURL_DISABLE_SMTP)

add_definitions(-DCURL_STATICLIB)

if (MINGW)
    include(zlib)
    find_package(OpenSSL REQUIRED)
    include_directories(${OPENSSL_INCLUDE_DIR})
    add_definitions(-DUSE_OPENSSL -DUSE_SSLEAY)
    message(STATUS "using mingw openssl " ${OPENSSL_INCLUDE_DIR})
    add_definitions(-DHAVE_LIBZ)
    add_definitions(-DHAVE_ZLIB_H=1)
elseif (SQ_BUILD_PLATFORM_WPHONE OR SQ_BUILD_PLATFORM_WSTORE)
   include(zlib)
   add_definitions(-DHAVE_LIBZ)
   add_definitions(-DHAVE_ZLIB_H=1)
   add_definitions(-DWINPHONE)
elseif(WIN32)
   include(zlib)
   add_definitions(-DHAVE_LIBZ)
   add_definitions(-DHAVE_ZLIB_H=1)
else()
  add_definitions(-DHAVE_CONFIG_H)
endif (MINGW)

add_definitions(-DBUILDING_LIBCURL=1)

if (USE_OPENSSL)
	message (STATUS "USE_OPENSSL")
	add_definitions(-DUSE_OPENSSL -DUSE_SSLEAY)
endif()

# setup curl target
add_library(curl STATIC ${HEADER_FILES} ${SOURCE_FILES})
if (MINGW)
    add_definitions(-DCURL_PULL_WS2TCPIP_H)
    message(STATUS "using wimgw windows sockets libs ")

    set(LIBRARIES ${LIBRARIES}
        libpthreadGC2
        ws2_32
    )
    target_link_libraries(curl ${LIBRARIES})
endif (MINGW)

#add_xcode_attribute(curl 1)


