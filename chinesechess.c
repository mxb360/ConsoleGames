#define IN_CONSOLE_GAMES

#define IN_CHINESECHESS_C
#include "chinesechess.h"

/* 棋子名字 */
static const char *chess_name[][7] = {
    {"車", "馬", "象", "士", "帥", "炮", "兵",},
    {"車", "馬", "相", "士", "將", "炮", "卒",},
};

/* 主函数 */
int main(void)
{
    ChineseChess cc;
    cc.is_first = 1;

    while (1) {
        init_data(&cc);            // 数据初始化
        draw_face(&cc);            // 绘制游戏界面

        while (!cc.winner && !cc.is_return)
            resolve_key(&cc);      // 处理按键

        if (cc.winner)
            game_over(&cc);        // 结束游戏
        if (cc.is_return)
            break;                 // 返回主菜单
    }

    return 0;
}

/* 数据初始化 */
static void init_data(ChineseChess *cc)
{
    int i, j;

    cc->winner = 0;
    cc->cursor_x = 4;
    cc->cursor_y = 8;
    cc->is_chosen = 0;
    cc->chess_count = 0;
    cc->chosen_x = cc->chosen_y = -1;
    cc->is_return = 0;
    
    if (cc->is_first) {
        cc->is_first = 0;
        cc->game_count = 1;
        cc->blue_win_count = 0;
        cc->red_win_count = 0;
    }

    for (i = 0; i < 9; i++)
        for (j = 0; j < 10; j++)
            cc->chess[i][j] = 0;
}

/* 绘制游戏界面 */
static void draw_face(ChineseChess *cc)
{
    // 游戏窗口设置
    clrscr();
    set_origin(0, 0);
    set_axis(1, 1);
    set_size(WIN_WIDTH, WIN_HEIGHT);
    set_title("控制台小游戏 - 中国象棋");
    fill_color(BK_CLR, TSTR_CLR);
    // 绘制并初始化棋盘
    draw_board();
    init_chess(cc);
    // 屏幕上的相关信息显示
    set_color(BK_CLR, TSTR_CLR);
    gotoxy(39, 1);   strprint("控制台小游戏 -- 中国象棋");
    set_color(BK_CLR, ASTR_CLR);
    gotoxy(38, 2);   strprint("作者：Ma Xiaobo");
    gotoxy(38, 3);   strprint("日期：2018-8");
    set_color(BK_CLR, FSTR_CLR);
    gotoxy(38, 4);   strprint("操作：");
    gotoxy(38, 5);   strprint("    WSAD/上下左右 ：移动");
    gotoxy(38, 6);   strprint("    空格：选择/落子");
    gotoxy(38, 7);   strprint("    Z键： 悔棋  Q键：返回主菜单");
    show_game_info(cc);       // 显示游戏信息
    set_cursorxy(cc->cursor_x, cc->cursor_y);
}

/* 在棋盘上放棋子 */
static void set_chess(ChineseChess *cc, int x, int y, int type, int chess, int choose)
{
    if (type == RED_CHESS)
        choose ? set_color(CHOOSE_CLR, RED) : set_color(GRID_BK_CLR, RED);
    else
        choose ? set_color(CHOOSE_CLR, LIGHT_BLUE) : set_color(GRID_BK_CLR, LIGHT_BLUE);

    set_cursorxy(x, y);
    strprint(chess_name[type - 1][chess - 1]);
    set_cursorxy(x, y);
    cc->chess[x][y] = type * 10 + chess;
}

