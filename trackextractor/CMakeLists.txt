
cmake_minimum_required(VERSION 3.17)
project(TrackExtractorProject VERSION 1.0 LANGUAGES CXX)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")


### DEPENDENCIES ###

add_library(cv STATIC IMPORTED GLOBAL) 

set_target_properties(cv PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/extern/lib/cv.lib"
    IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/extern/lib/cv.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/extern/include/opencv/"
    
)
add_library(yaml STATIC IMPORTED GLOBAL) 

set_target_properties(yaml PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/extern/lib/yamlcpp.lib"
    IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/extern/lib/yamlcpp.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/extern/include/yaml-cpp/"
)

include_directories(
    "${CMAKE_SOURCE_DIR}/include/tictoc/"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/"
    "${CMAKE_SOURCE_DIR}/extern/include/opencv/"
    "${CMAKE_SOURCE_DIR}/extern/include/yaml-cpp/"
)

LINK_DIRECTORIES(
    "${CMAKE_SOURCE_DIR}/extern/lib/"
)


######################### TICTOC LIBRARY ############################

set(TT_HEADER_LIST
    ${CMAKE_SOURCE_DIR}/include/tictoc/tictoc.h
    ${CMAKE_SOURCE_DIR}/include/tictoc/Timer.h
    ${CMAKE_SOURCE_DIR}/include/tictoc/subpart.h
)
add_library(
    tictoc STATIC
    ${CMAKE_SOURCE_DIR}/src/tictoc/tictoc.cpp
    ${CMAKE_SOURCE_DIR}/src/tictoc/Timer.cpp
    ${CMAKE_SOURCE_DIR}/src/tictoc/subpart.cpp
    ${TT_HEADER_LIST}
    )

install(
    TARGETS tictoc
    EXPORT tictoc-export
    LIBRARY DESTINATION installed/lib
    ARCHIVE DESTINATION installed/lib
    )

install(
    EXPORT
        tictoc-export
    FILE
        tictoc.cmake
    DESTINATION
        installed/lib/cmake/tictoc
)

install(
    FILES
        ${TT_HEADER_LIST}
    DESTINATION 
        installed/include/tictoc/
)

install(
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tictocConfig.cmake
    DESTINATION "lib/cmake/tictoc" 
)

### TRACKEXTRACTOR LIBRARY ###

set(TE_HEADER_LIST 
    "${CMAKE_SOURCE_DIR}/include/trackextractor/AutoThresholder.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/BatchExtractor.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/communicator.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/ContourPoint.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/CvUtilsPlusPlus.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/FrameLoader.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/ImLoader.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/ImTrackPoint.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/MaggotTrackBuilder.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/MaggotTrackExtractor.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/MaggotTrackPoint.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/PointExtractor.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/track.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/trackbuilder.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/TrackExtractor.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/trackmatch.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/trackpoint.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/WormPointExtractor.h"
    "${CMAKE_SOURCE_DIR}/include/trackextractor/WormTrackBuilder.h"
)

add_library(trackextractor 
    "${CMAKE_SOURCE_DIR}/src/trackextractor/BatchExtractor.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/AutoThresholder.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/communicator.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/ContourPoint.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/CvUtilsPlusPlus.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/FrameLoader.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/ImLoader.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/ImTrackPoint.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/MaggotTrackBuilder.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/MaggotTrackExtractor.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/PointExtractor.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/track.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/trackbuilder.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/TrackExtractor.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/trackmatch.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/trackpoint.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/WormPointExtractor.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackextractor/WormTrackBuilder.cpp"
    "${TE_HEADER_LIST}"
    
)

target_link_libraries(
    trackextractor 
    tictoc
)
target_link_libraries(
    trackextractor
    yamlcpp
)
target_link_libraries(
    trackextractor
    cv
)
