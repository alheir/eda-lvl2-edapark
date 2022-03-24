/**
 * @file controllerEDAbot.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef CONTROLLER_EDA_BOT_H
#define CONTROLLER_EDA_BOT_H

#include "raylib-cpp.hpp"
#include "MQTTClient.h"

class motor
{
public:
    float getVoltage();
    float getCurrent();
    float getRpm();
    float getTemperature();

    void updateVoltage(float data);
    void updateCurrent(float data);
    void updateRpm(float data);
    void updateTemperature(float data);

private:
    float voltage;
    float current;
    float rpm;
    float temperature;

};

class controllerEDAbot
{
public:
    controllerEDAbot();
    ~controllerEDAbot();

    void getInfo();
    
    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    void rotateRight();
    void rotateLeft();
    void stop();

    motor *getMotorInfo(int motorID);

private:
    MQTTClient *client;
    motor *motorHandler;

    const float power = 6.0f;
    const bool method = 1;

    float batteryLevel;
    float powerConsumption;

    raylib::Vector3 position;
    raylib::Vector3 velocity;
    raylib::Vector3 rotation;
    raylib::Vector3 angularVelocity;
};

#endif // CONTROLLER_EDA_BOT_H
