cmake_minimum_required(VERSION 3.0)
project(examples)

link_libraries(pthread)
include_directories(.)

aux_source_directory(state_online ONLINE)
aux_source_directory(state_offline OFFLINE)
aux_source_directory(state_startup STARTUP)

add_executable(main main.c ${ONLINE} ${OFFLINE} ${STARTUP})
target_link_libraries(main efsm)

add_executable(efsm-test thread.c ${ONLINE} ${OFFLINE} ${STARTUP})
target_link_libraries(efsm-test efsm)

#install(TARGETS efsm-test DESTINATION bin)

