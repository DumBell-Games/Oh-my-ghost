#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL.h"
#include "SString.h"
#include <vector>
#include "Point.h"
#include "Log.h"
#define MAX_FRAMES 1000

struct SDL_Texture;

class Animation
{
public:
	SString name;
	float speed = 1.0f;
	std::vector<SDL_Rect> frames;
	std::vector<int> durations;
	std::vector<iPoint> pivots;
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingpong = false;
	float currentFrame = 0.0f;
	int currentFrameInt = 0;
	int totalFrames = 0;

private:
	int loopCount = 0;
	int pingpongDirection = 1; 
	float timeStep = 1000 / 60; //tiempo que se le resta a la duracion
	float timeLeft = 0.0f;

public:

	void PushBack(const SDL_Rect& rect, int duration, const iPoint& pivot = { 0,0 })
	{
		durations.push_back(duration);
		if (frames.size() == 0) timeLeft = duration;
		frames.push_back(rect);
		totalFrames = frames.size();
		pivots.push_back(pivot);
	}

	void Reset()
	{
		currentFrame = 0;
		loopCount = 0;
	}

	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update()
	{
		//cuando baja de 0 pasamos a dibujar el siguiente frame 
		if (timeLeft <= 0) {
			currentFrame++;
			if (currentFrame >= totalFrames)
			{
				currentFrame = (loop || pingpong) ? 0 : totalFrames - 1;
				++loopCount;
				if (pingpong)
					pingpongDirection = -pingpongDirection;
			}
			//le asignamos a la variable timeLeft la duraci?n del nuevo frame
			timeLeft += durations[currentFrameInt];
		}
		//vamos restando a la duraci?n del frame
		timeLeft -= timeStep * speed;
	}

	void Render(SDL_Texture* texture, const iPoint& position, bool useCamera = true);

	SDL_Rect& GetCurrentFrame()
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}

	void DeleteAnim() {
		totalFrames = 0;
		frames.clear();
		durations.clear();
		pivots.clear();
	}

};

#endif