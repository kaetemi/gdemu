
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdtoccata
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdtoccata
	libgdemu
	)
