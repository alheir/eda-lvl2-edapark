/**
 * @file main.cpp
 * @author Grupo 19: Alejandro Nahuel Heir, Anna Candela Gioia Perez
 * @subject Algoritmos y estructura de datos
 * @brief main file. Inicializa el controlador y muestra información que obtiene del EDABot
 * @version 0.1
 * @date 2022-03-23
 *
 * @copyright Copyright (c) 2022
 *
 * CONTROLES DEL EDABOT POR TECLADO:
 *  -Movimientos de traslacion: flechas
 *  -Rotación a derecha: D
 *  -Rotación a izquierda: A
 *  -Aumentar tensión/corriente: I
 *  -Reducir tensión/corriente: U
 *  -Cambiar método de control: M
 *  -Mover dribbler hacia delante: K
 *  -Mover dribbler hacia atrás: L
 *  -Encender/apagar display: C
 *
 */

#include <iostream>
#include "controllerEDAbot.h"

using namespace std;

int main()
{
    int screenWidth = 450;
    int screenHeight = 700;
    raylib::Window window(screenWidth, screenHeight, "EDAbot Data Visualizer");

    controllerEDAbot control;

    SetTargetFPS(60);

    // Paquete RGB a mandarle al display ("sin mucho esfuerzo")
    std::vector<char> displayMsg;
    displayMsg.resize(768);

    while (!window.ShouldClose())
    {
        control.update();

        // Marc's code for blinking EDABot eyes
        double time = GetTime();
        double period = time - (long)time;
        static bool isLEDOn = false;
        bool shouldEyesBeOn = (period < 0.1);
        if (isLEDOn != shouldEyesBeOn)
        {
            char redColor = shouldEyesBeOn ? 0xff : 0;

            control.setEyes({redColor, 0, 0}, {redColor, 0, 0});

            isLEDOn = shouldEyesBeOn;
        }
        // Marc's code for blinking EDABot eyes

        // Manejo básico del display del EDABot
        // (escala de grises y dividido en 3 franjas)
        static bool shouldLCDBeOn = false;
        bool shouldLCDChangeColor = (period < 0.9);
        if (!shouldLCDBeOn)
        {
            std::memset(displayMsg.data(), 0, 768);
            control.setLCD(displayMsg);
        }
        if (shouldLCDChangeColor && shouldLCDBeOn)
        {
            static unsigned char color1 = 0;
            static unsigned char color2 = 2;
            static unsigned char color3 = 4;

            std::memset(displayMsg.data(), color1, 256);
            std::memset(displayMsg.data() + 256, color2, 256);
            std::memset(displayMsg.data() + 512, color3, 256);

            control.setLCD(displayMsg);

            color1 += 2;
            color2 += 4;
            color3 += 6;

            shouldLCDChangeColor = false;
        }
        // Manejo básico del display del EDABot

        // Detección de teclas con raylib
        raylib::Vector2 keysVector = {(float)(IsKeyDown(KEY_RIGHT) -
                                              IsKeyDown(KEY_LEFT)),
                                      (float)(IsKeyDown(KEY_UP) -
                                              IsKeyDown(KEY_DOWN))};
        control.moveRobot(keysVector);
        
        if (IsKeyDown(KEY_D))
        {
            control.rotateRight();
        }
        else if (IsKeyDown(KEY_A))
        {
            control.rotateLeft();
        }
        else if (IsKeyPressed(KEY_U))
        {
            control.decreaseControlValue();
        }
        else if (IsKeyPressed(KEY_I))
        {
            control.increaseControlValue();
        }
        else if (IsKeyPressed(KEY_M))
        {
            control.changeControlMethod();
        }
        else if (IsKeyDown(KEY_K))
        {
            control.setDribblerForward();
        }
        else if (IsKeyDown(KEY_L))
        {
            control.setDribblerBackward();
        }
        else if (IsKeyPressed(KEY_C))
        {
            shouldLCDBeOn = !shouldLCDBeOn;
        }
        else
        {
            control.stop();
        }
        // Detección de teclas con raylib

        // Visualización de la información del EDABot mediante raylib.
        // (constantes para posicionar texto totalmente empíricas)
        BeginDrawing();
        {
            window.ClearBackground(BLACK);

            // Modo de control de motores
            bool mode = control.getControlMethod();
            if (mode == VOLTAGE)
            {
                DrawText("Mode Voltage", 0, 500, 20, GOLD);
            }
            else if (mode == CURRENT)
            {
                DrawText("Mode Current", 0, 500, 20, GOLD);
            }
            DrawText(to_string(control.getPower()).data(), 200, 500, 20, GOLD);
            // Modo de control de motores

            // Consumo
            string consumptionString = "Power Consumption: " +
                                       to_string(control.getPowerConsumption());
            DrawText(consumptionString.data(), 0, 550, 20, GOLD);
            // Consumo

            // Nivel de batería
            float batteryLevel = control.getBatteryLevel();
            if (batteryLevel < 0.2f)
            {
                string batteryString = "Battery Level Low: " + to_string(batteryLevel);
                DrawText(batteryString.data(), 0, 600, 20, RED);
            }
            else
            {
                string batteryString = "Battery Level: " + to_string(batteryLevel);
                DrawText(batteryString.data(), 0, 600, 20, GOLD);
            }
            // Nivel de batería

            // Información de motores.
            for (int i = 0; i < control.getMotorNum(); i++)
            {
                string parameterMsg;

                parameterMsg = "Motor " +
                               to_string(i + 1) +
                               " current = " +
                               to_string(control.getMotorInfo()[i].current);

                DrawText(parameterMsg.data(), 0, 0 + i * 90, 20, GOLD);

                parameterMsg = "Motor " +
                               to_string(i + 1) +
                               " voltage = " +
                               to_string(control.getMotorInfo()[i].voltage);
                DrawText(parameterMsg.data(), 0, 20 + i * 90, 20, GOLD);

                parameterMsg = "Motor " +
                               to_string(i + 1) +
                               " rmp = " +
                               to_string(control.getMotorInfo()[i].rpm);
                DrawText(parameterMsg.data(), 0, 40 + i * 90, 20, GOLD);

                float temperature = control.getMotorInfo()[i].temperature;
                parameterMsg = "Motor " +
                               to_string(i + 1) +
                               " temperature = " +
                               to_string(temperature);

                // Aviso de temperatura peligrosa
                //
                // Mencionable que el controlador ya frena los motores
                // si se llega a maxTemperature
                static const float warningWindow = 5.0f;
                if (temperature < control.getMaxTemperature() - warningWindow)
                {
                    DrawText(parameterMsg.data(), 0, 60 + i * 90, 20, GOLD);
                }
                else
                {
                    DrawText(parameterMsg.data(), 0, 60 + i * 90, 20, RED);
                }
                // Aviso de temperatura peligrosa
            }
            // Información de motores
        }
        EndDrawing();
    }

    return 0;
}
