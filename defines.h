#ifndef DEFINES_H
#define DEFINES_H

#include <QtWidgets>

enum GameScreens    {
    Logo,
    Language,
    StartMenu,
    PauseMenu,
    DeadMenu,
    Game
};

    // engine consts
#define TILE_SIZE           25.0
#define GLOBAL_G            0.3
#define ON_GAME_PERIOD      15
#define ON_ANIMATION_PERIOD 150
#define COLLIDE_CORRECTOR   1

    // world size
#define SCREEN_TILE_WIDTH    64
#define SCREEN_TILE_HEIGHT   32

#define MOVE_STEP           2.0f
#define SHIP_V              0.1f
#define SHIP_V_MAX          4.0f
#define ROTATE_STEP         0.08f

#define SPAWN_TIMER_EASY    200
#define SPAWN_TIMER_MEDIUM  150
#define SPAWN_TIMER_HARD    100
#define SPAWN_TIMER_VERY    50
#define SPAWN_TIMER_SUICIDE 10

#define MAX_HP              100

    // buttons for controll
#define BUTTON_COUNT    13

#define BUTTON_UP       0
#define BUTTON_RIGHT    1
#define BUTTON_DOWN     2
#define BUTTON_LEFT     3
#define BUTTON_ACT1     4
#define BUTTON_ACT2     5
#define BUTTON_ACT3     6
#define BUTTON_ACT4     7
#define BUTTON_ACT5     8
#define BUTTON_ACT6     9
#define BUTTON_ACT7     10
#define BUTTON_ACT8     11
#define BUTTON_MENU     12

    // sensor touch buttons
static int TouchButtonGeometry[BUTTON_COUNT][6] = {

    /* Description    */ /*  X,   Y, W, H, Key   */ // geometry multyplies TILE_SIZE
    /* BUTTON_UP      */ {   6,  20, 6, 6, true, Qt::Key_W},
    /* BUTTON_RIGHT   */ {  12,  24, 6, 6, true, Qt::Key_D},
    /* BUTTON_DOWN    */ {   6,  26, 6, 6, true, Qt::Key_S},
    /* BUTTON_LEFT    */ {   0,  24, 6, 6, true, Qt::Key_A},
    /* BUTTON_ACT1    */ {  56,  24, 6, 6, true, Qt::Key_U},
    /* BUTTON_ACT2    */ {  56,  16, 6, 6, true, Qt::Key_J},
    /* BUTTON_ACT3    */ {  31,  12, 4, 4, false, Qt::Key_P},
    /* BUTTON_ACT4    */ {  35,  10, 4, 4, false, Qt::Key_I},
    /* BUTTON_ACT5    */ {   0,   0, 4, 4, false, Qt::Key_Q},
    /* BUTTON_ACT6    */ {   4,   0, 4, 4, false, Qt::Key_E},
    /* BUTTON_ACT7    */ {  27,   8, 4, 4, false, Qt::Key_H},
    /* BUTTON_ACT8    */ {  10,   0, 4, 4, false, Qt::Key_Space},
    /* BUTTON_MENU    */ {  58,   0, 6, 6, true, Qt::Key_Escape}
};

enum Fonts   {
    FONT_SIZE_MICRO ,
    FONT_SIZE_SMALL ,
    FONT_SIZE_MEDIUM,
    FONT_SIZE_LARGE
};

#endif // DEFINES_H
