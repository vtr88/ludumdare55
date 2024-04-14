#include "raylib.h"
#include "screens.h"

static int framesCounter = 0;
static int finishScreen = 0;
int win = 0;
Texture2D over;

void InitEndingScreen(void) {
    framesCounter = 0;
    finishScreen = 0;
    if(win==0) {
        UnloadTexture(over);
        over = LoadTexture("./resources/over.png");
    } else {
        UnloadTexture(over);
        over = LoadTexture("./resources/win.png");
    }
}

void UpdateEndingScreen(void) {
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

void DrawEndingScreen(void) {
    DrawTextureEx(over, (Vector2){ 0, 0 }, 0.0f, (float)GetScreenWidth()/over.width, WHITE);
}

void UnloadEndingScreen(void) {
    //UnloadTexture(over); 
}

int FinishEndingScreen(void) {
    return finishScreen;
}
