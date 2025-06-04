#include "raylib.h"
#include <string>

enum SwingType { LIGHT, NORMAL, POWER, LOB };

enum GameState { TITLE, PLAYING, GAME_OVER };

const int screenWidth = 1600;    // Larger window for 4K displays
const int screenHeight = 900;

const float courtLeft = 80;
const float courtRight = screenWidth - 80;
const float courtTop = 50;
const float courtBottom = screenHeight - 50;

struct Paddle {
    Rectangle rect;
    float speed;
    float minX;
    float maxX;
};

struct Ball {
    Vector2 pos;
    Vector2 vel;
    float radius;
    float height; // simulated vertical height for scaling
    float velZ;
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
    ball.velZ = 0.0f;
}

void ScorePoint(int scorer, int& playerScore, int& aiScore, std::string& msg, Ball& ball)
{
    if (scorer == 1) playerScore++; else aiScore++;
    const char* playerCalls[] = {
        "ACE!", "Nice shot!", "Point to Player!", "Unreturnable!", "Did you bribe the umpire?" };
    const char* aiCalls[] = {
        "OUT!", "FAULT!", "Too wide!", "Point to AI!", "Were you aiming for the parking lot?" };
    msg = scorer == 1 ? playerCalls[GetRandomValue(0,4)] : aiCalls[GetRandomValue(0,4)];
    ResetBall(ball, scorer==1 ? -1 : 1);
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Raylib Tennis - Spicy Edition!");
    SetTargetFPS(60);

    // Paddles
    Paddle player = { { 60, screenHeight / 2 - 60, 20, 120 }, 8.0f, 20, courtRight - 200 };
    Paddle ai = { { screenWidth - 80, screenHeight / 2 - 60, 20, 120 }, 7.0f, courtLeft + 180, screenWidth - 40 };

    // Ball
    Ball ball;
    ball.radius = 16;
    ball.height = 0.0f;
    ResetBall(ball, (GetRandomValue(0, 1) * 2 - 1));

    // Scoring & umpire
    int playerScore = 0, aiScore = 0;
    std::string umpireMsg = "Press SPACE to serve";
    GameState state = TITLE;
    std::string winner;
    int lastHitter = 0; // 1=player,2=AI

    while (!WindowShouldClose())
    {
        if (state == TITLE)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                state = PLAYING;
                playerScore = aiScore = 0;
                winner.clear();
                umpireMsg = "Let's play!";
                player.rect.x = 60; player.rect.y = screenHeight / 2 - 60;
                ai.rect.x = screenWidth - 80; ai.rect.y = screenHeight / 2 - 60;
                lastHitter = 0;
                ResetBall(ball, (GetRandomValue(0,1)*2-1));
            }

            BeginDrawing();
            ClearBackground(DARKGREEN);
            DrawRectangleLines(courtLeft, courtTop, courtRight-courtLeft, courtBottom-courtTop, RAYWHITE);
            DrawText("Raylib Tennis - Spicy Edition", screenWidth/2 - MeasureText("Raylib Tennis - Spicy Edition", 40)/2, screenHeight/2 - 60, 40, WHITE);
            DrawText("Press SPACE to start", screenWidth/2 - MeasureText("Press SPACE to start", 30)/2, screenHeight/2, 30, YELLOW);
            EndDrawing();
            continue;
        }

        if (state == GAME_OVER)
        {
            if (IsKeyPressed(KEY_R))
            {
                state = TITLE;
                umpireMsg = "Press SPACE to serve";
            }
            BeginDrawing();
            ClearBackground(DARKGREEN);
            DrawRectangleLines(courtLeft, courtTop, courtRight-courtLeft, courtBottom-courtTop, RAYWHITE);
            DrawText(TextFormat("%s Wins!", winner.c_str()), screenWidth/2 - MeasureText("Player Wins!", 40)/2, screenHeight/2 - 60, 40, WHITE);
            DrawText("Press R to restart or ESC to quit", screenWidth/2 - MeasureText("Press R to restart or ESC to quit", 30)/2, screenHeight/2, 30, YELLOW);
            EndDrawing();
            continue;
        }

        // PLAYING STATE ---------------------
        // Player movement
        if (IsKeyDown(KEY_W) && player.rect.y > courtTop) player.rect.y -= player.speed;
        if (IsKeyDown(KEY_S) && player.rect.y < courtBottom - player.rect.height) player.rect.y += player.speed;
        if (IsKeyDown(KEY_A) && player.rect.x > player.minX) player.rect.x -= player.speed;
        if (IsKeyDown(KEY_D) && player.rect.x < player.maxX) player.rect.x += player.speed;

        // AI paddle movement
        if (ball.pos.y < ai.rect.y + ai.rect.height / 2 && ai.rect.y > courtTop) ai.rect.y -= ai.speed;
        if (ball.pos.y > ai.rect.y + ai.rect.height / 2 && ai.rect.y < courtBottom - ai.rect.height) ai.rect.y += ai.speed;
        if (ball.pos.x > screenWidth/2)
        {
            if (ball.pos.x < ai.rect.x && ai.rect.x > ai.minX) ai.rect.x -= ai.speed;
            if (ball.pos.x > ai.rect.x && ai.rect.x < ai.maxX) ai.rect.x += ai.speed;
        }
        else if (ai.rect.x < ai.maxX) ai.rect.x += ai.speed*0.5f;

        // Ball movement
        ball.pos.x += ball.vel.x;
        ball.pos.y += ball.vel.y;
        ball.velZ -= 0.4f;
        ball.height += ball.velZ;
        if (ball.height < 0) { ball.height = 0; ball.velZ = -ball.velZ * 0.6f; if (fabs(ball.velZ) < 1) ball.velZ = 0; }

        // Top/bottom collision
        if (ball.pos.y < courtTop + ball.radius || ball.pos.y > courtBottom - ball.radius) ball.vel.y *= -1;

        // Paddle collision (player)
        if (CheckCollisionCircleRec(ball.pos, ball.radius, player.rect) && ball.vel.x < 0)
        {
            SwingType swing = NORMAL;
            if (IsKeyDown(KEY_J)) swing = LIGHT;
            else if (IsKeyDown(KEY_L)) swing = POWER;
            else if (IsKeyDown(KEY_I)) swing = LOB;
            else if (IsKeyDown(KEY_K)) swing = NORMAL;

            float speedMod = 1.0f;
            float zVel = 12.0f;
            switch(swing)
            {
                case LIGHT: speedMod = 0.8f; zVel = 6.0f; umpireMsg = "Soft return!"; break;
                case POWER: speedMod = 1.4f; zVel = 16.0f; umpireMsg = "Big smash!"; break;
                case LOB:   speedMod = 0.7f; zVel = 26.0f; umpireMsg = "High lob!"; break;
                default:    speedMod = 1.0f; zVel = 12.0f; umpireMsg = "Solid hit!"; break;
            }
            ball.vel.x = -ball.vel.x * speedMod;
            ball.vel.y += (GetRandomValue(-30,30)/100.0f)*speedMod;
            ball.velZ = zVel;
            lastHitter = 1;
        }

        // Paddle collision (AI)
        if (CheckCollisionCircleRec(ball.pos, ball.radius, ai.rect) && ball.vel.x > 0)
        {
            float aiSwing = GetRandomValue(7, 13) / 10.0f; // Simulate AI swing power
            ball.vel.x = -ball.vel.x * aiSwing;
            ball.vel.y += (GetRandomValue(-20, 20) / 100.0f) * aiSwing;
            ball.velZ = (aiSwing > 1.1f) ? 15.0f : 8.0f;
            umpireMsg = (aiSwing > 1.1f) ? "Ace return!" : "Steady rally!";
            lastHitter = 2;
        }

        // Point scored or out of bounds
        if (ball.pos.x < 0) { ScorePoint(2, playerScore, aiScore, umpireMsg, ball); lastHitter = 0; }
        if (ball.pos.x > screenWidth) { ScorePoint(1, playerScore, aiScore, umpireMsg, ball); lastHitter = 0; }

        if (ball.height == 0 && ball.velZ == 0 &&
            (ball.pos.x < courtLeft || ball.pos.x > courtRight || ball.pos.y < courtTop || ball.pos.y > courtBottom))
        {
            int scorer = (lastHitter == 1) ? 2 : 1;
            ScorePoint(scorer, playerScore, aiScore, umpireMsg, ball);
            lastHitter = 0;
        }

        if (playerScore >= 5) { state = GAME_OVER; winner = "Player"; }
        if (aiScore >= 5)    { state = GAME_OVER; winner = "AI"; }

        // DRAW
        BeginDrawing();
        ClearBackground(DARKGREEN);
        DrawRectangleLines(courtLeft, courtTop, courtRight-courtLeft, courtBottom-courtTop, RAYWHITE);
        DrawLine(screenWidth/2, courtTop, screenWidth/2, courtBottom, LIGHTGRAY);
        DrawLine(courtLeft, screenHeight/2, courtRight, screenHeight/2, LIGHTGRAY);
        DrawStickman(player, SKYBLUE, true);
        DrawStickman(ai, ORANGE, false);
        DrawCircle(ball.pos.x, ball.pos.y - ball.height, ball.radius + ball.height/3, YELLOW);

        // Scores and umpire
        DrawText(TextFormat("Player: %d", playerScore), screenWidth/4 - 60, 20, 30, WHITE);
        DrawText(TextFormat("AI: %d", aiScore), 3*screenWidth/4 - 30, 20, 30, WHITE);
        DrawText(umpireMsg.c_str(), screenWidth/2 - MeasureText(umpireMsg.c_str(),28)/2, 60, 28, MAROON);

        DrawText("WASD = Move | J = Light | K = Normal | L = Power | I = Lob | ESC Quit", 70, screenHeight-40, 20, WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
