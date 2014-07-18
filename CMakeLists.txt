# -*- coding: utf-8 -*-
######################################################################
# File: CMakeLists.txt                          Part of Raven Server #
#                                                                    #
# Copyright (C) 2012, 2013, Joachim Pileborg and individual          #
# contributors. All rights reserved.                                 #
#                                                                    #
# Redistribution and use in source and binary forms, with or without #
# modification, are permitted provided that the following conditions #
# are met:                                                           #
#                                                                    #
#   o Redistributions of source code must retain the above copyright #
#     notice, this list of conditions and the following disclaimer.  #
#   o Redistributions in binary form must reproduce the above        #
#     copyright notice, this list of conditions and the following    #
#     disclaimer in the documentation and/or other materials         #
#     provided with the distribution.                                #
#   o Neither the name of The Raven nor the names of its             #
#     contributors may be used to endorse or promote products        #
#     derived from this software without specific prior written      #
#     permission.                                                    #
#                                                                    #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             #
# CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        #
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           #
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           #
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS  #
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,#
# OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,           #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY       #
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR     #
# TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF #
# THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF    #
# SUCH DAMAGE.                                                       #
#                                                                    #
######################################################################

cmake_minimum_required(VERSION 2.6)

project(Raven_Server CXX)

if(NOT Raven_Server_VERSION_MAJOR)
        set(Raven_Server_VERSION_MAJOR 0)
endif()
if(NOT Raven_Server_VERSION_MINOR)
        set(Raven_Server_VERSION_MINOR 1)
endif()
if(NOT Raven_Server_VERSION_PATCH)
        set(Raven_Server_VERSION_PATCH 0)
endif()
if(NOT Raven_Server_VERSION_EXTRA)
        set(Raven_Server_VERSION_EXTRA "")
endif()

include(CheckCXXSourceCompiles)
include(CheckCXXCompilerFlag)

######################################################################
# Private macros

macro(check_header_file HEADER DESTVAR)
    message(STATUS "Looking for header file ${HEADER}")

    set(
        CMAKE_CONFIGURABLE_FILE_CONTENT
        "#include <${HEADER}>\nint main() {}"
        )

    configure_file(
        "${CMAKE_ROOT}/Modules/CMakeConfigurableFile.in"
        "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/CheckHeaderFile.cpp"
        @ONLY IMMEDIATE
        )

    set(CHECK_HEADER_FILE_FLAGS ${CMAKE_REQUIRED_FLAGS})

    if(CMAKE_REQUIRED_INCLUDES)
        set(CHECK_HEADER_FILE_INCLUDE_DIRS "${CMAKE_REQUIRED_INCLUDES}")
    else()
        set(CHECK_HEADER_FILE_INCLUDE_DIRS)
    endif()

    if(${ARGC} GREATER 2)
        list(APPEND CHECK_HEADER_FILE_INCLUDE_DIRS ${ARGV2})
    endif()

    if(CHECK_HEADER_FILE_INCLUDE_DIRS)
        set(CHECK_HEADER_FILE_INCLUDE_DIRS "-DINCLUDE_DIRECTORIES=${CHECK_HEADER_FILE_INCLUDE_DIRS}")
    endif()

    try_compile(
        ${DESTVAR}
        ${CMAKE_BINARY_DIR}
        ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/CheckHeaderFile.cpp
        COMPILE_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS}
        CMAKE_FLAGS
            -DCOMPILE_DEFINITIONS:STRING=${CHECK_HEADER_FILE_FLAGS}
            "${CHECK_HEADER_FILE_INCLUDE_DIRS}"
        OUTPUT_VARIABLE OUTPUT
        )

    if(${DESTVAR})
        message(STATUS "Looking for header file ${HEADER} - found")
    else()
        message(STATUS "Looking for header file ${HEADER} - not found")
        # message(STATUS "${OUTPUT}")
    endif()
endmacro()

