#ifndef _REVEESI_H_
#define _REVERSI_H_

#ifdef   IN_REVERSI_C
#include "console.h"

#ifdef  IN_CONSOLE_GAMES
#define main reversi_main
#endif

#define ASIZE      8     // 棋盘大小
#define BLACK_CHESS   1  // 表示黑棋
#define WHITE_CHESS   2  // 表示白棋

#define WIN_WIDTH    60  // 窗口大小
#define WIN_HEIGHT   16  // 窗口大小

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
    int cursor_x, cursor_y;      // 当前光标的所在位置
    int chess[ASIZE][ASIZE];     // 记录棋盘上的棋子
    int game_model;              // 游戏模式，默认为人机对战
    int model_changed;           // 游戏模式被改变
    int chess_count;             // 已落子个数
    int black_count;             // 黑子数
    int white_count;             // 白子数
    int black_cannt_down;
    int white_cannt_down;
    int game_count;              // 已玩局数
    int white_win_count;         // 白棋获胜局数
    int black_win_count;         // 黑棋获胜局数
    int winner;                  // 获胜方
    int is_return;               // 是否返回主菜单
    int is_first;                // 是否首次进入游戏
} Reversi;

/* 内部函数 */
static void init_data(Reversi *ri);
static void draw_face(Reversi *ri);
static int  player_down(Reversi *ri);
static void computer_down(Reversi *ri);
static int  chess_can_down_pos(Reversi *ri, int x, int y, int chess_type);
static void change_chess(Reversi *ri, int x, int y);
static int  num_chess_pos(Reversi *ri, int x, int y, int chess_type, int px, int py);
static int  chess_can_down(Reversi *ri, int go_type);
static void resolve_key(Reversi *ri);
static void set_chess(Reversi *ri, int x, int y, int chess_type);
static void set_cursorxy(int x, int y);
static void show_game_info(Reversi *ri);
static void draw_board(void);
static void game_over(Reversi *ri);

#endif

int reversi_main(void);

#endif
