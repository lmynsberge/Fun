cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)

if (NOT ParaView_BINARY_DIR)
  find_package(ParaView REQUIRED)
  include(${PARAVIEW_USE_FILE})
endif()

include(ParaViewPlugins)

ADD_PARAVIEW_PLUGIN(SMTagBlock "1.0"
  SERVER_MANAGER_XML TagBlockFilter.xml
  SERVER_MANAGER_SOURCES vtkTagBlockFilter.cxx)
