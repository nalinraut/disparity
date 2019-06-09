cmake_minimum_required(VERSION 2.8)
project(DISPARITY)

set(CMAKE_INCLUDE_CURRENT_DIR ON)



LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
find_package(OpenCV 3 REQUIRED)
include_directories($(OpenCV_INCLUDE_DIRS))

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse3")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

include_directories(
${PROJECT_SOURCE_DIR}
${PROJECT_SOURCE_DIR}/include
)

add_library(${PROJECT_NAME} SHARED
                            src/disparity.cpp
)

target_link_libraries(${PROJECT_NAME}
${OpenCV_LIBS}
${OpenMP_CXX_LIBRARIES}
)

add_executable(disparity-test src/disparity-test.cpp)
target_link_libraries(disparity-test ${OpenMP_CXX_LIBRARIES} ${PROJECT_NAME} )

