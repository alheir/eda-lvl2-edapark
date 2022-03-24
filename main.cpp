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
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

using namespace std;

#include "MQTTClient.h"

float charVectorToFloat(std::vector<char>& vector)
{
    void *pt = vector.data();
    return (*(float *)pt);
}

std::vector<char> floatToCharVector(float data)
{
    std::vector<char> vector;
    vector.reserve(sizeof(data));
    std::memcpy(vector.data(), &data, sizeof(data));

    return vector;
}

/**
 * @brief
 *
 * @param m1
 * @param m2
 * @param m3
 * @param m4
 * @param client
 */
void changeMotorVoltage(float m1, float m2, float m3, float m4, MQTTClient &client);

int main()
    controllerEDAbot controller;

    int screenWidth = 800;
    int screenHeight = 450;
    raylib::Color textColor = raylib::Color::LightGray();
    raylib::Window window(screenWidth, screenHeight, "raylib [core] example - basic window");
    std::vector<MQTTMessage> msg;
    controllerEDAbot control;
    MQTTClient client("controller");
    MQTTClient client("controller");

    SetTargetFPS(60);

    // Main game loop
    while (!window.ShouldClose())
    { // Detect window close button or ESC key
      // Update
      //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT))
        {
            DrawText("key right", 0, 0, 14, GOLD);
        }
        if (IsKeyDown(KEY_LEFT))
        {
            DrawText("key left", 0, 0, 14, GOLD);
        }
    }
        if (IsKeyDown(KEY_UP))
        {
            DrawText("key up", 0, 0, 14, GOLD);
        }

        if (IsKeyDown(KEY_DOWN))
        {
            DrawText("key down", 0, 0, 14, GOLD);
        }
            cout << "error";
        if (IsKeyDown(KEY_ENTER))
        {
            DrawText("key enter", 0, 0, 14, GOLD);
            std:vector<MQTTMessage> msg = client.getMessages();
           // std::vector<MQTTMessage> msg = client.getMessages();
/*
            int i = 0;
            for (auto x : msg)
            {
                for (int i = 0; i < sizeof(readingTopics) / sizeof(readingTopics[0]); i++)
                {
                    if (!x.topic.compare(readingTopics[i]))
                    {
                        DrawText(readingTopics[i].data(), 0, 10 * (i + 1), 12, GOLD);
                        DrawText(std::to_string(charVectorToFloat(x.payload)).data(), 70, 10 * (i + 1), 12, GOLD);
                    }
                }
            }*/
        }
            }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(BLACK);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

}

void changeMotorVoltage(float m1, float m2, float m3, float m4, MQTTClient &client)
{
    std::vector<char> payload;

    payload = floatToCharVector(m1);
    if (!client.publish("robot1/motor1/voltage/set", payload))
        cout << "error m1" << endl;

    payload = floatToCharVector(m2);
    if (!client.publish("robot1/motor2/voltage/set", payload))
        cout << "error m2" << endl;

    payload = floatToCharVector(m3);
    if (!client.publish("robot1/motor3/voltage/set", payload))
        cout << "error m3" << endl;

    payload = floatToCharVector(m4);
    if (!client.publish("robot1/motor4/voltage/set", payload))
        cout << "error m4" << endl;
}