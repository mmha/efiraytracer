add_subdirectory(lmi)

add_library(callable_traits INTERFACE)
target_include_directories(callable_traits SYSTEM INTERFACE callable_traits)

add_library(tl INTERFACE)
target_include_directories(tl SYSTEM INTERFACE tl)
