#pragma once
class Vector2d
{
public:
	Vector2d();
	Vector2d(int x, int y);
	void normalizeVector();
	void setXAndY(float x, float y);
	float getX();
	float getY();

private:
	float _x;
	float _y;
	float length;
};