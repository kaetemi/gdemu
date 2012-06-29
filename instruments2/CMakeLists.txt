
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdinstruments2
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdinstruments2
	libgdemu
	)
