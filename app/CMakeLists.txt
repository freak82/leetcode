
add_executable(leetcode
    main.cpp
    fizzbuzz.cpp
    dining_philosophers.cpp
    zero_odd_even.cpp
)

target_include_directories(leetcode
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(leetcode
    PRIVATE 
        Boost::range
        Boost::program_options
        flags_options
)

target_precompile_headers(leetcode
    PRIVATE precompiled.hpp
)
