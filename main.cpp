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
        bool mode = control.getPowerMethod();
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
        
        switch (mode)
        {
        case VOLTAGE:
            DrawText("Mode Voltage", 0, 700,20, GOLD);
                break;
        case CURRENT:
            DrawText("Mode Current", 0, 700, 20, GOLD);
            break;
        default:
            break;
        }
        DrawText(to_string(control.getPower()).data(), 200, 700, 20, GOLD);
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
        else if (IsKeyPressed(KEY_U))
        {
            control.decreasePowerValue();
            DrawText("key A", 0, 0, 14, GOLD);
        }
        else if (IsKeyPressed(KEY_I))
        {
            control.increasePowerValue();
            DrawText("key A", 0, 0, 14, GOLD);
        }
        else if (IsKeyDown(KEY_A))
        {
            control.rotateLeft();
            DrawText("key A", 0, 0, 14, GOLD);
        }
        else if (IsKeyPressed(KEY_M))
        {
            control.changePowerMethod();
        }
        else
        {
            control.stop();
        }

        if (IsKeyDown(KEY_ENTER))
        {
            DrawText("key enter", 0, 0, 14, GOLD);
        }

        for (int i = 0; i < 4; i++)
        {
            string motorCurrent = "Motor " + to_string(i + 1) + " current = " + to_string(control.getMotorInfo(i)->getCurrent());
            string motorVoltage = "Motor " + to_string(i + 1) + " voltage = " + to_string(control.getMotorInfo(i)->getVoltage());
            string motorRpm = "Motor " + to_string(i + 1) + " rmp = " + to_string(control.getMotorInfo(i)->getRpm());
            string motorTemperature = "Motor " + to_string(i + 1) + " temperature = " + to_string(control.getMotorInfo(i)->getTemperature());
            DrawText(motorCurrent.data(), 0, 0 + i * 90, 20, GOLD);
            DrawText(motorVoltage.data(), 0, 20 + i * 90, 20, GOLD);
            DrawText(motorRpm.data(), 0, 40 + i * 90, 20, GOLD);
            DrawText(motorTemperature.data(), 0, 60 + i * 90, 20, GOLD);
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
