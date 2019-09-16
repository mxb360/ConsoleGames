#define IN_CONSOLE_GAMES

#define IN_SWEEPER_C
#include "sweeper.h"

/* 主函数 */
int main(void)
{
    Sweeper sp;
    sp.is_first = 1;
    sp.is_return = 0;

    srand((unsigned)time(NULL));

    choice_mode(&sp);          // 模式选择
    if (!sp.is_return)
        while (1) {
            init_data(&sp);        // 数据初始化
            draw_face(&sp);        // 绘制游戏界面
            while (!sp.winner && !sp.is_return) {   // 分出胜负，结束循环
                update_time(&sp);  // 更新时间
                resolve_key(&sp);  // 处理按键
            }
            if (sp.winner)
                game_over(&sp);    // 结束游戏
            if (sp.is_return)      // 返回主菜单
                break;
        }

    return 0;
}

/* 数据初始化 */
static void init_data(Sweeper *sp)
{
    int i, j;
    // 光标的初始坐标随机
    sp->cursor_x = rand() % sp->size_x;
    sp->cursor_y = rand() % sp->size_y;

    sp->count = 0;
    sp->winner = 0;
    sp->time = 0;
    sp->flag = 0;
    sp->is_return = 0;

    if (sp->is_first) {
        sp->is_first = 0;
        sp->replay = 0;
        sp->advanced_min_time = MAX_SCORE;
        sp->medium_min_time = MAX_SCORE;
        sp->primary_min_time = MAX_SCORE;
        sp->game_count = 1;
        sp->win_count = 0;
    }

    for (i = 0; i < sp->size_x; i++)
        for (j = 0; j < sp->size_y; j++)
            if (sp->replay && sp->sweep[i][j] > 2)
                sp->sweep[i][j] = 3;
            else
                sp->sweep[i][j] = 0;
}

/* 绘制游戏界面 */
static void draw_face(Sweeper *sp)
{
    int i, j, w, h;
    // 游戏窗口设置
    set_origin(4, 2);
    set_axis(2, 1);
    w = sp->size_x + 21;
    h = sp->size_y < 18 ? 22 : sp->size_x + 5;
    set_size(w, h);
    fill_color(BK_CLR, TSTR_CLR);
    set_title("控制台小游戏 - 扫雷");

    // 绘制并初始化棋盘
    for (i = -1; i <= sp->size_x; i++)
        for (j = -1; j <= sp->size_y; j++)
            if (i == -1 || i == sp->size_x || j == -1 || j == sp->size_y) {
                _set_color(96);
                gotoxy(i, j);
                strprint("  ");
            }
            else
                print_sweep_pos(sp, i, j, 0);
    // 屏幕上的相关信息显示
    gotoxy(-1, sp->size_y + 1);
    set_color(BK_CLR, ISTR_CLR);
    strprint("时间：%d秒", sp->time);
    gotoxy(sp->size_x - 4, sp->size_y + 1);
    strprint("雷数：%d个", sp->num_sweep - sp->flag);

    set_color(BK_CLR, TSTR_CLR);
    gotoxy(sp->size_x + 5, -1);  strprint("控制台小游戏 -- 扫雷");
    set_color(BK_CLR, ASTR_CLR);
    gotoxy(sp->size_x + 3, 0);   strprint("作者：MXB");
    gotoxy(sp->size_x + 3, 1);   strprint("日期：2018-4-5");
    set_color(BK_CLR, FSTR_CLR);
    gotoxy(sp->size_x + 3, 2);   strprint("操作：");
    gotoxy(sp->size_x + 3, 3);   strprint("    WSAD/上下左右 ：移动");
    gotoxy(sp->size_x + 3, 4);   strprint("    I键：点开  O键：全开");
    gotoxy(sp->size_x + 3, 5);   strprint("    P键：标记  Q键：返回主菜单");
    show_game_info(sp);       // 显示游戏信息
    gotoxy(sp->cursor_x, sp->cursor_y);
}

