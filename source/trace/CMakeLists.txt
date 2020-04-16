# SubterraneanRose Trace Module

aux_source_directory(./src trace_src_list)

add_library("srose-trace" STATIC ${trace_src_list})
target_include_directories("srose-trace" PUBLIC ./include)
if(WIN32)
    find_package(Boost REQUIRED COMPONENTS stacktrace_windbg)
    target_link_libraries("srose-trace" PUBLIC Boost::stacktrace_windbg)
    target_link_libraries("srose-trace" PRIVATE ole32 dbgeng)
else()
    find_package(Boost REQUIRED COMPONENTS 	stacktrace_basic)
    target_link_libraries("srose-trace" PUBLIC Boost::stacktrace_basic)
    target_link_libraries("srose-trace" PRIVATE dl)
endif()
target_link_libraries("srose-trace" PRIVATE "srose-core")