/* 初始棋盘布局 */
static void init_chess(ChineseChess *cc)
{
    set_chess(cc, 0, 0, BLUE_CHESS, CHESS_CHE, 0);   set_chess(cc, 0, 9, RED_CHESS, CHESS_CHE, 0);
    set_chess(cc, 1, 0, BLUE_CHESS, CHESS_MA, 0);    set_chess(cc, 1, 9, RED_CHESS, CHESS_MA, 0);
    set_chess(cc, 2, 0, BLUE_CHESS, CHESS_XIANG, 0); set_chess(cc, 2, 9, RED_CHESS, CHESS_XIANG, 0);
    set_chess(cc, 3, 0, BLUE_CHESS, CHESS_SHI, 0);   set_chess(cc, 3, 9, RED_CHESS, CHESS_SHI, 0);
    set_chess(cc, 4, 0, BLUE_CHESS, CHESS_JIANG, 0); set_chess(cc, 4, 9, RED_CHESS, CHESS_JIANG, 0);
    set_chess(cc, 5, 0, BLUE_CHESS, CHESS_SHI, 0);   set_chess(cc, 5, 9, RED_CHESS, CHESS_SHI, 0);
    set_chess(cc, 6, 0, BLUE_CHESS, CHESS_XIANG, 0); set_chess(cc, 6, 9, RED_CHESS, CHESS_XIANG, 0);
    set_chess(cc, 7, 0, BLUE_CHESS, CHESS_MA, 0);    set_chess(cc, 7, 9, RED_CHESS, CHESS_MA, 0);
    set_chess(cc, 8, 0, BLUE_CHESS, CHESS_CHE, 0);   set_chess(cc, 8, 9, RED_CHESS, CHESS_CHE, 0);
    set_chess(cc, 1, 2, BLUE_CHESS, CHESS_PAO, 0);   set_chess(cc, 1, 7, RED_CHESS, CHESS_PAO, 0);
    set_chess(cc, 7, 2, BLUE_CHESS, CHESS_PAO, 0);   set_chess(cc, 7, 7, RED_CHESS, CHESS_PAO, 0);
    set_chess(cc, 0, 3, BLUE_CHESS, CHESS_ZU, 0);    set_chess(cc, 0, 6, RED_CHESS, CHESS_ZU, 0);
    set_chess(cc, 2, 3, BLUE_CHESS, CHESS_ZU, 0);    set_chess(cc, 2, 6, RED_CHESS, CHESS_ZU, 0);
    set_chess(cc, 4, 3, BLUE_CHESS, CHESS_ZU, 0);    set_chess(cc, 4, 6, RED_CHESS, CHESS_ZU, 0);
    set_chess(cc, 6, 3, BLUE_CHESS, CHESS_ZU, 0);    set_chess(cc, 6, 6, RED_CHESS, CHESS_ZU, 0);
    set_chess(cc, 8, 3, BLUE_CHESS, CHESS_ZU, 0);    set_chess(cc, 8, 6, RED_CHESS, CHESS_ZU, 0);
}

/* 按键处理 */
static void resolve_key(ChineseChess *cc)
{
    switch (get_key(1)) {
        // 方向键/WSAD键也可以移动
    case KEY_UP:
    case 'W': cc->cursor_y = (cc->cursor_y + 9) % 10; break;
    case KEY_DOWN:
    case 'S': cc->cursor_y = (cc->cursor_y + 1) % 10; break;
    case KEY_LEFT:
    case 'A': cc->cursor_x = (cc->cursor_x + 8) % 9; break;
    case KEY_RIGHT:
    case 'D': cc->cursor_x = (cc->cursor_x + 1) % 9; break;
        // 功能键
    case 'Q': cc->is_return = 1; return;     // 返回主菜单
    case 'Z': undo(cc); return;              // 悔棋
    case ' ': choose_chess(cc); move_chess(cc); return;  // 选择棋子，落子
    }
    set_cursorxy(cc->cursor_x, cc->cursor_y);
}

/* 选中棋子 */
static void choose_chess(ChineseChess *cc)
{
    int chess = cc->chess[cc->cursor_x][cc->cursor_y];

    if ((cc->chess_count % 2 != 0 && chess / 10 == BLUE_CHESS) ||
        (cc->chess_count % 2 == 0 && chess / 10 == RED_CHESS)) {
        if (cc->is_chosen)
            unchoose_chess(cc);

        set_chess(cc, cc->cursor_x, cc->cursor_y, chess / 10, chess % 10, 1);
        cc->chosen_x = cc->cursor_x;
        cc->chosen_y = cc->cursor_y;
        cc->is_chosen = 1;
    }
}

