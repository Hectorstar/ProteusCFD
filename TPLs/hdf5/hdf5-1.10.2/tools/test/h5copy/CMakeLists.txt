cmake_minimum_required (VERSION 3.10)
PROJECT (HDF5_TOOLS_TEST_H5COPY)

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (${HDF5_TOOLS_DIR}/lib)

# --------------------------------------------------------------------
# Add the h5copy test executables
# --------------------------------------------------------------------

if (HDF5_BUILD_GENERATORS)
  add_executable (h5copygentest ${HDF5_TOOLS_TEST_H5COPY_SOURCE_DIR}/h5copygentest.c)
  TARGET_C_PROPERTIES (h5copygentest STATIC " " " ")
  target_link_libraries (h5copygentest ${HDF5_LIB_TARGET})
  set_target_properties (h5copygentest PROPERTIES FOLDER generator/tools)

  #add_test (NAME h5copygentest COMMAND $<TARGET_FILE:h5copygentest>)
endif ()

#-----------------------------------------------------------------------------
# If plugin library tests can be tested
#-----------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
  set (HDF5_TOOL_PLUGIN_LIB_CORENAME         "dynlibcopy")
  set (HDF5_TOOL_PLUGIN_LIB_NAME             "${HDF5_EXTERNAL_LIB_PREFIX}${HDF5_TOOL_PLUGIN_LIB_CORENAME}")
  set (HDF5_TOOL_PLUGIN_LIB_TARGET           ${HDF5_TOOL_PLUGIN_LIB_CORENAME})
  add_definitions (${HDF_EXTRA_C_FLAGS})
  INCLUDE_DIRECTORIES (${HDF5_SRC_DIR})

  add_library (${HDF5_TOOL_PLUGIN_LIB_TARGET} SHARED dynlib_copy.c)
  TARGET_C_PROPERTIES (${HDF5_TOOL_PLUGIN_LIB_TARGET} SHARED " " " ")
  target_link_libraries (${HDF5_TOOL_PLUGIN_LIB_TARGET} ${HDF5_TEST_LIB_TARGET})
  H5_SET_LIB_OPTIONS (${HDF5_TOOL_PLUGIN_LIB_TARGET} ${HDF5_TOOL_PLUGIN_LIB_NAME} SHARED ${HDF5_PACKAGE_SOVERSION})

  # make plugins dir
  file (MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")
  #-----------------------------------------------------------------------------
  # Copy plugin library to a plugins folder
  #-----------------------------------------------------------------------------
  add_custom_command (
      TARGET     ${HDF5_TOOL_PLUGIN_LIB_TARGET}
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different
          "$<TARGET_FILE:${HDF5_TOOL_PLUGIN_LIB_TARGET}>"
          "${CMAKE_BINARY_DIR}/plugins/$<TARGET_FILE_NAME:${HDF5_TOOL_PLUGIN_LIB_TARGET}>"
  )
endif ()

include (CMakeTests.cmake)
