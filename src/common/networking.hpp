/*
This file contains common networking utilities and definitions.

It defines how data is serialized and deserialized for network transmission



each packet is optimized. It only sends entity around each player.
*/
#pragma once

#include "entities.hpp"
#include "gamestate.hpp"
#include "spdlog/spdlog.h"
#include "visitor.hpp"
#include <SFML/Network.hpp>
#include <zstd.h>

// tried 22, no visible difference
constexpr int COMPRESSION_LEVEL = 1;

namespace net {
enum struct message_type : uint8_t {
    None,
    JoinRequest,
    GameUpdate,
    PlayerInput,
};


struct header_t
{
    // lenght is already included in sf::Packet header
    message_type type{ 0 };
    friend sf::Packet& operator<<(sf::Packet& packet, const header_t& data)
    {
        return packet << std::to_underlying(data.type);
    }

    friend sf::Packet& operator>>(sf::Packet& packet, header_t& data)
    {
        uint8_t type = 0;
        packet >> type;
        data.type = static_cast<message_type>(type);
        return packet;
    }
};

struct gamestate_packet_t
{
    uint32_t entity_count{ 0 };
    std::vector<entity_t> entities;

    friend sf::Packet& operator<<(sf::Packet& packet, const gamestate_packet_t& data)
    {
        packet << data.entity_count;
        for (const auto& entity : data.entities) {
            static constexpr auto visitor = make_visitor([](const auto& ent, sf::Packet& packet) { packet << ent; });

            visit_ctx(visitor, entity, packet);
        }
        return packet;
    }
    friend sf::Packet& operator>>(sf::Packet& packet, gamestate_packet_t& data)
    {
        packet >> data.entity_count;
        data.entities.resize(data.entity_count);
        for (auto& entity : data.entities) {
            static constexpr auto visitor = make_visitor([](auto& ent, sf::Packet& packet) { packet >> ent; });
            visit_ctx(visitor, entity, packet);
        }
        return packet;
    }

    void from_gamestate(gamestate_t& state)
    {
        entities = state.entities;
        entity_count = static_cast<uint32_t>(entities.size());
    }

    [[nodiscard]] gamestate_t to_gamestate() const
    {
        gamestate_t state;
        state.entities = entities;
        return state;
    }
};

template<message_type Msg = message_type::None>
struct packet_t : sf::Packet
{
    static constexpr message_type type = Msg;

    constexpr packet_t() : sf::Packet{}
    {
        header_t header{ type };
        *this << header;
    }

    const void* onSend(std::size_t& size) override
    {
        const auto bound = ZSTD_compressBound(getDataSize());
        std::vector<std::byte> compressed_data(bound);


        const auto csize = ZSTD_compress(compressed_data.data(), bound, getData(), getDataSize(), COMPRESSION_LEVEL);

        if (ZSTD_isError(csize)) {
            spdlog::error("ZSTD compression error: {}", ZSTD_getErrorName(csize));
            size = getDataSize();
            return getData();
        }
        
        clear();
        append(compressed_data.data(), csize);
        size = csize;

        spdlog::trace("Sending packet of size: {}", size);
        return getData();
    }

    void onReceive(const void* data, std::size_t size) override
    {

        const auto osize = ZSTD_getFrameContentSize(data, size);
        if (osize == ZSTD_CONTENTSIZE_ERROR) {
            spdlog::error("ZSTD decompression error: not compressed by zstd");
            return;
        }
        if (osize == ZSTD_CONTENTSIZE_UNKNOWN) {
            spdlog::error("ZSTD decompression error: original size unknown");
            return;
        }

        std::vector<std::byte> decompressed_data(osize);
        const auto dsize = ZSTD_decompress(decompressed_data.data(), osize, data, size);
        if (ZSTD_isError(dsize) != 0U) {
            spdlog::error("ZSTD decompression error: {}", ZSTD_getErrorName(dsize));
            return;
        }

        append(decompressed_data.data(), osize);
    }

    template<message_type M = Msg>
    void add_data(gamestate_packet_t& state)
        requires(M == message_type::GameUpdate)
    {
        *this << state;
    }

    template<message_type M = Msg>
    void extract_data(gamestate_packet_t& state)
        requires(M == message_type::GameUpdate)
    {
        *this >> state;
    }
};

} // namespace net
