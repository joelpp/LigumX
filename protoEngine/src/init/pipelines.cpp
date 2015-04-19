#include "../game.h"

void Game::init_pipelines()
{
    init_pipelines_buildingSides();
    init_pipelines_filledBuildings();
    init_pipelines_generalLines();
    init_pipelines_groundTriangles();
    init_pipelines_roads();
    init_pipelines_screenQuad();
    init_pipelines_envmap();
}
