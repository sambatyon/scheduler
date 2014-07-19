# - Try to find JsonCpp
# Once done it will define
#  JSONCPP_FOUND - System has LibXml2
#  LIBXML2_INCLUDE_DIRS - The LibXml2 include directories
#  LIBXML2_LIBRARIES - The libraries needed to use LibXml2

set(JSONCPP_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../other_libs/jsoncpp)


find_path(JSONCPP_INCLUDE_DIR json/json.h
          HINTS ${JSONCPP_ROOT}/include)

find_library(JSONCPP_LIBRARY_DEBUG NAMES jsoncpp-d
             HINTS ${JSONCPP_ROOT}/lib )

find_library(JSONCPP_LIBRARY_RELEASE NAMES jsoncpp
             HINTS ${JSONCPP_ROOT}/lib )

set(JSONCPP_LIBRARIES optimized;${JSONCPP_LIBRARY_RELEASE};debug;${JSONCPP_LIBRARY_DEBUG} )
set(JSONCPP_INCLUDE_DIRS ${JSONCPP_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(JsonCpp  DEFAULT_MSG
                                  JSONCPP_LIBRARIES JSONCPP_INCLUDE_DIRS)

mark_as_advanced(JSONCPP_INCLUDE_DIRS JSONCPP_LIBRARIES )
