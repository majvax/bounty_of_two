#pragma once
#include <cstddef>

/**
 * @brief A non-owning smart pointer that observes an object but does not manage its lifetime.
 * 
 * observer_ptr is a lightweight wrapper around a raw pointer, providing pointer-like semantics
 * without ownership. It is useful for observing objects managed elsewhere (e.g., by unique_ptr or shared_ptr).
 * 
 * @tparam T The type of the object being observed.
 * 
 * @note observer_ptr does not delete or manage the lifetime of the pointed-to object.
 */
template <typename T>
class observer_ptr {

private:
    T* ptr_;

public:
    constexpr observer_ptr() noexcept : ptr_(nullptr) {}
    constexpr observer_ptr(std::nullptr_t) noexcept : ptr_(nullptr) {}
    constexpr explicit observer_ptr(T* ptr) noexcept : ptr_(ptr) {}

    constexpr observer_ptr& operator=(T* ptr) noexcept {
        ptr_ = ptr;
        return *this;
    }

    constexpr T* get() const noexcept { return ptr_; }
    constexpr T& operator*() const noexcept { return *ptr_; }
    constexpr T* operator->() const noexcept { return ptr_; }
    constexpr explicit operator bool() const noexcept { return ptr_ != nullptr; }

    constexpr bool operator==(const observer_ptr& other) const noexcept { return ptr_ == other.ptr_; }
    constexpr bool operator!=(const observer_ptr& other) const noexcept { return ptr_ != other.ptr_; }
    constexpr bool operator==(std::nullptr_t) const noexcept { return ptr_ == nullptr; }
    constexpr bool operator!=(std::nullptr_t) const noexcept { return ptr_ != nullptr; }

};
