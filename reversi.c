#define  IN_CONSOLE_GAMES

#define  IN_REVERSI_C
#include "reversi.h"

/* 主函数 */
int main(void)
{
    Reversi ri;
    ri.is_first = 1;

    while (1) {                // 游戏大循环
        init_data(&ri);        // 数据初始化
        draw_face(&ri);        // 绘制游戏界面
        while (!ri.is_return && !ri.winner && !ri.model_changed)
            resolve_key(&ri);  // 处理按键，游戏逻辑由按键决定
        if (ri.winner)
            game_over(&ri);    // 赢家产生，当前游戏结束
        if (ri.is_return)
            break;
    }
    return 0;
}

/* 变量初始化 */
static void init_data(Reversi *ri)
{
    int i, j;

    ri->cursor_x = ASIZE / 2 - 1;
    ri->cursor_y = ASIZE / 2 - 1;
    ri->chess_count = 0;
    ri->black_count = 0;
    ri->white_count = 0;
    ri->winner = 0;
    ri->black_cannt_down = 0;
    ri->white_cannt_down = 0;
    ri->is_return = 0;
    /* 首次运行 */
    if (ri->is_first) {
        ri->is_first = 0;
        ri->game_model = 1;
        ri->model_changed = 1;
    }
    /* 模式切换 */
    if (ri->model_changed) {
        ri->model_changed = 0;
        ri->game_count = 1;
        ri->white_win_count = 0;
        ri->black_win_count = 0;
        ri->game_model = !ri->game_model;
    }
    /* 清空棋盘数据 */
    for (i = 0; i < ASIZE; i++)
        for (j = 0; j < ASIZE; j++)
            ri->chess[i][j] = 0;
}

/* 显示游戏界面 */
static void draw_face(Reversi *ri)
{
    // 设置窗口
    clrscr();
    set_origin(0, 0);
    set_axis(1, 1);
    set_size(ASIZE * 9, ASIZE * 2 + 3);
    fill_color(BK_CLR, TSTR_CLR);
    set_title("控制台小游戏 - 黑白棋");
    // 绘制棋盘
    draw_board();
    set_chess(ri, ASIZE / 2, ASIZE / 2, BLACK_CHESS);
    set_chess(ri, ASIZE / 2 - 1, ASIZE / 2 - 1, BLACK_CHESS);
    set_chess(ri, ASIZE / 2 - 1, ASIZE / 2, WHITE_CHESS);
    set_chess(ri, ASIZE / 2, ASIZE / 2 - 1, WHITE_CHESS);
    // 屏幕上的相关信息显示
    set_color(BK_CLR, TSTR_CLR);
    gotoxy(40, 1);   strprint("控制台小游戏 -- 黑白棋");
    set_color(BK_CLR, ASTR_CLR);
    gotoxy(38, 2);   strprint("作者：MXB");
    gotoxy(38, 3);   strprint("日期：2018-4-3");
    set_color(BK_CLR, FSTR_CLR);
    gotoxy(38, 4);   strprint("操作：");
    gotoxy(38, 5);   strprint("    WSAD/上下左右 ：移动");
    gotoxy(38, 6);   strprint("    空格：落子  Q键：返回主菜单");
    gotoxy(38, 7);   strprint("    P键： 切换模式");
    show_game_info(ri);       // 显示游戏信息
    set_cursorxy(ri->cursor_x, ri->cursor_y);
}

/* 玩家落子 */
static int player_down(Reversi *ri)
{
    int down_succeed = 0;

    if (ri->chess_count % 2) {
        if (chess_can_down_pos(ri, ri->cursor_x, ri->cursor_y, WHITE_CHESS)) {
            set_chess(ri, ri->cursor_x, ri->cursor_y, WHITE_CHESS);
            ri->chess_count++;
            change_chess(ri, ri->cursor_x, ri->cursor_y);

            if (ri->black_cannt_down = !chess_can_down(ri, BLACK_CHESS))
                ri->chess_count++;

            show_game_info(ri);
            down_succeed = 1;
        }
    } else {
        if (chess_can_down_pos(ri, ri->cursor_x, ri->cursor_y, BLACK_CHESS)) {
            set_chess(ri, ri->cursor_x, ri->cursor_y, BLACK_CHESS);
            ri->chess_count++;
            change_chess(ri, ri->cursor_x, ri->cursor_y);

            if (ri->white_cannt_down = !chess_can_down(ri, WHITE_CHESS))
                ri->chess_count++;

            show_game_info(ri);
            down_succeed = 1;
        }
    }
    if (!chess_can_down(ri, WHITE_CHESS) && !chess_can_down(ri, BLACK_CHESS))
        ri->winner = 1;

    return down_succeed;
}

