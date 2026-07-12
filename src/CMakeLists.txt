file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_17
)

if(MSVC)

    target_compile_options(${PROJECT_NAME}
        PRIVATE
            /W4
    )

else()

    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )

endif()