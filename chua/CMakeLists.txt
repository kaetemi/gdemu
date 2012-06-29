
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

ADD_EXECUTABLE(gdchua
	${SRCS}
	${HDRS}
	)

target_link_libraries(gdchua
	libgdemu
	)
