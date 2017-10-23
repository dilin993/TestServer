cmake_minimum_required(VERSION 3.5)
project(server)

set(CMAKE_CXX_STANDARD 11)
find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED COMPONENTS system)
include_directories( ${OpenCV_INCLUDE_DIRS})
include_directories( ${Boost_INCLUDE_DIR})

file(GLOB SOURCE_FILES
        "src/*.h"
        "src/*.cpp"
        "src/*.hpp"
        )

#set(SOURCE_FILES src/main.cpp src/Server.cpp src/Server.h src/MyTypes.h)

add_executable(server ${SOURCE_FILES} src/BackSubDetector.cpp src/BackSubDetector.h src/main.cpp)

target_link_libraries(server ${OpenCV_LIBS} ${Boost_LIBRARIES} -lpthread)