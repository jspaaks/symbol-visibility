add_executable(tgt_exe_calculator)

set_property(TARGET tgt_exe_calculator PROPERTY OUTPUT_NAME calculator)

target_compile_definitions(
    tgt_exe_calculator
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_exe_calculator
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_exe_calculator
    PRIVATE
        -Wall
        -Wextra
        -pedantic
        -fPIE
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_exe_calculator
    PRIVATE
        ../../include
)

target_link_libraries(
    tgt_exe_calculator
    PRIVATE
        m
        tgt_lib_operations
)
 
target_link_options(
    tgt_exe_calculator
    PRIVATE
        -pie
)

target_sources(
    tgt_exe_calculator
    PRIVATE
        main.c
)

install(TARGETS tgt_exe_calculator)
