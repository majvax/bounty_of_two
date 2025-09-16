#include "server.hpp"


constexpr auto TCP_PORT = 54000;
constexpr auto UDP_PORT = 54001;

std::optional<std::string> Server::init()
{
    const auto logger = spdlog::get("network");
    m_data_socket.setBlocking(false);

    // the m_connections vector always has one empty connection ready to be filled
    // by `m_listener.accept()`. To avoid unnecessary branching, in the `accept_new_connections`
    // method, we always add a new empty connection after accepting a new one. We need to initialize
    // the socket of the empty connection to non-blocking mode here.
    m_connections.emplace_back().socket.setBlocking(false);

    if (m_listener.listen(TCP_PORT) != sf::Socket::Status::Done) {
        logger->critical("Failed to bind listener to port {}", TCP_PORT);
        return std::optional{ "Failed to bind listener" };
    }
    if (m_data_socket.bind(UDP_PORT) != sf::Socket::Status::Done) {
        logger->critical("Failed to bind data socket to port {}", UDP_PORT);
        return std::optional{ "Failed to bind data socket" };
    }
    m_selector.add(m_listener);
    m_selector.add(m_data_socket);

    setup_callbacks();
    logger->trace("Server initialized on TCP port {} and UDP port {}", TCP_PORT, UDP_PORT);
    return std::nullopt;
}

void Server::accept_new_connections()
{
    const auto logger = spdlog::get("network");
    auto& conn = m_connections.back();

    if (m_listener.accept(conn.socket) == sf::Socket::Status::Done) {
        conn.valid = true;
        logger->info("New connection accepted from {}:{}", conn.get_address().toString(), conn.get_port());
        m_selector.add(conn.socket);

        m_connections.emplace_back().socket.setBlocking(false);
    } else {
        logger->error("Failed to accept new connection");
    }
}

void Server::recv(gamestate_t& state)
{
    constexpr auto TIMEOUT_MS = 5;
    if (m_selector.wait(sf::milliseconds(TIMEOUT_MS))) {
        if (m_selector.isReady(m_listener)) { accept_new_connections(); }
        if (m_selector.isReady(m_data_socket)) { process_udp_message(state); }


        for (auto& conn : m_connections) {
            if (!conn.valid || !m_selector.isReady(conn.socket)) { continue; }
            process_tcp_message(conn, state);
        }
    }
}

void Server::send(const gamestate_t& state)
{
    const auto logger = spdlog::get("network");
    net::packet_t<net::message_type::GameUpdate> packet{};

    net::gamestate_packet_t gs_packet;
    gs_packet.from_gamestate(state);

    if (!(packet << gs_packet)) {
        logger->error("Failed to insert gamestate into packet");
        return;
    }

    for (const auto& conn : m_connections) {
        if (!conn) { continue; }
        auto status = m_data_socket.send(packet, conn.get_address(), conn.udp_port);
        if (status != sf::Socket::Status::Done) {
            logger->error("Failed to send game state to {}:{}", conn.get_address().toInteger(), conn.get_port());
        } else {
            logger->trace(
              "Sent {} bytes to {}:{}", packet.getDataSize(), conn.get_address().toInteger(), conn.get_port());
        }
    }
}

void Server::process_tcp_message(connection_t& conn, gamestate_t& state)
{
    const auto logger = spdlog::get("network");
    net::packet_t packet{};
    auto status = conn.socket.receive(packet);
    if (status == sf::Socket::Status::NotReady) { return; }

    if (status != sf::Socket::Status::Done) {
        logger->error("Failed to receive data from {}:{}", conn.get_address().toInteger(), conn.get_port());
        return;
    }

    logger->trace("Received {} bytes from {}:{}", packet.getDataSize(), conn.get_address().toInteger(), conn.get_port());

    net::header_t header;
    if (!(packet >> header)) {
        logger->error("Failed to extract header from packet");
        return;
    }

    logger->trace("Message type: {}", static_cast<uint8_t>(header.type));

    auto handler_it = m_message_handlers.find(header.type);
    if (handler_it != m_message_handlers.end()) {
        handler_it->second(this, callback_params_t{ .packet = &packet, .conn = &conn, .state = &state });
    } else {
        logger->warn("No handler for message type: {}", static_cast<uint8_t>(header.type));
    }
}

void Server::process_udp_message(gamestate_t& state)
{
    const auto logger = spdlog::get("network");
    std::optional<sf::IpAddress> sender;
    uint16_t port{ 0 };
    net::packet_t packet{};
    auto status = m_data_socket.receive(packet, sender, port);
    if (status == sf::Socket::Status::NotReady) { return; }
    if (status != sf::Socket::Status::Done || !sender) {
        logger->error("Failed to receive data");
        return;
    }
    logger->trace("Received {} bytes from {}:{}", packet.getDataSize(), sender->toInteger(), port);
    net::header_t header;
    if (!(packet >> header)) {
        logger->error("Failed to extract header from packet");
        return;
    }

    auto conn_it = std::ranges::find_if(
      m_connections, [&](const auto& conn) { return conn.get_address() == *sender && conn.udp_port == port; });
    if (conn_it == m_connections.end() || !*conn_it) {
        logger->warn("Received packet from unknown sender {}:{}", sender->toInteger(), port);
        return;
    }

    auto handler_it = m_message_handlers.find(header.type);
    if (handler_it != m_message_handlers.end()) {
        handler_it->second(this, callback_params_t{ .packet = &packet, .conn = &(*conn_it), .state = &state });
    } else {
        logger->warn("No handler for message type: {}", static_cast<uint8_t>(header.type));
    }
}

void Server::setup_callbacks()
{
    using enum net::message_type;
    using namespace callbacks;

    m_message_handlers[JoinNotification] = join_notification;
    m_message_handlers[LeaveNotification] = leave_notification;
    m_message_handlers[ChatMessage] = chat_message;
    m_message_handlers[PlayerInput] = player_input;
}
