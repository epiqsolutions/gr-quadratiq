INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_QUADRATIQ quadratiq)

FIND_PATH(
    QUADRATIQ_INCLUDE_DIRS
    NAMES quadratiq/api.h
    HINTS $ENV{QUADRATIQ_DIR}/include
        ${PC_QUADRATIQ_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    QUADRATIQ_LIBRARIES
    NAMES gnuradio-quadratiq
    HINTS $ENV{QUADRATIQ_DIR}/lib
        ${PC_QUADRATIQ_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(QUADRATIQ DEFAULT_MSG QUADRATIQ_LIBRARIES QUADRATIQ_INCLUDE_DIRS)
MARK_AS_ADVANCED(QUADRATIQ_LIBRARIES QUADRATIQ_INCLUDE_DIRS)

