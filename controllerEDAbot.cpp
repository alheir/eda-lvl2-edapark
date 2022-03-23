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

#include "controllerEDAbot.h"
#include <iostream>

using namespace std;

controllerEDAbot::controllerEDAbot()
{
    MQTTClient client("controller");

    if (client.connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez"))
    {
        cout << "Conectado..." << endl;
    }

    if (!client.subscribe("robot1/power/batteryLevel"))
    {
        cout << "error subscribing" << endl;
    }
}