
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdcp437
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdcp437
	libgdemu
	)
