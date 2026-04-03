#pragma once

namespace lcd
{
// The types which have both std::formatter specialization and operator<<
// needs to be listed to avoid compilation errors for ambitious calls.
template <typename T>
concept stream_formattable = requires(std::ostream& os, const T& v) {
    requires !std::is_same_v<T, char>;
    requires !std::is_same_v<T, const char*>;
    requires !std::is_same_v<T, const void*>;
    { os << v } -> std::convertible_to<std::ostream&>;
};

} // namespace lcd
////////////////////////////////////////////////////////////////////////////////
// The NOLINT cooment is needed because clang-tidy complains that the below
// specialization may lead to UB because we use concept to constraint the
// specialization and not user defined type.
template <lcd::stream_formattable T>
struct std::formatter<T> : std::formatter<std::string_view> // NOLINT
{
    auto format(const T& v, auto& ctx) const noexcept
    {
        std::ostringstream ss;
        ss << v;
        return std::formatter<std::string_view>::format(ss.view(), ctx);
    }
};

