#define  IN_CONSOLE_GAMES
#define  IN_GOBANG_C
#include "gobang.h"

/* 主函数 */
int main(void)
{
    Gobang gb; 
    gb.is_first = 1;

    while (1) {                // 游戏大循环
        init_data(&gb);        // 数据初始化
        draw_face(&gb);        // 绘制游戏界面
                               // 当赢家产生或者游戏模式被改变时退出循环
        while (!gb.is_return && !gb.winner && !gb.model_changed)
            resolve_key(&gb);  // 处理按键，游戏逻辑由按键决定
        if (gb.winner)
            game_over(&gb);    // 赢家产生，当前游戏结束
        if (gb.is_return)      // 返回主菜单
            break;
    }

    return 0;
}

/* 变量初始化 */
static void init_data(Gobang *gb)
{
    int i, j;
    
    gb->cursor_x = ASIZE / 2;
    gb->cursor_y = ASIZE / 2;
    gb->winner = 0;
    gb->is_return = 0;
    gb->chess_count = 0;

    if (gb->is_first) {
        gb->is_first = 0;
        gb->game_model = 0;
        gb->model_changed = 1;
    }
    
    if (gb->model_changed) {
        gb->model_changed = 0; 
        gb->white_win_count = 0;
        gb->game_count = 1; 
        gb->black_win_count = 0;
        gb->game_model = !gb->game_model;
    }
    // 清空棋盘数据
    for (i = 0; i < ASIZE; i++)
        for (j = 0; j < ASIZE; j++)
            gb->undo_chess[i * ASIZE + j] = gb->chess[i][j] = 0;
}

/* 显示游戏界面 */
static void draw_face(Gobang *gb)
{
    int i, j;

    set_title("控制台小游戏 - 五子棋");
    set_origin(2, 1);
    set_axis(2, 1);
    set_size(ASIZE * 2 + 5, ASIZE + 2);
    fill_color(BK_CLR, TSTR_CLR);
    // 绘制棋盘
    for (i = 0; i < ASIZE; i++) {
        for (j = 0; j < ASIZE; j++)
            draw_grid(j, i);
        strprint("\n");
    }
    // 屏幕上的相关信息显示
    set_color(BK_CLR, TSTR_CLR);
    gotoxy(ASIZE + 3, 0); strprint("控制台小游戏 -- 五子棋");
    set_color(BK_CLR, ASTR_CLR);
    gotoxy(ASIZE + 1, 1); strprint("作者：Ma Xiaobo");
    gotoxy(ASIZE + 1, 2); strprint("日期：2018-4");
    set_color(BK_CLR, FSTR_CLR);
    gotoxy(ASIZE + 1, 3); strprint("操作：");
    gotoxy(ASIZE + 1, 4); strprint("    WSAD/上下左右 ：移动");
    gotoxy(ASIZE + 1, 5); strprint("    空格：落子      Z键：撤销");
    gotoxy(ASIZE + 1, 6); strprint("    P键： 切换模式  Q键：返回主菜单");
    show_game_info(gb);       // 显示游戏信息
}

/* 玩家落子 */
static int player_down(Gobang *gb, int x, int y)
{
    if (gb->chess[x][y])
        return 0;

    if (gb->chess_count % 2) {
        set_color(GRID_BK_CLR, WHITE);
        gb->chess[x][y] = WHITE_CHESS;
    } else {
        set_color(GRID_BK_CLR, BLACK);
        gb->chess[x][y] = BLACK_CHESS;
    }

    // 将坐标记录下来，方便悔棋
    gb->undo_chess[gb->chess_count++] = x * 100 + y;
    gotoxy(x, y);
    strprint("●\a");
    show_game_info(gb);
    // 如果当前的连子数达到5个，则胜负已分，当前落子的一方获胜
    if (num_gobang_of_pos_xy(gb, x, y, gb->chess[x][y]) >= 5)
        gb->winner = gb->chess[x][y];

    return 1;
}

/* 电脑落子 */
// 落子策略：电脑始终在能使白子或黑子形成连子最多的地方落子
static void computer_down(Gobang *gb)
{
    int x, y, max_nums = 0, max_num = 0, num_white, num_black;

    sleep(300);                      // 短暂的延时，假装电脑在“思考”
    for (x = 0; x < ASIZE; x++)      // 扫描整个棋盘
        for (y = 0; y < ASIZE; y++) {
            if (gb->chess[x][y])    // 若此处已经有棋子，便不能在此落子，跳过
                continue;

            num_white = num_gobang_of_pos_xy(gb, x, y, WHITE_CHESS); // 能够连成白子的数目
            num_black = num_gobang_of_pos_xy(gb, x, y, BLACK_CHESS); // 能够连成黑子的数目
            max_num = num_white > num_black ? num_white : num_black; // 两种棋子数目的最大值

            if (max_nums < max_num) {
                max_nums = max_num;     // 将当前的最大值设置为总的最大值
                gb->cursor_x = x;       // 将当前坐标暂定为目标坐标
                gb->cursor_y = y;
            }
        }
    player_down(gb, gb->cursor_x, gb->cursor_y); // 接下来的操作与玩家落子是一样的
}

