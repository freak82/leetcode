#pragma once

namespace lcd
{

class fizzbuzz
{
    std::unique_ptr<class fizzbuzz_impl> impl_;

public:
    fizzbuzz(int argc, char** argv);
    ~fizzbuzz() noexcept;

    void run();
};

}; // namespace lcd
