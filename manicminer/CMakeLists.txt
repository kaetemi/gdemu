
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdmanicminer
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdmanicminer
	libgdemu
	)
