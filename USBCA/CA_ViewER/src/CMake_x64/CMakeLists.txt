cmake_minimum_required(VERSION 3.1.0)

project(CA_ViewER)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Include directories for this project
set(INCLUDE_DIR
  ".."
  "${PROJECT_SOURCE_DIR}/../../opencv/include"
  "${PROJECT_SOURCE_DIR}/../../../USBCAExtensionUnit/inc"
)

# Add a include files
include_directories("${INCLUDE_DIR}")

set(LIBRARIES
  "${PROJECT_SOURCE_DIR}/../../opencv/x64/lib/opencv_world342.lib"
  "${PROJECT_SOURCE_DIR}/../../opencv/x64/lib/opencv_world342d.lib"
  "${PROJECT_SOURCE_DIR}/../../../USBCAExtensionUnit/lib/x64/USBCAExtensionUnit.lib"
)

# Add user definition
add_compile_options(-DCA378_AOIS)

# Set the linker subsystem
set(CMAKE_EXE_LINKER_FLAGS
	"${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)

# use Qt_DIR approach so you can find Qt after cmake has been invoked 
SET(Qt5Widgets_DIR "${PROJECT_SOURCE_DIR}/../../Qt/x64/lib/cmake/Qt5Widgets")

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

# Populate a CMake variable with the sources
set(CA_ViewER_SRCS
    ../mainwindow.ui
    ../mainwindow.cpp
    ../mainwindow.h
    ../main.cpp
    ../capture.cpp
    ../capture.h
    ../demo_control.cpp
    ../demo_control.h
    ../af_control.cpp
    ../af_control.h
    ../tool.hpp
    ../CA_ViewER_resource.rc
    ../mode.cpp
    ../mode.h
    ../usbinfo.cpp
    ../usbinfo.h
)
# Tell CMake to create the CA_ViewER executable
add_executable(CA_ViewER ${CA_ViewER_SRCS})
# Use the Widgets module from Qt 5
target_link_libraries(CA_ViewER Qt5::Widgets ${LIBRARIES})
