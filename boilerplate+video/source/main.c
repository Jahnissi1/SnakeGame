#include "gpio.h"
#include "uart.h"
#include "framebuffer.h"
#include "gpio.h"
#include "uart.h"
#include "framebuffer.h"
#include "snes.h"
#include "heart.h"
#include "mainmenu.h"
#include "startbutton.h"
#include "startbuttonselect.h"
#include "quitgame.h"
#include "quitgameselect.h"
#include "restartgame.h"
#include "restartgameselect.h"
#include "messages.h"
#include "apple.h"
#include "fire.h"
#include "coin.h"
#include "youwin.h"
#include "transition.h"

#define CLK 11  // Clock pin
#define LAT 9   // Latch pin
#define DAT 10  // Data pin
#define SYSTMR_LO        ((volatile unsigned int*)(MMIO_BASE+0x00003004))   // Address for the system timer
unsigned *clo1 = (unsigned*)SYSTMR_LO;

#define GREEN 0x91B0FB
#define RED 0xEA3922
#define BLUE 0x22EACE
#define BROWN 0x8B5E09
#define DARK_GREEN 0x1C8E2E
#define WHITE 0xFFFFFF
#define BLACK 0
#define PURPLE 0x7F00FF
#define MAGENTA 0xFF00FF
#define LIME 0x00FF00
#define YELLOW  0xFFFF00
#define MAROON 0x800000
#define CYAN 0x00FFFF
#define MEDIUM_BLUE 0x0000CD
#define CHOCOLATE 0xD2691E
#define MISTY_ROSE 0xFFE4E1
#define GOLD 0xFFD700

#define TILE_SIZE 32
#define EMPTY 0
#define BORDER 1
#define GOLDEN 5
#define OBSTACLE 4
#define HEART 2
#define APPLE 8
#define SNAKE 3
#define FOOD 6
#define FIRE 12
#define COIN 9
// different screens
#define MAIN 0
#define PAUSED 5
#define LEVEL1 1
#define LEVEL2 2
#define LEVEL3 3
#define LEVEL4 4
#define TRANSITION 9
#define EXIT 6
#define WON 7
#define LOST 8
// buttons
#define RESTART 2
#define START 0
#define QUIT 1

#define HEIGHT 720
#define WIDTH 1280

#define true 1
#define false 0
#define MAX_LENGTH 10


#define BUTTON_B 1      // 1 << 0
#define BUTTON_Y 2      // 1 << 1
#define BUTTON_SEL 4    // 1 << 2
#define BUTTON_START 8  // 1 << 3
#define BUTTON_UP 16    // 1 << 4
#define BUTTON_DOWN 32  // 1 << 5
#define BUTTON_LEFT 64  // 1 << 6
#define BUTTON_RIGHT 128// 1 << 7
#define BUTTON_A 256    // 1 << 8
#define BUTTON_X 512    // 1 << 9
#define BUTTON_L 1024   // 1 << 10
#define BUTTON_R 2048   // 1 << 11

int map1[22][40] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
        {1,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,1,0,0,0,0,0,1,0,0,0,2,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,2,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,9,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    int map2[22][40] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1},
        {1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,1,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1},
        {1,0,0,1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,1,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1},
        {1,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1},
        {1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,1,1,1,0,2,2,2,2,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,1,0,1,0,0,2,2,2,0,0,0,0,2,0,0,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,2,2,2,2,2,2,0,0,0,1,0,0,0,0,0,5,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,9,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        };

    int map3[22][40] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,5,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
    {1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

int map4[22][40] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,0,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,5,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1},
    {1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};




struct GameMap{
    int gridcolor;
    int bordercolor;
    int snakeColor;
    int transitionColor;
    int messageColor;
    int tiles[22][40];
};
struct Obj{
    int pos[2];
    int id;
};

struct Obj level1[15];
struct Obj level2[15];
struct Obj level2[15];
struct Obj level4[15];

struct Snake{
    int body[11][2];
    int colour;
    int size;
    int lives;
};



struct State{
    struct Snake snake;
    struct GameMap map;
    struct Obj objects[16];
    int score, win, lose, Selected, last_level, objectsSize, scene,lives, ti, end[2], control[3];
    double time;
} GameState;


//struct State GameState;
void *memset(void *ptr, int value, unsigned int num) {
    unsigned char *p = ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;

}
typedef unsigned int size_t_custom;

