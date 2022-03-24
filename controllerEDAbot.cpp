/**
 * @file controllerEDAbot.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-03-23
 *
 * @copyright Copyright (c) 2022
 *
 */

using namespace std;

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "controllerEDAbot.h"
#include <iostream>

using namespace std;

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

void setMotor(float m1, float m2, float m3, float m4, MQTTClient *client, bool method);
float charVectorToFloat(std::vector<char> &vector);
std::vector<char> floatToCharVector(float data);

controllerEDAbot::controllerEDAbot()
{
    client = new MQTTClient("controller");
    motorHandler = new motor[4];

    batteryLevel = 0.0f;
    powerConsumption = 0.0f;

    if (client->connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez"))
    {
        cout << "Conectado..." << endl;
    }
    for (auto x : readingTopics)
    {
        if (!client->subscribe(x))
        {
            cout << "error subscribing" << x << endl;
        }
    }
}

controllerEDAbot::~controllerEDAbot()
{
    delete client;
    delete[] motorHandler;
}

void controllerEDAbot::getInfo()
{
    std::vector<MQTTMessage> msg = client->getMessages();

    // Imprime todos los topics siempre
    for(int i = 0; i < sizeof(readingTopics) / sizeof(readingTopics[0]); i++)
    {
        DrawText(readingTopics[i].data(), 0, 20 * (i + 1), 16, GOLD);
    }

    // Actualiza valor del topic, si llega
    for (auto x : msg)
    {
        for (int i = 0; i < sizeof(readingTopics) / sizeof(readingTopics[0]); i++)
        {
            if (!x.topic.compare(readingTopics[i]))
            {
                DrawText(std::to_string(charVectorToFloat(x.payload)).data(), 200, 20 * (i + 1), 16, GOLD);
            }
        }
    }

    
}

void controllerEDAbot::moveForward()
{
    setMotor(power, -power, -power, power, client, method);
}
void controllerEDAbot::moveBackward()
{
    setMotor(-power, power, power, -power, client, method);
}

void controllerEDAbot::moveRight()
{
    setMotor(-power, -power, power, power, client, method);
}

void controllerEDAbot::moveLeft()
{
    setMotor(power, power, -power, -power, client, method);
}

void controllerEDAbot::rotateRight()
{
    setMotor(-0.5f, -0.5f, -0.5f, -0.5f, client, method);
}

void controllerEDAbot::rotateLeft()
{
    setMotor(0.5f, 0.5f, 0.5f, 0.5f, client, method);
}

void controllerEDAbot::stop()
{
    setMotor(0.0f, 0.0f, 0.0f, 0.0f, client, method);
}

motor *controllerEDAbot::getMotorInfo(int motorID)
{
    return motorHandler;
}

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

void setMotor(float m1, float m2, float m3, float m4, MQTTClient *client, bool method)
{
    std::vector<char> payload;

    payload = floatToCharVector(m1);
    if (!client->publish(writingTopics[2 + method * 4], payload))
        cout << "error m1" << endl;

    payload = floatToCharVector(m2);
    if (!client->publish(writingTopics[3 + method * 4], payload))
        cout << "error m2" << endl;

    payload = floatToCharVector(m3);
    if (!client->publish(writingTopics[4 + method * 4], payload))
        cout << "error m3" << endl;

    payload = floatToCharVector(m4);
    if (!client->publish(writingTopics[5 + method * 4], payload))
        cout << "error m4" << endl;
}