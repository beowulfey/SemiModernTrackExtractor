#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "trackextractor" for configuration "RelWithDebInfo"
set_property(TARGET trackextractor APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(trackextractor PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "/out/lib/trackextractor.lib"
  )

list(APPEND _cmake_import_check_targets trackextractor )
list(APPEND _cmake_import_check_files_for_trackextractor "/out/lib/trackextractor.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
