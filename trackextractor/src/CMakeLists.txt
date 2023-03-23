# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Optionally glob, but only for CMake 3.12 or later:
# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")
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

# Make an automatic library - will be static or dynamic based on user setting
add_library(trackextractor 
    "${CMAKE_SOURCE_DIR}/src/BatchExtractor.cpp"
    "${CMAKE_SOURCE_DIR}/src/AutoThresholder.cpp"
    "${CMAKE_SOURCE_DIR}/src/communicator.cpp"
    "${CMAKE_SOURCE_DIR}/src/ContourPoint.cpp"
    "${CMAKE_SOURCE_DIR}/src/CvUtilsPlusPlus.cpp"
    "${CMAKE_SOURCE_DIR}/src/FrameLoader.cpp"
    "${CMAKE_SOURCE_DIR}/src/ImLoader.cpp"
    "${CMAKE_SOURCE_DIR}/src/ImTrackPoint.cpp"
    "${CMAKE_SOURCE_DIR}/src/MaggotTrackBuilder.cpp"
    "${CMAKE_SOURCE_DIR}/src/MaggotTrackExtractor.cpp"
    "${CMAKE_SOURCE_DIR}/src/PointExtractor.cpp"
    "${CMAKE_SOURCE_DIR}/src/track.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackbuilder.cpp"
    "${CMAKE_SOURCE_DIR}/src/TrackExtractor.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackmatch.cpp"
    "${CMAKE_SOURCE_DIR}/src/trackpoint.cpp"
    "${CMAKE_SOURCE_DIR}/src/WormPointExtractor.cpp"
    "${CMAKE_SOURCE_DIR}/src/WormTrackBuilder.cpp"
    "${TE_HEADER_LIST}"
)

# We need this directory, and users of our library will need it too
target_include_directories(trackextractor PUBLIC "${TE_HEADER_LIST}")

# This depends on (header only) boost
target_link_libraries(
    trackextractor 
    PRIVATE tictoc  
    STATIC IMPORTED cv
    STATIC IMPORTED yaml
)

# IDEs should put the headers in a nice place
#source_group(
#  TREE "${PROJECT_SOURCE_DIR}/include"
#  PREFIX "Header Files"
#  FILES ${HEADER_LIST})

