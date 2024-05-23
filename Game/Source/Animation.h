#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "SString.h"
#define MAX_FRAMES 1000

class Animation
{
public:
	SString name;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	int durations[MAX_FRAMES];
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

	void PushBack(const SDL_Rect& rect, int duration)
	{
		durations[totalFrames] = duration;
		if (totalFrames == 0) timeLeft = duration; //first time
		frames[totalFrames++] = rect;
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

	SDL_Rect& GetCurrentFrame()
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}

	void DeleteAnim() {
		totalFrames = 0;
	}

};

#endif