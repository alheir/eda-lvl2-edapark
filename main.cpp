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
#include <stdlib.h>
#include <cstring>

using namespace std;

#include "MQTTClient.h"

float charVectorToFloat(std::vector<char>& vector)
{
    void *pt = vector.data();
    return (*(float *)pt);
}

std::vector<char> floatToCharVector(float data)
{
    std::vector<char> vector;
    vector.reserve(sizeof(data));
    std::memcpy(vector.data(), &data, sizeof(data));

    return vector;
}

int main()
{
    std::vector<MQTTMessage> msg;

    MQTTClient client("controller");

    if (client.connect("127.0.0.1", 1883, "user", "vdivEMMN3SQWX2Ez"))
    {
        cout << "Conectado..." << endl;
    }

    if (!client.subscribe("robot1/power/batteryLevel"))
    {
        cout << "error subscribing" << endl;
    }



    while (getchar())
    {
        cout << "looping" << endl;

        std::vector<char> payload = floatToCharVector(10.0f);

        if(!client.publish("robot1/motor2/current/set", payload));
            cout << "error";

        while(getchar())
        {
            std:vector<MQTTMessage> msg = client.getMessages();

            for(auto x : msg)
            {
                cout << x.topic << " ~ valor:";
                cout << charVectorToFloat(x.payload) << endl;
            }

        }

    }
    
}