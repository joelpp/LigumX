#include "Game.h"
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <glm/gtc/random.hpp>
#include <iostream>
#include "glm/gtc/type_ptr.hpp"

using namespace glm;
using namespace std;

Game* game;

int main()
{
    game = new Game();
    while(game->running) {
        glfwPollEvents();
        game->mainLoop();
        glfwSwapBuffers(game->pWindow);
    }
    glfwTerminate();
    return 0;
}

void Game::init()
{
    //=============================================================================
    // Parameters, camera setup.
    //=============================================================================
    running = true;
    windowWidth = 800;
    windowHeight = 800;
    windowTitle = "LigumX";

    camera = new Camera();
    camera->translateTo(vec2(0,0));
    camera->setViewSize(1);
    draggingCamera = false;

    //=============================================================================
    // create window and GLcontext, register callbacks.
    //=============================================================================

    // set window paramaters
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);


    // Initialise GLFW
    if( !glfwInit() )
    {
        cerr << "Failed to initialize GLFW.\n";
        return;
    } else {
        clog << "Initialized GLFW." << endl;
    }

    // create GLFW window
    pWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), 0, NULL);
    glfwMakeContextCurrent(pWindow);
    if( pWindow == NULL )
    {
        cerr << "Failed to open GLFW window.\n";
        glfwTerminate();
        return;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Needed for core profile
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cerr << "Failed to initialize GLEW\n"
             << glewGetErrorString(err) << endl;
        return;
    } else {
        clog << "Initialized GLEW." << endl;
    }

    // register GLFW and GLdebug callbacks
    glfwSetMouseButtonCallback( pWindow, glfwMouseButtonCallback );
    glfwSetKeyCallback( pWindow, glfwKeyCallback );
    glfwSetCharCallback( pWindow, glfwCharCallback );
    glfwSetCursorPosCallback( pWindow, glfwMousePositionCallback );
    glfwSetCursorEnterCallback( pWindow, glfwMouseEntersCallback );
    glfwSetScrollCallback( pWindow, glfwMouseScrollCallback );
    glfwSetWindowPosCallback( pWindow, glfwWindowPositionCallback );
    glfwSetWindowSizeCallback( pWindow, glfwWindowSizeCallback );
    glfwSetWindowCloseCallback( pWindow, glfwWindowClosedCallback );
    glfwSetWindowRefreshCallback( pWindow, glfwWindowRefreshCallback );
    glfwSetWindowFocusCallback( pWindow, glfwWindowFocusCallback );
    glfwSetWindowIconifyCallback( pWindow, glfwWindowIconifyCallback );
    glfwSetFramebufferSizeCallback( pWindow, glfwWindowFramebufferSizeCallback );
