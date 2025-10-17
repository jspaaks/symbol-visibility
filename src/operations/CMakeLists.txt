add_library(tgt_lib_operations SHARED)

include(GenerateExportHeader)
generate_export_header(
    tgt_lib_operations
    BASE_NAME operations
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/operations/export.h
)

set_property(
    TARGET
        tgt_lib_operations
    PROPERTY
        C_VISIBILITY_PRESET hidden
)
set_property(
    TARGET
        tgt_lib_operations
    PROPERTY
        OUTPUT_NAME operations
)
set_property(
    TARGET
        tgt_lib_operations
    PROPERTY
        VISIBILITY_INLINES_HIDDEN YES
)

target_compile_definitions(
    tgt_lib_operations
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_lib_operations
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_lib_operations
    PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fPIC
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_lib_operations
    PRIVATE
        ../../include
)

target_sources(
    tgt_lib_operations
    PRIVATE
        division.c
        multiplication.c
    PUBLIC
        FILE_SET fset_lib_operations_headers
            TYPE HEADERS
            BASE_DIRS
                ../../include
                ${CMAKE_BINARY_DIR}/include                          # contains cmake-generated file
            FILES
                ../../include/operations/division.h
                ../../include/operations/multiplication.h
                ${CMAKE_BINARY_DIR}/include/operations/export.h      # cmake-generated file
)

install(
    TARGETS tgt_lib_operations
    LIBRARY
    FILE_SET fset_lib_operations_headers
)
