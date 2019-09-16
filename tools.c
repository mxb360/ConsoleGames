#include "tools.h"
#include <string.h>

/* 菜单选项结构体 （私有变量）*/
static struct {
    const char *title;
    const char **options;
    int len;
    int lines;
    int bcolor;
    int choose_color;
    int text_color;
    int showlines;
    int window_width;
    int x, y;
} _options;

/* 获取菜单项的最大字符串的长度 （私有函数）
 */
static int _get_max_options_len(void)
{
    int i;
    int this_len = strlen(_options.options[0]);
    int max_len = this_len;
    
    for (i = 1; i < _options.len; i++) {
        this_len = strlen(_options.options[i]);
        if (max_len < this_len)
           max_len = this_len;
    }

    return max_len;
}

/* 显示分隔符  （私有函数）
 */
static void _show_opt_lines(int y, int maxlen)
{
    int i;

    set_color(_options.bcolor, _options.text_color);
    gotoxy(_options.x, y);
    for (i = 0; i < maxlen + 7; i++)
        strprint("-");
}


/* 显示长度选项前对窗口的基本配置 （私有函数）
 */
static void _set_window_before_get_options(int max_len)
{
    //clrscr();
    set_origin(0, 2);
    set_axis(1, 1);

    gotoxy(_options.x, _options.y);
    strprint(_options.title);
    if (_options.showlines)
        _show_opt_lines(_options.y + 1, max_len);
}

/* 显示菜单字符串  （私有函数）
 */
static void _show_opt_str(int y, const char **p, int opt, int maxlen)
{
    int len; 

    gotoxy(_options.x, y);
    if (p - _options.options == opt) {       // 被选中的菜单栏
        set_color(_options.choose_color, _options.text_color);
        strprint(" ◆  ");
    }
    else {                                    // 未被选中的菜单栏
        set_color(_options.bcolor, _options.text_color);
        strprint("     ");
    }
    strprint(*p);                              // 打印字符
    len = strlen(*p);
    while (len++ < maxlen + 2)                 // 不足的空格填充
        strprint(" ");
}

/* 初始化菜单选项
 * title：菜单标题
 * opt：  菜单栏每一项的字符
 * len：  菜单栏数
 */
void set_options(const char *title, const char *opt[], int len)
{
    _options.title = title;
    _options.len = len;
    _options.options = opt;

    // 其他项使用了settings.h中的全局配置
    _options.lines = OPTIONS_LINES;
    _options.bcolor = OPTIONS_BK_COLOR;
    _options.choose_color = OPTIONS_CHOOSE_COLOR;
    _options.text_color = ISTR_CLR;
    _options.showlines = OPTIONS_SHOWLINES;
    _options.window_width = WINDOW_WIDTH;
    _options.x = 0;
    _options.y = 0;
}

/* 获取选中的菜单项序号(从0开始编号)
 * 调用此函数后，会显示菜单选项
 * 通过上下方向键移动选项卡，只有按下回车键后，或按下Q键后此函数才返回
 * 如果按下的是Q键，函数返回-1，如果按下回车键，函数返回当前序号
 * 使用此函数前务必先调用set_options初始化菜单信息
 */
int get_options(void)
{
    int opt_choose = 0;
    int i, max_len, show_lines;  
    const char **start, **end, **p;

    show_lines = _options.len < _options.lines ? _options.len : _options.lines;
    max_len = _get_max_options_len();                        // 字符串最大长度
    _options.x = (_options.window_width - max_len) / 2 - 5;  // 起始输出横坐标
    start = _options.options;                                // 第一个菜单栏字符串
    end = _options.options + show_lines;                     // 最后一个菜单栏字符串

    _set_window_before_get_options(max_len);
    while (1) {
        i = 1 + _options.showlines;
        for (p = start; p < end; p++) {
            _show_opt_str(i++ + _options.y, p, opt_choose, max_len);
            if (_options.showlines)
                _show_opt_lines(i++ + _options.y, max_len);
        }

        switch (get_key(1)) {               // 方向键（上、下）改变被选中项
        case KEY_UP: 
            if (opt_choose > 0)  
                opt_choose--; 
            else {                         // 已经到第一个菜单项，改为最后一个
                opt_choose = _options.len - 1;
                start = _options.options + _options.len - show_lines - 1;
                end = _options.options + _options.len - 1;
            }
            break;
        case KEY_DOWN: 
            if (opt_choose < _options.len - 1)  
                opt_choose++; 
            else {                          // 已经到最后一个菜单项，改为第一个
                opt_choose = 0;
                start = _options.options;
                end = _options.options + show_lines;
            }
            break;
        case KEY_ENTER: 
            return opt_choose;  // 回车键确认选项并返回
        case 'Q': return -1;    // 直接退出
        }

        if (opt_choose >= end - _options.options)
            start++, end++;         // 如果已经移动到菜单栏最下方，菜单选项整体上移一格
        else if (opt_choose < start - _options.options)
            start--, end--;         // 如果已经移动到菜单栏最上方，菜单选项整体下移一格
    }
}