/* 初始化雷区 */
static void init_sweep(Sweeper *sp, int x, int y)
{
    int xx, yy, have_sweep_num = 0;
    // 随机产生坐标生成地雷
    while (have_sweep_num < sp->num_sweep) {
        xx = rand() % sp->size_x;
        yy = rand() % sp->size_y;
        if ((abs(x - xx) <= 1 && abs(y - yy) <= 1) || sp->sweep[xx][yy])
            continue;         // 保证雷不重复，第一个点开的格子及周围没有雷
        sp->sweep[xx][yy] = 3, have_sweep_num++;
    }
}

/* 更新时间 */
static void update_time(Sweeper *sp)
{
    static time_t old_t = 0;
    time_t t = time(NULL);

    if (sp->count && sp->time < 10000)
        sp->time += (int)(t - old_t);
    if (old_t != t) {
        gotoxy(2, sp->size_y + 1);
        set_color(BK_CLR, ISTR_CLR);
        strprint("%d秒", sp->time);
        gotoxy(sp->cursor_x, sp->cursor_y);
    }
    old_t = t;
}

/* 标记 */
static void flag_sweep(Sweeper *sp, int x, int y)
{
    if (sp->sweep[x][y] == 2)             // 已被标记
        sp->sweep[x][y] = 0, sp->flag--;  // 取消标记
    else if (sp->sweep[x][y] == 4)
        sp->sweep[x][y] = 3, sp->flag--;
    else if (sp->sweep[x][y] == 0)        // 未标记
        sp->sweep[x][y] = 2, sp->flag++;  // 标记
    else if (sp->sweep[x][y] == 3)
        sp->sweep[x][y] = 4, sp->flag++;
    else
        return;

    print_sweep_pos(sp, x, y, 0);
    set_color(BK_CLR, ISTR_CLR);
    gotoxy(sp->size_x - 1, sp->size_y + 1);
    strprint("%d个  ", sp->num_sweep - sp->flag);
}

