#pragma once

#include "formatters.hpp"
#include "throw.hpp"

// Small helper functionality for more succinct usage of boost::program_options
namespace lcd
{

inline void
add_opt(bpo::options_description& opts, const char* name, const char* desc)
{
    bpo::options_description_easy_init{&opts}(name, desc);
}

inline void add_opt(bpo::options_description& opts,
                    const char* name,
                    const bpo::value_semantic* sem,
                    const char* desc)
{
    bpo::options_description_easy_init{&opts}(name, sem, desc);
}

inline bpo::variables_map
parse_opts(int argc, char** argv, const bpo::options_description& opts)
{
    bpo::variables_map vm;
    bpo::store(bpo::command_line_parser(argc, argv)
                   .options(opts)
                   .allow_unregistered()
                   .run(),
               vm);
    bpo::notify(vm);
    if (vm.count("help") || vm.count("info")) throw opts;
    return vm;
}

template <typename T>
inline T read_opt(const char* opt_name,
                  const bpo::variables_map& vm,
                  const bpo::options_description& opts)
{
    const auto& val = vm[opt_name];
    if (val.empty()) {
        lcd::throw_invalid_argument("Missing option: {}\n{}", opt_name, opts);
    }
    const auto* p = boost::any_cast<T>(&val.value());
    if (!p) {
        lcd::throw_invalid_argument("Invalid option: {}\n{}", opt_name, opts);
    }
    return *p;
}

} // namespace lcd
