#include "engine/engine.hpp"
#include "scene/title.hpp"
#include "scene/cube.hpp"

int main()
{
            
    Engine engine;

    engine.pushScene<TitleScene>();
    engine.pushScene<SceneCube>();

    engine.run();
}