macro(check_flag_coverage DESTVAR)
    if(NOT FLAG_COVERAGE_CHECKED)
        set(FLAG_COVERAGE_CHECKED)

        # For clang++, we might need to link with the profile_rt library
        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            find_library(
                HAVE_LIBPROFILE_RT
                profile_rt
                HINTS /lib /usr/lib /usr/local/lib /opt/lib
            )
            if (HAVE_LIBPROFILE_RT)
                set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lprofile_rt")
                list(APPEND CMAKE_REQUIRED_LIBRARIES "${HAVE_LIBPROFILE_RT}")
            endif()
        endif()

        set(TMP_CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
        set(TMP_CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

        set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

        check_cxx_compiler_flag(--coverage ${DESTVAR})

        if(NOT ${DESTVAR})
            set(CMAKE_REQUIRED_FLAGS "${TMP_CMAKE_REQUIRED_FLAGS}")
            set(CMAKE_EXE_LINKER_FLAGS "${TMP_CMAKE_EXE_LINKER_FLAGS}")
        endif()
    endif()
endmacro()

######################################################################
# Flag handling

check_cxx_compiler_flag(-std=c++1y     HAS_FLAG_STD_CXX1Y)
check_cxx_compiler_flag(-stdlib=libc++ HAS_FLAG_STDLIB_LIBCXX)
check_cxx_compiler_flag(-pipe          HAS_FLAG_PIPE)
check_cxx_compiler_flag(-pthread       HAS_FLAG_PTHREAD)

if(NOT HAS_FLAG_STD_CXX1Y)
    check_cxx_compiler_flag(-std=c++11     HAS_FLAG_STD_CXX11)
endif()
if(NOT HAS_FLAG_STD_CXX11)
    check_cxx_compiler_flag(-std=c++0x     HAS_FLAG_STD_CXX0X)
endif()

if(EXTRA_WARNINGS)
    check_cxx_compiler_flag(-Wall                HAS_WARNING_ALL)
    check_cxx_compiler_flag(-Wextra              HAS_WARNING_EXTRA)
    check_cxx_compiler_flag(-Weffc++             HAS_WARNING_EFFCXX)
    check_cxx_compiler_flag(-Wconversion         HAS_WARNING_CONVERSION)
    check_cxx_compiler_flag(-Wsign-conversion    HAS_WARNING_SIGN_CONVERSION)
    check_cxx_compiler_flag(-Woverloaded-virtual HAS_WARNING_OVERLOADED_VIRTUAL)
    # check_cxx_compiler_flag(-Weverything         HAS_WARNING_EVERYTHING)

    check_cxx_compiler_flag(-Wpedantic HAS_WARNING_WPEDANTIC)
    if(NOT HAS_WARNING_WPEDANTIC)
        check_cxx_compiler_flag(-pedantic HAS_WARNING_PEDANTIC)
    endif()

    if(HAS_WARNING_ALL)
        set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Wall")
    endif()
    if(HAS_WARNING_EXTRA)
        set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Wextra")
    endif()
    if(HAS_WARNING_EFFCXX)
        set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Weffc++")
    endif()
    if(HAS_WARNING_CONVERSION)
        set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Wconversion")
    endif()
    if(HAS_WARNING_SIGN_CONVERSION)
        set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Wsign-conversion")
    endif()
    if(HAS_WARNING_OVERLOADED_VIRTUAL)
        set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Woverloaded-virtual")
    endif()
    # if(HAS_WARNING_EVERYTHING)
    #   set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Weverything")
    # endif()
    if(HAS_WARNING_WPEDANTIC)
        set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -Wpedantic")
    endif()
    if(HAS_WARNING_PEDANTIC)
        set(CXX_WARNING_FLAGS "${CXX_WARNING_FLAGS} -pedantic")
    endif()
endif()

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS                "")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g -DDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DDEBUG")

if(HAS_FLAG_STD_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
elseif(HAS_FLAG_STD_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(HAS_FLAG_STD_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

if(HAS_FLAG_STDLIB_LIBCXX)
    # Check that libc++ actually exists
    set(CMAKE_REQUIRED_FLAGS "-stdlib=libc++")
    CHECK_CXX_SOURCE_COMPILES(
        "int main() { }"
        HAVE_LIBCXX
        FAIL_REGEX "cannot find -lc")
    if(HAVE_LIBCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
endif()

if(HAS_FLAG_PIPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
endif()

if(HAS_FLAG_PTHREAD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNING_FLAGS}")

######################################################################
# Other compiler specifics (`nullptr` etc.)

set(CMAKE_REQUIRED_FLAGS "${CMAKE_CXX_FLAGS}")

CHECK_CXX_SOURCE_COMPILES("int main() { void* p = nullptr; }"
    HAVE_NULLPTR
    FAIL_REGEX "use of undeclared identifier 'nullptr'")
if(NOT HAVE_NULLPTR)
    message(FATAL_ERROR "no `nullptr`")
endif()

CHECK_CXX_SOURCE_COMPILES("int main() { char c1[] = u8\"a\"; char16_t c2[] = u\"a\"; char32_t c3[] = U\"a\"; }"
    HAVE_UNICODE_LITERALS
    # clang++
    FAIL_REGEX "use of undeclared identifier 'u8'"
    FAIL_REGEX "use of undeclared identifier 'u'"
    FAIL_REGEX "use of undeclared identifier 'U'"
    FAIL_REGEX "unknown type name 'char16_t'"
    FAIL_REGEX "unknown type name 'char32_t'"
    # g++
    FAIL_REGEX "‘u8’ was not declared in this scope"
    FAIL_REGEX "‘char16_t’ was not declared in this scope"
    FAIL_REGEX "‘char32_t’ was not declared in this scope")

CHECK_CXX_SOURCE_COMPILES("int main() { char s[] = R\"delim(string)delim\"; }"
    HAVE_RAW_STRING_LITERALS
    # clang++
    FAIL_REGEX "use of undeclared identifier 'R'"
    # g++
    FAIL_REGEX "‘R’ was not declared in this scope")

# CHECK_CXX_SOURCE_COMPILES("int main() { const char# f = __PRETTY_FUNCTION__; }")
# CHECK_CXX_SOURCE_COMPILES("int main() { const char# f = __FUNCTION__; }")
# CHECK_CXX_SOURCE_COMPILES("int main() { const char# f = __func__; }")

CHECK_CXX_SOURCE_COMPILES("int main(int argc __attribute__((unused)), char* argv[] __attribute__((unused))) { }"
    HAVE_ATTRIBUTE_UNUSED
    # clang++
    FAIL_REGEX "warning: unknown attribute 'unused' ignored"
    # g++
    FAIL_REGEX "warning: ‘unused’ attribute directive ignored")

# TODO: more __attribute__(...) support?

# TODO: Computed gotos?

######################################################################
# TODO: Header files

######################################################################
# TODO: Structures

######################################################################
# TODO: Libraries

######################################################################
# TODO: Library functions

######################################################################
# TODO: System services (mtrace etc.)

######################################################################
# Boost

include(FindBoost)

list(APPEND REQUIRED_BOOST_LIBRARIES program_options)
list(APPEND REQUIRED_BOOST_LIBRARIES log)
if(ENABLE_TESTS)
    list(APPEND REQUIRED_BOOST_LIBRARIES unit_test_framework)
endif()

find_package(Boost 1.55.0 REQUIRED COMPONENTS ${REQUIRED_BOOST_LIBRARIES})

if(Boost_FOUND)
    include_directories("${Boost_INCLUDE_DIRS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK")
    list(APPEND CMAKE_REQUIRED_LIBRARIES ${Boost_LIBRARIES})
endif()

find_path(
    Boost_ASIO_PATH
    NAMES boost/asio.hpp
    PATHS ${Boost_INCLUDE_DIRS})
if(Boost_ASIO_PATH STREQUAL "Boost_ASIO_PATH-NOTFOUND")
    message(FATAL_ERROR "Boost.ASIO not found")
endif()

######################################################################
# Check if the "c++abi" library is needed

if(Boost_FOUND)
    set(TEMP_CMAKE_REQUIRED_LIBRARIES, ${CMAKE_REQUIRED_LIBRARIES})
endif()

CHECK_CXX_SOURCE_COMPILES("#include <iostream>\nint main() { std::cout << 'c'; }"
    LIBCXXABI_NOT_NEEDED
    FAIL_REGEX "undefined reference to symbol '__gxx_personality"
    FAIL_REGEX "undefined reference to symbol '__cxa_end_catch")

if(Boost_FOUND AND TEMP_CMAKE_REQUIRED_LIBRARIES)
    set(CMAKE_REQUIRED_LIBRARIES, ${TEMP_CMAKE_REQUIRED_LIBRARIES})
endif()

if(NOT LIBCXXABI_NOT_NEEDED)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi")
    list(APPEND CMAKE_REQUIRED_LIBRARIES "c++abi")
endif()

######################################################################

if(ENABLE_PROFILING)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # If we're using clang, we need to manually link with
        # <path>/clang/<version>/lib/linux/libclang_rt.profile-<system>.a

        message(STATUS "Performing test LIBCLANG_RT_PROFILE")

        # Create a list of possible locations
        list(APPEND CLANG_LOCATIONS "/usr/local")
        list(APPEND CLANG_LOCATIONS "/usr")
        list(APPEND CLANG_LOCATIONS "/opt")

        # Create a list of possible version variants
        string(REGEX MATCH
            "[0-9]+\\.[0-9]+\\.[0-9]+"
            CLANG_VERSION_3
            "${CMAKE_CXX_COMPILER_VERSION}"
        )
        string(REGEX MATCH
            "[0-9]+\\.[0-9]+"
            CLANG_VERSION_2
            "${CMAKE_CXX_COMPILER_VERSION}"
        )
        string(REGEX MATCH
            "[0-9]+"
            CLANG_VERSION_1
            "${CMAKE_CXX_COMPILER_VERSION}"
        )
        if (CLANG_VERSION_3)
            list(APPEND CLANG_VERSION_LIST ${CLANG_VERSION_3})
        endif()
        if (CLANG_VERSION_2)
            list(APPEND CLANG_VERSION_LIST ${CLANG_VERSION_2})
        endif()
        if (CLANG_VERSION_1)
            list(APPEND CLANG_VERSION_LIST ${CLANG_VERSION_1})
        endif()

        foreach(LOC ${CLANG_LOCATIONS})
            foreach(VER ${CLANG_VERSION_LIST})
                find_file(
                    LIBCLANG_RT_PROFILE
                    NAMES libclang_rt.profile-${CMAKE_SYSTEM_PROCESSOR}.a
                    PATHS ${LOC}/lib/clang/${VER}/lib/linux
                )


                if(NOT LIBCLANG_RT_PROFILE MATCHES "-NOTFOUND")
                    message(STATUS "Performing test LIBCLANG_RT_PROFILE - ${LIBCLANG_RT_PROFILE}")
                    break()
                endif()
            endforeach()

            if(NOT LIBCLANG_RT_PROFILE MATCHES "-NOTFOUND")
                break()
            endif()
        endforeach()

        if(LIBCLANG_RT_PROFILE MATCHES "-NOTFOUND")
            message(STATUS "Performing test LIBCLANG_RT_PROFILE - Failed")
        else()
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LIBCLANG_RT_PROFILE}")
            list(APPEND CMAKE_REQUIRED_LIBRARIES "${LIBCLANG_RT_PROFILE}")
        endif()
    endif()

    check_cxx_compiler_flag(-fprofile-arcs       HAS_PROFILE_ARCS)
    check_cxx_compiler_flag(-ftest-coverage      HAS_TEST_COVERAGE)

    if (HAS_PROFILE_ARCS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
    endif()
    if (HAS_TEST_COVERAGE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
    endif()
endif()

if(ENABLE_TESTS)
    enable_testing()

    # List all the tests
    list(APPEND ALL_TESTS dummy_test)

    # Add all the tests
    foreach(TEST ${ALL_TESTS})
        add_test(NAME "${TEST}"
                 COMMAND "${CMAKE_HOME_DIRECTORY}/build/src/tests/${TEST}"
                 WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}"
        )
    endforeach()

    # check_flag_coverage(HAS_FLAG_COVERAGE)
    # if(HAS_FLAG_COVERAGE)
    #     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

    # endif()
endif()

######################################################################
# Subdirectories and auto-generated files
add_subdirectory(src)

configure_file(
    "${PROJECT_SOURCE_DIR}/src/host/autoconf.h.in"
    "${PROJECT_SOURCE_DIR}/src/host/autoconf.h")
