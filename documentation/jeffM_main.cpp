/**********************************************************************************************
*
*   raylib-extras, examples-cpp * examples for Raylib in C++
*
*   pew * an example of movement and shots
*
*   LICENSE: MIT
*
*   Copyright (c) 2023 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <list>

struct Spring
{
	float RestDistance = 50;

	size_t Anchor = 0;

	float SpringConstant = 6;
};

struct PhysicsBody
{
	Vector2 Position = { 0 };
	Vector2 Velocity = { 0 };

	float Radius = 25;

	bool Static = true;

	float Mass = 5;

	Color Tint = WHITE;

	std::vector<Spring> Springs;
};

const Vector2 Gravity = { 0, 100.0f };

std::vector<PhysicsBody> Bodies;

void GameInit()
{
	const float staticY = 100;

	PhysicsBody staticBall;
	staticBall.Position = Vector2{ GetScreenWidth() * 0.5f, staticY };
	staticBall.Static = false;
	staticBall.Tint = SKYBLUE;

	Bodies.emplace_back(std::move(staticBall));

    PhysicsBody staticBall2;
	staticBall2.Position = Vector2{ GetScreenWidth() * 0.25f, staticY };
	staticBall2.Static = false;
	staticBall2.Tint = DARKBLUE;

    Bodies.emplace_back(std::move(staticBall2));

    PhysicsBody movingBall;
	movingBall.Position = Vector2{ GetScreenWidth() * 0.75f, GetScreenHeight() * 0.5f };
	movingBall.Static = true;
	movingBall.Tint = DARKGREEN;

	movingBall.Springs.push_back(Spring{ 50, 0 });
	movingBall.Springs.push_back(Spring{ 50, 1 });
	movingBall.Springs.push_back(Spring{ 50, 3 });

    Bodies.emplace_back(std::move(movingBall));

    PhysicsBody movingBall2;
	movingBall2.Position = Vector2{ GetScreenWidth() * 0.35f, GetScreenHeight() * 0.75f };
	movingBall2.Static = true;
	movingBall2.Tint = GREEN;
	movingBall2.Mass = 10;

	movingBall2.Springs.push_back(Spring{ 50, 2 });

    Bodies.emplace_back(std::move(movingBall2));
}

bool GameUpdate()
{
	// apply gravity
	for (auto& body : Bodies)
	{
		// solid as a rock
		if (!body.Static)
			continue;

		body.Velocity = Vector2Add(body.Velocity, Vector2Scale(Gravity, GetFrameTime()));
	}

	// apply spring forces
    for (auto& body : Bodies)
    {
		// skip if can't be affected
        if (!body.Static || body.Springs.empty())
            continue;

		for (const auto& spring : body.Springs)
		{
			// get the vector to the anchor
			Vector2 vectorToAnchor = Vector2Subtract(body.Position, Bodies[spring.Anchor].Position);

			// find out how far we are from the anchor
			float magToAnchor = Vector2Length(vectorToAnchor);

			// find out how far we are extended
			float springExtension = magToAnchor - spring.RestDistance;

			// if we aren't extended, we can't apply forces
			if (springExtension <= 0)
				continue;

            // normalize
            vectorToAnchor = Vector2Scale(vectorToAnchor, 1.0f / magToAnchor);

			// the further out we are the more foce we apply, but it's scaled down by how heavy the mass is
			float springForce = (springExtension * spring.SpringConstant) / body.Mass;

			// compute the force vector back
            Vector2 forceVector = Vector2Scale(vectorToAnchor, -springForce);
            // apply the force as an acceleration
            body.Velocity = Vector2Add(body.Velocity, Vector2Scale(forceVector, GetFrameTime()));

			//dampen
			float damping = 0.25f;

			// project the velocity along the spring vector to only dampen in that direction
			Vector2 dampingForce = Vector2Scale(vectorToAnchor, Vector2DotProduct(body.Velocity, vectorToAnchor) * -damping);

            // apply the dampening force as an acceleration
            body.Velocity = Vector2Add(body.Velocity, Vector2Scale(dampingForce, GetFrameTime()));
		}
    }

	// apply motion
    for (auto& body : Bodies)
    {
		// update us on our final velocity
        body.Position = Vector2Add(body.Position, Vector2Scale(body.Velocity, GetFrameTime()));
    }

	return true;
}

void GameDraw()
{
	BeginDrawing();
	ClearBackground(DARKGRAY);

    for (auto& body : Bodies)
    {
		DrawCircleV(body.Position, body.Radius, body.Tint);

		for (const auto& spring : body.Springs)
		{
			DrawLineV(body.Position, Bodies[spring.Anchor].Position, ORANGE);
		}
    }

	EndDrawing();
}

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(1280, 800, "Example");
	SetTargetFPS(144);

	GameInit();

	while (!WindowShouldClose())
	{
		if (!GameUpdate())
			break;
		
		GameDraw();
	}

	CloseWindow();
	return 0;
}