# Computer Graphics and Media Informatics
# University of Konstanz
# 2015

cmake_minimum_required (VERSION 2.8)

set (project_name CG_framework)

# create project
project (${project_name})

# set module path
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# using utils
include(utils)

# build directory has to be in a different directory
ENSURE_OUT_OF_SOURCE_BUILD(  "${CMAKE_PROJECT_NAME} requires an out of source build." )

# sources
set(SOURCES main.cpp)

# shaders 
set(SHADERS "Shader/Default33.frag.glsl" "Shader/Default33.vert.glsl")

set(prog_output)
test_wrap_cpp(prog_output ${SHADERS})

# add headers to the project
file(GLOB MY_HEADERS "src/*.h")
list(APPEND HEADERS ${MY_HEADERS})

# add sources to the project
file(GLOB MY_SOURCES "src/*.cpp")
list(APPEND SOURCES ${MY_SOURCES})                 
				  
				  
# OpenGL
find_package(OpenGL REQUIRED)	
if (NOT OPENGL_FOUND)
	message (FATAL_ERROR "OpenGL not found!")
	return()
endif ()
include_directories(${OPENGL_INCLUDE_DIRS})
list(APPEND used_libaries  ${OPENGL_LIBRARIES})

if(OPENGL_FOUND)
	MESSAGE(STATUS "OPENGL_INCLUDE_DIRS = ${OPENGL_INCLUDE_DIRS}")
	MESSAGE(STATUS "OPENGL_LIBRARIES = ${OPENGL_LIBRARIES}")
endif()

# GLFW
find_package(GLFW REQUIRED)
if (NOT GLFW_FOUND)
	message (FATAL_ERROR "GLFW not found!")
	return()
endif ()
include_directories(${GLFW_INCLUDE_DIRS})
list(APPEND used_libaries  ${GLFW_LIBRARIES})

if(GLFW_FOUND)
	MESSAGE(STATUS "GLFW_INCLUDE_DIRS = ${GLFW_INCLUDE_DIRS}")
	MESSAGE(STATUS "GLFW_LIBRARIES = ${GLFW_LIBRARIES}")
endif()


# GLEW not needed for Apple
find_package(GLEW)
if (NOT GLEW_FOUND AND NOT APPLE)
	message (FATAL_ERROR "GLEW not found!")
	return()
elseif(GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIRS})
	list(APPEND used_libaries  ${GLEW_LIBRARIES})
	
	MESSAGE(STATUS "GLEW_INCLUDE_DIRS = ${GLEW_INCLUDE_DIRS}")
	MESSAGE(STATUS "GLEW_LIBRARIES = ${GLEW_LIBRARIES}")
endif ()


find_package(GLM)
if(NOT GLM_FOUND)
	message(FATAL_ERROR "GLM not found")
elseif(GLM_FOUND)
	include_directories(${GLM_INCLUDE_DIRS})
endif ()


# executable
add_executable(${project_name} ${SOURCES} ${HEADERS} ${SHADERS})

target_link_libraries(${project_name}  ${used_libaries})
