#include "raylib.h"
#include "screens.h"

static int framesCounter = 0;
static int finishScreen = 0;
static int logoPositionX = 0;
static int logoPositionY = 0;
static int lettersCount = 0;
static int topSideRecWidth = 0;
static int leftSideRecHeight = 0;
static int bottomSideRecWidth = 0;
static int rightSideRecHeight = 0;
static int state = 0; 
static float alpha = 1.0f;
Texture2D backgroundImage;

void InitLogoScreen(void) {
    finishScreen = 0;
    framesCounter = 0;
    lettersCount = 0;
    logoPositionX = GetScreenWidth()/2 - 128;
    logoPositionY = GetScreenHeight()/2 - 128;
    topSideRecWidth = 16;
    leftSideRecHeight = 16;
    bottomSideRecWidth = 16;
    rightSideRecHeight = 16;
    state = 0;
    alpha = 1.0f;
}

void UpdateLogoScreen(void) {
    if (state == 0) {
        framesCounter++;
        if (framesCounter == 80) {
            state = 1;
            framesCounter = 0;
        }
    }
    else if (state == 1) {
        topSideRecWidth += 8;
        leftSideRecHeight += 8;
        if (topSideRecWidth == 256) state = 2;
    }
    else if (state == 2) {
        bottomSideRecWidth += 8;
        rightSideRecHeight += 8;
        if (bottomSideRecWidth == 256) state = 3;
    }
    else if (state == 3) {
        framesCounter++;

        if (lettersCount < 10) {
            if (framesCounter/12) {
                lettersCount++;
                framesCounter = 0;
            }
        }
        else {
            if (framesCounter > 200) {
                alpha -= 0.02f;
                if (alpha <= 0.0f) {
                    alpha = 0.0f;
                    finishScreen = 1;
                }
            }
        }
    }
}

void DrawLogoScreen(void) {
    if(backgroundImage.id == 0) {
        backgroundImage = LoadTexture("./resources/farm.png");
    }
    
    DrawTextureEx(backgroundImage, (Vector2){ 0, 0 }, 0.0f, (800.0/backgroundImage.width + 480.0 / backgroundImage.height)/2, WHITE);

    if (state == 0) {
        if ((framesCounter/10)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
    }
    else if (state == 1) {
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
        DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
    }
    else if (state == 2) {
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
        DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);

        DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
        DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
    }
    else if (state == 3) {
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
        DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));
        DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
        DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));
        DrawRectangle(GetScreenWidth()/2 - 112, GetScreenHeight()/2 - 112, 224, 224, Fade(GREEN, 0.3f));
        DrawText(TextSubtext("vt......", 0, lettersCount), GetScreenWidth()/2 - 44, GetScreenHeight()/2 + 48, 50, Fade(BLACK, alpha));
        if (framesCounter > 20) DrawText("um jogo de", logoPositionX, logoPositionY - 27, 20, Fade(BLACK, alpha));
    }
}

void UnloadLogoScreen(void)
{
}

int FinishLogoScreen(void)
{
    return finishScreen;
}
