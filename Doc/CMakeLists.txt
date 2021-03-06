#TODO: DOXYGEN_SKIP_DOT
function(set_up_build_doc)
  FIND_PACKAGE(Doxygen)
  if(DOXYGEN_FOUND)
    if(DOXYGEN_VERSION LESS 1.8)
      MESSAGE(WARNING "Your Doxygen is very old!")
    endif(DOXYGEN_VERSION LESS 1.8)

    if(DOXYGEN_DOT_FOUND)
      MESSAGE(STATUS "Dot ready for your doxygen!\n")
    else(DOXYGEN_DOT_FOUND)
      MESSAGE(WARNING "-- Dot not ready for your doxygen!")
    endif(DOXYGEN_DOT_FOUND)

    FILE(MAKE_DIRECTORY  ${PROJECT_BINARY_DIR}/Doc)
    SET(DOXYGEN_INPUT    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    SET(DOXYGEN_OUTPUT   ${CMAKE_CURRENT_BINARY_DIR}/Doc/html/index.html)
    CONFIGURE_FILE(
       ${CMAKE_CURRENT_SOURCE_DIR}/Doc/CMake/Doxyfile.in
       ${DOXYGEN_INPUT} @ONLY
    )

    ######################################################
    ADD_CUSTOM_COMMAND(
       OUTPUT  ${DOXYGEN_OUTPUT}
       COMMAND ${CMAKE_COMMAND} -E echo_append "Building Documentation..."
       COMMAND ${DOXYGEN} ${DOXYGEN_INPUT}
       COMMAND ${CMAKE_COMMAND} -E echo "Done."
       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
       DEPENDS ${DOXYGEN_INPUT}
    )
    ADD_CUSTOM_TARGET(
       doc ALL
       DEPENDS ${DOXYGEN_OUTPUT}
    )

    ADD_CUSTOM_TARGET(doc_forced
       COMMAND ${CMAKE_COMMAND} -E echo_append "Building Documentation..."
       COMMAND ${DOXYGEN} ${DOXYGEN_INPUT}
       COMMAND ${CMAKE_COMMAND} -E echo "Done."
       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
       DEPENDS ${DOXYGEN_INPUT}
    )
  else(DOXYGEN_FOUND)
    MESSAGE(WARNING "Doxygen not are available in your environment!")
  endif(DOXYGEN_FOUND)
endfunction(set_up_build_doc)


include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLinter.cmake)
