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
#include "controllerEDAbot.h"


int main()
{
	controllerEDAbot controller;

	int screenWidth = 450;
	int screenHeight = 800;
	raylib::Color textColor = raylib::Color::LightGray();
	raylib::Window window(screenWidth, screenHeight, "raylib [core] example - basic window");
	controllerEDAbot control;

	SetTargetFPS(60);

	// Main game loop
	while (!window.ShouldClose())
	{ // Detect window close button or ESC key
	  // Update
	  //----------------------------------------------------------------------------------

		if (IsKeyDown(KEY_RIGHT))
		{
			control.moveRight();
			DrawText("key right", 0, 0, 14, GOLD);
		}
		else if (IsKeyDown(KEY_LEFT))
		{
			control.moveLeft();
			DrawText("key left", 0, 0, 14, GOLD);
		}
		else if (IsKeyDown(KEY_UP))
		{
			control.moveForward();
			DrawText("key up", 0, 0, 14, GOLD);
		}

		else if (IsKeyDown(KEY_DOWN))
		{
			control.moveBackward();
			DrawText("key down", 0, 0, 14, GOLD);
		}
		else if (IsKeyDown(KEY_D))
		{
			control.rotateRight();
			DrawText("key D", 0, 0, 14, GOLD);
		}
		else if (IsKeyDown(KEY_A))
		{
			control.rotateLeft();
			DrawText("key A", 0, 0, 14, GOLD);
		}
        else
        {
            control.stop();
        }

		if (IsKeyDown(KEY_ENTER))
		{
			DrawText("key enter", 0, 0, 14, GOLD);

			// std::vector<MQTTMessage> msg = client.getMessages();
 /*
			 int i = 0;
			 for (auto x : msg)
			 {
				 for (int i = 0; i < sizeof(readingTopics) / sizeof(readingTopics[0]); i++)
				 {
					 if (!x.topic.compare(readingTopics[i]))
					 {
						 DrawText(readingTopics[i].data(), 0, 10 * (i + 1), 12, GOLD);
						 DrawText(std::to_string(charVectorToFloat(x.payload)).data(), 70, 10 * (i + 1), 12, GOLD);
					 }
				 }
			 }*/
		}
		
		BeginDrawing();
		{
			window.ClearBackground(BLACK);
            control.getInfo();
		}
		EndDrawing();
	}
	return 0;
}



