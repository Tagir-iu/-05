add_library(solver_lib STATIC solver.cpp)
target_include_directories(solver_lib PUBLIC .)
install(TARGETS solver_lib ARCHIVE DESTINATION lib)
install(FILES solver.h DESTINATION include)
