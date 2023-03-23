 

cmake_minimum_required(VERSION 3.17)
project(TrackExtractorProject VERSION 1.0 LANGUAGES CXX)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")

### DEPENDENCIES ###

add_library(cv STATIC IMPORTED) 

set_target_properties(cv PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/extern/lib/cv.lib"
    #IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/extern/lib/cv.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/extern/include/opencv/"
    
)
add_library(yaml STATIC IMPORTED) 

set_target_properties(yaml PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/extern/lib/yamlcpp.lib"
    #IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/extern/lib/yamlcpp.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/extern/include/yaml-cpp/"
)

add_library(trackextractor STATIC IMPORTED)

set_target_properties(trackextractor PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/extern/lib/trackextractor.lib"
    #IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/extern/lib/trackextractor.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/extern/include/trackextractor/"
)


include_directories(
    #"${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/extern/include/trackextractor/"
    "${CMAKE_SOURCE_DIR}/extern/include/opencv/"
    "${CMAKE_SOURCE_DIR}/extern/include/yaml-cpp/"
)

LINK_DIRECTORIES(
    "${CMAKE_SOURCE_DIR}/extern/lib/"
)

add_executable(extracktapp main.cpp)

#target_link_libraries(
#    extracktapp
#    cv
#)

target_link_libraries(
    extracktapp
    yaml
)

target_link_libraries(
    extracktapp
    trackextractor
)