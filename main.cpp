#include <SDL.h>
#include <stdio.h>
#include <Box2D/Box2D.h>

//int main(int argc, char* argv[])
//{
//
//    // returns zero on success else non-zero
//    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
//        printf("error initializing SDL: %s\n", SDL_GetError());
//    }
//    SDL_Window* win = SDL_CreateWindow("GAME",
//        SDL_WINDOWPOS_CENTERED,
//        SDL_WINDOWPOS_CENTERED,
//        720, 720, 0);
//    while (1)
//        ;
//
//    return 0;
//}

/*
* Copyright (c) 2006-2007 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCALE = 1;


// This is a simple example of building and running a simulation
// using Box2D. Here we create a large ground box and a small dynamic
// box.
// There are no graphics for this example. Box2D is meant to be used
// with your rendering engine in your game engine.
int main(int argc, char** argv)
{


	// returns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	SDL_Renderer* renderer = nullptr;
	SDL_Window* window = nullptr;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

	// https://github.com/erincatto/box2d/blob/main/docs/hello.md

	// Define world
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = { 0.0f, -100.0f };
	b2WorldId worldId = b2CreateWorld(&worldDef);

	// Create ground
	b2BodyDef groundBodyDef = b2DefaultBodyDef();
	groundBodyDef.position = { SCREEN_WIDTH / 2 / SCALE, -SCREEN_HEIGHT / SCALE};
	b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);
	b2Polygon groundBox = b2MakeBox(SCREEN_WIDTH / 2 / SCALE, SCREEN_HEIGHT / 10 / SCALE);
	b2ShapeDef groundShapeDef = b2DefaultShapeDef();
	b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

	// Create render parameters for ground
	SDL_Rect rectg;
	rectg.w = SCREEN_WIDTH;
	rectg.h = SCREEN_HEIGHT / 10;
	rectg.x = 0;
	rectg.y = SCREEN_HEIGHT - rectg.h;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = { SCREEN_WIDTH / 2 / SCALE, 50.0f / SCALE };
	b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

	int boxw = 100;
	int boxh = 100;
	b2Polygon dynamicBox = b2MakeBox(boxw, boxh);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 10.0f;
	shapeDef.friction = 0.3f;
	b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);

	float timeStep = 1.0f / 60.0f;
	int subStepCount = 4;
	SDL_Rect rect;
	rect.w = boxw;
	rect.h = boxh;
	for (int i = 0; i < 5000; ++i)
	{
		b2World_Step(worldId, timeStep, subStepCount);
		b2Vec2 position = b2Body_GetPosition(bodyId);
		b2Rot rotation = b2Body_GetRotation(bodyId);
		b2Vec2 groundPos = b2Body_GetPosition(groundId);
		printf("%4.2f %4.2f %4.2f %4.2f\n", position.x, position.y, b2Rot_GetAngle(rotation), groundPos.y);

		// Render
		rect.x = position.x;
		rect.y = -position.y;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &rect);
		SDL_RenderDrawRect(renderer, &rectg);
		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}
	return 0;
}
