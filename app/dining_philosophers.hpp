#pragma once

namespace lcd
{

class dining_philosophers
{
    std::unique_ptr<class dining_philosophers_impl> impl_;

public:
    dining_philosophers(int argc, char** argv);
    ~dining_philosophers() noexcept;

    void run();
};

} // namespace lcd
