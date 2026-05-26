add_library(formatter STATIC formatter.cpp)
target_include_directories(formatter PUBLIC .)
install(TARGETS formatter ARCHIVE DESTINATION lib)
install(FILES formatter.h DESTINATION include)
