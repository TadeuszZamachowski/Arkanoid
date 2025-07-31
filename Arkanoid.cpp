#include "Framework.h"
#include "Ball.h"
#include "Player.h"
#include <iostream>
#include "Vector2d.h"

class BlockGrid {
public:
    static const int ROWS = 5;
    static const int COLUMNS = 6;
    
    Block blocks[ROWS][COLUMNS];
    Sprite* sprites[ROWS][COLUMNS];
    int totalBlocks = 0;

    void Initialize(float initX, float initY, int spaceX, int spaceY) {
        // Initialize blocks array to nullptr
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                sprites[i][j] = nullptr;
            }
        }

        // Populate the board with blocks
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                if (i % 2 == 1 && j == COLUMNS - 1) {
                    // Skip last block in odd rows
                    continue;
                }

                if (i == 0 && j == 0) {
                    blocks[i][j].setCoordinates(initX, initY);
                }
                else if (j == 0) {
                    float yPos = (i % 2 == 1) 
                        ? blocks[i-1][j].getY() + blocks[i-1][j].getHeight() + spaceY
                        : blocks[i-2][j].getY() + blocks[i-1][j].getHeight() + spaceY;
                    
                    float xPos = (i % 2 == 1) 
                        ? blocks[i-1][j].getX() + 1.5f * blocks[i-1][j].getWidth()
                        : blocks[i-2][j].getX();
                    
                    blocks[i][j].setCoordinates(xPos, yPos);
                }
                else {
                    float xPos = blocks[i][j-1].getX() + blocks[i][j-1].getWidth() + spaceX;
                    blocks[i][j].setCoordinates(xPos, blocks[i][j-1].getY());
                }
                totalBlocks++;
            }
        }
    }

    void CreateSprites() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                if (i % 2 == 1 && j == COLUMNS - 1) continue;
                
                sprites[i][j] = createSprite("data/13-Breakout-Tiles.png");
                setSpriteSize(sprites[i][j], blocks[i][j].getWidth(), blocks[i][j].getHeight());
            }
        }
    }

    void Cleanup() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                if (sprites[i][j]) {
                    destroySprite(sprites[i][j]);
                    sprites[i][j] = nullptr;
                }
            }
        }
    }
};

class MyFramework : public Framework {
    enum class AbilityState {
        NONE,
        EXTRA_LIFE,
        SHRINK_PADDLE
    };

    // Game state
    struct GameState {
        bool started = false;
        bool lost = false;
        bool won = false;
        AbilityState ability = AbilityState::NONE;
    };

    // Game constants
    const float PLAYER_WIDTH = 100.0f;
    const float PLAYER_HEIGHT = 20.0f;
    const int ABILITY_COST = 20;
    const float UI_SCALE = 0.06f;

    // Game elements
    GameState state;
    Player player;
    Ball ball;
    BlockGrid blocks;
    Vector2d aim;

    // Sprites
    Sprite* sBackground = nullptr;
    Sprite* sPlayer = nullptr;
    Sprite* sBall = nullptr;
    Sprite* sAim = nullptr;
    Sprite* sGameOver = nullptr;
    Sprite* sVictory = nullptr;
    Sprite* sCombo = nullptr;
    Sprite* sAbilityAvailable = nullptr;
    Sprite* sExtraLife = nullptr;
    Sprite* sShrink = nullptr;

    // Positions
    float playerInitialX = 0.0f;
    float playerInitialY = 0.0f;
    float ballInitialX = 0.0f;
    float ballInitialY = 0.0f;

    // Framework parameters
    int _width;
    int _height;
    bool _fullscreen;

public:
    MyFramework(int width, int height, bool fullscreen) 
        : _width(width), _height(height), _fullscreen(fullscreen) {}

    virtual void PreInit(int& width, int& height, bool& fullscreen) {
        width = _width;
        height = _height;
        fullscreen = _fullscreen;
    }

    virtual bool Init() {
        InitializeGameState();
        CalculateInitialPositions();
        LoadResources();
        InitializeBlocks();
        return true;
    }

    virtual void Close() {
        CleanupResources();
    }

    virtual bool Tick() {
        DrawGame();
        UpdateGame();
        CheckGameConditions();
        return false;
    }

    virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
        aim.setXAndY(x, y);
    }

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
        if (button == FRMouseButton::LEFT && !isReleased) {
            HandleLeftClick();
        }
        else if (button == FRMouseButton::RIGHT && !isReleased) {
            HandleRightClick();
        }
    }

    virtual void onKeyPressed(FRKey k) {
        HandleKeyPress(k, false);
    }

    virtual void onKeyReleased(FRKey k) {
        HandleKeyPress(k, true);
    }

    virtual const char* GetTitle() override {
        return "Arcanoid";
    }

private:
    void InitializeGameState() {
        state.started = false;
        state.lost = false;
        state.won = false;
        state.ability = AbilityState::NONE;
        player.reset();
        ball.reset();
        showCursor(false);
    }

    void CalculateInitialPositions() {
        playerInitialX = _width / 2 - PLAYER_WIDTH / 2;
        playerInitialY = _height * 0.9f;
        ballInitialX = playerInitialX + PLAYER_WIDTH / 2 - ball.getDiameter() / 2;
        ballInitialY = playerInitialY - PLAYER_HEIGHT;

        player.setWidthHeight(PLAYER_WIDTH, PLAYER_HEIGHT);
        player.setCoordinates(playerInitialX, playerInitialY);
        ball.setCoordinates(ballInitialX, ballInitialY);
    }

    void LoadResources() {
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

        SetSpriteSizes();
    }

    void SetSpriteSizes() {
        setSpriteSize(sBackground, _width, _height);
        setSpriteSize(sPlayer, player.getWidth(), player.getHeight());
        setSpriteSize(sBall, ball.getDiameter(), ball.getDiameter());
        setSpriteSize(sAim, ball.getDiameter(), ball.getDiameter());
        setSpriteSize(sGameOver, _width, _height);
        setSpriteSize(sVictory, _width, _height);
        setSpriteSize(sCombo, _width * 0.175f, _height * UI_SCALE);
        setSpriteSize(sAbilityAvailable, _width * 0.4f, _height * UI_SCALE);
        setSpriteSize(sExtraLife, _width * 0.2f, _height * UI_SCALE);
        setSpriteSize(sShrink, _width * 0.2f, _height * UI_SCALE);
    }

    void InitializeBlocks() {
        float blockInitX = _width * 0.093f;
        float blockInitY = _height * 0.1f;
        int spaceX = _width * 0.1f;
        int spaceY = _height * 0.05f;
        
        blocks.Initialize(blockInitX, blockInitY, spaceX, spaceY);
        blocks.CreateSprites();
    }

    void CleanupResources() {
        blocks.Cleanup();
        
        // Destroy other sprites
        Sprite* sprites[] = {sBackground, sPlayer, sBall, sAim, sGameOver, 
                            sVictory, sCombo, sAbilityAvailable, sExtraLife, sShrink};
        for (auto sprite : sprites) {
            if (sprite) destroySprite(sprite);
        }
    }

    void DrawGame() {
        drawSprite(sBackground, 0, 0);
        DrawBlocks();
        DrawPlayer();
        DrawBall();
        DrawAim();
        DrawUI();
    }

    void DrawBlocks() {
        for (int i = 0; i < BlockGrid::ROWS; i++) {
            for (int j = 0; j < BlockGrid::COLUMNS; j++) {
                if (i % 2 == 1 && j == BlockGrid::COLUMNS - 1) continue;
                
                if (!blocks.blocks[i][j].isDestroyed()) {
                    drawSprite(blocks.sprites[i][j], blocks.blocks[i][j].getX(), 
                              blocks.blocks[i][j].getY());
                }
            }
        }
    }

    void DrawPlayer() {
        drawSprite(sPlayer, player.getX(), playerInitialY);
    }

    void DrawBall() {
        drawSprite(sBall, ball.getX(), ball.getY());
    }

    void DrawAim() {
        drawSprite(sAim, aim.getX(), aim.getY());
    }

    void DrawUI() {
        if (ball.checkCombo()) {
            drawSprite(sCombo, _width * 0.8f, 0);
        }

        if (state.ability == AbilityState::EXTRA_LIFE) {
            drawSprite(sExtraLife, _width / 10.0f, 0);
        }
        else if (state.ability == AbilityState::SHRINK_PADDLE) {
            drawSprite(sShrink, _width / 10.0f, _height * UI_SCALE);
        }

        if (player.getPoints() >= ABILITY_COST) {
            drawSprite(sAbilityAvailable, _width / 3.0f, 0);
        }
    }

    void UpdateGame() {
        if (!state.started) return;
        
        ball.updatePosition(ball.getXVelocity(), ball.getYVelocity());
        ball.checkBounds(_width, _height);
        CheckCollisions();
    }

    void CheckCollisions() {
        for (int i = 0; i < BlockGrid::ROWS; i++) {
            for (int j = 0; j < BlockGrid::COLUMNS; j++) {
                if (i % 2 == 1 && j == BlockGrid::COLUMNS - 1) continue;
                
                Block& block = blocks.blocks[i][j];
                if (!block.isDestroyed() && ball.blockCollision(block)) {
                    destroySprite(blocks.sprites[i][j]);
                    block.setDestroyed();
                    player.addDestroyedBlock();
                    ball.addToCombo();
                    player.addPoint(ball.checkCombo());
                }
            }
        }

        if (ball.playerCollision(player)) {
            ball.resetCombo();
        }
    }

    void CheckGameConditions() {
        state.lost = ball.checkLossCondition(_height);
        state.won = (player.getDestroyedBlocks() == blocks.totalBlocks);

        if (state.won && ball.isCaught(player)) {
            ball.setVelocity(0, 0);
            drawSprite(sVictory, 0, 0);
        }

        if (state.lost) {
            HandleGameLoss();
        }
    }

    void HandleGameLoss() {
        if (state.ability == AbilityState::EXTRA_LIFE) {
            ResetForNewLife();
        }
        else {
            drawSprite(sGameOver, 0, 0);
        }
        state.ability = AbilityState::NONE;
        state.started = false;
    }

    void ResetForNewLife() {
        player.setWidthHeight(PLAYER_WIDTH, PLAYER_HEIGHT);
        setSpriteSize(sPlayer, PLAYER_WIDTH, PLAYER_HEIGHT);
        player.setCoordinates(playerInitialX, playerInitialY);
        ball.setCoordinates(ballInitialX, ballInitialY);
        ball.setVelocity(0, 0);
        ball.resetCombo();
    }

    void HandleLeftClick() {
        if (!state.started) {
            Vector2d direction(aim.getX() - ball.getX(), aim.getY() - ball.getY());
            direction.normalizeVector();
            ball.setVelocity(direction.getX(), direction.getY());
            state.started = true;
        }
    }

    void HandleRightClick() {
        if (state.lost || state.won) {
            Init();
            return;
        }

        if (player.getPoints() >= ABILITY_COST) {
            ActivateRandomAbility();
        }
    }

    void ActivateRandomAbility() {
        srand(static_cast<unsigned>(time(nullptr)));
        bool isPositiveAbility = (rand() % 10) < 5;

        if (isPositiveAbility) {
            state.ability = AbilityState::EXTRA_LIFE;
        }
        else {
            state.ability = AbilityState::SHRINK_PADDLE;
            player.setWidthHeight(player.getWidth() / 2, player.getHeight());
            setSpriteSize(sPlayer, player.getWidth(), player.getHeight());
        }
        player.substractPoints(ABILITY_COST);
    }

    void HandleKeyPress(FRKey key, bool isReleased) {
        if (!state.started) return;
        
        float velocity = isReleased ? 0.0f : player.getVelocity();
        
        switch (key) {
            case FRKey::LEFT:
                player.updatePosition(-velocity);
                if (!state.started) ball.updatePosition(-velocity, 0);
                break;
            case FRKey::RIGHT:
                player.updatePosition(velocity);
                if (!state.started) ball.updatePosition(velocity, 0);
                break;
            default:
                break;
        }
    }
};

int main(int argc, char* argv[]) {
    int width = 900;
    int height = 600;
    bool fullscreen = false;

    if (argc >= 4) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        fullscreen = atoi(argv[3]) != 0;
    }

    MyFramework framework(width, height, fullscreen);
    return run(&framework);
}
