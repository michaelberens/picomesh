cmake_minimum_required (VERSION 2.6)

project (picomesh-test)

add_subdirectory(src)
add_subdirectory(tests)

set (PM_COMPLETE
    ${HEADER_PM}
    ${SOURCE_PM}
    )

#message (STATUS "picomesh sources: ${PM_COMPLETE}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O1 /openmp")

add_executable(picomesh-test ${SOURCE_PM_MAIN} ${PM_COMPLETE})

