set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

# Enable strict errors/warnings by compiler.
# TODO: option()(s?) and compiler checks etc... more warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat=2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

################ ASN1 print support set up ################
# Pass -DASN_PRINT=1 to cmake to enable ASN1 print support
###########################################################
if(ASN_PRINT)
message(STATUS "Using ASN1 library with print support")
add_definitions(-DASN_PRINT)
endif(ASN_PRINT)
############ Doxygen set up ############
find_package(Doxygen)
if(DOXYGEN_FOUND)
    if (NOT TARGET docs)
        add_custom_target(docs
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

        add_custom_target(${PROJECT_NAME}-doc
               ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMENT "Generating API documentation with Doxygen" VERBATIM
        )

        add_dependencies(docs ${PROJECT_NAME}-doc)
    endif()
endif(DOXYGEN_FOUND)

#

if (SOURCES)
    ######### clang-format set up ##########
    find_program(CLANG_FORMAT NAMES clang-format clang-format-3.7 clang-format-3.6 clang-format-3.5 clang-format-3.4)
    if (${CLANG_FORMAT} MATCHES "CLANG_FORMAT-NOTFOUND")
            MESSAGE(WARNING "\n### WARNING: Could not find clang-format\n")
    endif()

    set(CLANG_CHECK_FLAGS -output-replacements-xml)
    set(CLANG_CHECK_OUTFILE clang_format_check.log)

    # Create top target if not already exist
    if (NOT TARGET checkformat)
        add_custom_target(checkformat
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMENT "Check format with clang format" VERBATIM
        )
    endif()

    # Create project specific target
    add_custom_target(${PROJECT_NAME}-checkformat
                      ${CLANG_FORMAT} ${CLANG_CHECK_FLAGS} ${SOURCES} > ${CMAKE_CURRENT_BINARY_DIR}/${CLANG_CHECK_OUTFILE}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    add_dependencies(checkformat ${PROJECT_NAME}-checkformat)                      

    ######### cpplint set up ##########
    find_program(CPPLINT NAMES cpplint.py)
    if (${CPPLINT} MATCHES "CPPLINT-NOTFOUND")
            MESSAGE(WARNING "\n### WARNING: Could not find cpplint.py\n")
    endif()
    # Create top target if not already exist
    if (NOT TARGET cpplint)
        add_custom_target(cpplint
               WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
               COMMENT "Run cpplint" VERBATIM
        )
    endif()

    # Create project specific target
    if (CPPLINT_FILTERS)
        set(FILTERS "--filter=${CPPLINT_FILTERS}")
    endif()
    add_custom_target(${PROJECT_NAME}-cpplint
           ${CPPLINT} --linelength=120 ${FILTERS} ${SOURCES}
           WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
           COMMENT "Run cpplint" VERBATIM
    )
    add_dependencies(cpplint ${PROJECT_NAME}-cpplint) 

elseif(!DISABLE_SOURCES_WARNING)
    message(WARNING "No formatting checks will be made since SOURCES is not defined")
endif(SOURCES)

# Handle coverage for subcomponent
if (DEFINED ENABLE_COVERAGE)
    # Set compiler and linker flags for unit test / coverage builds

    # GCOV operates on object code.
    # To get coverage value representative for our own code / scope of control only,
    # asserts, and enable basic optimization to get rid of 'implicit branches'.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage -DENABLE_TESTS=1 -DNDEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage -DENABLE_TESTS=1 -DNDEBUG")
    
    set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} --coverage")
    find_program(GCOVR_PATH gcovr)
    find_program(CTEST_PATH ctest)

    if (NOT TARGET coverage)
        # XML/cobertura coverage
        add_custom_target(coverage
                          COMMENT "Generating coverage report to ${CMAKE_CURRENT_BINARY_DIR}/coverage.xml"
                          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                          COMMAND ${CMAKE_MAKE_PROGRAM} all
                          COMMAND ${CTEST_PATH} --timeout 60 --output-on-failure
                          COMMAND ${GCOVR_PATH} --root=${CMAKE_CURRENT_SOURCE_DIR}/../.. --branches --print-summary --xml --exclude='test/.*' --exclude='generated/.*' --output=${CMAKE_CURRENT_BINARY_DIR}/coverage.xml
                         )
        # HTML coverage for developers
        add_custom_target(coverage-html
                          COMMENT "Generating coverage report to ${CMAKE_CURRENT_BINARY_DIR}/coverage.xml"
                          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                          COMMAND ${CMAKE_MAKE_PROGRAM} all
                          COMMAND ${CTEST_PATH} --timeout 60 --output-on-failure

                          COMMAND ${GCOVR_PATH} --root=${CMAKE_CURRENT_SOURCE_DIR}/../.. --branches --print-summary --html --html-details --exclude='test/.*' --exclude='generated/.*' --output=${CMAKE_CURRENT_BINARY_DIR}/coverage.html
                         )
    endif (NOT TARGET coverage)
endif (DEFINED ENABLE_COVERAGE)