void* memcpy(void* destination, const void* source, size_t_custom num_bytes) {
    char* dest = (char*)destination;
    const char* src = (const char*)source;

    // Copying bytes one by one
    for (size_t_custom i = 0; i < num_bytes; ++i) {
        dest[i] = src[i];
    }

    return destination;
}


void setMap(struct GameMap *map, int new_tiles[22][40]){
     for(int row = 0; row < 22; row++){
        for(int column = 0; column < 40; column++ ){
            map->tiles[row][column] = new_tiles[row][column];
        }
    }
}
void copyObj(struct Obj *dest, struct Obj *src) {
    dest->pos[0] = src->pos[0];
    dest->pos[1] = src->pos[1];
    dest->id = src->id;
}
void initialize_game(){
    //setMap(&GameState.map, map1);
     GameState.map.bordercolor = BROWN;
    GameState.map.gridcolor = WHITE;
	GameState.end[0] = WIDTH-32;
    GameState.end[1] = HEIGHT-32;
    GameState.lives = 4;
    GameState.lose = false;
    GameState.score = 0;
    GameState.time = 120;
    GameState.win = false;
    GameState.control[0] = true;
    GameState.control[1]= true;
    GameState.control[2] = true;
    GameState.scene = 0;
    
    GameState.ti = *clo1;
   resetSnake();
}
void setObj(int a, int b, int c, int i){
    struct Obj o;
    o.pos[0] = a;
    o.pos[1] = b;
    o.id = c;
     copyObj(&GameState.objects[i], &o);

}
void initialize_obj(){
    if (GameState.scene == LEVEL1){
        
    setObj(38,9,FOOD, 0);
    setObj(5,17,FIRE, 1);
    setObj(9,17,FIRE, 2);
    setObj(15,10,FIRE, 3);
    setObj(35,5,FOOD, 4);
    setObj(30,19,FOOD, 5);
    setObj(21,4,HEART, 6);
    setObj(13,20,COIN, 7);
    setObj(19,5,COIN, 8);
    setObj(19,10,COIN, 9);
    setObj(32,17,FIRE, 10);
    setObj(15,12,FIRE, 11);
    setObj(19,18,FOOD, 12);
    //setObj(17,2,FOOD, 13);
    setObj(17,2,COIN, 13);
    setObj(36,17,FOOD, 14);

        GameState.objectsSize = 15;

    }else if(GameState.scene == LEVEL2){
        setObj(38,9,FOOD, 0);
        setObj(5,17,FIRE, 1);
        setObj(12,18,FIRE, 2);
        setObj(15,20,FIRE, 3);
        setObj(35,6,FOOD, 4);
        setObj(30,19,FOOD, 5);
        setObj(21,4,HEART, 6);
        setObj(13,20,COIN, 7);
        setObj(19,5,COIN, 8);
        setObj(19,10,COIN, 9);
        setObj(32,17,FIRE, 10);
        setObj(15,12,FIRE, 11);
        setObj(19,18,FOOD, 12);
        //setObj(17,2,FOOD, 13);
        setObj(17,2,COIN, 13);
        setObj(36,17,FOOD, 14);

         GameState.objectsSize = 15;

    }else if(GameState.scene == LEVEL3){
        setObj(38,9,FOOD, 0);
        setObj(5,17,FIRE, 1);
        setObj(9,17,FIRE, 2);
        setObj(15,10,FIRE, 3);
        setObj(35,5,FOOD, 4);
        setObj(30,19,FOOD, 5);
        setObj(21,4,HEART, 6);
        setObj(13,20,COIN, 7);
        setObj(19,5,COIN, 8);
        setObj(18,10,COIN, 9);
        setObj(32,17,FIRE, 10);
        setObj(15,12,FIRE, 11);
        setObj(18,18,FOOD, 12);
        //setObj(17,2,FOOD, 13);
        setObj(17,2,COIN, 13);
        setObj(36,17,FOOD, 14);
         GameState.objectsSize = 15;

    }else if(GameState.scene == LEVEL4){
        setObj(35,9,FOOD, 0);
        setObj(5,12,FIRE, 1);
        setObj(11,17,FIRE, 2);
        setObj(16,10,FIRE, 3);
        setObj(35,9,FOOD, 4);
        setObj(26,19,FOOD, 5);
        setObj(21,9,HEART, 6);
        setObj(14,20,COIN, 7);
        setObj(19,5,COIN, 8);
        setObj(19,10,COIN, 9);
        setObj(32,17,FIRE, 10);
        setObj(15,12,FIRE, 11);
        setObj(19,18,FOOD, 12);
        //setObj(17,2,FOOD, 13);
        setObj(17,2,COIN, 13);
        setObj(36,17,FOOD, 14);
    GameState.objectsSize = 15;

    }
}
void resetSnake(){
    int b[11][2] = {{1,1}, {2,1}, {3,1}};
    for(int i=0; i<10; i++){
        for(int j=0; j < 2; j++){
            GameState.snake.body[i][j] = b[i][j];
        }
    }
    GameState.snake.size = 3;
}

