#include "engine/engine.hpp"
#include "scene/title.hpp"
#include "scene/cube.hpp"

int main()
{  
    Engine engine;

    engine.pushScene(std::make_unique<TitleScene>(engine));
    engine.pushScene(std::make_unique<SceneCube>(engine));

    engine.run();
}
