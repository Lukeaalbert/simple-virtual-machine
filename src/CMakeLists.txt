set(HEADER_FILES
	SrcMain.h
	Parser.hpp
	Ops.h
	Machine.h
	Exceptions.h
)

set(SOURCE_FILES
	SrcMain.cpp
	Ops.cpp
	Machine.cpp
)

add_library(src ${SOURCE_FILES} ${HEADER_FILES})