void mapControl(){
    if (GameState.scene == MAIN || GameState.scene == PAUSED){
        setMap(&GameState.map, map1);
        GameState.map.bordercolor = BROWN;
        GameState.map.gridcolor = WHITE;
        GameState.map.snakeColor = DARK_GREEN;
        GameState.map.transitionColor = GOLD;
        GameState.map.messageColor = RED;
        GameState.scene = LEVEL1;
        initialize_obj();
        resetSnake();
       

    } else if (GameState.scene == LEVEL1){
        setMap(&GameState.map, map2);
        GameState.map.bordercolor = MAROON;
        GameState.map.gridcolor = MAGENTA;
        GameState.map.snakeColor = DARK_GREEN;
        GameState.map.transitionColor = MISTY_ROSE;
        GameState.map.messageColor = RED;
        GameState.scene = LEVEL2;
        initialize_obj();
        resetSnake();
         
    }else if (GameState.scene == LEVEL2){
        setMap(&GameState.map, map3);
        GameState.map.bordercolor = MEDIUM_BLUE;
        GameState.map.gridcolor = WHITE;
        GameState.map.snakeColor = YELLOW;
        GameState.map.transitionColor = GOLD;
        GameState.map.messageColor = RED;
        GameState.scene = LEVEL3;
        initialize_obj();
        resetSnake();
         
    }else if (GameState.scene == LEVEL3){
        setMap(&GameState.map, map4);
        GameState.map.bordercolor = CHOCOLATE;
        GameState.map.gridcolor = WHITE;
        GameState.map.snakeColor = CYAN;
        GameState.map.transitionColor = GOLD;
        GameState.map.messageColor = RED;
        GameState.scene = LEVEL4;
        initialize_obj();
        resetSnake();
        

    }else if (GameState.scene == LEVEL4){
        GameState.scene = WON;
        GameState.win = true;
    }
    GameState.control[0] = true;
    GameState.control[1] = true;
    GameState.control[2] = true;
}
void hitObstacle(int i){
    GameState.lives-- ;
    // removes the obstacle from the map:
    for (int p = i; p < GameState.objectsSize - 1; ++p) {
        copyObj(&GameState.objects[p], &GameState.objects[p+1]);
    }
    GameState.objectsSize--;
}
void hitCoin(int i){
    GameState.score += 10;
    // removes the obstacle from the map:
    for (int p = i; p < GameState.objectsSize - 1; ++p) {
        copyObj(&GameState.objects[p], &GameState.objects[p+1]);
    }
    GameState.objectsSize--;
}

void hitHeart(int i){
    GameState.lives++;
    // removes the obstacle from the map:
    for (int p = i; p < GameState.objectsSize - 1; ++p) {
        copyObj(&GameState.objects[p], &GameState.objects[p+1]);
    }
    GameState.objectsSize--;
}

void growSnake( int i){
    if(GameState.snake.size < MAX_LENGTH){
        GameState.snake.body[GameState.snake.size][0] = GameState.objects[i].pos[0];
        GameState.snake.body[GameState.snake.size][1] = GameState.objects[i].pos[1];
        GameState.snake.size++;
    }
    for (int p = i; p < GameState.objectsSize - 1; ++p) {
        copyObj(&GameState.objects[p], &GameState.objects[p+1]);
        //GameState.objects[i] = GameState.objects[i + 1];
    // Decrement array size
    

    }
    GameState.objectsSize--;
}



