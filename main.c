#include <raylib.h>
#include <stdlib.h>

typedef struct Paddle {
    Rectangle rect;
    float speed;
} Paddle;

typedef struct Ball {
    int radius;
    Vector2 position;
    Vector2 speed;
} Ball;

typedef enum State {
    MENU,
    PLAYING,
    PAUSED,
    ROUND_OVER,
    GAME_OVER,
} State;

typedef struct Game {
    int leftPaddleScore;
    int rightPaddleScore;
    Paddle *leftPaddle;
    Paddle *rightPaddle;
    Ball *ball;
    State state;
} Game;

const int screenWidth = 800;
const int screenHeight = 450;
const int winningScore = 2;
const Color textColor = {38, 185, 154, 255};

void resetBall(Ball *ball) {
    ball->position = (Vector2) {
        .x = screenWidth / 2,
        .y = screenHeight / 2,
    };
    float direction = GetRandomValue(0, 1) ? 1 : -1;
    ball->speed = (Vector2) {
        .x = 300 * direction,
        .y = (float) GetRandomValue(-300, 300)
    };
}

Game *initGame() {
    float paddleHeight = 100;
    Paddle *leftPaddle = (Paddle *) malloc(sizeof(Paddle));
    leftPaddle->rect = (Rectangle) {
        .x = 20,
        .y = (screenHeight / 2.f) - (paddleHeight / 2.f),
        .width = 20,
        .height = paddleHeight,
    };
    leftPaddle->speed = 300;

    Paddle *rightPaddle = (Paddle *) malloc(sizeof(Paddle));
    rightPaddle->rect = (Rectangle) {
        .x = screenWidth - 40,
        .y = (screenHeight / 2.f) - (paddleHeight / 2.f),
        .width = 20,
        .height = paddleHeight,
    };
    rightPaddle->speed = 300;

    Ball *ball = (Ball *) malloc(sizeof(Ball));
    ball->radius = 10;
    resetBall(ball);

    Game *game = (Game *) malloc(sizeof(Game));
    game->leftPaddleScore = 0;
    game->rightPaddleScore = 0;
    game->leftPaddle = leftPaddle;
    game->rightPaddle = rightPaddle;
    game->ball = ball;
    game->state = MENU;
    return game;
}

void deleteGame(Game *game) {
    free(game->leftPaddle);
    free(game->rightPaddle);
    free(game->ball);
    free(game);
}

void handleMenuInput(Game *game, float deltaTime) {
    if (IsKeyPressed(KEY_ENTER)) {
        game->state = PLAYING;
    }
    if (IsKeyReleased(KEY_ENTER)) {
    }
}

static bool canPause = true;

void handlePlayingInput(Game *game, float deltaTime) {
//    if (IsKeyReleased(KEY_P)) {
//        enterPressed = false;
//    }
    if (IsKeyDown(KEY_P) && game->state == PLAYING && canPause) {
        canPause = false;
        game->state = PAUSED;
        return;
    }
    if (IsKeyDown(KEY_W)) {
        game->leftPaddle->rect.y -= game->leftPaddle->speed * deltaTime;
    }
    if (IsKeyDown(KEY_S)) {
        game->leftPaddle->rect.y += game->leftPaddle->speed * deltaTime;
    }
    if (IsKeyDown(KEY_UP)) {
        game->rightPaddle->rect.y -= game->rightPaddle->speed * deltaTime;
    }
    if (IsKeyDown(KEY_DOWN)) {
        game->rightPaddle->rect.y += game->rightPaddle->speed * deltaTime;
    }
}

void handlePausedInput(Game *game, float deltaTime) {
    if (IsKeyPressed(KEY_P) && game->state == PAUSED) {
        game->state = PLAYING;
    }
}

void handleGameOverInput(Game *game, float deltaTime) {
    if (IsKeyDown(KEY_ENTER) && game->state == ROUND_OVER) {
        game->state = PLAYING;
    }
    if (IsKeyDown(KEY_R)) {
        game->leftPaddleScore = 0;
        game->rightPaddleScore = 0;
        resetBall(game->ball);
        game->state = PLAYING;
    }
}

void handleInput(Game *game, float deltaTime) {
    switch (game->state) {
        case MENU:
            handleMenuInput(game, deltaTime);
            break;
        case PLAYING:
            handlePlayingInput(game, deltaTime);
            break;
        case PAUSED:
            handlePausedInput(game, deltaTime);
            break;
        case GAME_OVER:
        case ROUND_OVER:
            handleGameOverInput(game, deltaTime);
            break;
    }
}

void drawGame(Game *game) {
    // scores
    DrawText(TextFormat("%i", game->leftPaddleScore), screenWidth * 0.25f, 30, 60, textColor);
    DrawText(TextFormat("%i", game->rightPaddleScore), screenWidth * 0.75f, 30, 60, textColor);

    // Stage
    //  - Walls
    Color wallsColor = {.r = 38, .g = 185, .b = 154, .a = 255 / 3};
    DrawRectangle(0, screenHeight - 10, screenWidth, 10, wallsColor);
    DrawRectangle(0, 0, screenWidth, 10, wallsColor);
    //  - Middle line
    Color lineColor = {.r = 38, .g = 185, .b = 154, .a = 255 / 3};
    DrawRectangle(screenWidth / 2, 0, 2, screenHeight, lineColor);

    //  - Middle circle
    DrawCircle(screenWidth / 2, screenHeight / 2, 100, lineColor);

    //  - Players and ball
    Color playerColor = {.r = 112, .g = 152, .b = 255, .a = 255};
    DrawRectangleRounded(game->leftPaddle->rect, 10.f, 2, playerColor);
    DrawRectangleRounded(game->rightPaddle->rect, 10.f, 2, playerColor);
    DrawCircleV(game->ball->position, game->ball->radius, playerColor);
}

