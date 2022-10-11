#include "Ball.h"
#include "Player.h"
#include <iostream>

const double PI = 3.141592653589793238463;
const float maxBounceAngle = (5 * PI / 12);

Ball::Ball() {
	xVelocity = 0;
	yVelocity = 0;
	combo = 0;
	diameter = 15;
}


float Ball::getXVelocity() {
	return xVelocity;
}

float Ball::getYVelocity() {
	return yVelocity;
}

float Ball::getX() {
	return xPosition;
}

float Ball::getY() {
	return yPosition;
}

void Ball::setCoordinates(float x, float y) {
	xPosition = x;
	yPosition = y;
}

void Ball::setVelocity(float x, float y) {
	xVelocity = x;
	yVelocity = y;
}

void Ball::updatePosition(float velX, float velY) {
	xPosition += velX;
	yPosition += velY;
}

void Ball::checkBounds(int width, int height) {
	if (xPosition < 0 || xPosition > width - diameter) {
		xVelocity = -xVelocity;
	}
	if (yPosition < 0) { 
		yVelocity = -yVelocity;
	}
}

bool Ball::checkLossCondition(int height) {
	if (yPosition > height) {
		return true;
	}
	else {
		return false;
	}
}

bool Ball::blockCollision(Block block) {
	if (block.isDestroyed()) {
		return false;
	}

	bool collision = (int)xPosition >= (int)block.getX() && (int)xPosition <= (int)block.getX() + (int)block.getWidth() &&
		(int)yPosition >= (int)block.getY() && (int)yPosition <= (int)block.getY() + (int)block.getHeight();

	if (collision) {
		if ((int)yPosition <= (int)block.getY() || (int)yPosition >= (int)block.getY()  + ((int)block.getHeight())) { // collision on top or bottm
			yVelocity = -yVelocity;
			return true;
		}
		else { // collision on the sides
			xVelocity = -xVelocity;
			return true;
		}
	}

	

	return false;
}

//TODO compare floats
bool Ball::playerCollision(Player player) {

	if ((int)xPosition + diameter >= (int)player.getX() && (int)xPosition + diameter <= (int)player.getX() + (int)player.getWidth() &&
		(int)yPosition - diameter == (int)player.getY() - (int)player.getHeight()) {
		float relative = (player.getX() + player.getWidth() /2) - xPosition - diameter/2;
		float normalized = relative / (player.getWidth() / 2);
		float angle = normalized * maxBounceAngle;
		xVelocity = 1 * -sin(angle);
		yVelocity = 1 * -cos(angle); 
		return true;
	}
	else {
		return false;
	}
}



void Ball::addToCombo() {
	combo++;
}
void Ball::resetCombo() {
	combo = 0;
}

bool Ball::checkCombo() {
	return combo >= 3;
}

int Ball::getCombo() {
	return combo;
}

int Ball::getDiameter() {
	return diameter;
}

bool Ball::isCaught(Player player) {
	if ((int)xPosition + diameter >= (int)player.getX() && (int)xPosition + diameter <= (int)player.getX() + (int)player.getWidth() &&
		(int)yPosition - diameter == (int)player.getY() - (int)player.getHeight()) {
		return true;
	}
	return false;
}
