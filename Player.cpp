#include "Player.h"

Player::Player() {
	xVelocity = 40;
	destroyedBlocks = 0;
	points = 0;
}

float Player::getVelocity() {
	return xVelocity;
}

float Player::getX() {
	return xPosition;
}

float Player::getY() {
	return yPosition;
}

void Player::setCoordinates(float x, float y) {
	xPosition = x;
	yPosition = y;
}

void Player::updatePosition(float xVel) {
	xPosition += xVel;
}

void Player::addDestroyedBlock() {
	destroyedBlocks++;
}

int Player::getDestroyedBlocks() {
	return destroyedBlocks;
}

void Player::resetDestroyedBlocks() {
	destroyedBlocks = 0;
}

void Player::addPoint(bool combo) {
	if (combo) {
		points += 3;
	}
	else {
		points++;
	}
}

int Player::getPoints() {
	return points;
}
void Player::resetPoints() {
	points = 0;
}

void Player::substractPoints() {
	points -= 20;
}

void Player::setWidthHeight(float w, float h) {
	width = w;
	height = h;
}
float Player::getWidth() {
	return width;
}
float Player::getHeight() {
	return height;
}
