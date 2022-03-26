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

enum POWER_METHODS
{
    VOLTAGE,
    CURRENT
};
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

    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    void rotateRight();
    void rotateLeft();
    void stop();

    motor *getMotorInfo(int motorID);

    bool getPowerMethod();
    float getPower();
    float getBatteryLevel();
    float getPowerConsumption();

    void getInfo();

    void changePowerMethod();
    void increasePowerValue();
    void decreasePowerValue();

    //TODO: cambiar esto!
    void publishColor(bool eye, unsigned char red);

private:
    MQTTClient *client;
    motor *motorHandler;

    const float powerStep = 0.5f;
    const float maxCurrent = 10.0f;
    const float maxVoltage = 24.0f;

    float power;
    float powerCurrent;
    float powerVoltage;
    bool powerMethod;

    float batteryLevel;
    float powerConsumption;

    float valuesHandler[22];

    raylib::Vector3 position;
    raylib::Vector3 velocity;
    raylib::Vector3 rotation;
    raylib::Vector3 angularVelocity;

    void setMotor(float m1, float m2, float m3, float m4, bool powerMethod);
};

#endif // CONTROLLER_EDA_BOT_H
