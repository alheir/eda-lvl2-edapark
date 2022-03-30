/**
 * @file main_test.cpp
 * @author Grupo 19: Alejandro Nahuel Heir, Anna Candela Gioia Perez
 * @subject Algoritmos y estructura de datos
 * @brief Tests principales de conexión con EDABot.
 * Requieren EDAPark corriendo.
 * @version 0.1
 * @date 2022-03-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include "MQTTClient.h"

using namespace std;

int fail();
int pass();

int main()
{
    MQTTClient client("controller");

    cout << "Conexión con el cliente MQTT... ";
    if (!client.connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez"))
        return fail();
    pass();

    cout << "Subscripción a todos los topics... ";
    if (!client.subscribe("robot1/+/+"))
        return fail();
    pass();

    cout << "Desubscripción a todos los topics... ";
    if (!client.unsubscribe("robot1/+/+"))
        return fail();

    cout << "Resubscripción a todos los topics... ";
    if (!client.subscribe("robot1/+/+"))
        return fail();
    pass();

    cout << "Conversión de float a vector de chars... ";
    float floatValue = 10.25f;
    std::vector<char> payload;
    payload.resize(sizeof(floatValue));
    std::memcpy(payload.data(), &floatValue, sizeof(floatValue));
    if (payload[0] != 0x00 ||
        payload[1] != 0x00 ||
        payload[2] != 0x24 ||
        payload[3] != 0x41)
        return fail();
    pass();

    cout << "Conversión de vector de chars a float... ";
    void *pt = payload.data();
    if((*(float*)pt) != floatValue)
        return fail();
    pass();

    return 0;
}

int fail()
{
    cout << "FAIL" << endl;
    return 1;
}

int pass()
{
    cout << "PASS" << endl;
    return 0;
}