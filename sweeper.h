#ifndef _SWEEPER_H_
#define _SWEEPER_H_

#ifdef IN_SWEEPER_C
#include "console.h"
#include "tools.h"

#ifdef IN_CONSOLE_GAMES
#define main sweeper_main
#endif

#define PRIMARY_LEVEL 0
#define MEDIUM_LEVEL  1
#define ADVANCE_LEVEL 2
#define USER_LEVEL    3
#define MAX_SIZE_X    30
#define MAX_SIZE_Y    20
#define MAX_SCORE     99999

#define PRIMARY_X       9
#define PRIMARY_Y       9
#define PRIMARY_SWEEP   10
#define MEDIUM_X        16
#define MEDIUM_Y        16
#define MEDIUM_SWEEP    40
#define ADVANCE_X       30
#define ADVANCE_Y       16
#define ADVANCE_SWEEP   99
#define MAX_X           MAX_SIZE_X
#define MAX_Y           MAX_SIZE_Y
#define MIN_X           9
#define MIN_Y           9
#define MIN_SWEEP       10

/* 颜色配置 */
#undef BK_CLR 
#undef TSTR_CLR 
#undef ASTR_CLR   
#undef FSTR_CLR   
#undef ISTR_CLR   
#undef OSTR_CLR   
#undef OBK_CLR    
#define BK_CLR       LIGHT_AQUA
#define TSTR_CLR     LIGHT_RED
#define ASTR_CLR     GREEN
#define FSTR_CLR     RED
#define ISTR_CLR     LIGHT_BLUE
#define OSTR_CLR     LIGHT_RED
#define OBK_CLR      LIGHT_GREEN

/* 内部函数 */
typedef struct {
    int cursor_x, cursor_y;             // 当前光标在棋盘中的坐标
    int size_x, size_y;                 // 当前模式下的雷区规格
    int sweep[MAX_SIZE_X][MAX_SIZE_Y];  // 储存雷区信息
    int model;                          // 游戏模式
    int winner;                         // 游戏结束
    int num_sweep;                      // 当前模式下的雷数
    int count;                          // 已被点开的雷数
    int time;                           // 已用时
    int flag;                           // 已插旗数
    int game_count;                     // 进行的游戏局数
    int win_count;                      // 胜利局数
    int replay;                         // 是否重新开始游戏
    int primary_min_time;               // 初级模式最高记录
    int medium_min_time;                // 中级模式最高记录
    int advanced_min_time;              // 高级模式最高记录
    int is_return;                      // 是否返回主菜单
    int is_first;                       // 是否首次进入游戏
} Sweeper;

static void init_data(Sweeper *sp);
static void draw_face(Sweeper *sp);
static void init_sweep(Sweeper *sp, int x, int y);
static void update_time(Sweeper *sp);
static void flag_sweep(Sweeper *sp, int x, int y);
static void mines_sweep(Sweeper *sp, int x, int y);
static void sweep_all(Sweeper *sp, int x, int y);
static void resolve_key(Sweeper *sp);
static void print_sweep_pos(Sweeper *sp, int x, int y, int num);
static void print_sweep_face(Sweeper *sp);
static void show_game_info(Sweeper *sp);
static void game_over(Sweeper *sp);
static void choice_mode(Sweeper *sp);
static void set_user_model(Sweeper *sp);

#endif

int sweeper_main(void);

#endif 
