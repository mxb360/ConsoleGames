#ifndef _TETRIS_H_
#define _TETRIS_H_

#ifdef IN_TETRIS_C
#include "console.h"

#ifdef IN_CONSOLE_GAMES
#define main tetris_main
#endif

#define SIZE_X     10  // 界面宽
#define SIZE_Y     20  // 界面高
#define WIN_WIDTH  60  // 窗口大小
#define WIN_HEIGHT 16  // 窗口大小

/* 颜色配置 */
#define TETRIS_BK_CLR  WHITE
#define WALL_CLR       LIGHT_YELLOW
#define WALL_BK_CLR    YELLOW

#define BK_CLR       LIGHT_AQUA
#define TSTR_CLR     LIGHT_RED
#define ASTR_CLR     GREEN
#define FSTR_CLR     RED
#define ISTR_CLR     BLUE
#define OSTR_CLR     RED
#define OBK_CLR      GREEN

typedef struct {
    int cursor_x, cursor_y;         // 方块的位置
    int winner;                     // 是否获胜
    int map[SIZE_X][SIZE_Y];        // 保存已经落下的方块
    int type, direction;            // 当前方块的种类，方向
    int next_type, next_direction;  // 下一个方块的种类，方向
    int move_direction;             // 记录方块移动的方向
    int pause_time;                 // 下落的间隔时间
    int score, best_score;          // 得分，最高分
    int key_down;                   // 接收按键
    int game_count;                 // 游戏局数
    int is_return;                  // 是否返回主菜单
    int is_first;                   // 是否首次进入游戏
} Tetris;

static void init_data(Tetris *ts);
static void draw_face(Tetris *ts);
static void create_tetris(Tetris *ts);
static int  is_tetris_can_move(Tetris *ts, int dx, int dy, int ddir);
static void tetris_down(Tetris *ts);
static void save_tetris_map(Tetris *ts);
static void clear_tetris(Tetris *ts);
static void draw_tetris(int x, int y, int the_tetris, int dir, int show);
static void resolve_key(Tetris *ts);
static void show_game_info(Tetris *ts);
static void game_over(Tetris *ts);

#endif

int tetris_main(void);

#endif