/* 取消选中棋子 */
static void unchoose_chess(ChineseChess *cc)
{
    int type;
    int chess;

    if (!cc->is_chosen)
        return;

    type = cc->chess[cc->chosen_x][cc->chosen_y] / 10;
    chess = cc->chess[cc->chosen_x][cc->chosen_y] % 10;
    set_chess(cc, cc->chosen_x, cc->chosen_y, type, chess, 0);
    cc->is_chosen = 0;
    cc->chosen_x = cc->chosen_y = -1;
}

/* 移动棋子 */
static void move_chess(ChineseChess *cc)
{
    int chess, y_chess;
    if (!cc->is_chosen || (cc->chosen_x == cc->cursor_x && cc->chosen_y == cc->cursor_y) ||
        !chess_can_move(cc, cc->chosen_x, cc->chosen_y, cc->cursor_x, cc->cursor_y))
        return;

    chess = cc->chess[cc->chosen_x][cc->chosen_y];
    y_chess = cc->chess[cc->cursor_x][cc->cursor_y];
    // 如果將(帥)被吃，则胜负已分
    if (y_chess % 10 == CHESS_JIANG)
        cc->winner = cc->chess_count % 2 ? BLUE_CHESS : RED_CHESS;

    cc->chess[cc->chosen_x][cc->chosen_y] = 0;
    // 将这两个棋子的信息放进撤销数组中，用于悔棋
    cc->undo_chess[cc->chess_count++] = cc->chosen_x * 10000000 + cc->chosen_y * 1000000
        + chess * 10000 + cc->cursor_x * 1000 + cc->cursor_y * 100 + y_chess;

    set_chess(cc, cc->cursor_x, cc->cursor_y, chess / 10, chess % 10, 0);
    draw_board_pos(cc->chosen_x, cc->chosen_y);
    set_cursorxy(cc->cursor_x, cc->cursor_y);
    cc->chosen_x = cc->chosen_y = -1;
    cc->is_chosen = 0;
    strprint("\a");
    show_game_info(cc);
}

/* 获得两点之间存在的棋子数 */
static int get_chess_conut_pos(ChineseChess *cc, int x1, int y1, int x2, int y2)
{
    int i, count = 0;
    int abs_x = abs(x2 - x1);
    int abs_y = abs(y2 - y1);
    int dx = x2 - x1 ? (x2 - x1) / abs_x : 0;
    int dy = y2 - y1 ? (y2 - y1) / abs_y : 0;

    if (!dx || !dy) { // 横排，纵排，针对车，炮
        for (i = 1; i < abs_x || i < abs_y; i++)
            if (cc->chess[x1 + i * dx][y1 + i * dy])
                count++;
    } else if (abs_x == abs_y)  // 对角，针对相，象
        count = cc->chess[(x1 + x2) / 2][(y1 + y2) / 2];
    else if (abs_x == 1)            // 针对马
        count = cc->chess[x1][(y1 + y2) / 2];
    else if (abs_y == 1)            // 针对马
        count = cc->chess[(x1 + x2) / 2][y1];
    return count;
}

