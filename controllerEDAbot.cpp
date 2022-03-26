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
        "robot1/motor4/temperature" }; // 18

float charVectorToFloat(std::vector<char> &vector);
std::vector<char> floatToCharVector(float data);

controllerEDAbot::controllerEDAbot()
{
    client = new MQTTClient("controller");
    motorHandler = new motor[4];

    powerMethod = VOLTAGE;
    power = 0.0f; // Arranca con tensión 0 asignada
    powerCurrent = 0.0f;
    powerVoltage = 0.0f;
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

    // Actualiza valor del topic, si llega
    for (auto x : msg)
    {
        for (int i = 0; i < sizeof(readingTopics) / sizeof(readingTopics[0]); i++)
        {
            if (!x.topic.compare(readingTopics[i]))
            {
                valuesHandler[i] = charVectorToFloat(x.payload);
                switch (i)
                {
                case 0:
                    powerConsumption = valuesHandler[i];
                    break;
                case 1:
                    batteryLevel = valuesHandler[i];
                    break;
                case 2:
                    motorHandler[0].updateVoltage(valuesHandler[i]);
                    break;
                case 3:
                    motorHandler[1].updateVoltage(valuesHandler[i]);
                    break;
                case 4:
                    motorHandler[2].updateVoltage(valuesHandler[i]);
                    break;
                case 5:
                    motorHandler[3].updateVoltage(valuesHandler[i]);
                    break;
                case 6:
                    motorHandler[0].updateCurrent(valuesHandler[i]);
                    break;
                case 7:
                    motorHandler[1].updateCurrent(valuesHandler[i]);
                    break;
                case 8:
                    motorHandler[2].updateCurrent(valuesHandler[i]);
                    break;
                case 9:
                    motorHandler[3].updateCurrent(valuesHandler[i]);
                    break;
                case 10:
                    motorHandler[0].updateRpm(valuesHandler[i]);
                    break;
                case 11:
                    motorHandler[1].updateRpm(valuesHandler[i]);
                    break;
                case 12:
                    motorHandler[2].updateRpm(valuesHandler[i]);
                    break;
                case 13:
                    motorHandler[3].updateRpm(valuesHandler[i]);
                    break;
                case 14:
                    motorHandler[0].updateTemperature(valuesHandler[i]);
                    break;
                case 15:
                    motorHandler[1].updateTemperature(valuesHandler[i]);
                    break;
                case 16:
                    motorHandler[2].updateTemperature(valuesHandler[i]);
                    break;
                case 17:
                    motorHandler[3].updateTemperature(valuesHandler[i]);
                    break;
                default:
                    break;
                }
            }
        }
    }

}

void controllerEDAbot::moveForward()
{
    setMotor(power, -power, -power, power, powerMethod);
}
void controllerEDAbot::moveBackward()
{
    setMotor(-power, power, power, -power, powerMethod);
}

void controllerEDAbot::moveRight()
{
    setMotor(-power, -power, power, power, powerMethod);
}

void controllerEDAbot::moveLeft()
{
    setMotor(power, power, -power, -power, powerMethod);
}

void controllerEDAbot::rotateRight()
{
    setMotor(-0.5f, -0.5f, -0.5f, -0.5f, powerMethod);
}

void controllerEDAbot::rotateLeft()
{
    setMotor(0.5f, 0.5f, 0.5f, 0.5f, powerMethod);
}

void controllerEDAbot::stop()
{
    setMotor(0.0f, 0.0f, 0.0f, 0.0f, powerMethod);
}

motor *controllerEDAbot::getMotorInfo(int motorID)
{
    return motorHandler;
}

bool controllerEDAbot::getPowerMethod()
{
    return powerMethod;
}

float controllerEDAbot::getPower()
{
    return power;
}

float controllerEDAbot::getBatteryLevel()
{
    return batteryLevel;
}

float controllerEDAbot::getPowerConsumption()
{
    return powerConsumption;
}


void controllerEDAbot::changePowerMethod()
{
    powerMethod = !powerMethod;
    if (powerMethod == VOLTAGE)
    {
        powerVoltage = power;
        power = powerCurrent;
    }
    else if (powerMethod == CURRENT)
    {
        powerCurrent = power;
        power = powerVoltage;
    }
    
}

void controllerEDAbot::increasePowerValue()
{
    if ((powerMethod == VOLTAGE && power < maxVoltage) ||
        (powerMethod == CURRENT && power < maxCurrent))
    {
        power += powerStep;
    }
}

void controllerEDAbot::decreasePowerValue()
{
    if (power > 0)
    {
        power -= powerStep;
    }
}

void controllerEDAbot::publishColor(bool eye, unsigned char red)
{
    std::vector<char> payload;
    payload.resize(3);

    payload[0] = red;
    payload[1] = 0;
    payload[2] = 0;

    if (!eye)
    {
        if (!client->publish("robot1/display/rightEye/set", payload))
            cout << "error eye 0" << endl;
    }
    else
    {
        if (!client->publish("robot1/display/leftEye/set", payload))
            cout << "error eye 1" << endl;
    }
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

void controllerEDAbot::setMotor(float m1, float m2, float m3, float m4, bool powerMethod)
{
    std::vector<char> payload;

    payload = floatToCharVector(m1);
    if (!client->publish(writingTopics[2 + powerMethod * 4], payload))
        cout << "error m1" << endl;

    payload = floatToCharVector(m2);
    if (!client->publish(writingTopics[3 + powerMethod * 4], payload))
        cout << "error m2" << endl;

    payload = floatToCharVector(m3);
    if (!client->publish(writingTopics[4 + powerMethod * 4], payload))
        cout << "error m3" << endl;

    payload = floatToCharVector(m4);
    if (!client->publish(writingTopics[5 + powerMethod * 4], payload))
        cout << "error m4" << endl;
}


float motor::getVoltage() 
{
    return voltage;
}

float motor::getCurrent()
{
    return current;
}

float motor::getRpm()
{
    return rpm;
}

float motor::getTemperature()
{
    return temperature;
}

void motor::updateVoltage(float data)
{
    voltage = data;
}

void motor::updateCurrent(float data)
{
    current = data;
}

void motor::updateRpm(float data)
{
    rpm = data;
}

void motor::updateTemperature(float data)
{
    temperature = data;
}
