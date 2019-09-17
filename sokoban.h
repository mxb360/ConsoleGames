#ifndef _SOKOBAN_H_
#define _SOKOBAN_H_

#ifdef IN_SOKOBAN_C
#include "console.h"

#ifdef IN_CONSOLE_GAMES
#define main sokoban_main
#endif

#define MAP_SIZE 15

/* 颜色配置 */
#define MAP_BK_CLR   GRAY
#define WALL_CLR     BLUE
#define WALL_BK_CLR  BLUE
#define BK_CLR       LIGHT_AQUA
#define M00_CLR      BLUE
#define M01_CLR      LIGHT_BLUE
#define M02_CLR      BLUE
#define M03_CLR      LIGHT_PURPLE
#define M06_CLR      GREEN
#define M07_CLR      LIGHT_PURPLE
#define M10_CLR      LIGHT_RED
#define M11_CLR      LIGHT_PURPLE

#define TSTR_CLR     LIGHT_RED
#define ASTR_CLR     GREEN
#define FSTR_CLR     RED
#define ISTR_CLR     BLUE
#define OSTR_CLR     RED
#define OBK_CLR      GREEN

/* 游戏信息 */
typedef struct {
    int cursor_x, cursor_y;          // 玩家（光标）的位置
    int map[MAP_SIZE][MAP_SIZE];     // 当前的地图信息
    int step;                        // 当前步数
    int level;                       // 当前关卡数
    int winner;                      // 是否过关
    int box_count;                   // 总箱子数
    int solve_count;                 // 位置正确的箱子数
    int restart;                     // 重新开始
    int undo_dir[MAP_SIZE*MAP_SIZE]; // 记录步骤，用于撤销
    int is_return;                   // 是否返回主菜单
    int is_first;                    // 是否首次进入游戏
} Sokoban;

/* 内部函数 */
static void init_data(Sokoban *sb);
static void draw_face(Sokoban *sb);
static void move(Sokoban *sb, int dx, int dy);
static void undo(Sokoban *sb);
static void resolve_key(Sokoban *sb);
static void draw_map_pos(Sokoban *sb, int x, int y);
static void show_game_info(Sokoban *sb);
static void game_over(Sokoban *sb);

#endif

int sokoban_main(void);

#endif
