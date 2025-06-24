#pragma once
#include <vector>
#include <memory>
#include "../utils/getter_setter.hpp"

class Player;
class EnemyBase;

class GameState
{
    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::unique_ptr<EnemyBase>> entities;


public:
    BOO_GETTER_IMPL(const auto&, Players, players)
    BOO_GETTER_IMPL(const auto&, Entities, entities)

    void update(float deltatime);


    // WARNING: theses methods will all be deprecated in
    // the future when the multiplayer system is implemented
    void add_player(std::unique_ptr<Player> player);
    void add_entity(std::unique_ptr<EnemyBase> entity);
    void remove_player(Player* player);
    void remove_entity(EnemyBase* entity);
};
