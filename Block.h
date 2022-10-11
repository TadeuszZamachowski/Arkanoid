#pragma once
#include "Framework.h"

class Block 
{
public:
	Block();
	float getX();
	float getY();
	int getWidth();
	int getHeight();
	void setCoordinates(float x, float y);
	bool isDestroyed();
	void setDestroyed();

private:
	float xPosition;
	float yPosition;
	int width;
	int height;
	bool destroyed;
};

