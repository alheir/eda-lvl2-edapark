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

        "robot1/motor1/voltage/set", 
        "robot1/motor2/voltage/set",
        "robot1/motor3/voltage/set",
        "robot1/motor4/voltage/set",
        "robot1/motor1/current/set", 
        "robot1/motor2/current/set",
        "robot1/motor3/current/set",
        "robot1/motor4/current/set",
        "robot1/dribbler/voltage/set", // 10
        "robot1/dribbler/current/set",
        "robot1/kicker/charge/set", // 12
        "robot1/kicker/kick/cmd",
        "robot1/kicker/chip/cmd",
        "robot1/display/lcd/set",    // 15
        "robot1/display/leftEye/set" // 16
        "robot1/display/rightEye/set"

};

const std::string readingTopics[] =
{
    "robot1/power/powerConsumption",
    "robot1/power/batteryLevel",
    "robot1/motor1/voltage",
    "robot1/motor2/voltage",
    "robot1/motor3/voltage",
    "robot1/motor4/voltage",
    "robot1/motor5/voltage",
    "robot1/motor1/current",
    "robot1/motor2/current",
    "robot1/motor3/current",
    "robot1/motor4/current",
    "robot1/motor5/current",
    "robot1/motor1/rpm",
    "robot1/motor2/rpm",
    "robot1/motor3/rpm",
    "robot1/motor4/rpm",
    "robot1/motor5/rpm",
    "robot1/motor1/temperature",
    "robot1/motor2/temperature",
    "robot1/motor3/temperature",
    "robot1/motor4/temperature",
    "robot1/motor5/temperature" //22
};
controllerEDAbot::controllerEDAbot()
{
    client = new MQTTClient("controller");
    motorHandler = new motor[AMOUNTMOTORS];
    valuesHandler = new float[sizeof(readingTopics) / sizeof(readingTopics[0])];
    powerMethod = VOLTAGE;
    power = 0.0f; // Arranca con tensión 0 asignada
    powerCurrent = 0.0f;
    powerVoltage = 0.0f;
    if (client->connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez"))
    {
        cout << "Conectado..." << endl;
    }
    if (!client->subscribe("robot1/+/+"))
    {
        cout << "error subscribing" << endl;
    }
}

controllerEDAbot::~controllerEDAbot()
{

    if (!client->unsubscribe("robot1/+/+"))
    {
        cout << "error subscribing" << endl;
    }

    client->disconnect();

    delete client;
    delete[] motorHandler;
    delete[] valuesHandler;
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
                    motorHandler[4].updateVoltage(valuesHandler[i]);                  
                    break;
                case 7:
                    motorHandler[0].updateCurrent(valuesHandler[i]);
                    break;
                case 8:
                    motorHandler[1].updateCurrent(valuesHandler[i]);
                    break;
                case 9:
                    motorHandler[2].updateCurrent(valuesHandler[i]);
                    break;
                case 10:
                    motorHandler[3].updateCurrent(valuesHandler[i]);
                    break;
                case 11:
                    motorHandler[4].updateCurrent(valuesHandler[i]);
                    break;
                case 12:
                    motorHandler[0].updateRpm(valuesHandler[i]);  
                    break;
                case 13:
                    motorHandler[1].updateRpm(valuesHandler[i]);                  
                    break;
                case 14:
                    motorHandler[2].updateRpm(valuesHandler[i]);  
                    break;
                case 15:
                    motorHandler[3].updateRpm(valuesHandler[i]);
                    break;
                case 16:
                    motorHandler[4].updateRpm(valuesHandler[i]);    
                    break;
                case 17:
                    motorHandler[0].updateTemperature(valuesHandler[i]);
                    break;
                case 18:
                    motorHandler[1].updateTemperature(valuesHandler[i]);
                    break;
                case 19:
                    motorHandler[2].updateTemperature(valuesHandler[i]);
                    break;
                case 20:
                    motorHandler[3].updateTemperature(valuesHandler[i]);
                    break;
                case 21:
                    motorHandler[4].updateTemperature(valuesHandler[i]);
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
    setMotors(-power/scaleRotation, -power / scaleRotation, -power / scaleRotation, -power / scaleRotation);
}

void controllerEDAbot::rotateLeft()
{
    setMotors(power / scaleRotation, power / scaleRotation, power / scaleRotation, power / scaleRotation);
}

void controllerEDAbot::stop()
{
    setMotors(0.0f, 0.0f, 0.0f, 0.0f);
}

void controllerEDAbot::toggleDribblerPositive()
{

    client->publishType(writingTopics[8], dribblerVoltage * 1);

}

void controllerEDAbot::toggleDribblerNegative()
{

    client->publishType(writingTopics[8], dribblerVoltage * -1);

}

void controllerEDAbot::stopDribbler()
{
    client->publishType(writingTopics[8], 0);
}

motor *controllerEDAbot::getMotorInfo()
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

void controllerEDAbot::setEyes(std::vector <char> rgbLeftEye, std::vector<char> rgbRightEye)
{
    if (!client->publish("robot1/display/leftEye/set", rgbLeftEye))
    {
        cout << "error left eye" << endl;
    }

    if (!client->publish("robot1/display/rightEye/set", rgbRightEye))
    {
        cout << "error right eye" << endl;
    }
}
void controllerEDAbot::setLCD(std::vector<char> ledsColors)
{
    if (!client->publish(writingTopics[13], ledsColors))
    {
        cout << "error LEDs" << endl;
    }
}
void controllerEDAbot::setMotors(float m1, float m2, float m3, float m4)
{
        int methodIndexShifter = powerMethod * 4;

        if (!client->publishType(writingTopics[methodIndexShifter++], m1))
        {
            cout << "error m1" << endl;
        }

        if (!client->publishType(writingTopics [methodIndexShifter++], m2))
        {
            cout << "error m2" << endl;
        }

        if (!client->publishType(writingTopics[methodIndexShifter++], m3))
        {
            cout << "error m3" << endl;
        }

        if (!client->publishType(writingTopics[methodIndexShifter++], m4))
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

float controllerEDAbot::getMaxTemperature()
{
    return maxTemperature;
}
void controllerEDAbot::checkTemperature()
{
    for (int i = 0; i < AMOUNTMOTORS; i++)
    {
 
        float temperatureMotor = motorHandler[i].getTemperature();
        if (temperatureMotor > maxTemperature )
        {
            power = 0.0f;  
          
        }

    }
}
