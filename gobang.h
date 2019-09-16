#ifndef _GOBANG_H_
#define _GOBANG_H_

#ifdef  IN_GOBANG_C
#include "console.h"

#ifdef  IN_CONSOLE_GAMES
#define main gobang_main
#endif

#define ASIZE       15    // 棋盘大小
#define MSIZE       3     // 棋盘星位
#define LSIZE       11    // 棋盘星位
#define BLACK_CHESS 1     // 表示黑棋
#define WHITE_CHESS 2     // 表示白棋

#define WIN_WIDTH   60    // 窗口大小
#define WIN_HEIGHT  16    // 窗口大小

/* 颜色配置 */
#define GRID_CLR     BLACK
#define GRID_BK_CLR  YELLOW
#define BK_CLR       LIGHT_AQUA
#define TSTR_CLR     LIGHT_RED
#define ASTR_CLR     GREEN
#define FSTR_CLR     RED
#define ISTR_CLR     LIGHT_BLUE
#define OSTR_CLR     LIGHT_RED
#define OBK_CLR      LIGHT_GREEN

/* 游戏数据信息 */
typedef struct {
    int cursor_x, cursor_y;               // 当前光标的所在位置
    int chess[ASIZE][ASIZE];       // 记录棋盘上的棋子
    int undo_chess[ASIZE * ASIZE]; // 记录已落子的坐标，用于悔棋
    int game_model;                       // 游戏模式，默认为人机对战 
    int model_changed;	                  // 游戏模式被改变
    int chess_count;				          // 已落子个数
    int game_count;                       // 已玩局数
    int white_win_count;                  // 白棋获胜局数
    int black_win_count;                  // 黑棋获胜局数
    int winner;                           // 获胜方
    int is_return;                        // 是否返回主菜单
    int is_first;                         // 是否首次进入游戏
} Gobang;

/* 内部函数 */
static void init_data(Gobang *gb);
static void draw_face(Gobang *gb);
static int  player_down(Gobang *gb, int x, int y);
static void computer_down(Gobang *gb);
static int  num_gobang_of_pos_xy(Gobang *gb, int x, int y, int type);
static void resolve_key(Gobang *gb);
static void undo(Gobang *gb);
static void draw_grid(int x, int y);
static void show_game_info(Gobang *gb);
static void game_over(Gobang *gb);

#endif

int gobang_main(void);

#endif
