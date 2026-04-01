#include "zero_odd_even.hpp"
#include "formatters.hpp"
#include "throw.hpp"

// name type
#define LEET_CODE_EXERCISES(MACRO) MACRO(zero_odd_even, lcd::zero_odd_even)

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
        // clang-format off
        bpo::options_description opts("Exercises");
        opts.add_options()
            ("help,H", "This help message")
            ("exercise,E", bpo::value<std::string>(), exercise_names().c_str());
        // clang-format on

        bpo::variables_map vm;
        bpo::store(bpo::parse_command_line(argc, argv, opts), vm);
        bpo::notify(vm);

        // The separate exercise classes may throw such errors too when
        // their expected options are not present.
        if (vm.count("help")) throw opts;

        std::string empty;
        const auto& vmv      = vm["exercise"];
        const auto& exercise = vmv.empty() ? empty : vmv.as<std::string>();
#define XXX(name, type)         \
    if (exercise == #name) {    \
        type(argc, argv).run(); \
        return EXIT_SUCCESS;    \
    }
        LEET_CODE_EXERCISES(XXX);
#undef XXX
        lcd::throw_logic_error("Missing/unsupported exercise: '{}'", exercise);

    } catch (const bpo::options_description& opts) {
        std::println(stderr, "{}", opts);
    } catch (const std::exception& ex) {
        std::println(stderr, "{}", ex.what());
    }
    return EXIT_FAILURE;
}

#undef LEET_CODE_EXERCISES
