#define IN_CONSOLE_GAMES

#define IN_TETRIS_C
#include "tetris.h"

/* 方块的形状，颜色，方向数据表 */
static const int tetris[7][4][9] = {           
    {{10, -1,0,0,0,0,1,1,1}, {10, -1,1,-1,2,0,0,0,1},  // S
     {10, -1,0,0,0,0,1,1,1}, {10, -1,1,-1,2,0,0,0,1}},
    {{12, -1,1,0,0,0,1,1,0}, {12, -1,0,-1,1,0,1,0,2},  // Z
     {12, -1,1,0,0,0,1,1,0}, {12, -1,0,-1,1,0,1,0,2}},
    {{14, 0,0,0,1,0,2,1,0},  {14, -1,0,-1,1,0,1,1,1},  // L
     {14, 0,2,1,0,1,1,1,2},  {14, -1,0,0,0,1,0,1,1 }},
    {{6,  -1,0,0,0,0,1,0,2}, {6,  -1,0,-1,1,0,0,1,0},  // J
     {6,  0,0,0,1,0,2,1,2},  {6,  -1,1,0,1,1,0,1,1 }},
    {{11, -1,0,0,0,1,0,2,0}, {11, 0,0,0,1,0,2,0,3  },  // I
     {11, -1,0,0,0,1,0,2,0}, {11, 0,0,0,1,0,2,0,3  }},
    {{9,  0,0,0,1,1,0,1,1 }, {9,  0,0,0,1,1,0,1,1  },  // O
     {9,  0,0,0,1,1,0,1,1 }, {9,  0,0,0,1,1,0,1,1  }},
    {{13, -1,1,0,0,0,1,1,1}, {13, -1,1,0,0,0,1,0,2 },  // T
     {13, -1,0,0,0,0,1,1,0}, {13, 0,0,0,1,0,2,1,1  }}
};

/* 主函数*/
int main(void)
{
    Tetris ts;
    ts.is_first = 1;
    srand((unsigned)time(NULL));

    while (1) {
        init_data(&ts);        // 初始化全局数据
        draw_face(&ts);        // 绘制界面
        while (!ts.winner && !ts.is_return) {
            resolve_key(&ts);  // 处理按键
            tetris_down(&ts);  // 方块下落
        }
        if (ts.winner)
            game_over(&ts);    // 游戏结束
        if (ts.is_return)
            break;          // 返回主菜单
    }
    return 0;
}


/* 初始化全局数据 */
static void init_data(Tetris *ts)
{
    int i, j;

    ts->cursor_y = -1;
    ts->cursor_x = SIZE_X / 2;
    ts->next_type = -1;
    ts->next_direction = -1;
    ts->winner = 0;
    ts->score= 0;
    ts->pause_time = 30;
    ts->key_down = 0;
    ts->is_return = 0;

    if (ts->is_first) {
        ts->is_first = 0;
        ts->best_score = 0;
        ts->game_count = 1;
        ts->move_direction = 0;
    }

    for (i = 0; i < SIZE_X; i++)
        for (j = 0; j < SIZE_Y; j++)
            ts->map[i][j] = 0;
}

/* 绘制界面 */
static void draw_face(Tetris *ts)
{
    int i, j;
    // 窗口设置

    set_size(70, 24);
    set_axis(2, 1);
    set_origin(4, 2);
    set_title("控制台小游戏 - 俄罗斯方块");
    fill_color(BK_CLR, TSTR_CLR);

    for (i = -1; i <= SIZE_X; i++)
        for (j = -1; j <= SIZE_Y; j++) {
            gotoxy(i, j);
            if (i == -1 || j == -1 || i == SIZE_X || j == SIZE_Y) {
                set_color(WALL_BK_CLR, WALL_CLR);
                strprint("▓");
            } else {
                set_color(TETRIS_BK_CLR, BLACK);
                strprint("  ");
            }
        }
    for (i = 0; i <= 5; i++)
        for (j = -1; j <= SIZE_Y; j++) {
            gotoxy(SIZE_X + i + 1, j);
            if (j == -1 || i == 5 || j == SIZE_Y) {
                set_color(WALL_BK_CLR, WALL_CLR);
                strprint("▓");
            }
            else if (j == 1 || j == 7 || j == 9 || j == 13 || j == 15) {
                set_color(WALL_BK_CLR, WALL_CLR);
                strprint("▓");
            }
            else {
                set_color(TETRIS_BK_CLR, BLACK);
                strprint("  ");
            }
        }

    set_color(BK_CLR, TSTR_CLR);
    gotoxy(18, 0);   strprint("控制台小游戏 -- 俄罗斯方块");
    set_color(BK_CLR, ASTR_CLR);
    gotoxy(18, 2);   strprint("作者：MXB");
    gotoxy(18, 3);   strprint("日期：2018-4-7");
    set_color(BK_CLR, FSTR_CLR);
    gotoxy(18, 5);   strprint("操作：");
    gotoxy(18, 6);   strprint("    AD/左右方向键：左右移动");
    gotoxy(18, 7);   strprint("    P：暂停/继续");
    gotoxy(18, 8);   strprint("    S/下方向键：加速移动");
    gotoxy(18, 9);   strprint("    回车： 转向  Q：返回主菜单");
    show_game_info(ts);       // 显示游戏信息
}

