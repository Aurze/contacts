# FindFuerte
# --------
#
# Find ArangoDB Fuerte
#
# ::
#
#   FUERTE_INCLUDE_DIRS   - where to find fuerte/fuerte.h, etc.
#   FUERTE_LIBRARIES      - List of libraries when using fuerte.
#   FUERTE_FOUND          - True if fuerte found.
#   FUERTE_VERSION_STRING - the version of fuerte found (since CMake 2.8.8)

# Look for the header file.
find_path(FUERTE_INCLUDE_DIR 
  NAMES fuerte.h
  PATHS "/usr/local/include/fuerte"
  NO_DEFAULT_PATH)

mark_as_advanced(FUERTE_INCLUDE_DIR)

# Look for the library (sorted from most current/relevant entry to least).
find_library(FUERTE_LIBRARY NAMES
    libfuerte
    libfuerte.a
  PATHS "/usr/local/lib"
  NO_DEFAULT_PATH)

mark_as_advanced(FUERTE_LIBRARY)

if(FUERTE_INCLUDE_DIR)
  if(EXISTS "${FUERTE_INCLUDE_DIR}/fuerte/fuerte-version-number.h")
    file(STRINGS "${FUERTE_INCLUDE_DIR}/fuerte/fuerte-version-number.h" fuerte_version_str REGEX "^#define[\t ]+FUERTE_VERSION[\t ]+\".*\"")

    string(REGEX REPLACE "^#define[\t ]+FUERTE_VERSION[\t ]+\"([^\"]*)\".*" "\\1" FUERTE_VERSION_STRING "${fuerte_version_str}")
    unset(fuerte_version_str)
  endif()
endif()

# handle the QUIETLY and REQUIRED arguments and set FUERTE_FOUND to TRUE if
# all listed variables are TRUE
include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FUERTE
                                  REQUIRED_VARS FUERTE_LIBRARY FUERTE_INCLUDE_DIR
                                  VERSION_VAR FUERTE_VERSION_STRING)



                                message( ${FUERTE_LIBRARY})
                                message( ${FUERTE_INCLUDE_DIR})
if(FUERTE_FOUND)
  set(FUERTE_LIBRARIES ${FUERTE_LIBRARY})
  set(FUERTE_INCLUDE_DIRS ${FUERTE_INCLUDE_DIR})
endif()