//    glDebugMessageCallback(Game::debugCallback, NULL);
//    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

    //=============================================================================
    // Load world data.
    // TODO: replace with XML loader.
    //=============================================================================


    // count total number of lines (used for the progress bar)
    int nbLines = 0;
    ifstream file("../data/result.txt", ios::in);
    string line;
    while (getline(file, line)) {
        ++nbLines;
    }
    file.close();

    std::unordered_map<string, vec2> nodes; // nodes without links, searchable by ID.
    vector<vec2> waysNodesPositions; // positions of nodes forming ways, possibly contains duplicates.
    vector<vec3> waysNodesColors;

    file.open("../data/result.txt", ios::in);
    stringstream stream;
    string word;
    vec3 white = vec3(1.0f,1.0f,1.0f);

    while (getline(file, line)) {
        // Handle the bounding box coordinates
        if (line.substr(0, 8)=="  <bound") {
            stream.clear();
            stream.str(line);

            while(stream.rdbuf()->in_avail() != 0) {
                stream >> word;
                if(word.substr(0,3)=="box") {
                    string subWord = word.substr(5,word.length());
                    auto ind = subWord.find_first_of("\"");
                    string nums = subWord.substr(0,ind);

                    ind = nums.find_first_of(",");
                    viewRectLeft = atof(nums.substr(0, ind).c_str());
                    nums = nums.substr(ind+1, nums.length());

                    ind = nums.find_first_of(",");
                    viewRectBottom = atof(nums.substr(0, ind).c_str());
                    nums = nums.substr(ind+1, nums.length());

                    ind = nums.find_first_of(",");
                    viewRectRight = atof(nums.substr(0, ind).c_str());
                    nums = nums.substr(ind+1, nums.length());

                    ind = nums.find_first_of(",");
                    viewRectTop = atof(nums.substr(0, ind).c_str());
                }
            }

            cout << viewRectLeft << " " << viewRectBottom << " " << viewRectRight << " " << viewRectTop << endl;
            viewRectBottomLeft = vec2(viewRectLeft, viewRectBottom);
            viewRectTopRight = vec2(viewRectRight, viewRectTop);
            viewRectVecDiago = viewRectBottomLeft - viewRectTopRight;
        //Handle nodes
        } else if(line.substr(0, 7)=="  <node") {

            vec2 point;
            string id;

            stream.clear();
            stream.str(line);
            while(stream.rdbuf()->in_avail() != 0) {
                stream >> word;
                if(word.substr(0,3)=="lat") {
                    string subWord = word.substr(5,word.length());
                    auto ind = subWord.find_first_of("\"");
                    string num = subWord.substr(0,ind);
                    point.x = atof(num.c_str());
                } else if(word.substr(0,3)=="lon") {
                    string subWord = word.substr(5,word.length());
                    auto ind = subWord.find_first_of("\"");
                    string num = subWord.substr(0,ind);
                    point.y = atof(num.c_str());
                } else if(word.substr(0,2)=="id") {
                    string subWord = word.substr(4,word.length());
                    auto ind = subWord.find_first_of("\"");
                    id = subWord.substr(0,ind);
                }
            }

            point = (point - viewRectBottomLeft)/viewRectVecDiago;
            point = -2.0f*vec2(point.y, point.x) - vec2(1,1);
            nodes[id] = point;

        // Handle ways
        } else if(line.substr(0, 6)=="  <way") {
            bool firstNode = true;
            vec2 oldPoint;
            vec3 color = vec3(glm::linearRand(0.5f, 1.0f),
                              glm::linearRand(0.5f, 1.0f),
                              glm::linearRand(0.5f, 1.0f));

            //Handle this way's nodes
            while (getline(file, line)) {
                // Add the next node to the way
                if (line.substr(0, 7)=="    <nd") {
                    auto ind1 = line.find_first_of("\"");
                    auto ind2 = line.find_last_of("\"");
                    string id = line.substr(ind1+1, ind2-ind1-1);
                    vec2 point = nodes[id];
                    if(!firstNode) {
                        waysNodesPositions.push_back(oldPoint);
                        waysNodesColors.push_back(color);
                        waysNodesPositions.push_back(point);
                        waysNodesColors.push_back(color);
                    }
                    oldPoint = point;
                    firstNode = false;

                } else if (line.substr(0, 7)=="  </way") {
                    break;
                }
            }
        }
        nbLines++;
        if(nbLines % 1000==0) cout << nbLines << endl;
    }
    file.close();

    //=============================================================================
    // create and fill VBOs with ways info.
    //=============================================================================

    nbWaysVertices = waysNodesPositions.size();

    glCreateBuffers(1, &glidWaysPositions);
    glNamedBufferStorage(glidWaysPositions, nbWaysVertices * 2 * 4, // nbWaysNodes * vec2 * float
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                         GL_MAP_WRITE_BIT);
    glNamedBufferSubData(glidWaysPositions, 0, nbWaysVertices * 2 * 4, waysNodesPositions.data());

    glCreateBuffers(1, &glidWaysColors);
    glNamedBufferStorage(glidWaysColors, nbWaysVertices * 3 * 4, // nbWaysNodes * vec2 * float
                         NULL,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT |
                         GL_MAP_WRITE_BIT);
    glNamedBufferSubData(glidWaysColors, 0, nbWaysVertices * 3 * 4, waysNodesColors.data());

    //=============================================================================
    // initialize graphics, VAO
    //=============================================================================


    const char* vertexShaderSource = " \
        #version 430 core\n \
        in layout(location=0) vec2 pos;\n \
        in layout(location=1) vec3 color;\n \
        uniform mat4 vpMat;\n \
//        uniform layout(location=4) float qweqwe;\n \
        out gl_PerVertex {\n \
            vec4 gl_Position;\n \
        };\n \
        out vec3 vColor;\n \
        void main() {\n \
            gl_Position = vpMat * vec4(pos, 0, 1);\n \
            //gl_Position = vec4(pos, 0, 1);\n \
//            gl_Position = vpMat * vec4(0.5, 0.5, 0, 1);\n \
            //gl_Position = vec4(qweqwe, 0.5, 0, 1);\n \
            vColor = color;\n \
        }";

    const char* fragmentShaderSource = " \
        #version 430 core\n \
        in vec3 vColor;\n \
        out vec3 color;\n \
        void main() {\n \
            color = vColor;\n \
            //color = vec3(1,1,1);\n \
        }";

    pVertexShader =
            new ProgramPipeline::ShaderProgram(GL_VERTEX_SHADER,
            vertexShaderSource, true);

    pFragmentShader =
            new ProgramPipeline::ShaderProgram(GL_FRAGMENT_SHADER,
            fragmentShaderSource, true);



    pPipeline = new ProgramPipeline();
    //pPipeline->useShaders({pVertexShader, pFragmentShader});
    pPipeline->useVertexShader(pVertexShader);
    pPipeline->useFragmentShader(pFragmentShader);

    // VAO
    glBindVertexArray(pPipeline->glidVao);
    glEnableVertexArrayAttrib(pPipeline->glidVao, 0);
    glVertexArrayVertexBuffer(pPipeline->glidVao, 0, glidWaysPositions, 0, 2*4);
    glVertexArrayAttribFormat(pPipeline->glidVao, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(pPipeline->glidVao, 1);
    glVertexArrayVertexBuffer(pPipeline->glidVao, 1, glidWaysColors, 0, 3*4);
    glVertexArrayAttribFormat(pPipeline->glidVao, 1, 3, GL_FLOAT, GL_FALSE, 0);
}


