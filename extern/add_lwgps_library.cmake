if(CMAKE_VERSION VERSION_GREATER 3.12)
        add_compile_definitions(LWGPS_IGNORE_USER_OPTS=1)
else()
    add_definitions("-DLWGPS_IGNORE_USER_OPTS=1")
endif()


add_library(
        lwgps STATIC
# lwgps base code
${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps/src/include/lwgps/lwgps_opt.h
${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps/src/include/lwgps/lwgps_opts_template.h
${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps/src/include/lwgps/lwgps.h
${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps/src/lwgps/lwgps.c
)

configure_file(${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps/src/include/lwgps/lwgps_opt.h           ${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps/lwgps_opt.h           COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps/src/include/lwgps/lwgps_opts_template.h ${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps/lwgps_opts_template.h COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps/src/include/lwgps/lwgps.h               ${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps/lwgps.h               COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps/src/lwgps/lwgps.c                       ${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps/lwgps.c               COPYONLY)

include_directories(${CMAKE_CURRENT_LIST_DIR}/lwgps/)
target_include_directories(lwgps PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lwgps/lwgps)
target_link_libraries(lwgps m)