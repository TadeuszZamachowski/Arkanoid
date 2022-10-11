#include "Framework.h"
#include "Ball.h"
#include "Player.h"
#include <iostream>
#include "Vector2d.h"


class MyFramework : public Framework {

public:

	int _width;
	int _height;
	bool _fullscreen;

	static const int _rows = 5;
	static const int _columns = 6;

	bool gameStarted;
	bool gameLost;
	bool gameWon;

	bool positiveAbility;
	bool negativeAbility;

	Player player;
	Ball ball;
	Vector2d aim;

	Block blocks[_rows][_columns];
	Sprite* sprites[_rows][_columns];

	Sprite* sBackground;
	Sprite* sPlayer;
	Sprite* sBall;
	Sprite* sAim;
	Sprite* sGameOver;
	Sprite* sVictory;
	Sprite* sCombo;
	Sprite* sAbilityAvailable;
	Sprite* sExtraLife; 
	Sprite* sShrink; 

	float playerInitialPositionX;
	float playerInitialPositionY;

	float playerWidth = 100;
	float playerHeight = 20;

	float ballInitialPositionX;
	float ballInitialPositionY;

	float _blockInitialPositionX;
	float _blockInitialPositionY;

	int _spaceBetweenBlocksX;
	int _spaceBetweenBlocksY;

