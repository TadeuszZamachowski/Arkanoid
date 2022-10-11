#pragma once
#include "Player.h"
#include "Block.h"
#include "Vector2d.h"

class Ball
{
public:
	Ball();
	float getXVelocity();
	float getYVelocity();
	float getX();
	float getY();
	void setCoordinates(float x, float y);
	void updatePosition(float velX, float velY);
	void checkBounds(int height, int width);
	void setVelocity(float x, float y);
	bool blockCollision(Block block);
	bool playerCollision(Player player);
	bool checkLossCondition(int height);
	void addToCombo();
	void resetCombo();
	bool checkCombo();
	int getCombo();
	int getDiameter();
	bool isCaught(Player player);

private:
	float xVelocity;
	float yVelocity;
	float xPosition;
	float yPosition;
	int diameter;
	int combo;
};

