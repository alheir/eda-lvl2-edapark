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

void controllerEDAbot::moveForward()
{
    changeMotorVoltage(-1.0f, 1.0f, 1.0f, -1.0f, client);
}
void controllerEDAbot::moveBackward()
{
    changeMotorVoltage(1.0f, -1.0f, -1.0f, 1.0f, client);
}

void controllerEDAbot::moveRight()
{
    changeMotorVoltage(-1.0f, 0.0f, -1.0f, 0.0f, client);
}

void controllerEDAbot::moveLeft()
{
    changeMotorVoltage(0.0f, 1.0f, 0.0f, 1.0f, client);
}

void controllerEDAbot::rotateRight()
{
    changeMotorVoltage(-1.0f, -1.0f, -1.0f, -1.0f, client);
}

void controllerEDAbot::rotateLeft()
{
    changeMotorVoltage(1.0f, 1.0f, 1.0f, 1.0f, client);
}

void controllerEDAbot::stop()
{
    changeMotorVoltage(0.0f, 0.0f, 0.0f, 0.0f, client);
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
void changeMotorVoltage(float m1, float m2, float m3, float m4, MQTTClient *client)
{
    std::vector<char> payload;

    payload = floatToCharVector(m1);
    if (!client->publish(writingTopics[2], payload))
        cout << "error m1" << endl;

    payload = floatToCharVector(m2);
    if (!client->publish(writingTopics[3], payload))
        cout << "error m2" << endl;

    payload = floatToCharVector(m3);
    if (!client->publish(writingTopics[4], payload))
        cout << "error m3" << endl;

    payload = floatToCharVector(m4);
    if (!client->publish(writingTopics[5], payload))
        cout << "error m4" << endl;
}