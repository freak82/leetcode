#include "zero_odd_even.hpp"

namespace lcd
{

class zero_odd_even_impl
{
    size_t iterations_;

    std::binary_semaphore sem_print_zero_{1};
    std::binary_semaphore sem_print_odd_{0};
    std::binary_semaphore sem_print_even_{0};

public:
    // TODO
    explicit zero_odd_even_impl(int, char**) : iterations_(5) {}

    void run()
    {
        auto print_fn = [](size_t token) { std::print(stdout, "{}", token); };
        {
            std::jthread t1([this, print_fn] { print_odd(print_fn); });
            std::jthread t2([this, print_fn] { print_even(print_fn); });
            print_zero(print_fn);
        }
        std::println();
    }

private:
    void print_zero(std::invocable<size_t> auto fn_print)
    {
        for (size_t i = 0; i < iterations_; ++i) {
            sem_print_zero_.acquire();
            fn_print(0uz);
            auto* sem = (i & 0x1) ? &sem_print_even_ : &sem_print_odd_;
            sem->release();
        }
    }

    void print_odd(std::invocable<size_t> auto fn_print)
    {
        for (size_t i = 1; i <= iterations_; i += 2) {
            sem_print_odd_.acquire();
            fn_print(i);
            sem_print_zero_.release();
        }
    }

    void print_even(std::invocable<size_t> auto fn_print)
    {
        for (size_t i = 2; i <= iterations_; i += 2) {
            sem_print_even_.acquire();
            fn_print(i);
            sem_print_zero_.release();
        }
    }
};

zero_odd_even::zero_odd_even(int argc, char** argv)
: impl_(std::make_unique<zero_odd_even_impl>(argc, argv))
{
}

void zero_odd_even::run()
{
    impl_->run();
}

} // namespace lcd
