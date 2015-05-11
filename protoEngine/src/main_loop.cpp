#include "game.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include "FreeImage.h"

using namespace glm;
using std::cout;
using std::endl;

const double static_dt = 1.0 / 100.0;

void Game::mainLoop()
{
    static double dt = 0.0;
//    static double curr_time = glfwGetTime();
    static double physic_accumulator = 0.0;

    renderer.render(camera);

    physic_accumulator += dt;

//    camera->moveFromUserInput(renderer.pWindow);
    camera->handlePresetNewFrame(renderer.pWindow);
 //    TIME(std::vector<SpatialIndex::IData*>* a = world->getNearest(world->waysIndex, -0.65, 0.45, 1));
	// std::vector<Way*> closests;
	// std::vector<double> distances;
	// std::vector<vec2> directions;
	// std::vector<std::pair<Node*, Node*>> nodePairs;
	// int filter = OSMElement::ANY_TYPE;
	// TIME(world->findNClosestWays(1, vec2(-0.65, 0.45), filter, distances, directions, nodePairs));
    // delete a;
    // Physic update step
//    while ( physic_accumulator >= static_dt ) {
//        entityManager.Update(static_dt);
//        physic_accumulator -= static_dt;
//    }

    // move sun
//    if(sunMoveAuto) {
//        sunTime += sunSpeed/1000.0;
//    }



//    std::stringstream fpsString;
//    fpsString << round(1.f/dt);
//    fpsString << " fps";
//    RenderText(fpsString.str(), 750.0f, 750.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f), false);
//    RenderText("LIGUMX BITCHES", -0.7f, 0.5f, 0.0001f, glm::vec3(0.5, 0.8f, 0.2f), true);

//    int filter = OSMElement::HIGHWAY_SECONDARY | OSMElement::HIGHWAY_TERTIARY | OSMElement::HIGHWAY_RESIDENTIAL | OSMElement::HIGHWAY_UNCLASSIFIED;
//    for (auto it = theWays.begin(); it != theWays.end(); ++it){
//        Way* way = it->second;

//        if ((way->eType & filter) == 0) continue;
//        std::string name;
//        try{name = way->tags.at("name");}
//        catch(...){continue;}

//        vec2 xy = (way->nodes[0]->getLatLong() + way->nodes[1]->getLatLong()) / vec2(2,2);

//        RenderText(name.c_str(), xy.x, xy.y, 0.000001f, glm::vec3(0.5, 0.8f, 0.2f), true);

//    }

//    if (renderText)
//        for (int i = 0; i < texts.size(); i++){
////            RenderText(texts[i].text.c_str(), texts[i].position.x, texts[i].position.y, 0.000001f, glm::vec3(0.5, 0.8f, 0.2f), texts[i].projected);
//            RenderText(texts[i]);
//        }
    if(showTweakBar) TwDraw();

    // screenshot
//    if(saveScreenshot) {
//        static unsigned int frameCount = 0;
//        ++frameCount;
//        std::stringstream filename;
//        filename.clear();
//        filename << "../output/frame" << std::setfill('0') << std::setw(5) << frameCount << ".png";

//        BYTE* pixels = new BYTE[4 * windowWidth * windowHeight];
//        glReadPixels(0, 0, windowWidth, windowHeight, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
//        FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, windowWidth, windowHeight, 4 * windowWidth, 32, 0x0000FF, 0xFF0000, 0x00FF00, false);
//        if(FreeImage_Save(FIF_BMP, image, filename.str().c_str(), 0)) {
//            std::cout << "screenshot '" << filename.str() << "' taken." << std::endl;
//        } else {
//            std::cout << "screenshot failed. Did you create the 'protoEngine/output' directory?" << std::endl;
//        }
//        FreeImage_Unload(image);
//        delete [] pixels;
//    }
//    double new_time = glfwGetTime();
//    dt = new_time - curr_time;
//    curr_time = new_time;


}

