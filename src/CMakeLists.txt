CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE(Boost COMPONENTS system thread filesystem date_time)
find_package(Log4cplus)

FILE(GLOB_RECURSE scheduler_src *.cpp)
FILE(GLOB_RECURSE scheduler_hdr *.hpp)

SOURCE_GROUP("Header Files" FILES ${scheduler_hdr})
SOURCE_GROUP("Source Files" FILES ${scheduler_src})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${LOG4CPLUS_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE(scheduler ${scheduler_src} ${scheduler_hdr})

TARGET_LINK_LIBRARIES(scheduler ${Boost_LIBRARIES} ${LOG4CPLUS_LIBRARIES})

#SET_TARGET_PROPERTIES(test-app PROPERTIES DEBUG_POSTFIX "-d")
