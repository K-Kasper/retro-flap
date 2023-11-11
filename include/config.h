#pragma once

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define FPS 60
#define FRAME_TIME (1000 / FPS)

#define MAX_VELOCITY 300.0f
#define JUMP_VELOCITY 200.0f
#define GRAVITY 150.0f

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 50
#define PLAYER_START_X 40
#define PLAYER_START_Y SCREEN_HEIGHT / 2

#define MAX_PIPES 4
#define PIPE_WIDTH 80
#define PIPE_SPEED 150
#define PIPE_GAP SCREEN_WIDTH / (MAX_PIPES - 1)

#define HURT_SOUND_PATH "assets/hurt.wav"
#define JUMP_SOUND_PATH "assets/jump.wav"
#define HIGHSCORE_PATH "assets/data"
#define FONT_PATH "assets/PressStart2P.ttf"
#define FONT_SIZE 22

#define GAME_TITLE "Retro Flap"
