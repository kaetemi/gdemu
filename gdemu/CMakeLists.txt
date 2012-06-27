
FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h avr/*.h)

ADD_LIBRARY(libgdemu
	${SRCS}
	${HDRS}
	)

target_link_libraries(libgdemu
	)

option(WITH_SDL "Build an SDL application." ON)

if (WITH_SDL)

  # SDL base package
  find_package (SDL)
  if (NOT SDL_FOUND)
    message (FATAL_ERROR "SDL not found!")
  endif (NOT SDL_FOUND)
  include_directories(
    ${SDL_INCLUDE_DIR}
    ${INCLUDE_DIRECTORIES}
    )
  target_link_libraries(libgdemu
    ${SDL_LIBRARY}
    ${TARGET_LINK_LIBRARIES}
    )

  SET(CMAKE_C_FLAGS "-DGDEMU_SDL ${PLATFORM_CFLAGS}" CACHE STRING "" FORCE)
  SET(CMAKE_CXX_FLAGS "-DGDEMU_SDL ${PLATFORM_CXXFLAGS}" CACHE STRING "" FORCE)

endif (WITH_SDL)