/* 展开可以展开的格子 */
static void mines_sweep(Sweeper *sp, int x, int y)
{
    int i, this_conut = 0;
    int over_sweep[8] = { 0 };   // 记录周围八格的雷的情况
                                 // 用于遍历八个方向
    static const int aix[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    static const int aiy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

    if (sp->sweep[x][y] == 3) {    // 此格是雷，则游戏结束
        sp->winner = 1;
        sp->sweep[x][y] = 5;
        return;
    }
    if (sp->sweep[x][y] != 0)       // 已经点开或者已被标记
        return;

    if (!sp->replay && sp->count == 0)  // 第一次点开，需要初始化雷区
        init_sweep(sp, x, y);
    sp->replay = 0;

    sp->sweep[x][y] = 1;            // 表示已经点开
    sp->count++;

    for (i = 0; i < 8; i++)     // 扫描周围格子是否有雷
        if (x + aix[i] < 0 || x + aix[i] >= sp->size_x || y + aiy[i] < 0 || y + aiy[i] >= sp->size_y)
            over_sweep[i] = -1; // 越界
        else if (sp->sweep[x + aix[i]][y + aiy[i]] > 2)
            over_sweep[i] = 1, this_conut++;  // 记录雷
    print_sweep_pos(sp, x, y, this_conut);

    if (this_conut == 0)        // 如果周围没有雷，递归展开周围的格子
        for (i = 0; i < 8; i++)
            if (over_sweep[i] == 0 && sp->sweep[x + aix[i]][y + aiy[i]] == 0)
                mines_sweep(sp, x + aix[i], y + aiy[i]);
    gotoxy(sp->cursor_x, sp->cursor_y);
    // 如果所有不是雷的方块都被点开，游戏结束，玩家胜利
    if (sp->size_x*sp->size_y == sp->count + sp->num_sweep)
        sp->winner = 2;
}

// 点开所有未标记的方块
static void sweep_all(Sweeper *sp, int x, int y)
{
    int i, flag = 0, this_count = 0;
    // 用于遍历八个方向
    static const int aix[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    static const int aiy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

    if (sp->sweep[x][y] != 1)    // 这个格子没有点开
        return;

    for (i = 0; i < 8; i++)  // 遍历周围八个格子，找出总雷数和标记数
        if (x + aix[i] >= 0 && x + aix[i] < sp->size_x && y + aiy[i] >= 0 && y + aiy[i] < sp->size_y) {
            if (sp->sweep[x + aix[i]][y + aiy[i]] == 2 || sp->sweep[x + aix[i]][y + aiy[i]] == 4)
                flag++;
            if (sp->sweep[x + aix[i]][y + aiy[i]] > 2)
                this_count++;
        }

    if (this_count == flag)  // 如果总雷数和标记数相等，则展开所有没有标记的格子
        for (i = 0; i < 8; i++)
            if (x + aix[i] >= 0 && x + aix[i] < sp->size_x && y + aiy[i] >= 0 && y + aiy[i] < sp->size_y)
                mines_sweep(sp, x + aix[i], y + aiy[i]);
}

/* 按键处理 */
static void resolve_key(Sweeper *sp)
{
    if (!have_key())
        return;

    switch (get_key(1)) {
        // 方向键/WSAD键也可以移动
    case KEY_UP:
    case 'W': sp->cursor_y = (sp->cursor_y + sp->size_y - 1) % sp->size_y; break;
    case KEY_DOWN:
    case 'S': sp->cursor_y = (sp->cursor_y + 1) % sp->size_y; break;
    case KEY_LEFT:
    case 'A': sp->cursor_x = (sp->cursor_x + sp->size_x - 1) % sp->size_x; break;
    case KEY_RIGHT:
    case 'D': sp->cursor_x = (sp->cursor_x + 1) % sp->size_x; break;
        // 功能键
    case 'Q': sp->is_return = 1; return;                     // 返回主菜单
    case 'I': mines_sweep(sp, sp->cursor_x, sp->cursor_y);  return;  // 选择棋子，落子
    case 'P': flag_sweep(sp, sp->cursor_x, sp->cursor_y);   return;  // 标记
    case 'O': sweep_all(sp, sp->cursor_x, sp->cursor_y);    return;  // 点击全部未标记的方块
    }
    gotoxy(sp->cursor_x, sp->cursor_y);
}

/* 绘制雷区 */
static void print_sweep_pos(Sweeper *sp, int x, int y, int num)
{
    gotoxy(x, y);
    if (sp->sweep[x][y] == 0 || sp->sweep[x][y] == 3) { // 未开
        _set_color(42);
        strprint("█");
    }
    else if (sp->sweep[x][y] == 1) {              // 已开，无雷
        switch (num) {
        case 0: _set_color(119); strprint("　"); break;
        case 1: _set_color(121); strprint("１"); break;
        case 2: _set_color(114); strprint("２"); break;
        case 3: _set_color(124); strprint("３"); break;
        case 4: _set_color(112); strprint("４"); break;
        case 5: _set_color(116); strprint("５"); break;
        case 6: _set_color(115); strprint("６"); break;
        case 7: _set_color(117); strprint("７"); break;
        case 8: _set_color(117); strprint("８"); break;
        default: break;
        }
    }
    else if (sp->sweep[x][y] == 2 || sp->sweep[x][y] == 4) { // 标记
        _set_color(46);
        strprint("Ｐ");
    }
    gotoxy(x, y);
}

/* 绘制游戏结束后的雷区 */
static void print_sweep_face(Sweeper *sp)
{
    int i, j;

    for (i = 0; i < sp->size_x; i++)
        for (j = 0; j < sp->size_y; j++) {
            gotoxy(i, j);
            switch (sp->sweep[i][j]) {
            case 2: _set_color(36);  strprint("×"); break;  // 错误的标记
            case 4: _set_color(46);  strprint("★"); break;  // 正确的标记
            case 5: _set_color(202); strprint("★"); break;  // 点爆的雷
            case 3:                                         // 未点开，未标记的雷
                if (sp->winner == 1) _set_color(36);
                else                 _set_color(42);
                strprint("★");     break;
            default:                                        // 其他情况
                if (sp->sweep[i][j] != 1)
                    print_sweep_pos(sp, i, j, 0);
            }
        }
    gotoxy(sp->cursor_x, sp->cursor_y);
}

/* 显示游戏数据信息 */
static void show_game_info(Sweeper *sp)
{
    set_color(BK_CLR, ISTR_CLR);
    gotoxy(sp->size_x + 3, 6);
    switch (sp->model) {
    case PRIMARY_LEVEL: strprint("当前难度：初级");   break;
    case MEDIUM_LEVEL:  strprint("当前难度：中级");   break;
    case ADVANCE_LEVEL: strprint("当前难度：高级");   break;
    case USER_LEVEL:    strprint("当前难度：自定义 (%d×%d:%d)",
        sp->size_x, sp->size_y, sp->num_sweep); break;
    }
    gotoxy(sp->size_x + 3, 7);  strprint("当前局数：第 %-2d 局", sp->game_count);
    gotoxy(sp->size_x + 3, 8);  strprint("获胜局数：   %-2d 局", sp->win_count);
    gotoxy(sp->size_x + 3, 9);  strprint("最高记录：");
    gotoxy(sp->size_x + 3, 10);
    if (sp->primary_min_time < MAX_SCORE)
        strprint("    初级：%d秒", sp->primary_min_time);
    else
        strprint("    初级：无记录");
    gotoxy(sp->size_x + 3, 11);
    if (sp->medium_min_time < MAX_SCORE)
        strprint("    中级：%d秒", sp->medium_min_time);
    else
        strprint("    中级：无记录");
    gotoxy(sp->size_x + 3, 12);
    if (sp->advanced_min_time < MAX_SCORE)
        strprint("    高级：%d秒", sp->advanced_min_time);
    else
        strprint("    高级：无记录");
    gotoxy(sp->cursor_x, sp->cursor_y);
}

/* 当前游戏结束 */
static void game_over(Sweeper *sp)
{
    int ch, pfx;

    print_sweep_face(sp);
    set_color(BK_CLR, OSTR_CLR);
    sp->game_count++;
    gotoxy(sp->size_x + 3, 13);
    if (sp->winner == 1) {
        strprint("不好意思，你输了！");
    } else {
        gotoxy(sp->size_x + 3, 13); strprint("恭喜你，你赢了！  ");
        gotoxy(sp->size_x + 3, 14); strprint("耗时：%d秒。", sp->time);
        sp->win_count++;
        gotoxy(sp->size_x + 3, 15);
        // 是否打破记录
        if (sp->model == PRIMARY_LEVEL && sp->primary_min_time > sp->time) {
            sp->primary_min_time = sp->time;
            strprint("您所用的时间是这个级别中最短的！");
        } else if (sp->model == MEDIUM_LEVEL && sp->medium_min_time > sp->time) {
            sp->medium_min_time = sp->time;
            strprint("您所用的时间是这个级别中最短的！");
        } else if (sp->model == ADVANCE_LEVEL && sp->advanced_min_time > sp->time) {
            sp->advanced_min_time = sp->time;
            strprint("您所用的时间是这个级别中最短的！");
        }
    }

    set_color(OBK_CLR, OSTR_CLR);
    if (sp->winner == 1) {
        pfx = 14;
        gotoxy(sp->size_x + 3, pfx + 3);  strprint("N键 重新开始本局  ");
    } else
        pfx = 16;
    gotoxy(sp->size_x + 3, pfx);     strprint("回车键 再来一局   ");
    gotoxy(sp->size_x + 3, pfx + 1); strprint("M键 难度切换      ");
    gotoxy(sp->size_x + 3, pfx + 2); strprint("Q键 返回主菜单    ");

    while ((ch = get_key(1)) != KEY_ENTER)  // 等待按键
        if (ch == 'Q') {
            sp->is_return = 1;              // 按 Q键返回主菜单
            break;
        } else if (sp->winner == 1 && (ch == 'n' || ch == 'N')) {
            sp->replay = 1;                 // 重新开始本局游戏
            break;
        } else if (ch == 'm' || ch == 'M') {
            choice_mode(sp);                // 模式选择
            break;
        }
}

/* 选择模式 */
static void choice_mode(Sweeper *sp)
{
    char opt_buf[5][100];
    const char *opt_str[] = { opt_buf[0], opt_buf[1], opt_buf[2], opt_buf[3], opt_buf[4] };
    const char *title = "扫雷 - 模式选择(↑↓:移动选项  回车:确认  Q:返回主菜单)";

    set_axis(1, 1);
    set_size(WINDOW_WIDTH, WINDOW_HEIGHT);
    set_title("控制台小游戏 - 扫雷 - 模式选择");
    clrscr();
    fill_color(BK_CLR, BLUE);

    sprintf(opt_buf[0], "模式1： 初级 (宽度：%-2d  高度： %-2d  雷数： %-3d)",
        PRIMARY_X, PRIMARY_Y, PRIMARY_SWEEP);
    sprintf(opt_buf[1], "模式2： 中级 (宽度：%-2d  高度： %-2d  雷数： %-3d)",
        MEDIUM_X, MEDIUM_Y, MEDIUM_SWEEP);
    sprintf(opt_buf[2], "模式3： 高级 (宽度：%-2d  高度： %-2d  雷数： %-3d)",
        ADVANCE_X, ADVANCE_Y, ADVANCE_SWEEP);
    sprintf(opt_buf[3], "模式4： 自定义");
    sprintf(opt_buf[4], "                返回主菜单");

    set_options(title, opt_str, 5);
    switch (sp->model = get_options()) {
    case PRIMARY_LEVEL:
        sp->size_x = PRIMARY_X, sp->size_y = PRIMARY_Y, sp->num_sweep = PRIMARY_SWEEP; break;
    case MEDIUM_LEVEL:
        sp->size_x = MEDIUM_X, sp->size_y = MEDIUM_Y, sp->num_sweep = MEDIUM_SWEEP; break;
    case ADVANCE_LEVEL:
        sp->size_x = ADVANCE_X, sp->size_y = ADVANCE_Y, sp->num_sweep = ADVANCE_SWEEP; break;
    case USER_LEVEL: set_user_model(sp); break;
    default:
        sp->is_return = 1; break;
    }
}

/* 获取对宽度，高度，雷数的输入 仅在函数set_user_model中使用 */
static void _get_sweep_input(int *is_return, int i, const char *str, int *value, int min, int max)
{
    int ch;
    if (*is_return)
        return;
    set_color(BK_CLR, TSTR_CLR); gotoxy(15, 13 + i * 2);
    strprint("输入%s(%d-%d): ", str, min, max);
    while (scanf("%d", value) != 1 || *value < min || *value > max) {
        if ((ch = getchar()) == 'q' || ch == 'Q') {
            *is_return = 1;
            return;
        }
        fflush(stdin);
        gotoxy(17, 14 + i * 2); strprint("无效的输入！");
        sleep(500);
        gotoxy(17, 14 + i * 2); strprint("           ");
        gotoxy(23, 13 + i * 2); strprint("       ");
    }
    set_color(BK_CLR, ISTR_CLR); 
    gotoxy(17, 14 + i * 2); strprint("(%s：%d)", str, *value);
}

/* 用户自定义模式 */
static void set_user_model(Sweeper *sp)
{
    int x, y, m;

    set_origin(-25, -10);
    set_axis(2, 1);
    fill_color(BK_CLR, TSTR_CLR);
    clrscr();
    set_color(BK_CLR, TSTR_CLR); gotoxy(10, 10);
    strprint("扫雷小游戏 - 自定义模式： <Q + 回车 返回主菜单>");
    _get_sweep_input(&sp->is_return, 0, "宽度", &x, MIN_X, MAX_X);
    _get_sweep_input(&sp->is_return, 1, "高度", &y, MIN_Y, MAX_Y);
    _get_sweep_input(&sp->is_return, 2, "雷数", &m, MIN_SWEEP, x*y - x*y / 10 - 8);

    sleep(500);
    sp->size_x = x, sp->size_y = y, sp->num_sweep = m;
}
