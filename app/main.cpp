#include "fizzbuzz.hpp"
#include "zero_odd_even.hpp"
#include "prog_opts.hpp"
#include "throw.hpp"

// name type
#define LEET_CODE_EXERCISES(MACRO) \
    MACRO(fizzbuzz, lcd::fizzbuzz) \
    MACRO(zero_odd_even, lcd::zero_odd_even)

int main(int argc, char** argv)
{
    auto exercise_names = [] {
        bool first = true;
        std::string ret;
        ret.reserve(1024);
#define XXX(name, type) \
    if (first) {        \
        first = false;  \
        ret += #name;   \
    } else {            \
        ret += '|';     \
        ret += #name;   \
    }
        LEET_CODE_EXERCISES(XXX)
#undef XXX
        return ret;
    };
    try {
        bpo::options_description opts("General Options");
        lcd::add_opt(opts, "help,H", "This help message");
        lcd::add_opt(opts, "exercise,E", bpo::value<std::string>(),
                     exercise_names().c_str());

        const auto vm = lcd::parse_opts(argc, argv, opts);

        const auto exercise = lcd::read_opt<std::string>("exercise", vm, opts);
#define XXX(name, type)         \
    if (exercise == #name) {    \
        type(argc, argv).run(); \
        return EXIT_SUCCESS;    \
    }
        LEET_CODE_EXERCISES(XXX);
#undef XXX
        lcd::throw_invalid_argument("Unsupported exercise: '{}'", exercise);

    } catch (const bpo::options_description& opts) {
        // Used for printing help/options information
        std::println(stderr, "{}", opts);
    } catch (const std::exception& ex) {
        std::println(stderr, "{}", ex.what());
    }
    return EXIT_FAILURE;
}

#undef LEET_CODE_EXERCISES
