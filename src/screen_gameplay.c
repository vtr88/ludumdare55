#include "raylib.h"
#include "screens.h"

static int spriteWidth = 32, spriteHeight = 32, bumpCounter = 0, framesCounter = 0, finishScreen = 0, vida = 3, score = 0;
Texture2D grassTile, grassSheet, waterTile, character, bolinha, coracao, papai, mamae, pitaia, cdashing;
Sound fxStep, fxBark, fxOneup, fxMissed, fxDash, fxSummon;
Vector2 characterPosition = { 26, 0 }, bolinhaPos = {-32, -32};
Rectangle waterRect = {0}, characterRect = {0}, bolinhaRect = {0};
float characterSpeed = 200.0f; 
bool papaiIsOn = false, pitaiaIsOn = false, mamaeIsOn = false;
extern int win;

//Dash variables
bool isDashing = false;
int dashDuration = 30;
Vector2 dashDirection = {0, 0};
float dashSpeedMultiplier = 3.0f;
int dashCounter = 0;
int tempoBolinha = 350;

bool bolinhaNaPiscina(Vector2 point, Rectangle pool) {
    return point.x >= pool.x && point.x <= pool.x + pool.width &&
           point.y >= pool.y && point.y <= pool.y + pool.height;
}

Vector2 geraBolinha(int mapWidth, int mapHeight, Rectangle pool) {
    Vector2 position;
    do {
        position.x = GetRandomValue(0, mapWidth - 1);
        position.y = GetRandomValue(0, mapHeight - 1);
    }
    while (bolinhaNaPiscina(position, pool));
    return position;
}

void desenhoVida() {
    if (vida == 3) {
        DrawTexture(coracao, 1, 1, WHITE);
        DrawTexture(coracao, 1, 26, WHITE);
        DrawTexture(coracao, 1, 50, WHITE);
    } else if (vida == 2) {
        DrawTexture(coracao, 1, 1, WHITE);
        DrawTexture(coracao, 1, 26, WHITE);
    } else if (vida == 1) {
        DrawTexture(coracao, 1, 1, WHITE);
    } else {
        finishScreen = 1;
        win = 0;
    }
}

void InitGameplayScreen(void) {
    waterTile = LoadTexture("./resources/water.png");
    grassTile = LoadTexture("./resources/grass.png");
    grassSheet = LoadTexture("./resources/beira.png");
    bolinha = LoadTexture("./resources/bolinha.png");
    coracao = LoadTexture("./resources/heart.png");
    character = LoadTexture("./resources/fenku.png");
    cdashing = LoadTexture("./resources/frenkur.png");
    papai = LoadTexture("./resources/papai.png");
    mamae = LoadTexture("./resources/mamae.png");
    pitaia = LoadTexture("./resources/pitaia.png");
    fxStep = LoadSound("./resources/sound.wav");
    fxBark = LoadSound("./resources/bark.wav");
    fxOneup = LoadSound("./resources/oneup.wav");
    fxMissed = LoadSound("./resources/missed.wav");
    fxDash = LoadSound("./resources/dash.wav");
    fxSummon = LoadSound("./resources/summon.wav");
    framesCounter = 0;
    finishScreen = 0;
    win = 0;
    papaiIsOn = false;
    mamaeIsOn = false;
    pitaiaIsOn = false;
    vida = 3;
    score = 0;
    tempoBolinha = 350;
}

