cmake_minimum_required(VERSION 2.8)
project(libnbt CXX)

set(LIBNBT_MAJOR_VERSION 0)
set(LIBNBT_MINOR_VERSION 1)
set(LIBNBT_PATCH_VERSION 0)
set(LIBNBT_VERSION
  ${LIBNBT_MAJOR_VERSION}.${LIBNBT_MINOR_VERSION}.${LIBNBT_PATCH_VERSION})
  
find_package(ZLIB REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	add_definitions(-DDEBUG -O0 -ggdb3)
else()
	add_definitions(-O2 -s -march=native)
endif()

add_definitions(-Wextra -Wall -pipe -std=gnu++11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${ZLIB_INCLUDE_DIRS})

# Offer the user the choice of overriding the installation directories

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/cmake/LibNBT)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")
 
# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

set(HEADERS
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

set(SOURCES
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
)


add_library(nbt SHARED ${SOURCES} ${HEADERS})

target_link_libraries(nbt ${ZLIB_LIBRARIES})

set_target_properties(nbt PROPERTIES PUBLIC_HEADER "${HEADERS}")
  
  
# Add all targets to the build-tree export set
export(TARGETS nbt
  FILE "${PROJECT_BINARY_DIR}/LibNBTTargets.cmake")
  
# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE LibNBT)

# Create the LibNBTConfig.cmake and LibNBTConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
   
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}")
configure_file(cmake/LibNBTConfig.cmake.in
  "${PROJECT_BINARY_DIR}/LibNBTConfig.cmake" @ONLY)

# ... for the install tree
set(CONF_INCLUDE_DIRS "\${LIBNBT_CMAKE_DIR}/${REL_INCLUDE_DIR}/libnbt")
configure_file(cmake/LibNBTConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/LibNBTConfig.cmake" @ONLY)
# ... for both
configure_file(cmake/LibNBTConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/LibNBTConfigVersion.cmake" @ONLY)
 
# Install the LibNBTConfig.cmake and LibNBTConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/LibNBTConfig.cmake"
  "${PROJECT_BINARY_DIR}/LibNBTConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
 
# Install the export set for use with the install-tree
install(EXPORT LibNBTTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)

install(TARGETS nbt
	# IMPORTANT: Add the libnbt library to the "export-set"
	EXPORT LibNBTTargets
	RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
	LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
	PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/libnbt"
	COMPONENT dev)

