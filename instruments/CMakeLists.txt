
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdinstruments
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdinstruments
	libgdemu
	)
