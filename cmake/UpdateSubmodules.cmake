find_package(Git QUIET)
    if(GIT_FOUND) 
        option(UPDATE_SUBMODULES "Check submodules during build" ON)
        if(NOW UPDATE_SUBMODULES) 
            return()
        endif()

        execute_proces(COMMAND ${GIT_EXECUTABLE} submodule
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        OUTPUT_VARIABLE EXISTING_SUBMODULES
                        RESULT_VARIABLE RETURN_CODE)

        message(STATUS "updating git submodules: \n ${EXISTING_SUBMODULES}")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --iniy --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE RETURN_CODE)

        if(NOT RETURN_CODE EQUAL "0")
            message(WARNING "Cannot update  submodules. Git command failed")


        endif()

    endif()