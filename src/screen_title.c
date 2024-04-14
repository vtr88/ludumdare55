#include "raylib.h"
#include "screens.h"

static int framesCounter = 0;
static int finishScreen = 0;
Texture2D backgroundTitle, arrow;
Rectangle clipping;

void InitTitleScreen(void) {
    framesCounter = 0;
    finishScreen = 0;
    arrow = LoadTexture("/home/soth/Documentos/c/ludumdare53/src/resources/arrow.png");
    if(backgroundTitle.id == 0) {
        backgroundTitle = LoadTexture("/home/soth/Documentos/c/ludumdare53/src/resources/farm.png");
        Image image = LoadImageFromTexture(backgroundTitle);
        ImageColorGrayscale(&image);
        UnloadTexture(backgroundTitle);
        backgroundTitle = LoadTextureFromImage(image);
        UnloadImage(image);
    }
}

void UpdateTitleScreen(void) {
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        finishScreen = 2;
        PlaySound(fxCoin);
    }
}

void DrawTitleScreen(void) {
    framesCounter++;
    DrawTextureEx(backgroundTitle, (Vector2){ 0, 0 }, 0.0f, (800.0/backgroundTitle.width + 480.0 / backgroundTitle.height)/2, WHITE);

    if(framesCounter<4) clipping = (Rectangle){0, 0, 24, 24};
    else if(framesCounter>4 && framesCounter<8) clipping = (Rectangle){24,0,24,24};
    else if(framesCounter>8 && framesCounter<12) clipping = (Rectangle){48,0,24,24};
    else if(framesCounter>12 && framesCounter<=16) {
        clipping = (Rectangle){64,0,24,24};
        framesCounter = 0;
    }
        
    DrawTextureRec(arrow, clipping, (Vector2){((GetScreenWidth()/2.0)-80.0), GetScreenHeight()/2.0}, WHITE);
    DrawText("Começar jogo.", (GetScreenWidth()/2)-50, GetScreenHeight()/2, 25, BLACK);
}

void UnloadTitleScreen(void) {
    //UnloadTexture(backgroundTitle);
    //UnloadTexture(arrow);
}

int FinishTitleScreen(void) {
    return finishScreen;
}
