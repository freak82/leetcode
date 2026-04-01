#pragma once

namespace lcd // leetcode
{
// clang-format off

template <typename... Args>
[[gnu::cold]]
[[noreturn]] 
inline void throw_invalid_argument(std::format_string<Args...> fmtstr,
                                Args&&... args)
{
    char buf[512];
    auto [out, _] = std::format_to_n(
            buf, sizeof(buf)-1, fmtstr, std::forward<Args>(args)...);
    *out = '\0';
    throw std::invalid_argument(buf);
}

template <typename... Args>
[[gnu::cold]]
[[noreturn]] 
inline void throw_logic_error(std::format_string<Args...> fmtstr,
                                Args&&... args)
{
    char buf[512];
    auto [out, _] = std::format_to_n(
            buf, sizeof(buf)-1, fmtstr, std::forward<Args>(args)...);
    *out = '\0';
    throw std::logic_error(buf);
}

template <typename... Args>
[[gnu::cold]]
[[noreturn]] 
inline void throw_runtime_error(std::format_string<Args...> fmtstr,
                                Args&&... args)
{
    char buf[512];
    auto [out, _] = std::format_to_n(
            buf, sizeof(buf)-1, fmtstr, std::forward<Args>(args)...);
    *out = '\0';
    throw std::runtime_error(buf);
}

// clang-format on
} // namespace lcd
