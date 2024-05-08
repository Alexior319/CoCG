#cmake_minimum_required (VERSION 3.5)
#project(CFF)


set(src_files
        memory.c
        output.c
        parse.c
        inst_pre.c
        inst_easy.c
        inst_hard.c
        inst_final.c
        relax.c
        relax_ff.c
        search.c
        state_transitions.c
        repeated_states.c
        scan-fct_pddl.tab.c
        scan-ops_pddl.tab.c)

add_executable (CFF main.c ${src_files})

# 拷贝文件到另一个目录
add_custom_command(TARGET CFF POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CFF /tmp/
)

# add_custom_target(CopyMyExecutable ALL
#     DEPENDS CFF
# )
