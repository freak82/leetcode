#pragma once

namespace lcd
{

class zero_odd_even
{
    std::unique_ptr<class zero_odd_even_impl> impl_;

public:
    zero_odd_even(int argc, char** argv);
    ~zero_odd_even() noexcept;

    void run();
};

} // namespace lcd
