
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdjoytest
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdjoytest
	libgdemu
	)
