
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdselftest
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdselftest
	libgdemu
	)
