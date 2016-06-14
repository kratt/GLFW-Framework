MACRO(ENSURE_OUT_OF_SOURCE_BUILD MSG)
     STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}"
     "${CMAKE_BINARY_DIR}" insource)
     GET_FILENAME_COMPONENT(PARENTDIR ${CMAKE_SOURCE_DIR} PATH)
     STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}"
     "${PARENTDIR}" insourcesubdir)
    IF(insource OR insourcesubdir)
        MESSAGE(FATAL_ERROR "${MSG}")
    ENDIF(insource OR insourcesubdir)
ENDMACRO(ENSURE_OUT_OF_SOURCE_BUILD)


macro (test_wrap_cpp outfiles )
  # get include dirs
  
  set(_SHADERS)
  set(_PROGRAMS)
  set(_PROGRAMS_CPP)
  foreach(file ${ARGN})
    if(${file} MATCHES "\\.vert$")
        list(APPEND _SHADERS ${file})
    elseif(${file} MATCHES "\\.frag$")
        list(APPEND _SHADERS ${file})
    elseif(${file} MATCHES "\\.geom$")
        list(APPEND _SHADERS ${file})
    elseif(${file} MATCHES "\\.tess$")
        list(APPEND _SHADERS ${file})
    elseif(${file} MATCHES "\\.eval$")
        list(APPEND _SHADERS ${file})
    elseif(${file} MATCHES "\\.glsl$")
        list(APPEND _SHADERS ${file})
    elseif( ${file} MATCHES "\\.glpr$" )
        list(APPEND _PROGRAMS ${file})
        
        add_custom_command(
          PRE_BUILD
          OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${file}.cpp
          COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/${file}.cpp
          COMMENT "Preprocessing file"
          DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${file}
          VERBATIM)
        
        list(APPEND _PROGRAMS_CPP ${CMAKE_CURRENT_BINARY_DIR}/${file}.cpp)
    endif()
  endforeach()
  

  add_custom_target(shaders DEPENDS ${_PROGRAMS_CPP})
  SET(${outfiles} ${_PROGRAMS_CPP})
  
  source_group("glsl::Shader Files" FILES ${_SHADERS})
  source_group("glsl::Program Files" FILES ${_PROGRAMS})
endmacro ()