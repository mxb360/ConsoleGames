#include "tools.h"

#include "gobang.h"
#include "reversi.h"
#include "chinesechess.h"
#include "tetris.h"
#include "sokoban.h"
#include "sweeper.h"
#include "suduko.h"
#include "sweeper.h"

/* 退出 */
int quit(void) { exit(0); return 0; }

/* 游戏菜单选项 */
const char *games_opt[] = {
    "  01   五子棋",
    "  02   黑白棋",
    "  03   中国象棋",
    "  04   俄罗斯方块",
    "  05   推箱子",
    "  06   扫雷",
    "  07   数独",
    "            退    出                  ",
};

/* 每个游戏的入口函数
 */
int (*games_main_func[])(void) = {    
    quit,
    gobang_main,
    reversi_main, 
    chinesechess_main,
    tetris_main,
    sokoban_main,
    sweeper_main,
    suduko_main,
    quit,
};

const int count = sizeof(games_opt) / sizeof(games_opt[0]);

/* 打印注意事项 */
void print_note(void)
{
    set_title("控制台小游戏合集");
    strprint(
    "\n 控制台小游戏合集\n"
    "     这是一款基于Windows控制台的小游戏合集。\n\n"
    "     这个游戏会改变控制台行为，你应该通过双击exe文件打开，\n\n"
	"     而不是通过命令行打开。为了有一个愉快的体验，在首次运行这个程序时，\n"
	"     你应该需要设置控制台的字体和模式：\n\n"
    "     1. 单击左上角控制台图标，点击属性\n"
    "     2. 在选项栏，取消“快速编辑模式”和“插入模式”的勾选\n"
    "     3. 如果是Win 10，请勾选“使用旧版控制台”（必须勾选，不然颜色显示会出问题）\n"
    "     4. 在字体栏，建议选择“新宋体”，字号设置为24\n"
    "     5. 点击确定，保存修改。\n\n"
    "     如果你完成了修改，按回车键进入游戏 . . .\n\n\n"
    "           ----| 控制台小游戏合集 -- Ma Xiaobo -- 2018-8 |----");
    while (get_key(1) != KEY_ENTER);
}

/*设置窗口 */
void set_window(void)
{
    set_axis(1, 1);
    set_origin(0, 0);
    set_size(WINDOW_WIDTH + 1, WINDOW_HEIGHT + 1);
    set_title("控制台小游戏合集");
    fill_color(BK_CLR, ISTR_CLR);
    clrscr();
    xycolor(15, 0, BK_CLR, TSTR_CLR); 
    strprint("控制台小游戏(%d合1)   ", count - 1);
    xycolor(10, 1, BK_CLR, ASTR_CLR);
    strprint("  作者：Ma Xiaobo        日期：2018-8");
    set_color(BK_CLR, ISTR_CLR);
}

int main(void)
{
    print_note();

    while (1) {
        set_options("↑↓:上下移动         回车:确认        Q:退出", games_opt, count);
        set_window();
		games_main_func[get_options() + 1]();
    }
    
    return 0;
}
 