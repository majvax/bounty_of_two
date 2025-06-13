#include "game_state.hpp"

#include "../entity/base.hpp"
#include "../entity/player.hpp"
#include <algorithm>


void GameState::update(float deltatime)
{
    for (auto& player : players)
    {
        if (player)
        {
            player->update(deltatime);
        }
    }

    for (auto& entity : entities)
    {
        if (entity)
        {
            entity->update(deltatime);
        }
    }
}


void GameState::add_player(std::unique_ptr<Player> player)
{
    players.push_back(std::move(player));
}

void GameState::add_entity(std::unique_ptr<EntityBase> entity)
{
    entities.push_back(std::move(entity));
}

void GameState::remove_player(Player* player)
{
    auto it = std::remove_if(players.begin(), players.end(),
        [player](const std::unique_ptr<Player>& p) { return p.get() == player; });
    players.erase(it, players.end());
}

void GameState::remove_entity(EntityBase* entity)
{
    auto it = std::remove_if(entities.begin(), entities.end(),
        [entity](const std::unique_ptr<EntityBase>& e) { return e.get() == entity; });
    entities.erase(it, entities.end());
}
