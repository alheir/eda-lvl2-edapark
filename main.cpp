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

<<<<<<< HEAD
#include <raylib-cpp.hpp>
#include "controllerEDAbot.h"

const std::string writingTopics[] =
    {
        "robot1/power/powerConsumption",
        "robot1/power/batteryLevel",
        "robot1/motor1/voltage/set",
        "robot1/motor2/voltage/set",
        "robot1/motor3/voltage/set",
        "robot1/motor4/voltage/set",
        "robot1/motor1/current/set",
        "robot1/motor2/current/set",
        "robot1/motor3/current/set",
        "robot1/motor4/current/set"};

const std::string readingTopics[] =
    {
        "robot1/power/powerConsumption",
        "robot1/power/batteryLevel",
        "robot1/motor1/voltage",
        "robot1/motor2/voltage",
        "robot1/motor3/voltage",
        "robot1/motor4/voltage",
        "robot1/motor1/current",
        "robot1/motor2/current",
        "robot1/motor3/current",
        "robot1/motor4/current",
        "robot1/motor1/rpm",
        "robot1/motor2/rpm",
        "robot1/motor3/rpm",
        "robot1/motor4/rpm",
        "robot1/motor1/temperature",
        "robot1/motor2/temperature",
        "robot1/motor3/temperature",
        "robot1/motor4/temperature",
        "robot1/motor1/temperature",
        "robot1/motor2/temperature",
        "robot1/motor3/temperature",
        "robot1/motor4/temperature"};

float charVectorToFloat(std::vector<char> &vector)
{
    void *pt = vector.data();
    return (*(float *)pt);
}

std::vector<char> floatToCharVector(float data)
{
    std::vector<char> vector;
    vector.resize(sizeof(data));
    std::memcpy(vector.data(), &data, sizeof(data));
    return vector;
}
=======
#include "MQTTClient.h"
#include "controllerEDAbot.h"



>>>>>>> 90337eb (Intento de mover)

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
{
<<<<<<< HEAD
    controllerEDAbot controller;

    int screenWidth = 800;
    int screenHeight = 450;
    raylib::Color textColor = raylib::Color::LightGray();
    raylib::Window window(screenWidth, screenHeight, "raylib [core] example - basic window");
=======
    std::vector<MQTTMessage> msg;
    controllerEDAbot control;
    MQTTClient client("controller");
>>>>>>> 90337eb (Intento de mover)

    SetTargetFPS(60);

<<<<<<< HEAD
    // Main game loop
    while (!window.ShouldClose())
    { // Detect window close button or ESC key
      // Update
      //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT))
        {
            DrawText("key right", 0, 0, 14, GOLD);
=======
    //if (!client.subscribe("robot1/power/batteryLevel"))
    //{
        //cout << "error subscribing" << endl;
    //}
    
    if (!client.subscribe("robot1/motor1/current"))
        cout << "error m1" << endl;

    
    if (!client.subscribe("robot1/motor2/current"))
        cout << "error m2" << endl;

    
    if (!client.subscribe("robot1/motor3/current"))
        cout << "error m3" << endl;


    if (!client.subscribe("robot1/motor4/current"))
        cout << "error m4" << endl;



    while (getchar())
    {

        cout << "looping" << endl;
       

        std:vector<MQTTMessage> msg = client.getMessages();

        for(auto x : msg)
        {

            cout << x.topic << " ~ valor:";
            cout << charVectorToFloat(x.payload) << endl;
>>>>>>> 90337eb (Intento de mover)
        }

        if (IsKeyDown(KEY_LEFT))
        {
            DrawText("key left", 0, 0, 14, GOLD);
        }

        if (IsKeyDown(KEY_UP))
        {
            DrawText("key up", 0, 0, 14, GOLD);
        }

        if (IsKeyDown(KEY_DOWN))
        {
            DrawText("key down", 0, 0, 14, GOLD);
        }

        if (IsKeyDown(KEY_ENTER))
        {
            DrawText("key enter", 0, 0, 14, GOLD);

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