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

#include "controllerEDAbot.h"
#include <iostream>

using namespace std;

controllerEDAbot::controllerEDAbot()
{
    MQTTClient client("controller");

    if (client.connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez"))
    {
        cout << "Conectado..." << endl;
    }

    if (!client.subscribe("robot1/power/batteryLevel"))
    {
        cout << "error subscribing" << endl;
    }
}
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "controllerEDAbot.h"


using namespace std;
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

}

void controllerEDAbot::rotateLeft()
{

}

void controllerEDAbot::stop()
{

}





float charVectorToFloat(std::vector<char>& vector)
{
    void* pt = vector.data();
    return (*(float*)pt);
}

std::vector<char> floatToCharVector(float data)
{
    std::vector<char> vector;
    vector.resize(sizeof(data));

    std::memcpy(vector.data(), &data, sizeof(data));

    return vector;
}
void changeMotorVoltage(float m1, float m2, float m3, float m4, MQTTClient& client)
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