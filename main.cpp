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

    while (!window.ShouldClose())
    {
        bool mode = control.getPowerMethod();

        /* Marc code for blinking EDABot eyes */
        double time = GetTime();
        double period = time - (long)time;
        bool shouldLEDBeOn = (period < 0.1);
        if (isLEDOn != shouldLEDBeOn)
        {
            char redColor = shouldLEDBeOn ? 0xff : 0;

            control.setEyes({redColor, 0, 0}, {redColor, 0, 0});

            isLEDOn = shouldLEDBeOn;
        }
        /* Marc code for blinking EDABot eyes */

        switch (mode)
        {
        case VOLTAGE:
            DrawText("Mode Voltage", 0, 700, 20, GOLD);
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
            
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            control.moveLeft();
            
        }
        else if (IsKeyDown(KEY_UP))
        {
            control.moveForward();
            
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            control.moveBackward();
            
        }
        else if (IsKeyDown(KEY_D))
        {
            control.rotateRight();
            
        }
        else if (IsKeyDown(KEY_A))
        {
            control.rotateLeft();

        }
        else if (IsKeyPressed(KEY_U))
        {
            control.decreasePowerValue();
            
        }
        else if (IsKeyPressed(KEY_I))
        {
            control.increasePowerValue();
            
        }
        else if (IsKeyPressed(KEY_M))
        {
            control.changePowerMethod();
        }
        else
        {
            control.stop();
        }

        for (int i = 0; i < AMOUNTMOTORS; i++)
        {
            string motorCurrent = "Motor " + to_string(i + 1) + " current = " + to_string(control.getMotorInfo(i)->getCurrent());
            string motorVoltage = "Motor " + to_string(i + 1) + " voltage = " + to_string(control.getMotorInfo(i)->getVoltage());
            string motorRpm = "Motor " + to_string(i + 1) + " rmp = " + to_string(control.getMotorInfo(i)->getRpm());
            string motorTemperature = "Motor " + to_string(i + 1) + " temperature = " + to_string(control.getMotorInfo(i)->getTemperature());
            DrawText(motorCurrent.data(), 0, 0 + i * 90, 20, GOLD);
            DrawText(motorVoltage.data(), 0, 20 + i * 90, 20, GOLD);
            DrawText(motorRpm.data(), 0, 40 + i * 90, 20, GOLD);

            //PONER CONSTANTES
            if (control.getMotorInfo(i)->getTemperature() < control.getMaxTemperature() - 1.0f)
            {
                DrawText(motorTemperature.data(), 0, 60 + i * 90, 20, GOLD);
            }
            else
            {
                DrawText(motorTemperature.data(), 0, 60 + i * 90, 20, RED);
            }
            
        }
        control.checkTemperature();
        //PONER CONSTANTES
        
        if (control.getBatteryLevel() < 0.2f)
        {
            DrawText("Battery Level Low", 0, 760, 20, RED);
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
