# Taken from https://github.com/Tariq-Abuhashim/rvgslam

#.rst:
# FindVLFeat
# -------
#
# Find the VLFEAT, an open source computer vision library.
#
# Once done this will define the following variables::
#
#   VLFEAT_INCLUDE_DIRS    - Vlfeat include directory
#   VLFEAT_LIBRARIES       - Vlfeat libraries 
#   VLFEAT_FOUND           - if false, you cannot build anything that requires VLFEAT



# Change this to your own VL folder
SET(VLFEAT_ROOT /home/shipu/facu/pf/libs/vlfeat)

include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)
find_path(VLFEAT_INCLUDE_DIR
          NAMES vl
          PATHS ${VLFEAT_ROOT}
          DOC "VLFEAT include directory")
find_library(VLFEAT_LIBRARY_RELEASE
             NAMES vl
             PATHS ${VLFEAT_ROOT}/bin/glnxa64
             DOC "VLFEAT library file (release version)")
find_library(VLFEAT_LIBRARY_DEBUG
             NAMES vld 
             PATHS ${VLFEAT_ROOT}/bin/glnxa64
             DOC "VLFEAT library file (debug version)") 

mark_as_advanced(VLFEAT_INCLUDE_DIR
                 VLFEAT_LIBRARY_RELEASE
                 VLFEAT_LIBRARY_DEBUG)

select_library_configurations(VLFEAT)

set(VLFEAT_LIBRARIES ${VLFEAT_LIBRARY})
set(VLFEAT_INCLUDE_DIRS ${VLFEAT_INCLUDE_DIR}) 

# find_package_handle_standard_args(VLFEAT
#                                   FOUND_VAR VLFEAT_FOUND
#                                   REQUIRED_VARS VLFEAT_LIBRARIES VLFEAT_INCLUDE_DIRS)

# Set package properties if FeatureSummary was included
if(COMMAND set_package_properties)
    set_package_properties(Libedit PROPERTIES DESCRIPTION "The VLFeat open source library implements popular computer vision algorithms specializing in image          understanding and local features extraction and matching. "
                                              URL "http://www.vlfeat.org/")
endif()

