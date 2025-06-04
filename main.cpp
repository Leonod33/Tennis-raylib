#include "raylib.h"
#include <string>

enum SwingType { LIGHT, NORMAL, POWER };

const int screenWidth = 1600;    // Larger window for 4K displays
const int screenHeight = 900;

struct Paddle {
    Rectangle rect;
    float speed;
};

struct Ball {
    Vector2 pos;
    Vector2 vel;
    float radius;
    float height; // simulated vertical height for scaling
};

// Draws a simple stickman with a racket. "facingRight" determines racket side.
void DrawStickman(const Paddle& p, Color color, bool facingRight)
{
    // Body
    DrawRectangleRec(p.rect, color);
    // Head
    DrawCircle(p.rect.x + p.rect.width / 2, p.rect.y - 10, p.rect.width / 2, color);
    // Racket
    Vector2 grip = { p.rect.x + (facingRight ? p.rect.width + 4 : -4), p.rect.y + p.rect.height / 2 };
    Vector2 tip = { grip.x + (facingRight ? 20 : -20), grip.y };
    DrawLineEx(grip, tip, 4, RAYWHITE);
    DrawCircleV(tip, 6, RAYWHITE);
}

void ResetBall(Ball& ball, int direction)
{
    ball.pos = { screenWidth / 2.0f, screenHeight / 2.0f };
    ball.vel = { (float)(direction * (250 + GetRandomValue(0, 150))) / 60.0f, (float)GetRandomValue(-180,180) / 60.0f };
    ball.height = 0.0f;
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Raylib Tennis - Spicy Edition!");
    SetTargetFPS(60);

    // Paddles
    Paddle player = { { 20, screenHeight / 2 - 60, 20, 120 }, 8.0f };
    Paddle ai = { { screenWidth - 40, screenHeight / 2 - 60, 20, 120 }, 7.0f };

    // Ball
    Ball ball;
    ball.radius = 16;
    ball.height = 0.0f;
    ResetBall(ball, (GetRandomValue(0, 1) * 2 - 1));

    // Scoring & umpire
    int playerScore = 0, aiScore = 0;
    std::string umpireMsg = "Let's play!";

    while (!WindowShouldClose())
    {
        // Player paddle movement
        if (IsKeyDown(KEY_W) && player.rect.y > 0) player.rect.y -= player.speed;
        if (IsKeyDown(KEY_S) && player.rect.y < screenHeight - player.rect.height) player.rect.y += player.speed;

        // AI paddle (simple chase)
        if (ball.pos.y < ai.rect.y + ai.rect.height / 2 && ai.rect.y > 0) ai.rect.y -= ai.speed;
        if (ball.pos.y > ai.rect.y + ai.rect.height / 2 && ai.rect.y < screenHeight - ai.rect.height) ai.rect.y += ai.speed;

        // Ball movement
        ball.pos.x += ball.vel.x;
        ball.pos.y += ball.vel.y;
        if (ball.height > 0) ball.height -= 0.5f;
        if (ball.height < 0) ball.height = 0;

        // Top/bottom collision
        if (ball.pos.y < ball.radius || ball.pos.y > screenHeight - ball.radius) ball.vel.y *= -1;

        // Paddle collision (player)
        if (CheckCollisionCircleRec(ball.pos, ball.radius, player.rect) && ball.vel.x < 0)
        {
            // SWING: A=light, S=normal, D=power
            SwingType swing = NORMAL;
            if (IsKeyDown(KEY_A)) swing = LIGHT;
            if (IsKeyDown(KEY_D)) swing = POWER;

            float speedMod = (swing == LIGHT ? 0.7f : swing == POWER ? 1.4f : 1.0f);
            ball.vel.x = -ball.vel.x * speedMod;
            ball.vel.y += (GetRandomValue(-30, 30) / 100.0f) * speedMod;
            ball.height = (swing == LIGHT ? 5.0f : swing == POWER ? 20.0f : 10.0f);
            umpireMsg = (swing == LIGHT) ? "Soft return!" : (swing == POWER) ? "Big smash!" : "Solid hit!";
        }

        // Paddle collision (AI)
        if (CheckCollisionCircleRec(ball.pos, ball.radius, ai.rect) && ball.vel.x > 0)
        {
            float aiSwing = GetRandomValue(7, 13) / 10.0f; // Simulate AI swing power
            ball.vel.x = -ball.vel.x * aiSwing;
            ball.vel.y += (GetRandomValue(-20, 20) / 100.0f) * aiSwing;
            ball.height = (aiSwing > 1.1f) ? 15.0f : 8.0f;
            umpireMsg = (aiSwing > 1.1f) ? "Ace return!" : "Steady rally!";
        }

        // Point scored
        if (ball.pos.x < 0)
        {
            aiScore++;
            const char* calls[] = {
                "OUT!",
                "FAULT!",
                "Too wide!",
                "Point to AI!",
                "Were you aiming for the parking lot?"
            };
            umpireMsg = calls[GetRandomValue(0, 4)];
            ResetBall(ball, 1);
        }
        if (ball.pos.x > screenWidth)
        {
            playerScore++;
            const char* calls[] = {
                "ACE!",
                "Nice shot!",
                "Point to Player!",
                "Unreturnable!",
                "Did you bribe the umpire?"
            };
            umpireMsg = calls[GetRandomValue(0, 4)];
            ResetBall(ball, -1);
        }

        // DRAW
        BeginDrawing();
        ClearBackground(DARKGREEN);
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, LIGHTGRAY);
        DrawStickman(player, SKYBLUE, true);
        DrawStickman(ai, ORANGE, false);
        DrawCircle(ball.pos.x, ball.pos.y - ball.height, ball.radius + ball.height/3, YELLOW);

        // Scores and umpire
        DrawText(TextFormat("Player: %d", playerScore), screenWidth / 4 - 60, 20, 30, WHITE);
        DrawText(TextFormat("AI: %d", aiScore), 3 * screenWidth / 4 - 30, 20, 30, WHITE);
        DrawText(umpireMsg.c_str(), screenWidth / 2 - MeasureText(umpireMsg.c_str(), 28) / 2, 60, 28, MAROON);

        DrawText("W/S = Move | A = Light | S = Normal | D = Power | ESC to Quit", 70, screenHeight - 40, 20, WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
