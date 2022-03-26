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
        "robot1/motor4/temperature"}; // 18

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
    for (auto x : readingTopics)
    {
        if (!client->unsubscribe(x))
        {
            cout << "error unsubscribing" << x << endl;
        }
    }

    client->disconnect();

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
                valuesHandler[i] = client->convertMessage<float>(x.payload);
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
    setMotors(power, -power, -power, power);
}
void controllerEDAbot::moveBackward()
{
    setMotors(-power, power, power, -power);
}

void controllerEDAbot::moveRight()
{
    setMotors(-power, -power, power, power);
}

void controllerEDAbot::moveLeft()
{
    setMotors(power, power, -power, -power);
}

void controllerEDAbot::rotateRight()
{
    setMotors(-0.5f, -0.5f, -0.5f, -0.5f);
}

void controllerEDAbot::rotateLeft()
{
    setMotors(0.5f, 0.5f, 0.5f, 0.5f);
}

void controllerEDAbot::stop()
{
    setMotors(0.0f, 0.0f, 0.0f, 0.0f);
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

void controllerEDAbot::setEyes(std::vector<unsigned char> rgbLeftEye, std::vector<unsigned char> rgbRightEye)
{
    if (!client->publishType("robot1/display/leftEye/set", rgbLeftEye))
    {
        cout << "error left eye" << endl;
    }

    if (!client->publishType("robot1/display/rightEye/set", rgbRightEye))
    {
        cout << "error right eye" << endl;
    }
}

void controllerEDAbot::setMotors(float m1, float m2, float m3, float m4)
{
    static int methodIndexShifter = powerMethod * 4;

    if (!client->publishType(writingTopics[2 + methodIndexShifter], m1))
    {
        cout << "error m1" << endl;
    }

    if (!client->publishType(writingTopics[3 + methodIndexShifter], m2))
    {
        cout << "error m2" << endl;
    }

    if (!client->publishType(writingTopics[4 + methodIndexShifter], m3))
    {
        cout << "error m3" << endl;
    }

    if (!client->publishType(writingTopics[5 + methodIndexShifter], m4))
    {
        cout << "error m4" << endl;
    }
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
