#ifndef COMMON_H
#define COMMON_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_TITLE "Space Shooter SDL3"

#define FPS 60
#define TARGET_FRAME_TIME (1000 / FPS)

#define MAX_ENTITIES 200
#define MAX_BULLETS 100
#define MAX_ENEMIES 50

#define PLAYER_START_HEALTH 3
#define ENEMY_SPAWN_DELAY 1.5f

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#endif // COMMON_H