/* 生成一个方块 */
static void create_tetris(Tetris *ts)
{
    if (ts->next_type == -1) {    // （最开始时）生成一个方块
        ts->type = rand() % 7;
        ts->direction = rand() % 4;
    }
    else {
        ts->type = ts->next_type;
        ts->direction = ts->next_direction;
        draw_tetris(SIZE_X + 3, 5, ts->next_type, ts->next_direction, 0);
    }
    // 生成下一个方块
    ts->next_type = rand() % 7;
    ts->next_direction = rand() % 4;
    draw_tetris(SIZE_X + 3, 5, ts->next_type, ts->next_direction, 1);
}

/* 方块是否能够继续下落 */
static int is_tetris_can_move(Tetris *ts, int dx, int dy, int ddir)
{
    int i, x, y;

    if (ts->cursor_y >= SIZE_Y - 1)   // 方块到了最底部
        return 0;
    for (i = 1; i < 9; i += 2) {
        x = ts->cursor_x + tetris[ts->type][(4 + ts->direction + ddir) % 4][i] + dx;
        y = ts->cursor_y - tetris[ts->type][(4 + ts->direction + ddir) % 4][i + 1] + dy;
        if ((x < 0 || x >= SIZE_X) || (y >= 0 && y < SIZE_Y && ts->map[x][y]))
            return 0;           // 下面已经有方块了，不可再下落
    }
    return 1;
}

/* 方块下落 */
static void tetris_down(Tetris *ts)
{
    static int n; 
    n = (n + 1) % 14;

    if (ts->cursor_y == -1)   // 生成一个方块（新局）
        create_tetris(ts);

    // 向下移一行
    if (ts->key_down || !n || ts->cursor_y == -1) {
        // 无法移动
        if (!is_tetris_can_move(ts, 0, 1, 0)) {
            save_tetris_map(ts);
            if (ts->winner)
                return;

            clear_tetris(ts);
            ts->cursor_y = -1;
            ts->cursor_x = SIZE_X / 2;
            return;
        }
        draw_tetris(ts->cursor_x, ts->cursor_y, ts->type, ts->direction, 0);
        ts->cursor_y++;
        draw_tetris(ts->cursor_x, ts->cursor_y, ts->type, ts->direction, 1);
    }

    // 左右移动，翻转
    if (ts->move_direction) {
        draw_tetris(ts->cursor_x, ts->cursor_y, ts->type, ts->direction, 0);
        if (ts->move_direction == 1 && is_tetris_can_move(ts, -1, 0, 0))
            ts->cursor_x--;
        else if (ts->move_direction == 2 && is_tetris_can_move(ts, 1, 0, 0))
            ts->cursor_x++;
        else if (ts->move_direction == 3 && is_tetris_can_move(ts, 0, 0, 1))
            ts->direction = (ts->direction + 1) % 4;
        ts->move_direction = 0;
        draw_tetris(ts->cursor_x, ts->cursor_y, ts->type, ts->direction, 1);
    }

    sleep(ts->pause_time);
}

