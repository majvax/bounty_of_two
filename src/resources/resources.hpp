#include <cstddef>
#include <span>


namespace resources {
std::span<const std::byte> get_hover();
std::span<std::byte> legacy_get_hover();

std::span<const std::byte> get_nunito();
std::span<std::byte> legacy_get_nunito();
} // namespace resources
