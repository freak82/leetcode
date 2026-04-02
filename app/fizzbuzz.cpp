#include "fizzbuzz.hpp"
#include "prog_opts.hpp"

namespace lcd
{

class fizzbuzz_impl
{
    size_t iterations_;

    std::binary_semaphore sem_print_number_{1};
    std::binary_semaphore sem_print_fizz_{0};
    std::binary_semaphore sem_print_buzz_{0};
    std::binary_semaphore sem_print_fizzbuzz_{0};

    std::atomic_bool done_{false};

public:
    fizzbuzz_impl(int argc, char** argv)
    {
        bpo::options_description opts("Options for fizzbuzz");
        lcd::add_opt(opts, "info,I", "Info about available options");
        lcd::add_opt(opts, "count,C", bpo::value<uint16_t>(),
                     "Count of iterations [0 - 65535]");

        const auto vm = lcd::parse_opts(argc, argv, opts);

        iterations_ = lcd::read_opt<uint16_t>("count", vm, opts);
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
        for (size_t i = 1; i <= iterations_; ++i) {
            const auto rem3 = i % 3;
            const auto rem5 = i % 5;

            sem_print_number_.acquire();
            if (rem3 == 0 && rem5 == 0) {
                sem_print_fizzbuzz_.release();
            } else if (rem3 == 0) {
                sem_print_fizz_.release();
            } else if (rem5 == 0) {
                sem_print_buzz_.release();
            } else if (i == 1) {
                fn_print(std::format("{}", i));
                sem_print_number_.release();
            } else {
                fn_print(std::format(",{}", i));
                sem_print_number_.release();
            }
        }

        sem_print_number_.acquire();
        done_.store(true);
        sem_print_fizz_.release();
        sem_print_buzz_.release();
        sem_print_fizzbuzz_.release();
    }

    void print_fizz(std::invocable<std::string_view> auto fn_print)
    {
        for (;;) {
            sem_print_fizz_.acquire();
            if (done_.load()) break;
            fn_print(",fizz");
            sem_print_number_.release();
        }
    }

    void print_buzz(std::invocable<std::string_view> auto fn_print)
    {
        for (;;) {
            sem_print_buzz_.acquire();
            if (done_.load()) break;
            fn_print(",buzz");
            sem_print_number_.release();
        }
    }

    void print_fizzbuzz(std::invocable<std::string_view> auto fn_print)
    {
        for (;;) {
            sem_print_fizzbuzz_.acquire();
            if (done_.load()) break;
            fn_print(",fizzbuzz");
            sem_print_number_.release();
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
