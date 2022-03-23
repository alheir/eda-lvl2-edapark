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

int main()
{

    std::vector<char> payload;
    payload.reserve(sizeof(float));

    float val = -15.23E23;

    std::memcpy(payload.data(), &val, sizeof(float));

    for(auto x : payload)
    {
        cout << x << endl;
    }

    void *pt = payload.data();

    float test = *(float*)pt;

    cout << test << endl;


    /*
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
    */
}