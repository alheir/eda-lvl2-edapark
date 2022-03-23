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

#include "raylib-cpp.hpp"

class motor
{
public:

private:
    float voltage;
    float current;
    float rpm;
    float temperature;
};

class controllerEDAbot
{
public:

private:
    float batteryLevel;
    float powerConsumption;

    raylib::Vector3 position;
    raylib::Vector3 velocity;
    raylib::Vector3 rotation;
    raylib::Vector3 angularVelocity;
};