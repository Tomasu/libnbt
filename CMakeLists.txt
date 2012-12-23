project(nbt CXX)

#add_definitions(-fPIC)

add_library(nbt SHARED
	NBT.cpp
	NBT_Buffer.cpp
	NBT_Debug.cpp
	NBT_Tag.cpp
	NBT_Tag_Byte.cpp
	NBT_Tag_Byte_Array.cpp
	NBT_Tag_Compound.cpp
	NBT_Tag_Double.cpp
	NBT_Tag_Float.cpp
	NBT_Tag_Int.cpp
	NBT_Tag_Int_Array.cpp
	NBT_Tag_List.cpp
	NBT_Tag_Long.cpp
	NBT_Tag_Short.cpp
	NBT_Tag_String.cpp
)

target_link_libraries(nbt ${ZLIB_LIBRARIES})

set(NBT_CFLAGS -I${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set(NBT_LIBS -Wl,--rpath,${CMAKE_CURRENT_BINARY_DIR} -L${CMAKE_CURRENT_BINARY_DIR} -lnbt PARENT_SCOPE)