/* 保存已下落的方块 */
static void save_tetris_map(Tetris *ts)
{
    int i, x, y;

    for (i = 1; i < 9; i += 2) {
        x = ts->cursor_x + tetris[ts->type][ts->direction][i];
        y = ts->cursor_y - tetris[ts->type][ts->direction][i + 1];
        if (y < 0) {    // 方块已经溢出，游戏结束
            ts->winner = 1;
            ts->game_count++;
            return;
        }
        // 保存方块
        ts->map[x][y] = tetris[ts->type][ts->direction][0];
    }
}

/* 清除已经满行的方块 */
static void clear_tetris(Tetris *ts)
{
    int i, j, x, y, is_all, clear_count = 0;
    // 扫描当前方块所在位置的4行
    for (y = 0; y < 4; y++) {
        if (ts->cursor_y - y < 0)
            break;
        is_all = 1;   // 当前行全有方块的标志变量
        for (x = 0; x < SIZE_X; x++)
            if (ts->map[x][ts->cursor_y - y] == 0)
                is_all = 0;  // 扫描到空位，当前行未满
        if (is_all) {        // 如果当前行满了，消除该行
            clear_count++;   // 记录已消的行数
            for (i = ts->cursor_y - y - 1; i >= 0; i--)  // 将上面是有的方块下移一行
                for (j = 0; j < SIZE_X; j++) {
                    ts->map[j][i + 1] = ts->map[j][i];
                    gotoxy(j, i + 1);
                    set_color(TETRIS_BK_CLR, ts->map[j][i]);
                    if (ts->map[j][i])
                        strprint("█");
                    else
                        strprint("  ");
                }
            y--;
        }
    }
    if (clear_count) {    // 计算得分
        ts->score += 1000 * (2 * clear_count - 1);
        if (ts->score > ts->best_score)
            ts->best_score = ts->score;
        show_game_info(ts);
    }
}

/* 绘制方块 */
static void draw_tetris(int x, int y, int the_tetris, int dir, int show)
{
    int i;
    char *str;

    if (show)  str = "█";
    else       str = "  ";
    set_color(TETRIS_BK_CLR, tetris[the_tetris][dir][0]);
    for (i = 1; i < 9; i += 2)
        if (y - tetris[the_tetris][dir][i + 1] >= 0) {
            gotoxy(x + tetris[the_tetris][dir][i], y - tetris[the_tetris][dir][i + 1]);
            strprint(str);
        }
}

/* 按键处理 */
static void resolve_key(Tetris *ts)
{
    ts->key_down = 0;

    if (!have_key())
        return;
    switch (get_key(1)) {
    // 方向键/WSAD 键
    case KEY_LEFT:  case 'A': ts->move_direction = 1; break;
    case KEY_RIGHT: case 'D': ts->move_direction = 2; break;
    case KEY_DOWN:  case 'S': ts->key_down = 1;    break;
    // 功能
    case KEY_SPACE: ts->move_direction = 3; break; // 翻转
    case 'Q': ts->is_return = 1; return;           // 返回主菜单
    case 'P':                                      // 暂停/继续
        while (get_key(1) != 'P');
        break;
    }
}

/* 显示游戏信息 */
static void show_game_info(Tetris *ts)
{
    set_color(TETRIS_BK_CLR, LIGHT_BLUE);
    gotoxy(SIZE_X + 1, 0);  strprint("  下一个  ");
    gotoxy(SIZE_X + 1, 8);  strprint("   得分   ");
    gotoxy(SIZE_X + 1, 14); strprint("  最高分  ");
    gotoxy(SIZE_X + 1, 11); strprint("  %d", ts->score);
    gotoxy(SIZE_X + 1, 17); strprint("  %d", ts->best_score);
    set_color(BK_CLR, LIGHT_BLUE);
    gotoxy(22, 12); strprint("第  %-2d  局", ts->game_count);
}

/* 游戏结束 */
static void game_over(Tetris *ts)
{
    int ch;

    set_color(BK_CLR, OSTR_CLR);
    gotoxy(18, 14); strprint("游戏结束");
    set_color(OBK_CLR, OSTR_CLR);
    gotoxy(18, 15); strprint("回车键： 开始新局  ");
    gotoxy(18, 16); strprint("Q键：返回主菜单    ");

    while ((ch = get_key(1)) != KEY_ENTER)    // 回车键按下
        if (ch == 'Q') {
            ts->is_return = 1;                // 返回主菜单
            break;
        }
}
 