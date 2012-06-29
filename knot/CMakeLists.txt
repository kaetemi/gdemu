
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdknot
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdknot
	libgdemu
	)