void UpdateGameplayScreen(void)
{
    Vector2 oldPosition = characterPosition;
    float deltaTime = GetFrameTime();
    
    if (IsKeyPressed(KEY_L) && !isDashing) {
        isDashing = true;
        dashDirection = (Vector2){0, 0};
        dashCounter = 0;
    }

    if(IsKeyPressed(KEY_F) && score>10) {
        papaiIsOn = true;
        PlaySound(fxSummon);
    }
    
    if(IsKeyPressed(KEY_F) && score>20) {
        mamaeIsOn = true;
        PlaySound(fxSummon);
    }
    
    if(IsKeyPressed(KEY_F) && score>29) {
        pitaiaIsOn = true;
        PlaySound(fxSummon);
    }
    
    if (isDashing) {
        if (dashCounter < dashDuration) {
            if (dashDirection.x == 0 && dashDirection.y == 0) {
                if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) dashDirection.x = 1;
                else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) dashDirection.x = -1;
                if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) dashDirection.y = -1;
                else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) dashDirection.y = 1;
            }
            if (dashDirection.x != 0 || dashDirection.y != 0) {
                PlaySound(fxDash);
                characterPosition.x += dashDirection.x * characterSpeed * dashSpeedMultiplier * deltaTime;
                characterPosition.y += dashDirection.y * characterSpeed * dashSpeedMultiplier * deltaTime;
            }
            dashCounter++;
        } else {
            isDashing = false;
        }
    } else {
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) characterPosition.x += characterSpeed * deltaTime;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) characterPosition.x -= characterSpeed * deltaTime;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) characterPosition.y -= characterSpeed * deltaTime;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) characterPosition.y += characterSpeed * deltaTime;
    }
    characterRect = (Rectangle){ characterPosition.x, characterPosition.y, character.width, character.height };
    
    if (CheckCollisionRecs(characterRect, waterRect)) {
        characterPosition = oldPosition;
        PlaySound(fxStep);
        bumpCounter += 1;
        if(bumpCounter==100) {
            PlaySound(fxBark);
            bumpCounter = 0;
        }
    }

    if (CheckCollisionRecs(characterRect, bolinhaRect)) {
        PlaySound(fxOneup);
        bolinhaPos.x = -32;
        bolinhaPos.y = -32;
        score++;
    }

    if (characterRect.x < 0) characterPosition.x = 0;
    if (characterRect.y < 0) characterPosition.y = 0;
    if (characterRect.x + characterRect.width > GetScreenWidth()) characterPosition.x = GetScreenWidth() - characterRect.width;
    if (characterRect.y + characterRect.height > GetScreenHeight()) characterPosition.y = GetScreenHeight() - characterRect.height;
    
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void) {
    framesCounter++;
    int waterWidth = GetScreenWidth() * 0.7; 
    int waterHeight = GetScreenHeight() * 0.5;
    int waterStartX = (GetScreenWidth() - waterWidth) / 2;
    int waterStartY = (GetScreenHeight() - waterHeight) / 2;
    ClearBackground(RAYWHITE);

    for (int y = 0; y < GetScreenHeight(); y += grassTile.height) {
        for (int x = 0; x < GetScreenWidth(); x += grassTile.width) {
            DrawTexture(grassTile, x, y, WHITE);
        }
    }

    for (int y = waterStartY; y < waterStartY + waterHeight; y += waterTile.height) {
        for (int x = waterStartX; x < waterStartX + waterWidth; x += waterTile.width) {
            DrawTexture(waterTile, x, y, WHITE);
        }
        DrawTextureRec(grassSheet, (Rectangle){spriteWidth * 3, 0, spriteWidth, spriteHeight},
                       (Vector2){waterStartX - spriteWidth, y}, WHITE); // Left
        DrawTextureRec(grassSheet, (Rectangle){spriteWidth, 0, spriteWidth, spriteHeight},
                       (Vector2){waterStartX + waterWidth, y}, WHITE); // Right
    }

    for (int x = (waterStartX-32); x < waterStartX + waterWidth; x += waterTile.width) {
        DrawTextureRec(grassSheet, (Rectangle){0, 0, spriteWidth, spriteHeight},
                       (Vector2){x, waterStartY - spriteHeight}, WHITE); // Top
        DrawTextureRec(grassSheet, (Rectangle){spriteWidth * 2, 0, spriteWidth, spriteHeight},
                       (Vector2){x, waterStartY + waterHeight}, WHITE); // Bottom
    }

    waterRect = (Rectangle){ waterStartX, waterStartY, waterWidth, waterHeight };
    
    if(framesCounter>=tempoBolinha) {
        if(bolinhaPos.x != -32) {
            PlaySound(fxMissed);
            vida--;
        }
        bolinhaPos = geraBolinha(GetScreenWidth(), GetScreenHeight(), waterRect);
        tempoBolinha = tempoBolinha - 2; 
        framesCounter = 0;
    }
    bolinhaRect = (Rectangle) {bolinhaPos.x, bolinhaPos.y, bolinha.width, bolinha.height};
    DrawTexture(bolinha, bolinhaPos.x, bolinhaPos.y, WHITE);
    if(!isDashing) {
        DrawTexture(character, characterPosition.x, characterPosition.y, WHITE);
    } else {
        DrawTexture(cdashing, characterPosition.x, characterPosition.y, WHITE);
    }
    desenhoVida();
    DrawText(TextFormat("%d", score), (GetScreenWidth() - 35), 5, 30, BLACK);

    if(score == 0) {
        DrawText("A S D W PARA SE MOVER!", ((GetScreenWidth()/2)-115), GetScreenHeight()/2, 20, WHITE);
        DrawText("L PARA DAR UM SPRINT!", ((GetScreenWidth()/2)-115), (GetScreenHeight()/2)-30, 20, WHITE);
    }
    if(score>10 && score<=20) {
        if(!papaiIsOn) {
            DrawText("Pressione F para CONVOCAR!!!", ((GetScreenWidth()/2)-85), GetScreenHeight()/2, 20, WHITE);
        } else {
            DrawTexture(papai, GetScreenWidth()/3, GetScreenHeight()/2, WHITE);
        }
    }
    if(score>20 && score<=30) {
        if(!mamaeIsOn) {
            DrawText("Pressione F para CONVOCAR!!!", ((GetScreenWidth()/2)-85), GetScreenHeight()/2, 20, WHITE);
            DrawTexture(papai, GetScreenWidth()/3, GetScreenHeight()/2, WHITE);
        } else {
            DrawTexture(papai, GetScreenWidth()/3, GetScreenHeight()/2, WHITE);
            DrawTexture(mamae, GetScreenWidth()*2/3, GetScreenHeight()/2, WHITE);
        }
    }
    if(score>29) {
        if(!pitaiaIsOn) {
            DrawText("Pressione F para CONVOCAR!!!", ((GetScreenWidth()/2)-85), GetScreenHeight()/2, 20, WHITE);
            DrawTexture(papai, GetScreenWidth()/3, GetScreenHeight()/2, WHITE);
            DrawTexture(mamae, GetScreenWidth()*2/3, GetScreenHeight()/2, WHITE);
        } else {
            DrawTexture(papai, GetScreenWidth()/3, GetScreenHeight()/2, WHITE);
            DrawTexture(mamae, GetScreenWidth()*2/3, GetScreenHeight()/2, WHITE);
            DrawTexture(pitaia, GetScreenWidth()/2, GetScreenHeight()/2, WHITE);
        }
     if(score==31) {
         win = 1;
         finishScreen = 1; 
     }
    }
}

void UnloadGameplayScreen(void) {
    UnloadTexture(waterTile);
    UnloadTexture(grassTile);
    UnloadTexture(grassSheet);
    UnloadTexture(bolinha);
    UnloadTexture(character);
    UnloadTexture(cdashing);
    UnloadTexture(papai);
    UnloadTexture(mamae);
    UnloadTexture(pitaia);
    UnloadSound(fxStep);
    UnloadSound(fxBark);
    UnloadSound(fxOneup);
    UnloadSound(fxMissed);
    UnloadSound(fxDash);
    UnloadSound(fxSummon);
}

int FinishGameplayScreen(void) {
    return finishScreen;
}
