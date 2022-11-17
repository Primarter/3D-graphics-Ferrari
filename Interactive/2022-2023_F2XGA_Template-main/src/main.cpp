// 3D Graphics and Animation - Main Template
// This uses Visual Studio Code - https://code.visualstudio.com/docs/cpp/introvideos-cpp
// Two versions available -  Win64 and Apple MacOS - please see notes
// Last changed August 2022

#include "GLApp/GLApp.hpp"
#include "Scene.hpp"

int main()
{
    GLApp::Context context;
    Scene scn;

    scn.sceneId = context.addScene(&scn);

    if (context.init() == GLApp::FAILURE)
        return 1;

    cout << endl << "Starting main loop and rendering..." << endl;

    context.run();

    context.destroy(); // Close and clean everything up...

    return GLApp::SUCCESS;
}