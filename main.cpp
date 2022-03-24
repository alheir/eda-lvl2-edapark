/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>

#include "controllerEDAbot.h"

using namespace std;

int main()
{
    int screenWidth = 450;
    int screenHeight = 800;
    raylib::Window window(screenWidth, screenHeight, "EDAbot Data Visualizer");

    controllerEDAbot control;

    SetTargetFPS(60);

    bool isLEDOn = false;

    // TODO: teclas para que el usuario elija método de control (tensión o corriente). Teclas para subir o bajar el valor.

    while (!window.ShouldClose())
    {

        double time = GetTime();
        double period = time - (long)time;
        bool shouldLEDBeOn = (period < 0.1);
        if (isLEDOn != shouldLEDBeOn)
        {
            unsigned char redColor = shouldLEDBeOn ? 0xff : 0;
            control.publishColor(0, redColor);
            control.publishColor(1, redColor);

            isLEDOn = shouldLEDBeOn;
        }

        if (IsKeyDown(KEY_RIGHT))
        {
            control.moveRight();
            DrawText("key right", 0, 0, 14, GOLD);
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            control.moveLeft();
            DrawText("key left", 0, 0, 14, GOLD);
        }
        else if (IsKeyDown(KEY_UP))
        {
            control.moveForward();
            DrawText("key up", 0, 0, 14, GOLD);
        }

        else if (IsKeyDown(KEY_DOWN))
        {
            control.moveBackward();
            DrawText("key down", 0, 0, 14, GOLD);
        }
        else if (IsKeyDown(KEY_D))
        {
            control.rotateRight();
            DrawText("key D", 0, 0, 14, GOLD);
        }
        else if (IsKeyDown(KEY_I))
        {
            control.decreasePowerValue();
            DrawText("key A", 0, 0, 14, GOLD);
        }
        else if (IsKeyDown(KEY_U))
        {
            control.increasePowerValue();
            DrawText("key A", 0, 0, 14, GOLD);
        }
        else if (IsKeyDown(KEY_A))
        {
            control.rotateLeft();
            DrawText("key A", 0, 0, 14, GOLD);
        }
        else
        {
            control.stop();
        }

        if (IsKeyDown(KEY_ENTER))
        {
            DrawText("key enter", 0, 0, 14, GOLD);
        }

        BeginDrawing();
        {
            window.ClearBackground(BLACK);
            control.getInfo();
        }
        EndDrawing();
    }

    return 0;
}