/* 计算位置(x, y)可以形成type(白子或黑子)类棋子的连子个数 */
// 这是游戏的核心算法部分
static int num_gobang_of_pos_xy(Gobang *gb, int x, int y, int type)
{
    int aix[] = { 1, -1, 0, 0, 1, -1, 1, -1 };  // 表示从点(x, y)开始的八个方向
    int aiy[] = { 0, 0, 1, -1, 1, -1, -1, 1 };  // 表示从点(x, y)开始的八个方向
    int tpx, tpy, a, b, i, n, max_num = 0;      // n记录了已经统计到的棋子数
     
    for (b = 0; b < 8; b += 2) {                // 四个大方向，每次的n会被重置为1
        for (n = 1, a = 0; a < 2; a++)          // 相反的两个方向，n不被重置，继续累加
            for (i = 1; ; i++, n++) {           // 累加同类型的棋子
                tpx = x + i * aix[a + b];
                tpy = y + i * aiy[a + b];
                if (tpx < 0 || tpx >= ASIZE || tpy < 0 || tpy >= ASIZE ||
                    gb->chess[tpx][tpy] != type)
                    break;      // 如果新位置与原棋子不同或者没有棋子便终止扫描
            }
        if (max_num < n)        // 得到四个大方向中每个方向连子的最大值
            max_num = n;
    }

    return max_num;
}

/* 按键处理 */
static void resolve_key(Gobang *gb)
{
    switch (get_key(1)) {
    case KEY_UP:
    case 'W': gb->cursor_y = (gb->cursor_y + ASIZE - 1) % ASIZE; break;
    case KEY_DOWN:
    case 'S': gb->cursor_y = (gb->cursor_y + 1) % ASIZE; break;
    case KEY_LEFT:
    case 'A': gb->cursor_x = (gb->cursor_x + ASIZE - 1) % ASIZE; break;
    case KEY_RIGHT:
    case 'D': gb->cursor_x = (gb->cursor_x + 1) % ASIZE; break;
        // 功能键
    case 'Q': gb->is_return = 1; return;       // 返回主菜单
    case 'P': gb->model_changed = 1; return;   // 改变模式
    case 'Z':  // 悔棋
        undo(gb);
        if (gb->game_model)  // 人机对战模式下要撤销电脑下的子
            undo(gb);
        return;
    case KEY_SPACE:          // 落子
        if (player_down(gb, gb->cursor_x, gb->cursor_y) && gb->game_model)
            computer_down(gb);
        return;
    }

    gotoxy(gb->cursor_x, gb->cursor_y);
}

/* 悔棋 */
static void undo(Gobang *gb)
{
    if (gb->chess_count <= 0)       // 没有棋可悔，直接返回
        return;

    gb->chess_count--;
    gb->cursor_x = gb->undo_chess[gb->chess_count] / 100;
    gb->cursor_y = gb->undo_chess[gb->chess_count] % 100;
    gb->chess[gb->cursor_x][gb->cursor_y] = 0;
    draw_grid(gb->cursor_x, gb->cursor_y);  // 用棋盘覆盖掉棋子
    show_game_info(gb);    // 更新数据
}

/* 绘制棋盘(x, y)处 （由于不同位置上的符号不一样，因此提供了这个函数） */
static void draw_grid(int x, int y)
{
    xycolor(x, y, GRID_BK_CLR, GRID_CLR);
    if (y == 0 && x == 0)              strprint("┌");
    else if (y == 0 && x == ASIZE - 1) strprint("┐");
    else if (y == ASIZE - 1 && x == 0) strprint("└");
    else if (y == ASIZE - 1 && x == ASIZE - 1) strprint("┘");
    else if (y == ASIZE / 2 && x == ASIZE / 2) strprint("╋");
    else if (y == MSIZE && x == MSIZE) strprint("╋");
    else if (y == LSIZE && x == LSIZE) strprint("╋");
    else if (y == LSIZE && x == MSIZE) strprint("╋");
    else if (y == MSIZE && x == LSIZE) strprint("╋");
    else if (y == 0)            strprint("┬");
    else if (y == ASIZE - 1)    strprint("┴");
    else if (x == 0)            strprint("├");
    else if (x == ASIZE - 1)    strprint("┤");
    else                        strprint("┼");
}

/* 显示游戏数据信息 */
static void show_game_info(Gobang *gb)
{
    set_color(BK_CLR, ISTR_CLR);
    gotoxy(ASIZE + 1, 7);
    if (gb->game_model)
        strprint("当前模式：人机模式");
    else
        strprint("当前模式：双人模式");

    gotoxy(ASIZE + 1, 8);
    strprint("当前局数：第 %-2d 局", gb->game_count);
    gotoxy(ASIZE + 1, 9);
    strprint("白子获胜：   %-2d 局", gb->white_win_count);
    gotoxy(ASIZE + 1, 10);
    strprint("黑子获胜：   %-2d 局", gb->black_win_count);
    gotoxy(ASIZE + 1, 11);
    strprint("当前步数：第 %-3d步 ", gb->chess_count);

    gotoxy(ASIZE + 1, 12);
    if (gb->chess_count % 2)
        strprint("等待落子：白子");
    else
        strprint("等待落子：黑子");
    gotoxy(gb->cursor_x, gb->cursor_y);
}

/* 当前游戏结束 */
static void game_over(Gobang *gb)
{
    int ch;
    set_color(BK_CLR, OSTR_CLR);
    gotoxy(ASIZE + 1, 13);
    strprint("游戏结束！  ");
    gb->game_count++;
    if (gb->winner == WHITE_CHESS) {
        strprint("白子获胜！  ");
        gb->white_win_count++;
    } else {
        strprint("黑子获胜！  ");
        gb->black_win_count++;
    }
    set_color(OBK_CLR, OSTR_CLR);
    gotoxy(ASIZE + 1, 14);
    strprint(" 按 回车键 键进入下一局 ");
    gotoxy(ASIZE + 1, 15);
    strprint(" 按 Q键 键返回主菜单    ");
    
    while ((ch = get_key(1)) != KEY_ENTER)  // 等待按键
        if (ch == 'Q') {
            gb->is_return = 1;              // 按 Q键返回主菜单
            break;
        }
}