void drawGameOver(Game *game) {
    if (game->state == ROUND_OVER || game->state == GAME_OVER) {
        const int fontSize = 20;
        const char *roundOverText = "Round Over";
        const int roundOverTextWidth = MeasureText(roundOverText, fontSize);
        const char *resetText = "Press R to reset";
        const int resetTextWidth = MeasureText(resetText, fontSize);
        const char *startText = "Press ENTER to start";
        const int startTextWidth = MeasureText(startText, fontSize);
        const char *winnerText = game->leftPaddleScore > game->rightPaddleScore ? "Left wins!" : "Right wins!";
        const int winnerTextWidth = MeasureText(winnerText, fontSize);
        const char *bottomText = game->state == GAME_OVER ? winnerText : startText;

        const int textY = screenHeight * 0.75f;
        const int textX = (screenWidth - startTextWidth) / 2;

        DrawText(roundOverText, textX, textY, fontSize, textColor);
        DrawText(resetText, textX, textY + 20, fontSize, textColor);
        DrawText(bottomText, textX, textY + 40, fontSize, textColor);
    }
}

void drawPaused(Game *game) {
    if (game->state == PAUSED) {
        DrawText("Paused", 250, 200, 20, BLACK);
        DrawText("Press P to resume", 250, 220, 20, BLACK);
    }
}

void handleGameUpdate(Game *game, float deltaTime) {
    if (game->leftPaddle->rect.y <= 0) {
        game->leftPaddle->rect.y = 0;
    }
    if (game->leftPaddle->rect.y + game->leftPaddle->rect.height >= screenHeight) {
        game->leftPaddle->rect.y = screenHeight - game->leftPaddle->rect.height;
    }
    if (game->rightPaddle->rect.y <= 0) {
        game->rightPaddle->rect.y = 0;
    }
    if (game->rightPaddle->rect.y + game->rightPaddle->rect.height >= screenHeight) {
        game->rightPaddle->rect.y = screenHeight - game->rightPaddle->rect.height;
    }

    game->ball->position.x += game->ball->speed.x * deltaTime;
    game->ball->position.y += game->ball->speed.y * deltaTime;

    if (game->ball->position.y <= 0 || game->ball->position.y + game->ball->radius >= screenHeight) {
        game->ball->speed.y = -game->ball->speed.y;
    }

    bool collidedWithLeftPaddle = CheckCollisionCircleRec(
        game->ball->position,
        game->ball->radius,
        game->leftPaddle->rect
    );
    bool collidedWithRightPaddle = CheckCollisionCircleRec(
        game->ball->position,
        game->ball->radius,
        game->rightPaddle->rect
    );
    if (collidedWithLeftPaddle && game->ball->speed.x < 0) {
        game->ball->speed.x = -game->ball->speed.x * 1.1f;
    }

    if (collidedWithRightPaddle && game->ball->speed.x > 0) {
        game->ball->speed.x = -game->ball->speed.x * 1.1f;
    }

    if (game->ball->position.x <= 0) {
        game->leftPaddleScore++;
        resetBall(game->ball);
        game->state = game->leftPaddleScore >= winningScore ? GAME_OVER : ROUND_OVER;
    }
    if (game->ball->position.x + game->ball->radius >= screenWidth) {
        game->rightPaddleScore++;
        resetBall(game->ball);
        game->state = game->leftPaddleScore >= winningScore ? GAME_OVER : ROUND_OVER;
    }
}

void handleUpdate(Game *game, float deltaTime) {
    switch (game->state) {
        case MENU:
            break;
        case PLAYING:
            handleGameUpdate(game, deltaTime);
            break;
        case PAUSED:
            break;
        case ROUND_OVER:
            break;
    }
}

void drawMenu(Game *game) {
    if (game->state == MENU) {
        char *message = "Press ENTER to start";
        int fontSize = 20;
        int textWidth = MeasureText(message, fontSize);
        int textX = (screenWidth - textWidth) / 2;
        DrawText(message, textX, screenHeight * 0.75f, fontSize, textColor);
    }
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "Pong, The Game!");
    SetTargetFPS(120);               // Set our game to run at 60 frames-per-second

    Game *game = initGame();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float deltaTime = GetFrameTime();
        handleInput(game, deltaTime);
        handleUpdate(game, deltaTime);

        BeginDrawing();
        {
//            ClearBackground((Color) {38, 185, 154, 255});
            ClearBackground((Color) {239, 252, 234, 255});
            drawGame(game);
            drawMenu(game);
            drawPaused(game);
            drawGameOver(game);
        }
        EndDrawing();
    }

    CloseWindow();
    deleteGame(game);

    return 0;
}

