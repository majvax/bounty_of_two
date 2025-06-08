
#include "../entity/player.hpp"
#include <vector>
class Scene {
private:
	std::vector<EntityBase*> entities;
public:
	Scene();
	void add_entity(EntityBase* entity);
	void remove_entity(EntityBase* entity);
	void update(float deltaTime);
	void draw();
};