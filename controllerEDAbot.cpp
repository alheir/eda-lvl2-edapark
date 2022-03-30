/**
 * @file controllerEDAbot.cpp
 * @author Grupo 19: Alejandro Nahuel Heir, Anna Candela Gioia Perez
 * @subject Algoritmos y estructura de datos
 * @brief Controlador del EDABot
 * @version 0.1
 * @date 2022-03-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <vector>
#include <cstring>
#include "controllerEDAbot.h"

using namespace std;

static const string writingTopics[] =
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
        "robot1/display/rightEye/set" // 16

};

static const string readingTopics[] =
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
        "robot1/motor5/temperature" // 22
};

/************************************************************************
/**************************Métodos de la clase***************************
************************************************************************/
controllerEDAbot::controllerEDAbot()
{
    client = new MQTTClient("controller");

    motorHandler = new motor[motorNum];
    valuesHandler = new float[sizeof(readingTopics) /
                              sizeof(readingTopics[0])];
    controlMethod = VOLTAGE;
    controlValue = 0.0f; // Arranca con tensión 0 asignada
    lastVoltageValue = 0.0f;
    lastCurrentValue = 0.0f;

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

/**
 * @brief Acciones rutinarias necesarias del controlador
 *
 */
void controllerEDAbot::update()
{
    getData();
    checkTemperature();
}

void controllerEDAbot::moveRobot(raylib::Vector2 vectorMove)
{
   /*
   *  (1,0) = (-1,-1,1,1)                   //Derecha
   *  (0,1) = (1,-1,-1,1)                   //Adelante
   *  (-1,0) = (1,1,-1,-1)                  //Izquierda
   *  (0,-1) = (-1,1,1,-1)                  //Atrás
   *  (1,1) = (1,0) + (0,1) = (0,-2,0,2)    //Derecha y adelante
   *  (1,-1) = (1,0) - (0.1) = (-2,0,2,0)   //Derecha y atrás
   *  (-1,-1) = -(1,0) - (0,1) = (0,2,0,-2) //Izquierda y atrás
   *  (-1,1) = -(1,0) + (0,1) = (2,0,-2,0)  //Izquierda y adelante 
   *
   */
   float x = unitX.GetX() * vectorMove.GetX() + unitY.GetX()* vectorMove.GetY();
   float y = unitX.GetY() * vectorMove.GetX() + unitY.GetY() * vectorMove.GetY();
   float z = unitX.GetZ() * vectorMove.GetX() + unitY.GetZ() * vectorMove.GetY();
   float w = unitX.GetW() * vectorMove.GetX() + unitY.GetW() * vectorMove.GetY();


    if (!(vectorMove == Vector2Zero()))
    {
        setMotors(x/2, y/2, z/2, w/2);
    }
    else
    {
        setMotors(x, y, z, w);
    }
}

/**
 * @brief Rota el robot hacia su derecha
 *
 */
void controllerEDAbot::rotateRight()
{
    setMotors(-controlValue / scaleRotation,
              -controlValue / scaleRotation,
              -controlValue / scaleRotation,
              -controlValue / scaleRotation);
}

/**
 * @brief Rota el robot hacia su izquierda
 *
 */
void controllerEDAbot::rotateLeft()
{
    setMotors(controlValue / scaleRotation,
              controlValue / scaleRotation,
              controlValue / scaleRotation,
              controlValue / scaleRotation);
}

/**
 * @brief Detiene todos los motores
 *
 */
void controllerEDAbot::stop()
{
    client->publishType(writingTopics[8], 0); // El dribbler
}

/**
 * @brief Gira el dribbler en la dirección del robot
 *
 */
void controllerEDAbot::setDribblerForward()
{
    client->publishType(writingTopics[8], dribblerVoltage * 1);
}

/**
 * @brief Gira el dribbler en dirección opuesta al robot
 *
 */
void controllerEDAbot::setDribblerBackward()
{
    client->publishType(writingTopics[8], dribblerVoltage * -1);
}

/**
 * @brief Devuelve puntero a motores
 *
 * @return motor*
 */
motor *controllerEDAbot::getMotorInfo()
{
    return motorHandler;
}

/**
 * @brief Devuelve método de control de motores. Ver enum CONTROL_METHODS
 *
 * @return true CURRENT
 * @return false VOLTAGE
 */
bool controllerEDAbot::getControlMethod()
{
    return controlMethod;
}

/**
 * @brief Devuelve valor de tensión o corriente en módulo que se aplica
 * a los motores de movimiento.
 *
 * @return float
 */
float controllerEDAbot::getPower()
{
    return controlValue;
}

/**
 * @brief Devuelve nivel de batería
 *
 * @return float Entre 0.0 y 1.0
 */
float controllerEDAbot::getBatteryLevel()
{
    return batteryLevel;
}

/**
 * @brief Devuelve el consumo de potencia del robot
 *
 * @return float
 */
float controllerEDAbot::getPowerConsumption()
{
    return powerConsumption;
}

/**
 * @brief Cambia el método de control de los motores de movimiento.
 *
 */
void controllerEDAbot::changeControlMethod()
{
    controlMethod = !controlMethod;
    if (controlMethod == VOLTAGE)
    {
        // Se guarda el valor de tensión anteriormente usado
        lastVoltageValue = controlValue;
        controlValue = lastCurrentValue;
    }
    else if (controlMethod == CURRENT)
    {
        // Se guarda el valor de corriente anteriormente usado
        lastCurrentValue = controlValue;
        controlValue = lastVoltageValue;
    }
}

/**
 * @brief Incrementa el módulo de tensión o cirrente que se aplica a los motores
 * de movimiento.
 *
 */
void controllerEDAbot::increaseControlValue()
{
    if ((controlMethod == VOLTAGE && controlValue < maxVoltage) ||
        (controlMethod == CURRENT && controlValue < maxCurrent))
    {
        controlValue += controlValueStep;
    }
}

/**
 * @brief Decrementa el módulo de tensión o cirrente que se aplica a los motores
 * de movimiento.
 *
 */
void controllerEDAbot::decreaseControlValue()
{
    if (controlValue > 0)
    {
        controlValue -= controlValueStep;
    }
}

/**
 * @brief Setea los dos ojos del EDABot con los colores dados
 *
 * @param rgbLeftEye vector<char> con el R8G8B8 ({R8, G8, B8}) del ojo izquiedo
 * @param rgbRightEye vector<char> con el R8G8B8 ({R8, G8, B8}) del ojo derecho
 */
void controllerEDAbot::setEyes(vector<char> rgbLeftEye, vector<char> rgbRightEye)
{
    client->publish("robot1/display/leftEye/set", rgbLeftEye);
    client->publish("robot1/display/rightEye/set", rgbRightEye);
}

/**
 * @brief Setea el display de 16x16 del EDABot.
 *
 * @param ledsColors vector<char> con el R8G8B8 de cada uno de los 256 pixeles
 */
void controllerEDAbot::setLCD(vector<char> ledsColors)
{
    client->publish(writingTopics[13], ledsColors);
}

/**
 * @brief Setea la tensión o corriente de los 4 motores de movimiento
 *
 * @param motor1 Corriente/tensión del primer motor
 * @param motor2 Corriente/tensión del segundo motor
 * @param motor3 Corriente/tensión del tercer motor
 * @param motor4 Corriente/tensión del cuarto motor
 */
void controllerEDAbot::setMotors(float motor1, float motor2, float motor3, float motor4)
{
    int methodIndexShifter = controlMethod * 4;

    client->publishType(writingTopics[methodIndexShifter++], motor1 * controlValue);
    client->publishType(writingTopics[methodIndexShifter++], motor2 * controlValue);
    client->publishType(writingTopics[methodIndexShifter++], motor3 * controlValue);
    client->publishType(writingTopics[methodIndexShifter++], motor4 * controlValue);
}

/**
 * @brief Devuelve la temperatura máxima que soportan los motores de movimiento
 *
 * @return float
 */
float controllerEDAbot::getMaxTemperature()
{
    return maxTemperature;
}

/**
 * @brief Devuelve la cantidad de motores que se controlan
 *
 * @return int
 */
int controllerEDAbot::getMotorNum()
{
    return motorNum;
}

/**
 * @brief Recibe la información de los tópicos subscritos
 *
 */
void controllerEDAbot::getData()
{
    vector<MQTTMessage> msg = client->getMessages();

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
                    motorHandler[0].voltage = valuesHandler[i];
                    break;
                case 3:
                    motorHandler[1].voltage = valuesHandler[i];
                    break;
                case 4:
                    motorHandler[2].voltage = valuesHandler[i];
                    break;
                case 5:
                    motorHandler[3].voltage = valuesHandler[i];
                    break;
                case 6:
                    motorHandler[4].voltage = valuesHandler[i];
                    break;
                case 7:
                    motorHandler[0].current = valuesHandler[i];
                    break;
                case 8:
                    motorHandler[1].current = valuesHandler[i];
                    break;
                case 9:
                    motorHandler[2].current = valuesHandler[i];
                    break;
                case 10:
                    motorHandler[3].current = valuesHandler[i];
                    break;
                case 11:
                    motorHandler[4].current = valuesHandler[i];
                    break;
                case 12:
                    motorHandler[0].rpm = valuesHandler[i];
                    break;
                case 13:
                    motorHandler[1].rpm = valuesHandler[i];
                    break;
                case 14:
                    motorHandler[2].rpm = valuesHandler[i];
                    break;
                case 15:
                    motorHandler[3].rpm = valuesHandler[i];
                    break;
                case 16:
                    motorHandler[4].rpm = valuesHandler[i];
                    break;
                case 17:
                    motorHandler[0].temperature = valuesHandler[i];
                    break;
                case 18:
                    motorHandler[1].temperature = valuesHandler[i];
                    break;
                case 19:
                    motorHandler[2].temperature = valuesHandler[i];
                    break;
                case 20:
                    motorHandler[3].temperature = valuesHandler[i];
                    break;
                case 21:
                    motorHandler[4].temperature = valuesHandler[i];
                    break;
                default:
                    break;
                }
            }
        }
    }
}

/**
 * @brief Chequea la temperatura de los motores. Si supera un valor máximo, fuerza un stop.
 *
 */
void controllerEDAbot::checkTemperature()
{
    for (int i = 0; i < motorNum; i++)
    {
        float temperatureMotor = motorHandler[i].temperature;
        if (temperatureMotor > maxTemperature)
        {
            controlValue = 0.0f;
            stop();
        }
    }
}
/************************************************************************
/**************************Métodos de la clase***************************
************************************************************************/