/* 电脑落子 */
// 落子策略：电脑始终在当前最优的位置落子，保证当前获得最多的棋子
static void computer_down(Reversi *ri)
{
    int i, num, max_num = 0;
    int x, aix[8] = { 1, -1, 1, -1, 0, 0, 1, -1 };
    int y, aiy[8] = { 1, -1, 0, 0, 1, -1, -1, 1 };

    sleep(300);
    for (x = 0; x < ASIZE; x++)
        for (y = 0; y < ASIZE; y++) {
            if (ri->chess[x][y])
                continue;

            for (num = 0, i = 0; i < 8; i++) {
                num += num_chess_pos(ri, x, y, WHITE_CHESS, aix[i], aiy[i]);
                if (max_num < num) {
                    max_num = num;
                    ri->cursor_x = x;
                    ri->cursor_y = y;
                }
            }
        }
    player_down(ri);
}

/* 能否在某个位置上翻转棋子 */
static int chess_can_down_pos(Reversi *ri, int x, int y, int type)
{
    if (x < 0 || x >= ASIZE || y < 0 || y >= ASIZE || ri->chess[x][y])
        return 0;
    if (num_chess_pos(ri, x, y, type, 1, 1) || num_chess_pos(ri, x, y, type, -1, -1)||
        num_chess_pos(ri, x, y, type, 0, 1) || num_chess_pos(ri, x, y, type, 0, -1) ||
        num_chess_pos(ri, x, y, type, 1, 0) || num_chess_pos(ri, x, y, type, -1, 0) ||
        num_chess_pos(ri, x, y, type, -1, 1)|| num_chess_pos(ri, x, y, type, 1, -1))
        return 1;
    return 0;
}

/* 翻转棋子 */
static void change_chess(Reversi *ri, int x, int y)
{
    int i, aix[8] = { 1, -1, 1, -1, 0, 0, 1, -1 };
    int j, aiy[8] = { 1, -1, 0, 0, 1, -1, -1, 1 };

    for (i = 0; i < 8; i++) {
        int num = num_chess_pos(ri, x, y, ri->chess[x][y], aix[i], aiy[i]);
        for (j = 1; j <= num; j++) 
            set_chess(ri, x + aix[i] * j, y + aiy[i] * j, ri->chess[x][y]);
    }
}

/* 某方能在某方向上翻转的棋子数 */
static int num_chess_pos(Reversi *ri, int x, int y, int chess_type, int px, int py)
{
    int n, dx, dy;

    for (n = 1; ; n++) {
        dx = x + n * px, dy = y + n * py;
        if (dx < 0 || dx >= ASIZE || dy < 0 || dy >= ASIZE || !ri->chess[dx][dy])
            return 0;
        if (ri->chess[dx][dy] == chess_type)
            break;
    }
    return n - 1;
}

/* 某方是否还能落子 */
static int chess_can_down(Reversi *ri, int chess_type)
{
    int i, j;

    for (i = 0; i < ASIZE; i++)
        for (j = 0; j < ASIZE; j++)
            if (chess_can_down_pos(ri, i, j, chess_type))
                return 1;
    return 0;
}

/* 按键处理 */
static void resolve_key(Reversi *ri)
{
    switch (get_key(1)) {
        // 方向键/WSAD键也可以移动
    case KEY_UP:
    case 'W': ri->cursor_y = (ri->cursor_y + ASIZE - 1) % ASIZE; break;
    case KEY_DOWN:
    case 'S': ri->cursor_y = (ri->cursor_y + 1) % ASIZE; break;
    case KEY_LEFT:
    case 'A': ri->cursor_x = (ri->cursor_x + ASIZE - 1) % ASIZE; break;
    case KEY_RIGHT:
    case 'D': ri->cursor_x = (ri->cursor_x + 1) % ASIZE; break;
        // 功能键
    case 'Q': ri->is_return = 1; return;       // 返回主菜单
    case 'P': ri->model_changed = 1; return;   // 改变模式
    case ' ':            // 落子
        if (player_down(ri) && ri->game_model)
            computer_down(ri);
        return;
    }
    set_cursorxy(ri->cursor_x, ri->cursor_y);
}

