#include "Vector2d.h"
#include <math.h>

Vector2d::Vector2d() {

}

Vector2d::Vector2d(int x, int y) {
	_x = x;
	_y = y;
}


void Vector2d::normalizeVector() {
	length = sqrt(_x * _x + _y * _y);
	this->_x = _x / length;
	this->_y = _y / length;
}

void Vector2d::setXAndY(float x, float y) {
	_x = x;
	_y = y;
}

float Vector2d::getX() {
	return _x;
}

float Vector2d::getY() {
	return _y;
}
