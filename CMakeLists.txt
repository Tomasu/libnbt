cmake_minimum_required(VERSION 2.6)
project(nbt CXX)

find_package(ZLIB REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	add_definitions(-DDEBUG -O0 -ggdb3)
else()
	add_definitions(-O2 -s -march=native)
endif()

add_definitions(-Wextra -Wall -pipe -std=gnu++11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${ZLIB_INCLUDE_DIRS})

add_library(nbt SHARED
	src/NBT.cpp
	src/NBT_Debug.cpp
	src/NBT_File.cpp
	src/NBT_Tag.cpp
	src/NBT_Tag_Byte.cpp
	src/NBT_Tag_Byte_Array.cpp
	src/NBT_Tag_Compound.cpp
	src/NBT_Tag_Double.cpp
	src/NBT_Tag_Float.cpp
	src/NBT_Tag_Int.cpp
	src/NBT_Tag_Int_Array.cpp
	src/NBT_Tag_List.cpp
	src/NBT_Tag_Long.cpp
	src/NBT_Tag_Short.cpp
	src/NBT_Tag_String.cpp
	include/NBT.h
	include/NBT_Debug.h
	include/NBT_File.h
	include/NBT_Tag.h
	include/NBT_Tag_Byte.h
	include/NBT_Tag_Byte_Array.h
	include/NBT_Tag_Compound.h
	include/NBT_Tag_Double.h
	include/NBT_Tag_Float.h
	include/NBT_Tag_Int.h
	include/NBT_Tag_Int_Array.h
	include/NBT_Tag_List.h
	include/NBT_Tag_Long.h
	include/NBT_Tag_Short.h
	include/NBT_Tag_String.h
)

target_link_libraries(nbt ${ZLIB_LIBRARIES})

#set(NBT_CFLAGS -I${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
#set(NBT_LIBS -Wl,--rpath,${CMAKE_CURRENT_BINARY_DIR} -L${CMAKE_CURRENT_BINARY_DIR} -lnbt PARENT_SCOPE)
