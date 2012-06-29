
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdikeda
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdikeda
	libgdemu
	)