int checkCollision(int x, int y){
    if(GameState.map.tiles[y][x] == GOLDEN){
        if (GameState.scene == LEVEL4){
            mapControl();
        }
        GameState.last_level = GameState.scene;
        GameState.scene = TRANSITION;
        display();
        GameState.scene = GameState.last_level;
        mapControl();
        return true;
        

    }
    // checks if the x, y corresponds to any of the game objects:
    for(int i=0; i < GameState.objectsSize; i++){
        //struct Obj GameState.objects[i]  = GameState.objects[i];
        if(x == GameState.objects[i].pos[0] && y == GameState.objects[i].pos[1]){
            if(GameState.objects[i].id == FOOD) growSnake(i); // theirs food in (x, y)
            else if(GameState.objects[i].id == FIRE) hitObstacle(i); // theirs an obstacle in (x, y)
            else if(GameState.objects[i].id == HEART) hitHeart(i);
            else if(GameState.objects[i].id == COIN) hitCoin(i);
            return true;
        }
    }
    // checks the snake isn't going over itself
    for(int i = 0; i< GameState.snake.size; i++){
        if (GameState.snake.body[i][0] == x && GameState.snake.body[i][1] == y) return true;
    }
    return false;
}

/*
Handles the logic for the snake moving into (x, y).
args(int x: the x position being moved to, int y: the y positiong being moved to)
returns(None)
*/
void moveSnake(int x, int y){
    if (GameState.map.tiles[y][x] != BORDER){    // ensures snake never moves to border
        // clears the snakes tail from the screen since it moved:
        drawRect(GameState.snake.body[0][0]*TILE_SIZE, GameState.snake.body[0][1]*TILE_SIZE, (GameState.snake.body[0][0]*TILE_SIZE)+TILE_SIZE, (GameState.snake.body[0][1]*TILE_SIZE)+TILE_SIZE, 0, 1);
        drawRect(GameState.snake.body[0][0]*TILE_SIZE, GameState.snake.body[0][1]*TILE_SIZE, (GameState.snake.body[0][0]*TILE_SIZE)+TILE_SIZE, (GameState.snake.body[0][1]*TILE_SIZE)+TILE_SIZE, GameState.map.gridcolor, 0);
        
       // if(GameState.map.tiles[y][x] == GOLDEN) mapControl();    // snake reached the end tile
        
        if(!checkCollision(x, y)){
            // updates the coordinated in GameState.snake.body[][]:
            for(int i=0; i < GameState.snake.size-1; i++){
                GameState.snake.body[i][0] = GameState.snake.body[i+1][0];
                GameState.snake.body[i][1] = GameState.snake.body[i+1][1];
            }
            // updates the head coordinates of GameState.snake.body[][]:
            GameState.snake.body[GameState.snake.size-1][0] = x;
            GameState.snake.body[GameState.snake.size-1][1] = y;
        }
    }
}



// draws the GameState.map.tiles
void drawGrid(){
    if(GameState.control[0]){
        // draws the background:
        if (GameState.scene == LEVEL1){
            fillScreen(BLACK);
            // draw respective background image
        }else if(GameState.scene == LEVEL2){
            fillScreen(BLACK);
            // draw respective background image
        }else if(GameState.scene == LEVEL3){
            fillScreen(BLACK);
            // draw respective background image
        }else if(GameState.scene == 4){
                fillScreen(BLACK);
        }

        // draws the grid:
        for(int col=0; col < 40; col++){
            for(int row=0; row < 22; row++){
                if (GameState.map.tiles[row][col] == EMPTY){ // empty tiles
                    drawRect(col*TILE_SIZE, row*TILE_SIZE, (col*TILE_SIZE)+TILE_SIZE, (row*TILE_SIZE)+TILE_SIZE, GameState.map.gridcolor, 0);
                } else if (GameState.map.tiles[row][col] == BORDER){ // border tiles
                    drawRect(col*TILE_SIZE, row*TILE_SIZE, (col*TILE_SIZE)+TILE_SIZE, (row*TILE_SIZE)+TILE_SIZE, GameState.map.bordercolor, 1);
                }else if (GameState.map.tiles[row][col] == GOLDEN && GameState.control[0]){
                    drawRect(col * 32, row * 32,(col * 32) + 32, (row * 32) + 32, GameState.map.transitionColor , 1);
                }
            }
        }
    }
}
// Draws the objects in GameState.objects[]
void drawObjects(){
    for (int i=0; i < GameState.objectsSize; i++){
        if (GameState.objects[i].id == FOOD){ // food image
            drawImage(apple.pixel_data, apple.width, apple.height, GameState.objects[i].pos[0]*TILE_SIZE+1,GameState.objects[i].pos[1]*TILE_SIZE+1);
        } else if (GameState.objects[i].id == HEART){ // heart image
            drawImage(heart.pixel_data, heart.width, heart.height, GameState.objects[i].pos[0]*TILE_SIZE+1,GameState.objects[i].pos[1]*TILE_SIZE+1);
        } else if (GameState.objects[i].id == FIRE){ // fire image
            drawImage(fire.pixel_data, fire.width, fire.height, GameState.objects[i].pos[0]*TILE_SIZE+1,GameState.objects[i].pos[1]*TILE_SIZE+1);
        }else if (GameState.objects[i].id == COIN){ // fire image
            drawImage(coin.pixel_data, coin.width, coin.height, GameState.objects[i].pos[0]*TILE_SIZE+1,GameState.objects[i].pos[1]*TILE_SIZE+1);
        }
    }   
}