void Game::mainLoop()
{
    camera->moveFromUserInput(pWindow);

    glClear(GL_COLOR_BUFFER_BIT);
    pPipeline->usePipeline();
    glProgramUniformMatrix4fv(pPipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram, glGetUniformLocation(pPipeline->getShader(GL_VERTEX_SHADER)->glidShaderProgram, "vpMat"), 1, false, value_ptr(camera->vpMat));
    glDrawArrays(GL_LINES, 0, nbWaysVertices);
}



void Game::insertDebugMessage(string message, GLenum severity, GLuint id)
{
    // TODO: make glDebug work.
//    glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR,
//                         id, severity, GLsizei(message.length()), message.c_str());
    cout << message << endl;
}

//void Game::debugCallback(GLenum, GLenum, GLuint, GLenum, GLsizei length, const GLchar* message, const void *)
//{
//    cout << "message :"  << endl;
//    for(int i=0; i<length; ++i) {
//        cout << message[i] << endl;
//    }
//}

void Game::glfwWindowClosedCallback(GLFWwindow* /*pWindow*/)
{
    game->running = false;
}

void Game::glfwMouseScrollCallback(GLFWwindow* /*pWindow*/, double xOffset, double yOffset)
{
    static const float factor = 1.1;
    if(yOffset < 0) {
        game->camera->multViewSizeBy(factor);
    } else {
        game->camera->multViewSizeBy(1.f/factor);
    }
}

void Game::glfwMouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{
    // Left-click
    if (button == GLFW_MOUSE_BUTTON_1){
        if (action == GLFW_PRESS){
            game->draggingCamera = true;
            double x; double y;
            glfwGetCursorPos(pWindow, &x, &y);
            game->oldMousePosition = vec2(x,y);
        }
        else if (action == GLFW_RELEASE){
            game->draggingCamera = false;
        }
    }
    //Right Click
    else if (button == GLFW_MOUSE_BUTTON_2){
        game->camera->translateTo(vec2(0,0));
    }


}

void Game::glfwMousePositionCallback(GLFWwindow* pWindow, double xOffset, double yOffset)
{
    if (game->draggingCamera){
        double x; double y;
        glfwGetCursorPos(pWindow, &x, &y);
        vec2 offset = vec2(x,y) - game->oldMousePosition;
        offset.y *= -1; // reversed controls? this should be an option

        game->camera->translateBy(offset/100.f);

        game->oldMousePosition = vec2(x,y);

    }
}
