/**
 * @file controllerEDAbot.cpp
 * @author Grupo 19: Alejandro Nahuel Heir, Anna Candela Gioia Perez 
 * @subject Algoritmos y estructura de datos
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
        "robot1/dribbler/voltage/set", 
        "robot1/dribbler/current/set",
        "robot1/kicker/charge/set", 
        "robot1/kicker/kick/cmd",
        "robot1/kicker/chip/cmd",
        "robot1/display/lcd/set",    
        "robot1/display/leftEye/set" 
        "robot1/display/rightEye/set"  //16

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

//Métodos de la clase controllerEDAbot
controllerEDAbot::controllerEDAbot()
{
    client = new MQTTClient("controller");
    motorHandler = new motor[AMOUNTMOTORS];
    valuesHandler = new float[sizeof(readingTopics) / sizeof(readingTopics[0])];
    powerMethod = VOLTAGE;
    power = 0.0f; // Arranca con tensión 0 asignada
    powerCurrent = 0.0f;
    powerVoltage = 0.0f;
    client->connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez");

    client->subscribe("robot1/+/+");

}

controllerEDAbot::~controllerEDAbot()
{

    client->unsubscribe("robot1/+/+");



    client->disconnect();

    delete client;
    delete[] motorHandler;
    delete[] valuesHandler;
}

// getInfo: Lee la información de los tópicos requeridos del robot.
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

//moveFoward: mueve hacia adelante al robot
void controllerEDAbot::moveForward()
{
    setMotors(power, -power, -power, power);
}

//moveBackward: mueve hacia atrás al robot
void controllerEDAbot::moveBackward()
{
    setMotors(-power, power, power, -power);
}


//moveRight: mueve hacia la derecha al robot
void controllerEDAbot::moveRight()
{
    setMotors(-power, -power, power, power);
}


//moveLeft: mueve hacia la izquierda al robot
void controllerEDAbot::moveLeft()
{
    setMotors(power, power, -power, -power);
}


//rotateRight: rota hacia la derecha al robot
void controllerEDAbot::rotateRight()
{
    setMotors(-power/scaleRotation, -power / scaleRotation, -power / scaleRotation, -power / scaleRotation);
}

//rotateLeft: rota hacia la izquierda al robot
void controllerEDAbot::rotateLeft()
{
    setMotors(power / scaleRotation, power / scaleRotation, power / scaleRotation, power / scaleRotation);
}

//stop: para al robot
void controllerEDAbot::stop()
{
    setMotors(0.0f, 0.0f, 0.0f, 0.0f);
}

//toggleDribblerPositive: gira el dribbler con tensión/corriente positiva
void controllerEDAbot::toggleDribblerPositive()
{
    client->publishType(writingTopics[8], dribblerVoltage * 1);
}

//toggleDribblerNegative: gira el dribbler con tensión/corriente negativa
void controllerEDAbot::toggleDribblerNegative()
{
    client->publishType(writingTopics[8], dribblerVoltage * -1);
}

//stopDibbler: para el dribbler
void controllerEDAbot::stopDribbler()
{
    client->publishType(writingTopics[8], 0);
}

//getMotorInfo: Devuelve el puntero al arreglo de las clases de los motores
motor *controllerEDAbot::getMotorInfo()
{
    return motorHandler;
}

//getPowerMethod: Devuelve el método de la potencia (si se utiliza corriente o tensión)
bool controllerEDAbot::getPowerMethod()
{
    return powerMethod;
}

//getPower: Devuelve un float que represenra el valor de corriente/tensión en módulo que se le aplica a los motores
float controllerEDAbot::getPower()
{
    return power;
}

//getBatteryLevel: Devuelve un float que representa el nivel de la batería
float controllerEDAbot::getBatteryLevel()
{
    return batteryLevel;
}

//getPowerConsumption: Devuelve un float que representa el consumo de potencia
float controllerEDAbot::getPowerConsumption()
{
    return powerConsumption;
}

//changePowerMethod: cambia el método de potencia (corriente o tensión) 
void controllerEDAbot::changePowerMethod()
{
    powerMethod = !powerMethod;
    if (powerMethod == VOLTAGE)
    {
        //Se guarda el valor de tensión anteriormente usado
        powerVoltage = power;
        power = powerCurrent;         
    }
    else if (powerMethod == CURRENT)
    {
        //Se guarda el valor de corriente anteriormente usado
        powerCurrent = power;   
        power = powerVoltage;
    }
}

//increasePowerValue: Incrementa el módulo de tensión o corriente que se le aplica a los motores
void controllerEDAbot::increasePowerValue()
{
    if ((powerMethod == VOLTAGE && power < maxVoltage) ||
        (powerMethod == CURRENT && power < maxCurrent)) 
    {
        power += powerStep;
    }
}

//decreasePowerValue: Decrementa el módulo de tensión o corriente que se le aplica a los motores
void controllerEDAbot::decreasePowerValue()
{
    if (power > 0)
    {
        power -= powerStep;
    }
}

//setEyes: setea los dos ejes del robot
// std::vector <char> rgbLeftEye: payload que informa el color del ojo derecho
// std::vector <char> rgbRightEye: payload que informa el color del ojo izquierdo
void controllerEDAbot::setEyes(std::vector <char> rgbLeftEye, std::vector<char> rgbRightEye)
{
    client->publish("robot1/display/leftEye/set", rgbLeftEye);
    client->publish("robot1/display/rightEye/set", rgbRightEye);
}

//setLCD: setea el display de 16x16 del robot
// std::vector>char> ledsColors: arreglo con todos los colores de todos los píxeles
void controllerEDAbot::setLCD(std::vector<char> ledsColors)
{
    client->publish(writingTopics[13], ledsColors);
}

//setMotors: setea la corriente/tensión de los 4 motores principales
// float m1: corriente/tensión del primer motor
// float m2: corriente/tensión del segundo motor
// float m3: corriente/tensión del tercer motor
// float m4: corriente/tensión del cuarto motor
void controllerEDAbot::setMotors(float m1, float m2, float m3, float m4)
{
        int methodIndexShifter = powerMethod * 4;

        client->publishType(writingTopics[methodIndexShifter++], m1);
        client->publishType(writingTopics[methodIndexShifter++], m2);
        client->publishType(writingTopics[methodIndexShifter++], m3);
        client->publishType(writingTopics[methodIndexShifter++], m4);


}

//getMaxTemperature: devuelve la temperatura máxima que soporta el robot
float controllerEDAbot::getMaxTemperature()
{
    return maxTemperature;
}

//checkTemperature: chequea la temperatura del robot, si se calienta mucho para el movimiento de este
void controllerEDAbot::checkTemperature()
{
    for (int i = 0; i < AMOUNTMOTORS; i++)
    {
        float temperatureMotor = motorHandler[i].getTemperature();
        if (temperatureMotor > maxTemperature)
        {
            power = 0.0f;
        }

    }
}

//Métodos de la clase motor

//getVoltage: devuelve un float que representa la tensión del motor
float motor::getVoltage()
{
    return voltage;
}

//getCurrent: devuelve un float que representa la corriente del motor
float motor::getCurrent()
{
    return current;
}

//getRpm: devuelve un float que representa el rpm del motor
float motor::getRpm()
{
    return rpm;
}

//getTemperature: devuelve un float que representa la temperatura del motor
float motor::getTemperature()
{
    return temperature;
}

//updateVoltage: actualiza la tensión del motor
// float data: tensión a aplicar al motor
void motor::updateVoltage(float data)
{
    voltage = data;
}

//updateCurrent: actualiza la corriente del motor
// float data: corriente a aplicar al motor
void motor::updateCurrent(float data)
{
    current = data;
}

//updateRpm: actualiza el rpm del motor
// float data: rpm a aplicar al motor
void motor::updateRpm(float data)
{
    rpm = data;
}

//updateTemperature: actualiza la temperatura del motor
// float data: temperatura a aplicar al motor
void motor::updateTemperature(float data)
{
    temperature = data;
}


