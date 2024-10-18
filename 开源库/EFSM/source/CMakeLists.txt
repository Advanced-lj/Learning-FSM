cmake_minimum_required(VERSION 3.0)
project(efsm)

add_library(efsm SHARED efsmt.c)
set_target_properties(efsm PROPERTIES VERSION 0.5.1 SOVERSION 1)

install (TARGETS efsm DESTINATION lib)
install (FILES efsm.h DESTINATION include)
install (FILES efsmt.h DESTINATION include)
install (FILES uthash.h DESTINATION include)
install (FILES efsm_conf.h DESTINATION include)
