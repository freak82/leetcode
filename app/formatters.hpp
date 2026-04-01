#pragma once

namespace lcd
{

template <typename T>
concept streamable = requires(std::ostream& os, const T& v) {
    { os << v } -> std::convertible_to<std::ostream&>;
};

} // namespace lcd
////////////////////////////////////////////////////////////////////////////////
template <lcd::streamable T>
struct std::formatter<T> : std::formatter<std::string_view>
{
    auto format(const T& v, auto& ctx) const noexcept
    {
        std::istringstream ss;
        ss << v;
        return std::formatter<std::string_view>::format(ss.view(), ctx);
    }
};