// Draws the screen

void display(){
    if (GameState.scene == LEVEL1 || GameState.scene == LEVEL2 || GameState.scene == LEVEL3 || GameState.scene == LEVEL4){ // the player is currently playing a level
        drawGrid();
        drawSnake();
        drawObjects();
        drawGameInfo();
        GameState.control[2] = false;
        GameState.control[0] = false;

    } else if (GameState.scene == MAIN){ // player is on the main screen
        if(GameState.control[1] == true) {
            fillScreen(BLACK);
            drawImage(mainmenu.pixel_data,mainmenu.width, mainmenu.height,0,0);
             GameState.control[1] = false;
        }
        

        if(GameState.Selected == QUIT){
            drawImage(quitgameselect.pixel_data, quitgameselect.width, quitgameselect.height, 560, 256);
            drawImage(startbutton.pixel_data, startbutton.width, startbutton.height, 560, 96);
        } else {
            drawImage(startbuttonselect.pixel_data, startbuttonselect.width, startbutton.height, 560, 96);
            drawImage(quitgame.pixel_data, quitgame.width, quitgame.height, 560, 256);
        }

    } else if(GameState.scene == TRANSITION){ // transitioning between levels
        // draw transition
        fillScreen(BLACK);
        drawImage(transition.pixel_data,transition.width,transition.height,0,0);

        // then wait
        wait(20000);
    
    } else if(GameState.scene == PAUSED){ // player is on the pause screen
        // draw main menu just with restart button
        if(GameState.control[1] == true) {
            fillScreen(0);
            drawImage(mainmenu.pixel_data,mainmenu.width, mainmenu.height,0,0);
            GameState.control[1] = false;
        }
        if (GameState.Selected == QUIT){
            drawImage(quitgameselect.pixel_data, quitgameselect.width, quitgameselect.height, 560, 256);
            drawImage(restartgame.pixel_data, restartgame.width, restartgame.height, 560, 96);
        } else {
            drawImage(restartgameselect.pixel_data, restartgameselect.width, restartgameselect.height, 560, 96);
            drawImage(quitgame.pixel_data, quitgame.width, quitgame.height, 560, 256);
        }
    }else if (GameState.scene == WON){
        if (GameState.control[1] = true){
            drawImage(youwin.pixel_data,youwin.width,youwin.height,0,0);
            GameState.control[1] = false;
        }
        
    }
}
void drawGameInfo(){
    drawString("SCORE", 1056-TILE_SIZE, 2*TILE_SIZE, 5, RED);
    drawString("LIVES", 1056-TILE_SIZE, TILE_SIZE, 5, RED);
    drawString("TIME", 1056-TILE_SIZE, 3*TILE_SIZE, 5, RED);
    drawNumber(GameState.time, 1216, 96, 5, WHITE);
    drawNumber(GameState.lives,1216,32,5,WHITE);
    drawNumber(GameState.score,1216,64,5,WHITE);
        
    if(GameState.control[2]){
         drawRect(1216,32,1248,32+32,GameState.map.bordercolor,1);
        drawRect(1216-32,64,1248,64+32,GameState.map.bordercolor,1);
        drawRect(1148,96,1280,96+32,GameState.map.bordercolor,1);
        drawNumber(GameState.lives,1216,32,5,WHITE);
        drawNumber(GameState.score,1216,64,5,WHITE);

        drawNumber(GameState.time,1216,96,5,WHITE);
    }

}
void drawSnake(){
    for (int i=0; i < GameState.snake.size; i++ ){
        
            drawRect(GameState.snake.body[i][0]*TILE_SIZE, GameState.snake.body[i][1]*TILE_SIZE, (GameState.snake.body[i][0]*TILE_SIZE)+TILE_SIZE, (GameState.snake.body[i][1]*TILE_SIZE)+TILE_SIZE, GameState.map.snakeColor, 1);
        
    }
}