	MyFramework(int width, int height, bool fullscreen) {
		this->_width = width;
		this->_height = height;
		this->_fullscreen = fullscreen;
	}


	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = this->_width;
		height = this->_height;
		fullscreen = this->_fullscreen;

	}

	virtual void InitBlocks(int rows, int columns, float initPosX, float initPosY, int blockSpaceX, int blockSpaceY) {
		//populating the board with blocks
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				if (i == 0 && j == 0) { // drawing the first block
					Block b;
					b.setCoordinates(initPosX, initPosY);
					blocks[i][j] = b;
				}
				else if (j == 0 && i % 2 == 1) { // every second row is moved to the right on the X axis
					Block b;
					b.setCoordinates(blocks[i - 1][j].getX() + 1.5 * blocks[i - 1][j].getWidth(), blocks[i - 1][j].getY() + blocks[i - 1][j].getHeight() + blockSpaceY);
					blocks[i][j] = b;
				}
				else if (j == 0) { // take coordinates from the row that hasn't been moved to the right
					Block b;
					b.setCoordinates(blocks[i - 2][j].getX(), blocks[i - 1][j].getY() + blocks[i - 1][j].getHeight() + blockSpaceY);
					blocks[i][j] = b;
				}
				else if (i % 2 == 1 && j == columns - 1) {
					//skipping the last block of every second row
				}
				else {
					Block b;
					b.setCoordinates(blocks[i][j - 1].getX() + blocks[i][j - 1].getWidth() + blockSpaceX, blocks[i][j - 1].getY());
					blocks[i][j] = b;
				}
			}

		}

		//creating a sprite for each block
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				
				Sprite* s = createSprite("data/13-Breakout-Tiles.png");
				setSpriteSize(s, blocks[i][j].getWidth(), blocks[i][j].getHeight());
				sprites[i][j] = s;
			}
		}
	}

	virtual bool Init() {
		gameStarted = false;
		gameLost = false;
		gameWon = false;

		positiveAbility = false;
		negativeAbility = false;

		showCursor(false);

		playerInitialPositionX = this->_width / 2 - playerWidth / 2;
		playerInitialPositionY = this->_height * 0.9;

		ballInitialPositionX = playerInitialPositionX + playerWidth / 2 - ball.getDiameter() / 2;
		ballInitialPositionY = playerInitialPositionY - playerHeight; 

		_blockInitialPositionX = this->_width * 0.093; 
		_blockInitialPositionY = this->_height * 0.1;

		player.setWidthHeight(playerWidth, playerHeight);
		player.setCoordinates(playerInitialPositionX, playerInitialPositionY);
		player.resetPoints();
		player.resetDestroyedBlocks();
		positiveAbility = false;
		negativeAbility = false;

		ball.setCoordinates(ballInitialPositionX, ballInitialPositionY);
		ball.setVelocity(0, 0);
		ball.resetCombo();

		sBackground = createSprite("data/background.jpg");
		sPlayer = createSprite("data/56-Breakout-Tiles.png");
		sBall = createSprite("data/64-Breakout-Tiles.png");
		sAim = createSprite("data/59-Breakout-Tiles.png");
		sGameOver = createSprite("data/gameover.jpg");
		sVictory = createSprite("data/youwin.jpg");
		sCombo = createSprite("data/combo.png");
		sAbilityAvailable = createSprite("data/abAvailable.png");
		sExtraLife = createSprite("data/1life.png");
		sShrink = createSprite("data/shrink.png");

		setSpriteSize(sPlayer, player.getWidth(), player.getHeight());
		setSpriteSize(sBall, ball.getDiameter(), ball.getDiameter());
		setSpriteSize(sAim, ball.getDiameter(), ball.getDiameter());
		setSpriteSize(sGameOver, this->_width, this->_height);
		setSpriteSize(sVictory, this->_width, this->_height);
		setSpriteSize(sBackground, this->_width, this->_height);
		setSpriteSize(sCombo, this->_width*0.175, this->_height*0.06);
		setSpriteSize(sAbilityAvailable, this->_width * 0.4, this->_height * 0.06);
		setSpriteSize(sExtraLife, this->_width * 0.2, this->_height * 0.06);
		setSpriteSize(sShrink, this->_width * 0.2, this->_height * 0.06);

		_spaceBetweenBlocksX = this->_width * 0.1;
		_spaceBetweenBlocksY = this->_height * 0.05;

		InitBlocks(_rows, _columns, _blockInitialPositionX, _blockInitialPositionY, _spaceBetweenBlocksX, _spaceBetweenBlocksY);

		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
		//drawing elements
		drawSprite(sBackground, 0, 0);

		drawSprite(sPlayer, player.getX(), playerInitialPositionY);
		drawSprite(sBall, ball.getX(), ball.getY());
		drawSprite(sAim, aim.getX(), aim.getY());

		ball.updatePosition(ball.getXVelocity(), ball.getYVelocity());
		ball.checkBounds(this->_width, this->_height);

		if (ball.checkCombo()) {
			drawSprite(sCombo, this->_width * 0.8, 0);
		}

		//checking for collision with blocks and drawing/destroying blocks
		for (int i = 0; i < _rows; i++)
		{
			for (int j = 0; j < _columns; j++)
			{
				if (blocks[i][j].getX() == 0 && blocks[i][j].getY() == 0) { // blocks that got skipped during init
					blocks[i][j].setDestroyed();
				}

				if (ball.blockCollision(blocks[i][j]) && blocks[i][j].isDestroyed() == false) {
					destroySprite(sprites[i][j]);
					blocks[i][j].setDestroyed();
					player.addDestroyedBlock();//for calculating when the game must end a victory
					ball.addToCombo();
					player.addPoint(ball.checkCombo());
					std::cout << "POINTS:  ->  " << player.getPoints() << std::endl;
				}
				else if (blocks[i][j].isDestroyed() == false) {
					drawSprite(sprites[i][j], blocks[i][j].getX(), blocks[i][j].getY());
				}
			}
		}

		if (positiveAbility) {
			drawSprite(sExtraLife, this->_width / 10, 0);
		}

		if (negativeAbility) {
			drawSprite(sShrink, this->_width / 10, this->_height * 0.06);
		}

		if (player.getPoints() >= 20) {
			drawSprite(sAbilityAvailable, this->_width / 3, 0);
		}
		
		gameLost = ball.checkLossCondition(this->_height);
		gameWon = player.getDestroyedBlocks() + _rows / 2 == _rows * _columns;//every second row is missing a block which unfortunetely gets added anyway
		

		if (gameWon) {
			if (ball.isCaught(player)) { //checking win conditions and stopping the ball
				ball.setVelocity(0, 0);
				drawSprite(sVictory, 0, 0);
			}	
		}
		else { //checking for collision with player
			if (ball.playerCollision(player)) {
				ball.resetCombo();
			}
		}

		//checking loss conditions and if positive ability is active
		if (gameLost) {
			if (positiveAbility) {
				player.setCoordinates(playerInitialPositionX, playerInitialPositionY);
				ball.setCoordinates(ballInitialPositionX, ballInitialPositionY);
				ball.setVelocity(0, 0);
				ball.resetCombo();
			}
			else {
				drawSprite(sGameOver, 0, 0);
			}
			positiveAbility = false;
			gameStarted = false;

		}

		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		
		aim.setXAndY(x, y);
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
		//launching the ball
		if (button == FRMouseButton::LEFT) { // game starts when clicking the left button
			if (!gameStarted) {
				Vector2d d(aim.getX() - ball.getX(), aim.getY() - ball.getY());
				d.normalizeVector();
				ball.setVelocity(d.getX(), d.getY());

			}
			gameStarted = true;
		}
		//restartng the game after victory/loss
		if (button == FRMouseButton::RIGHT) {
			if (gameLost || gameWon) {
				Init();
			}
			//activating random ability for 20 points
			if (player.getPoints() >= 20) {
				srand((unsigned)time(NULL));
				int x = 1 + (rand() % 10);
				if (x <= 5) {
					negativeAbility = false;
					positiveAbility = true;
					std::cout << "Positive ability activated" << std::endl;
				}
				else {
					negativeAbility = true;
					std::cout << "Negative ability activated" << std::endl;
					player.setWidthHeight(player.getWidth() / 2, player.getHeight());
					setSpriteSize(sPlayer, player.getWidth(), player.getHeight());
				}
				player.substractPoints();
			}
		}

	}

	//player movement 
	virtual void onKeyPressed(FRKey k) {
		if (k == FRKey::LEFT) {
			player.updatePosition(-player.getVelocity());
			if (!gameStarted) {
				ball.updatePosition(-player.getVelocity(), 0);
			}
		}
		else if (k == FRKey::RIGHT) {
			player.updatePosition(player.getVelocity());
			if (!gameStarted) {
				ball.updatePosition(player.getVelocity(), 0);
			}
		}

	}

	virtual void onKeyReleased(FRKey k) {
		if (k == FRKey::LEFT) {
			player.updatePosition(-player.getVelocity());
			if (!gameStarted) {
				ball.updatePosition(-player.getVelocity(), 0);
			}
		}
		else if (k == FRKey::RIGHT) {
			player.updatePosition(player.getVelocity());
			if (!gameStarted) {
				ball.updatePosition(player.getVelocity(), 0);
			}
		}
	}

	virtual const char* GetTitle() override
	{
		return "Arcanoid";
	}
};

int main(int argc, char* argv[])
{
	int width;
	int height;
	int full;

	if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
		width = 900;
		height = 600;
		full = 0;
	}
	else {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
		full = atoi(argv[3]);
	}

	MyFramework framework(width, height, full);
	return run(&framework);
}
