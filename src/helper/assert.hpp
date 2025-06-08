#pragma once
#include <iostream>

/**
 * @file assert.hpp
 * @brief Assertion utilities
 */
#define ASSERT_MISSING_IMGUI_CTX_LOCK(ctx) \
    do { \
        if (!(ctx.is_locked())) { \
            std::cerr << "[Error] likely due to a missing ImGui context lock.\n"; \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

#define ASSERT_PTR(ptr, msg) \
    do { \
        if (!(ptr)) { \
            std::cerr << "[Error] " << msg << '\n'; \
            exit(EXIT_FAILURE); \
        } \
    } while (0)
