#include "Block.h"

Block::Block() {
	width = 50;
	height = 20;
	destroyed = false;
}

float Block::getX() {
	return xPosition;
}

float Block::getY() {
	return yPosition;
}

int Block::getHeight() {
	return height;
}
int Block::getWidth() {
	return width;
}

void Block::setCoordinates(float x, float y) {
	xPosition = x;
	yPosition = y;
}

bool Block::isDestroyed() {
	return destroyed;
}

void Block::setDestroyed() {
	destroyed = true;
}