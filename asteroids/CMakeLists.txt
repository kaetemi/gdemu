
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdasteroids
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdasteroids
	libgdemu
	)
