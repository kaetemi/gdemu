
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdplayer
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdplayer
	libgdemu
	)
