/**
 * @file controllerEDAbot.h
 * @author Grupo 19: Alejandro Nahuel Heir, Anna Candela Gioia Perez
 * @subject Algoritmos y estructura de datos
 * @brief Controlador del EDABot
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

enum CONTROL_METHODS
{
    VOLTAGE,
    CURRENT
};

class motor
{
public:
    float voltage = 0;
    float current = 0;
    float rpm = 0;
    float temperature = 0;
};

class controllerEDAbot
{
public:
    controllerEDAbot();
    ~controllerEDAbot();

    void update();

    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    void rotateRight();
    void rotateLeft();
    void stop();

    void setDribblerForward();
    void setDribblerBackward();

    motor *getMotorInfo();

    bool getControlMethod();
    float getPower();
    float getBatteryLevel();
    float getPowerConsumption();
    float getMaxTemperature();
    int getMotorNum();

    void changeControlMethod();
    void increaseControlValue();
    void decreaseControlValue();

    void setEyes(std::vector<char> rgbLeftEye, std::vector<char> rgbRightEye);

    void setLCD(std::vector<char> ledsColors);

private:
    MQTTClient *client;
    motor *motorHandler;
    float *valuesHandler; // Para manejar arreglo de valores que envía EDABot

    const float controlValueStep = 0.5f; // Pasos del controlValue permitidos al usuario
    const float maxCurrent = 10.0f;
    const float maxVoltage = 24.0f;
    const float maxTemperature = 75.0f; // Valor empírico (a más de 80°C, se queman)
    const float scaleRotation = 5.0f;
    const float dribblerVoltage = 1.0f;
    const int motorNum = 5;

    bool controlMethod; // Método de control: tensión o corriente
    float controlValue; // Valor de tensión o corriente seleccionado
    float lastCurrentValue;
    float lastVoltageValue;
    float batteryLevel;
    float powerConsumption;

    // Actualmente, no implementado
    raylib::Vector3 position;
    raylib::Vector3 velocity;
    raylib::Vector3 rotation;
    raylib::Vector3 angularVelocity;
    // Actualmente, no implementado

    void setMotors(float motor1, float motor2, float motor3, float motor4);
    void getData();
    void checkTemperature();
};

#endif // CONTROLLER_EDA_BOT_H