/* 棋子能否从(x1, y1) 移动到(x2, y2)处 */
static int chess_can_move(ChineseChess *cc, int x1, int y1, int x2, int y2)
{
    // 描述了各个棋子的走法
    switch (cc->chess[x1][y1] % 10) {
    case CHESS_CHE:         // 车
        return (x1 == x2 || y1 == y2) && get_chess_conut_pos(cc, x1, y1, x2, y2) == 0;

    case CHESS_PAO:         // 炮
        return (x1 == x2 || y1 == y2) &&
            ((get_chess_conut_pos(cc, x1, y1, x2, y2) == 0 && !cc->chess[x2][y2]) ||
            (get_chess_conut_pos(cc, x1, y1, x2, y2) == 1 && cc->chess[x2][y2]));

    case CHESS_MA:          // 马
        return ((x2 == x1 - 1 && y2 == y1 - 2) || (x2 == x1 + 1 && y2 == y1 - 2) ||
            (x2 == x1 - 2 && y2 == y1 - 1) || (x2 == x1 + 2 && y2 == y1 - 1) ||
            (x2 == x1 - 2 && y2 == y1 + 1) || (x2 == x1 + 2 && y2 == y1 + 1) ||
            (x2 == x1 - 1 && y2 == y1 + 2) || (x2 == x1 + 1 && y2 == y1 + 2)) &&
            get_chess_conut_pos(cc, x1, y1, x2, y2) == 0;

    case CHESS_XIANG:       // 象，相
        if ((y1 <= 4 && y2 >= 5) || (y1 >= 5 && y2 <= 4))
            return 0;
        return ((x2 == x1 - 2 && y2 == y1 - 2) || (x2 == x1 - 2 && y2 == y1 + 2) ||
            (x2 == x1 + 2 && y2 == y1 - 2) || (x2 == x1 + 2 && y2 == y1 + 2)) &&
            get_chess_conut_pos(cc, x1, y1, x2, y2) == 0;

    case CHESS_SHI:         // 士
        if (x2 < 3 || x2 > 5 || (y1 <= 4 && y2 > 2) || (y1 >= 5 && y2 < 7))
            return 0;
        return ((x2 == x1 - 1 && y2 == y1 - 1) || (x2 == x1 - 1 && y2 == y1 + 1) ||
            (x2 == x1 + 1 && y2 == y1 - 1) || (x2 == x1 + 1 && y2 == y1 + 1));

    case CHESS_ZU:          // 兵，卒
        if (y1 <= 4 && cc->chess[x1][y1] / 10 == BLUE_CHESS)
            return x2 == x1 && y2 == y1 + 1;
        if (y1 >= 5 && cc->chess[x1][y1] / 10 == BLUE_CHESS)
            return (x2 == x1 && y2 == y1 + 1) || (y2 == y1 && abs(x2 - x1) == 1);
        if (y1 >= 5 && cc->chess[x1][y1] / 10 == RED_CHESS)
            return x2 == x1 && y2 == y1 - 1;
        if (y1 <= 4 && cc->chess[x1][y1] / 10 == RED_CHESS)
            return (x2 == x1 && y2 == y1 - 1) || (y2 == y1 && abs(x2 - x1) == 1);
        return 0;

    case CHESS_JIANG:     // 将，帅
        if (x2 == x1 && cc->chess[x2][y2] % 10 == CHESS_JIANG &&
            !get_chess_conut_pos(cc, x1, y1, x2, y2))
            return 1;
        if (x2 < 3 || x2 > 5 || (y1 <= 4 && y2 > 2) || (y1 >= 5 && y2 < 7))
            return 0;
        return ((x2 == x1 && abs(y2 - y1) == 1) || (y2 == y1 && abs(x2 - x1) == 1));
    }
    return 0;
}

/* 悔棋 */
static void undo(ChineseChess *cc)
{
    int x1, y1, x2, y2, chess1, chess2;

    if (cc->chess_count <= 0)
        return;

    cc->chess_count--;
    unchoose_chess(cc);
    // 从撤销数组中取出当前所走的棋子信息
    x1 = cc->undo_chess[cc->chess_count] / 10000000;
    y1 = cc->undo_chess[cc->chess_count] / 1000000 % 10;
    chess1 = cc->undo_chess[cc->chess_count] % 1000000 / 10000;
    x2 = cc->undo_chess[cc->chess_count] % 10000 / 1000;
    y2 = cc->undo_chess[cc->chess_count] % 1000 / 100;
    chess2 = cc->undo_chess[cc->chess_count] % 100;

    if (!chess2) {
        cc->chess[x2][y2] = 0;
        draw_board_pos(x2, y2);
    } else
        set_chess(cc, x2, y2, chess2 / 10, chess2 % 10, 0);

    set_chess(cc, x1, y1, chess1 / 10, chess1 % 10, 0);
    cc->cursor_x = x1, cc->cursor_y = y1;
    set_cursorxy(cc->cursor_x, cc->cursor_y);
    show_game_info(cc);
}

/* 将光标移动到棋盘(x, y)处 */
static void set_cursorxy(int x, int y)
{
    gotoxy(4 * x + 2, 2 * y + 1);
}

