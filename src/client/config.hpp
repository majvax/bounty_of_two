#pragma once

#include <SFML/Graphics.hpp>
#include <expected>
#include <functional>
#include <string>

constexpr const char* default_font_path() { return "C:/Windows/Fonts/segoeui.ttf"; }


struct Config
{
  public:
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(Config&&) = delete;

  private:
    Config() = default;
    ~Config() = default;

  public:
    static Config& instance()
    {
        static Config instance;
        return instance;
    }

    using FontResult = std::expected<std::reference_wrapper<sf::Font>, std::string>;

    static FontResult default_font()
    {
        if (is_font_loaded) { return FontResult{ std::ref(default_font_) }; }
        if (!default_font_.openFromFile(default_font_path())) {
            return std::unexpected(std::string("Failed to load default font from: ") + default_font_path());
        }
        default_font_.setSmooth(true);
        is_font_loaded = true;
        return FontResult{ std::ref(default_font_) };
    }

  private:
    static inline bool is_font_loaded = false;
    static inline sf::Font default_font_;
};
