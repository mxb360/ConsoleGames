#ifndef _CHINESECHESS_H_
#define _CHINESECHESS_H_

#ifdef IN_CHINESECHESS_C
#include "console.h"

#ifdef IN_CONSOLE_GAMES
#define main chinesechess_main
#endif

#define RED_CHESS   1     // 红方
#define BLUE_CHESS  2     // 蓝方

#define CHESS_CHE   1     // 車
#define CHESS_MA    2     // 馬
#define CHESS_XIANG 3     // 相，象
#define CHESS_SHI   4     // 士
#define CHESS_JIANG 5     // 將，帥
#define CHESS_PAO   6     // 炮
#define CHESS_ZU    7     // 卒，兵

#define WIN_WIDTH   70    // 窗口大小
#define WIN_HEIGHT  21    // 窗口大小

/* 颜色配置 */
#define CHOOSE_CLR   LIGHT_YELLOW

#define GRID_CLR     BLACK
#define GRID_BK_CLR  YELLOW
#define BK_CLR       LIGHT_AQUA
#define TSTR_CLR     LIGHT_RED
#define ASTR_CLR     GREEN
#define FSTR_CLR     RED
#define ISTR_CLR     BLUE
#define OSTR_CLR     RED
#define OBK_CLR      LIGHT_GREEN

/* 游戏信息 */
typedef struct {
    int cursor_x, cursor_y;          // 当前光标在棋盘中的坐标
    int chosen_x, chosen_y;          // 选中的棋子坐标（没有棋子选中的时值为-1）
    int chess_count;                 // 当前下棋步数
    int is_chosen;                   // 是否有棋子被选中
    int chess[9][10];                // 保存棋盘上的棋子信息
    int undo_chess[32];              // 记录已走的棋子信息，用于悔棋
    int winner;                      // 获胜者
    int game_count;                  // 游戏进行局数
    int blue_win_count;              // 蓝方获胜局数
    int red_win_count;               // 红方获胜局数
    int is_return;                   // 是否返回主菜单
    int is_first;                    // 是否首次进入游戏
} ChineseChess;

/* 内部函数 */
static void init_data(ChineseChess *cc);
static void draw_face(ChineseChess *cc);
static void set_chess(ChineseChess *cc, int x, int y, int type, int chess, int choose);
static void init_chess(ChineseChess *cc);
static void resolve_key(ChineseChess *cc);
static void choose_chess(ChineseChess *cc);
static void unchoose_chess(ChineseChess *cc);
static void move_chess(ChineseChess *cc);
static int  get_chess_conut_pos(ChineseChess *cc, int x1, int y1, int x2, int y2);
static int  chess_can_move(ChineseChess *cc, int x1, int y1, int x2, int y2);
static void undo(ChineseChess *cc);
static void set_cursorxy(int x, int y);
static void draw_board_pos(int x, int y);
static void draw_board(void);
static void show_game_info(ChineseChess *cc);
static void game_over(ChineseChess *cc);

#endif

int chinesechess_main(void);

#endif
