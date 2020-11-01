#pragma once

#include <vector>
using namespace std;
#include "GameObject.h"

class Game
{
public:
	bool        quit = false;
	float       deltaTime = 0.0f;    // Keep track of time per frame.
	float       lastTime = 0.0f;    // variable to keep overall time.
	bool        keyStatus[1024];    // Hold key status.
	bool		mouseEnabled = true; // keep track of mouse toggle.

	vector<GameObject>		game_element;
};
