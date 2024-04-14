#include "raylib.h"
#include "screens.h"

GameScreen currentScreen = LOGO;
Font font = { 0 };
Music music = { 0 }, gameplaym = {0};
Sound fxCoin = { 0 };

// Local Variables Definition (local to this module)
static const int screenWidth = 800;
static const int screenHeight = 450;

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static GameScreen transToScreen = UNKNOWN;

// Local Functions Declaration
static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)
static void UpdateDrawFrame(void);          // Update and draw one frame

int main(void)
{
    InitWindow(screenWidth, screenHeight, "frenku e as mizads.");
    //ToggleFullscreen();
    InitAudioDevice();
    font = LoadFont("./resources/mecha.png");
    music = LoadMusicStream("./resources/ambient.ogg");
    gameplaym = LoadMusicStream("./resources/output.ogg");
    fxCoin = LoadSound("./resources/coin.wav");
    
    SetMusicVolume(music, 0.9f);
    SetMusicVolume(gameplaym, 0.3f);
    PlayMusicStream(music);
    currentScreen = LOGO;
    InitLogoScreen();
    SetTargetFPS(60);

    Image icon = LoadImage("./resources/icon16.png"); 
    SetWindowIcon(icon);
    
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
    
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        default: break;
    }
    UnloadFont(font);
    UnloadMusicStream(music);
    UnloadMusicStream(gameplaym);
    UnloadSound(fxCoin);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

static void TransitionToScreen(GameScreen screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

static void UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;
            switch (transFromScreen)
            {
                case LOGO: UnloadLogoScreen(); break;
                case TITLE: UnloadTitleScreen(); break;
                case GAMEPLAY: UnloadGameplayScreen(); break;
                case ENDING: UnloadEndingScreen(); break;
                default: break;
            }
            switch (transToScreen)
            {
                case LOGO: InitLogoScreen(); break;
                case TITLE: InitTitleScreen(); break;
                case GAMEPLAY: InitGameplayScreen(); break;
                case ENDING: InitEndingScreen(); break;
                default: break;
            }
            currentScreen = transToScreen;
            transFadeOut = true;
        }
    }
    else
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = UNKNOWN;
        }
    }
}

static void DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

static void UpdateDrawFrame(void)
{
    UpdateMusicStream(music);
    UpdateMusicStream(gameplaym);

    if (!onTransition)
    {
        switch(currentScreen)
        {
            case LOGO:
            {
                UpdateLogoScreen();
                if (FinishLogoScreen()) TransitionToScreen(TITLE);
            } break;
            case TITLE:
            {
                UpdateTitleScreen();
                if (FinishTitleScreen() == 2) TransitionToScreen(GAMEPLAY);
            } break;
            case GAMEPLAY:
            {
                UpdateGameplayScreen();
                if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
                //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);
            } break;
            case ENDING:
            {
                UpdateEndingScreen();

                if (FinishEndingScreen() == 1) TransitionToScreen(TITLE);

            } break;
            default: break;
        }
    }
    else UpdateTransition();
    
    BeginDrawing();

    if (IsKeyPressed(KEY_F11)) {
        ToggleFullscreen();
    }
    ClearBackground(RAYWHITE);

    switch(currentScreen)
    {
        case LOGO: DrawLogoScreen(); break;
        case TITLE: DrawTitleScreen(); break;
        case GAMEPLAY: 
                      DrawGameplayScreen(); 
                      StopMusicStream(music);
                      PlayMusicStream(gameplaym);
                      break;
        case ENDING: 
                      DrawEndingScreen(); 
                      StopMusicStream(gameplaym);
                      PlayMusicStream(music);
                      break;
        default: break;
    }

    if (onTransition) DrawTransition();
    
    EndDrawing();
}
