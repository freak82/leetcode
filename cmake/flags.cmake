
add_library(flags_options INTERFACE)

# -fno-rtti is not used because it breaks the boost::any_cast conversion to
# std::string.
target_compile_options(flags_options
    INTERFACE
        -std=c++2c
        -march=native
        -Wall 
        -Wextra 
        -Wpedantic 
        -Werror
        -Winit-self
        -Wshadow
        -Wconversion
        -Wsign-conversion
)

# The libbcc links to these dynamically and thus there is no point for us to
# link statically to them.
#target_link_options(flags_options
#    INTERFACE
#        -static-libstdc++ -static-libgcc
#)
