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
#ifndef CONTROLLEREDABOT_H
#define CONTROLLEREDABOT_H
#include "raylib-cpp.hpp"
#include "MQTTClient.h"




class controllerEDAbot
{
public:
    controllerEDAbot();

    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    void rotateRight();
    void rotateLeft();
    void stop();

private:
    MQTTClient* client;

    float batteryLevel;
    float powerConsumption;

    raylib::Vector3 position;
    raylib::Vector3 velocity;
    raylib::Vector3 rotation;
    raylib::Vector3 angularVelocity;
    
};
#endif