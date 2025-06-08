/**
 * @file assert.hpp
 * @brief Assertion utilities
 */

#define ASSERT_MISSING_IMGUI_CTX_LOCK(ctx) \
    do { \
        if (!(ctx.is_locked())) { \
            fprintf(stderr, "[Error] likely due to a missing ImGui context lock.\n"); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)
