include(GNUInstallDirs)

find_library(
    LIBCV_LOC
    NAMES libcv
)

find_path(LIBCV_INCLOC
  NAMES Pipeline.hpp
  HINTS ${PROJECT_BINARY_DIR}/prebuilt/ ${CMAKE_INSTALL_INCLUDEDIR}
  PATH_SUFFIXES LibImagePipeline/native-linux-api/imagepipeline/ LibImagePipeline/imagepipeline)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(libImagePipeline DEFAULT_MSG
                                  LIBIMAGEPIPELINE_LIBRARY
                                  LIBIMAGEPIPELINE_INCLUDE_DIR)

mark_as_advanced(LIBIMAGEPIPELINE_LIBRARY LIBIMAGEPIPELINE_INCLUDE_DIR)

if(LIBIMAGEPIPELINE_FOUND AND NOT TARGET libImagePipeline::libImagePipeline)
  add_library(libImagePipeline::libImagePipeline SHARED IMPORTED)
  set_target_properties(
    libImagePipeline::libImagePipeline
    PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${LIBIMAGEPIPELINE_INCLUDE_DIR}"
      IMPORTED_LOCATION ${LIBIMAGEPIPELINE_LIBRARY})
endif()