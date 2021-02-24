SET(KFR_ROOT /home/shipu/facu/pf/libs/kfr)

include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)
find_path(KFR_INCLUDE_DIR
          NAMES kfr
          PATHS ${KFR_ROOT}
          DOC "KFR include directory")
find_library(KFR_LIBRARY_RELEASE
             NAMES kfr
             PATHS ${KFR_ROOT}/build/lib
             DOC "KFR library file (release version)")

mark_as_advanced(KFR_INCLUDE_DIR
    KFR_LIBRARY_RELEASE)

set(KFR_LIBRARIES ${KFR_LIBRARY})
set(KFR_INCLUDE_DIRS ${KFR_INCLUDE_DIR}) 

# find_package_handle_standard_args(KFR
#                                   FOUND_VAR KFR_FOUND
#                                   REQUIRED_VARS KFR_LIBRARIES KFR_INCLUDE_DIRS)

