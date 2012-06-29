
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdwireframe
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdwireframe
	libgdemu
	)
