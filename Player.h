#pragma once
#include "Framework.h"
#include <iostream>
#include <utility>

class Player
{
public:
	Player();
	float getVelocity();
	float getX();
	float getY();
	void setCoordinates(float x, float y);
	void updatePosition(float xVel);
	void addDestroyedBlock();
	int getDestroyedBlocks();
	void resetDestroyedBlocks();
	void addPoint(bool combo);
	int getPoints();
	void resetPoints();
	void substractPoints();
	void setWidthHeight(float w, float h);
	float getWidth();
	float getHeight();

private:
	float xPosition;
	float yPosition;
	float xVelocity;
	float width;
	float height;
	int destroyedBlocks;
	int points;
};