/* 绘制局部棋盘 */
static void draw_board_pos(int x, int y)
{
    set_color(GRID_BK_CLR, GRID_CLR);
    set_cursorxy(x, y);

    if (x == 0 && y == 0)                  strprint("┌");
    else if (y == 0 && x == 8)             strprint("┐");
    else if (y == 9 && x == 0)             strprint("└");
    else if (y == 9 && x == 8)             strprint("┘");
    else if (y == 2 && (x == 1 || x == 7)) strprint("╋");
    else if (y == 3 && (x == 2 || x == 4)) strprint("╋");
    else if (y == 3 && x == 6)             strprint("╋");
    else if (y == 3 && x == 0)             strprint("┣");
    else if (y == 3 && x == 8)             strprint("┫");
    else if (y == 6 && (x == 2 || x == 4)) strprint("╋");
    else if (y == 6 && x == 6)             strprint("╋");
    else if (y == 6 && x == 0)             strprint("┣");
    else if (y == 6 && x == 8)             strprint("┫");
    else if (y == 7 && (x == 1 || x == 7)) strprint("╋");
    else if (x == 0)                       strprint("├");
    else if (x == 8)                       strprint("┤");
    else if (y == 0 || y == 5)             strprint("┬");
    else if (y == 9 || y == 4)             strprint("┴");
    else                                   strprint("┼");
}

/* 绘制棋盘 */
static void draw_board(void)
{
    int i, j;

    set_color(BK_CLR, GRID_BK_CLR);
    for (i = 0; i <= 16; i++)
        for (j = 0; j <= 18; j++) {
            gotoxy(2 * i + 2, j + 1);
            if (i % 2 == 0 && j % 2 == 0)
                draw_board_pos(i / 2, j / 2);
            else if (i % 2 == 0 && j % 2 && (j != 9 || i == 0 || i == 16))
                strprint("│");
            else if (i % 2 && j % 2 == 0)
                strprint("─");
            else if ((j == 1 && i == 7) || (j == 3 && i == 9) ||
                     (j == 15 && i == 7) || (j == 17 && i == 9))
                strprint("╲");
            else if ((j == 1 && i == 9) || (j == 3 && i == 7) ||
                     (j == 15 && i == 9) || (j == 17 && i == 7))
                strprint("╱");
            else
                strprint("  ");
        }
}

/* 显示游戏数据信息 */
static void show_game_info(ChineseChess *cc)
{
    set_color(BK_CLR, ISTR_CLR);
    gotoxy(38, 9);
    strprint("当前局数：第 %-2d 局", cc->game_count);
    gotoxy(38, 10);
    strprint("蓝方获胜：   %-2d 局", cc->blue_win_count);
    gotoxy(38, 11);
    strprint("红方获胜：   %-2d 局", cc->red_win_count);
    gotoxy(38, 12);
    strprint("当前步数：第 %-3d步 ", cc->chess_count);
    gotoxy(38, 13);
    if (cc->chess_count % 2)
        strprint("等待落子：蓝方");
    else
        strprint("等待落子：红方");
    set_cursorxy(cc->cursor_x, cc->cursor_y);
}

/* 当前游戏结束 */
static void game_over(ChineseChess *cc)
{
    int ch;

    set_color(BK_CLR, OSTR_CLR);
    gotoxy(38, 15);
    strprint("游戏结束！  ");
    cc->game_count++;

    if (cc->winner == BLUE_CHESS) {
        strprint("蓝方获胜！  ");
        cc->blue_win_count++;
    } else {
        strprint("红方获胜！  ");
        cc->red_win_count++;
    }

    set_color(OBK_CLR, OSTR_CLR);
    gotoxy(38, 16);
    strprint(" 按 回车键 键进入下一局 ");
    gotoxy(38, 17);
    strprint(" 按 Q键 键返回主菜单    ");

    while ((ch = get_key(1)) != KEY_ENTER)    // 等待按键
        if (ch == 'Q') {
            cc->is_return = 1;                // 按 Q键返回主菜单
            break;
        }
}
