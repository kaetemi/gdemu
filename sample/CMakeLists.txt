
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdsample
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdsample
	libgdemu
	)
