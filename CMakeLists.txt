#cmake_minimum_required(VERSION 3.24...3.30)
cmake_minimum_required(VERSION 3.16...3.30)

project(HexAttack)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RAYLIB_VERSION 6.0)

FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(raylib)

add_executable(${PROJECT_NAME})

add_subdirectory(src)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
)

# Working directory (Visual Studio)
set_property(TARGET ${PROJECT_NAME}
    PROPERTY
        VS_DEBUGGER_WORKING_DIRECTORY
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raylib
)

if("${PLATFORM}" STREQUAL "Web")

    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND
            ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/resources
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/../resources
    )

    #DEPENDS ${PROJECT_NAME}

    # Tell Emscripten to build HTML file
    set_target_properties(${PROJECT_NAME}
        PROPERTIES
            SUFFIX ".html"
    )

    target_link_options(${PROJECT_NAME}
        PRIVATE
            -sUSE_GLFW=3

            -sINITIAL_MEMORY=64MB
            -sALLOW_MEMORY_GROWTH=1
            -sSTACK_SIZE=1048576

            -sASSERTIONS=2
            -sSAFE_HEAP=1
            -sSTACK_OVERFLOW_CHECK=2

            -sFORCE_FILESYSTEM=1

            --preload-file resources

            -sMINIFY_HTML=0

            -gsource-map

            --shell-file ${CMAKE_SOURCE_DIR}/src/minshell.html
    )

else()

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND
            ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/resources
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )

    #DEPENDS ${PROJECT_NAME}

endif()

if(APPLE)

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            "-framework Cocoa"
            "-framework OpenGL"
            "-framework IOKit"
    )

endif()