// Handles the logic for the SNES control
void update(){
    int x = GameState.snake.body[GameState.snake.size-1][0];
    int y = GameState.snake.body[GameState.snake.size-1][1];
    unsigned short data;
    data = read_snes();

    // unused buttons(no functionality):
    if((data & BUTTON_B) == BUTTON_B);
    if((data & BUTTON_Y) == BUTTON_Y);
    if((data & BUTTON_SEL) == BUTTON_SEL);
    if((data & BUTTON_X) == BUTTON_X);
    if((data & BUTTON_L) == BUTTON_L);
    if((data & BUTTON_R) == BUTTON_R);

    // A button was pressed:
    if((data & BUTTON_A) == BUTTON_A){
        if(GameState.scene == MAIN ){
            if(GameState.Selected == START){
                 mapControl(); 
            }
            else if (GameState.Selected == QUIT) GameState.scene = EXIT;
        }else if(GameState.scene == PAUSED){
            if(GameState.Selected == QUIT) GameState.scene = 0;
            else if(GameState.Selected == RESTART){
                
                initialize_game();
                GameState.scene = PAUSED;
                mapControl();
                
            }
        }
    }


    // START button was pressed:
    if((data & BUTTON_START) == BUTTON_START){
        if(GameState.scene == LEVEL1 || GameState.scene == LEVEL2 || GameState.scene == LEVEL3 || GameState.scene == LEVEL4 || GameState.scene == WON){
            GameState.Selected = RESTART;
            GameState.control[1] = true;
            GameState.control[0] = true;
            GameState.last_level = GameState.scene;
            GameState.scene = PAUSED;
        }else if (GameState.scene == PAUSED){
            GameState.scene = GameState.last_level;
            GameState.control[0] = true;
            GameState.control[2] = true;
        }
    }
    // UP button was pressed:
    if((data & BUTTON_UP) == BUTTON_UP){
        if(GameState.scene == LEVEL1 || GameState.scene == LEVEL2 || GameState.scene == LEVEL3 || GameState.scene == LEVEL4) moveSnake(x, y-1);
        else if (GameState.scene == MAIN) GameState.Selected = START;
        else if (GameState.scene == PAUSED) GameState.Selected = RESTART;
        else if (GameState.scene == WON) GameState.Selected = RESTART;
    }

    // DOWN button was pressed:
    if((data & BUTTON_DOWN) == BUTTON_DOWN){
        if (GameState.scene == LEVEL1 || GameState.scene == LEVEL2 || GameState.scene == LEVEL3 || GameState.scene == LEVEL4) moveSnake(x, y+1);
        else if (GameState.scene == MAIN) GameState.Selected = QUIT;
        else if (GameState.scene == PAUSED) GameState.Selected = QUIT;
        else if (GameState.scene == WON) GameState.Selected = QUIT;
    }

    // LEFT button was pressed:
    if((data & BUTTON_LEFT) == BUTTON_LEFT){
        if(GameState.scene == LEVEL1 || GameState.scene == LEVEL2 || GameState.scene == LEVEL3 || GameState.scene == LEVEL4) moveSnake(x-1,y);
    }

    // RIGHT button was pressed:
    if((data & BUTTON_RIGHT) == BUTTON_RIGHT){
        if(GameState.scene == LEVEL1 || GameState.scene == LEVEL2 || GameState.scene == LEVEL3 || GameState.scene == LEVEL4) moveSnake(x+1, y);
    }
}

int main() {
  

	// initialize GameState
    //setMap(&GameState.map, map1);
    
    initialize_game();
    
    init_framebuffer();
    

    init_GPIO(CLK, 000);
    init_GPIO(CLK, 001);
    init_GPIO(LAT, 000);
    init_GPIO(LAT, 001);
    init_GPIO(DAT, 000);

    fillScreen(BLACK); // clear the screen
    unsigned int color = 0xFFFFFF; // White color
    int scale = 5; // Scale of digits
    drawDigit(1,200,200,5,WHITE);

    // Draw number 123 at position (100, 100) with scale 5
    
    //int ti = *clo1;
    int ti2;
    // game loop:
    
    
   
    

    while(!GameState.win && !GameState.lose &&  GameState.lives > 0 && GameState.time > 0 ){
        //ti = *clo1/1000000;
        
        display();  
        update();
        wait(30000); // slow down the referesh rate
        GameState.time = 150 - (*clo1 -GameState.ti)/1000000;
        GameState.control[2] = true;

        
    }
    fillScreen(BLACK);
    drawString("LOSER", 1280/2, 720/2 , 10, RED);
    
    return 0;
}