/* 将某位置放置为某方棋子 */
static void set_chess(Reversi *ri, int x, int y, int chess_type)
{
    if (chess_type == BLACK_CHESS) {
        ri->black_count++;

        if (ri->chess[x][y] == WHITE_CHESS)
            ri->white_count--;
        set_color(GRID_BK_CLR, BLACK);
    } else {
        ri->white_count++;

        if (ri->chess[x][y] == BLACK_CHESS)
            ri->black_count--;

        set_color(GRID_BK_CLR, WHITE);
    }
    ri->chess[x][y] = chess_type;
    set_cursorxy(x, y);    strprint("●");
    set_cursorxy(x, y);
}

/* 将光标移动到棋盘坐标(x, y)处 */
static void set_cursorxy(int x, int y)
{
    gotoxy(4 * x + 4, 2 * y + 2);
}

/* 显示游戏数据信息 */
static void show_game_info(Reversi *ri)
{
    set_color(BK_CLR, ISTR_CLR);
    gotoxy(38, 8);
    if (ri->game_model)
        strprint("当前模式：人机模式");
    else
        strprint("当前模式：双人模式");
    gotoxy(38, 9);

    strprint("当前局数：第 %-2d 局", ri->game_count);
    gotoxy(38, 10);
    strprint("白子获胜：   %-2d 局", ri->white_win_count);
    gotoxy(38, 11);
    strprint("黑子获胜：   %-2d 局", ri->black_win_count);
    gotoxy(38, 12);
    set_color(BK_CLR, OSTR_CLR);

    if (!ri->winner && ri->black_cannt_down)
        strprint("黑子无路可走，被迫弃子");
    else if (!ri->winner && ri->white_cannt_down)
        strprint("白子无路可走，被迫弃子");
    else
        strprint("                     ");

    set_color(BK_CLR, ISTR_CLR);
    gotoxy(38, 13);
    if (ri->chess_count % 2)
        strprint("等待落子：白子");
    else
        strprint("等待落子：黑子");

    gotoxy(38, 14);
    strprint("白子数： %-2d 黑子数： %-2d  ", ri->white_count, ri->black_count);
    gotoxy(38, 15);
    if (ri->white_count > ri->black_count)
        strprint("白棋领先");
    else if (ri->white_count < ri->black_count)
        strprint("黑棋领先");
    else
        strprint("        ");
    set_cursorxy(ri->cursor_x, ri->cursor_y);
}

/* 绘制棋盘 */
static void draw_board(void)
{
    int i, j;

    set_color(GRID_BK_CLR, GRID_CLR);
    for (i = 0; i <= 2 * ASIZE; i++) {
        for (j = 0; j <= 2 * ASIZE; j++) {
            gotoxy(j * 2 + 2, i + 1);
            if (i == 0 && j == 0)                      strprint("┌");
            else if (i == 2 * ASIZE && j == 2 * ASIZE) strprint("┘");
            else if (i == 0 && j == 2 * ASIZE)         strprint("┐");
            else if (i == 2 * ASIZE && j == 0)         strprint("└");
            else if (i % 2 == 0 && j % 2 == 1)         strprint("─");
            else if (i % 2 == 1 && j % 2 == 0)         strprint("│");
            else if (i == 0 && j % 2 == 0)             strprint("┬");
            else if (i == 2 * ASIZE && j % 2 == 0)     strprint("┴");
            else if (i % 2 == 0 && j == 0)             strprint("├");
            else if (i % 2 == 0 && j == 2 * ASIZE)     strprint("┤");
            else if (i % 2 == 0 && j % 2 == 0)         strprint("┼");
            else                                       strprint("　");
        }
    }
}

/* 当前游戏结束 */
static void game_over(Reversi *ri)
{
    int ch;

    set_color(BK_CLR, OSTR_CLR);
    gotoxy(38, 15);
    strprint("游戏结束！  ");
    ri->game_count++;

    if (ri->white_count > ri->black_count) {
        strprint("白子获胜！  ");
        ri->white_win_count++;
    } else if (ri->white_count < ri->black_count) {
        strprint("黑子获胜！  ");
        ri->black_win_count++;
    } else
        strprint("平局！      ");

    set_color(OBK_CLR, OSTR_CLR);
    gotoxy(38, 16);
    strprint(" 按 回车键 键进入下一局 ");
    gotoxy(38, 17);
    strprint(" 按 Q键 返回主菜单      ");

    while ((ch = get_key(1)) != KEY_ENTER)          // 等待按键
        if (ch == 'Q') {
            ri->is_return = 1;                      // 按 Q键退出程序
            break;
        }
}
