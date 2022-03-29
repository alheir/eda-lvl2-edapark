/**
 * @file controllerEDAbot.h
 * @author Grupo 19: Alejandro Nahuel Heir, Anna Candela Gioia Perez
 * @subject Algoritmos y estructura de datos
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
#define AMOUNTMOTORS 5

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

    void toggleDribblerPositive();
    void toggleDribblerNegative();
    void stopDribbler();

    motor *getMotorInfo();

    bool getPowerMethod();
    float getPower();
    float getBatteryLevel();
    float getPowerConsumption();
    float getMaxTemperature();
    void getInfo();

    void changePowerMethod();
    void increasePowerValue();
    void decreasePowerValue();

    void setEyes(std::vector <char> rgbLeftEye, std::vector<char> rgbRightEye);
    void checkTemperature();

    void setLCD(std::vector<char> ledsColors);
private:
    MQTTClient *client;
    motor *motorHandler;

    const float powerStep = 0.5f;
    const float maxCurrent = 10.0f;
    const float maxVoltage = 24.0f;
    const float maxTemperature = 75.0f;     //Encontramos este valor ensayando en el simulador 
    const float scaleRotation = 5.0f;     
    const float dribblerVoltage = 1.0f;

    //Método de potencia: tensión o corriente 
    bool powerMethod;                       

    float power;
    float powerCurrent;
    float powerVoltage;
    float batteryLevel;
    float powerConsumption;

    float* valuesHandler;
    
    raylib::Vector3 position;
    raylib::Vector3 velocity;
    raylib::Vector3 rotation;
    raylib::Vector3 angularVelocity;

    void setMotors(float m1, float m2, float m3, float m4);
};

#endif // CONTROLLER_EDA_BOT_H
