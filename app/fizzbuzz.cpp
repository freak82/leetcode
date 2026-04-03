#include "fizzbuzz.hpp"
#include "prog_opts.hpp"

namespace lcd
{

class fizzbuzz_impl
{
    // The `current_` count doesn't need to be atomic type because it's
    // advanced only in the "critical section" of given semaphore.
    // However, the critical sections of the semaphores are serialized by the
    // current logic.
    uint32_t iterations_;
    uint32_t current_{1};

    std::binary_semaphore sem_print_num_{1};
    std::binary_semaphore sem_print_fizz_{0};
    std::binary_semaphore sem_print_buzz_{0};
    std::binary_semaphore sem_print_fizzbuzz_{0};

public:
    fizzbuzz_impl(int argc, char** argv)
    {
        bpo::options_description opts("Options for fizzbuzz");
        lcd::add_opt(opts, "info,I", "Info about available options");
        lcd::add_opt(opts, "count,C", bpo::value<uint16_t>(),
                     "Count of iterations [1 - 65535]");

        const auto vm = lcd::parse_opts(argc, argv, opts);

        iterations_ = lcd::read_opt<uint16_t>("count", vm, opts);
        if (iterations_ == 0) {
            lcd::throw_invalid_argument("'count' should be greater than 1");
        }
    }

    void run()
    {
        auto print_fn = [](std::string_view s) { std::print(stderr, "{}", s); };
        {
            std::jthread t1([this, print_fn] { print_fizz(print_fn); });
            std::jthread t2([this, print_fn] { print_buzz(print_fn); });
            std::jthread t3([this, print_fn] { print_fizzbuzz(print_fn); });
            print_number(print_fn);
        }
        std::println();
    }

private:
    void print_number(std::invocable<std::string_view> auto fn_print)
    {
        for (;;) {
            sem_print_num_.acquire();
            if (current_ > iterations_) {
                break;
            } else if (current_ == 1) {
                fn_print(std::format("{}", current_));
            } else {
                fn_print(std::format(",{}", current_));
            }
            advance_state();
        }
    }

    void print_fizz(std::invocable<std::string_view> auto fn_print)
    {
        for (;;) {
            sem_print_fizz_.acquire();
            if (current_ > iterations_) break;
            fn_print(",fizz");
            advance_state();
        }
    }

    void print_buzz(std::invocable<std::string_view> auto fn_print)
    {
        for (;;) {
            sem_print_buzz_.acquire();
            if (current_ > iterations_) break;
            fn_print(",buzz");
            advance_state();
        }
    }

    void print_fizzbuzz(std::invocable<std::string_view> auto fn_print)
    {
        for (;;) {
            sem_print_fizzbuzz_.acquire();
            if (current_ > iterations_) break;
            fn_print(",fizzbuzz");
            advance_state();
        }
    }

    void advance_state()
    {
        if (++current_ > iterations_) {
            sem_print_num_.release();
            sem_print_fizz_.release();
            sem_print_buzz_.release();
            sem_print_fizzbuzz_.release();
            return;
        }
        const auto rem3 = current_ % 3;
        const auto rem5 = current_ % 5;
        if (rem3 && rem5) {
            sem_print_num_.release();
        } else if (rem3) {
            sem_print_buzz_.release();
        } else if (rem5) {
            sem_print_fizz_.release();
        } else {
            sem_print_fizzbuzz_.release();
        }
    }
};

fizzbuzz::fizzbuzz(int argc, char** argv)
: impl_(std::make_unique<fizzbuzz_impl>(argc, argv))
{
}

fizzbuzz::~fizzbuzz() = default;

void fizzbuzz::run()
{
    impl_->run();
}

} // namespace lcd
