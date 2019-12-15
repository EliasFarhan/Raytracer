//
// Created by efarhan on 12/14/19.
//
#include <iostream>
#include <scene.h>
#include <renderer.h>

int main(int argc, char** argv)
{
    RenderConfig config;
    Scene scene;
    Renderer renderer;

    renderer.SetConfig(config);

    scene.GenerateRandomScene();

    renderer.Render(scene, true);
    renderer.Export();

    return EXIT_SUCCESS;
}