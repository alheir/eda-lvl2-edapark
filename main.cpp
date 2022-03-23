/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

#include "MQTTClient.h"

int main()
{

    std::vector<MQTTMessage> msg;

    MQTTClient client("controller");

    if (client.connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez"))
    {
        cout << "Conectado..." << endl;
    }

    if (!client.subscribe("robot1/power/powerConsumption"))
    {
        cout << "error subscribing" << endl;
    }

    while (getchar())
    {
        cout << "looping" << endl;

            
    }
}