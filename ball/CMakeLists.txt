
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdball
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdball
	libgdemu
	)
