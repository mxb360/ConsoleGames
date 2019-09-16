/* console.c   Windows实现
 * 本文件提供了对控制台的简单操作的函数
 * 包括显示位置，显示颜色，按键，窗口大小等操作
 * 封装了部分Windows控制台API
 * 目的之一是方便用C语言开发控制台游戏
 *
 * note: 
 *   此文件只能在Windows平台工作
 *   如果是Linux平台，请使用本文件的Linux实现
 *
 * auther:  Ma Xiaobo
 * version: 1.0
 * date:    2018-8
 */

#include "console.h"
#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include <stdarg.h>


/* COORD :
 * Windows API 中定义的结构体
 * 有成员 X 和成员 Y
 * 用来表示坐标
 *
 * 逻辑坐标：
 * 本文件中所有与坐标，长度，位置相关的函数(set_origin()除外)均采用逻辑坐标
 * 其原点的位置由函数set_origin()设置
 * X轴的单位长度和方向由函数set_axis()设置
 * y轴的单位长度和方向由函数set_axis()设置
 *
 * 窗口坐标（绝对坐标）
 * 原点位于窗口左上方的第一个字符处
 * X轴向右，单位长度是一个ascii字符的长度
 * Y轴向下，单位长度的行
 */
static COORD _origin = {0, 0};    // 逻辑坐标原点的绝对坐标位置
static COORD _axis = {1, 1};      // 逻辑x, y坐标轴的单位长度和方向
static char  _str_buf[256];       // 字符缓冲区

/* 设置逻辑坐标原点的绝对坐标位置
 * (x, y)为逻辑坐标原点的绝对坐标
 * 默认的原点为(0, 0)
 */
void set_origin(int x, int y)
{
    _origin.X = x;
    _origin.Y = y;
}

/* 设置逻辑x, y坐标轴的单位长度和方向(可以是负数) 
 * x, y分别表示x, y坐标轴的单位长度
 * 如果值为负，表示逻辑坐标的方向与绝对坐标方向相反
 * 默认的值为(1, 1)
 */
void set_axis(int dx, int dy)
{
    _axis.X = dx;
    _axis.Y = dy;
}

/* 移动光标到逻辑坐标(x, y)处
 * 此函数调用之后，程序的便从(x, y)处开始输出
 */
void gotoxy(int x, int y)
{
    COORD coord;

    // 将逻辑坐标转换为绝对坐标
    coord.X = _origin.X + x * _axis.X;
    coord.Y = _origin.Y + y * _axis.Y;

    // Windows API： 设置控制台光标位置
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/* 设置当前输出位置的显示颜色
 * bcolor： 背景色(即屏幕颜色)，取值范围(0-15)，可用头文件中定义的宏
 * fcolor： 前景色(即文字颜色)，取值范围(0-15)，可用头文件中定义的宏
 */
void set_color(int bcolor, int fcolor)
{  
    // Windows API： 设置控制台文本属性
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)(bcolor << 4 | fcolor));
}

void _set_color(int color)
{
    // Windows API： 设置控制台文本属性
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
}

/* 同时gotoxy和set_color
 */
void xycolor(int x, int y, int bcolor, int fcolor)
{
    gotoxy(x, y);
    set_color(bcolor, fcolor);
}

/* 刷屏函数
 * 整个控制台的背景被bcolor色填充，文字颜色变为fcolor
 * 注意： bcolor和fcolor不能一样
 */
void fill_color(int bcolor, int fcolor)
{
    // 将字符格式化后存入字符数组_str_buf
    sprintf(_str_buf, "color %x%x", bcolor, fcolor);
    // 调用系统(CMD)命令
    system(_str_buf);
}

/* 刷屏函数
 * 恢复默认的控制台颜色
 */
void refill_color(void)
{
    // 调用系统(CMD)命令
    system("color");
}

/* 清屏
 * 清除屏幕上显示的所有文字，并将光标移动至窗口坐标原点
 */
void clrscr(void)
{
    // 调用系统(CMD)命令
    system("cls");
}

/* 隐藏控制台光标 
 */
void hid_cursor(int size)
{
    // Windows API: 设置控制台光标信息
    CONSOLE_CURSOR_INFO cur_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);
}

/* 设置控制台窗口的标题为title
 */
void set_title(const char *title)
{
    // Windows API: 设置控制台标题
    SetConsoleTitle(title);
}

/* 设置控制台窗口的尺寸
 * 将窗口的宽和高分别设置为width和height
 * 其单位为函数set_axis()所设置的值
 * 注意：此函数最好在程序开始时就调用
 *       此函数会清空屏幕上已经显示了的内容
 */
void set_size(int width, int height)
{
    // 将字符格式化后存入字符数组_str_buf
    sprintf(_str_buf, "mode con cols=%d lines=%d",
        abs(_axis.X) * width, abs(_axis.Y) * height);
    // 调用系统(CMD)命令
    system(_str_buf);
}

/* 使程序暂停下来，显示提示字符串str后，按下任意键后继续
 * 如果str为NULL则显示系统默认的字符，一般是： 请按任意键继续. . .
 * 如果什么也不想显示，str可以为空串""
 */
void pause(const char *str)
{
    if (str == NULL)
        system("pause");

    strprint(str);
    system("pause>nul");
}

/* 延时ms毫秒 
 * 在这段时间内，程序会停在此处
 */
void sleep(int ms)
{
    // Windows API： 睡眠
    Sleep(ms);
}

/* 获取当前的按键
 * Case： 是否区分字母的大小写，0表示区分，否则不区分
 * 如果不区分大小写，对于所有字母，得到的都将是大写字母的ascii码
 * 返回字符对应的ascii码
 * 但是对于方向键，返回的是负数(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT)
 * 对于部分不可打印的可用的ascii码，提供了对应的宏使用(KEY_BACK, KEY_ESC, KEY_TAB, ...)
 * 如果当前没有键按下，此函数会一直等待，直到用户按下有效的键
 * 如果不想等待，你可以先调用函数have_key()判断当前是否有按键，如果有才调用此函数
 * 当调用此函数时，屏幕上不会有任何显示
 */
int get_key(int Case)
{
    int key;

    //while (1) {
        // 获取按键
        key = _getch();
        // 方向键
        if (key == 224) {
            switch (_getch()) {
            case 72: return KEY_UP;
            case 75: return KEY_LEFT;
            case 77: return KEY_RIGHT;
            case 80: return KEY_DOWN;
            }
        // 其他键
        } else if (key) 
            if (Case)               
                return toupper(key);  // 如果忽略大小写，对于字母，都转为大写
            else
                return key;
        // 如果得到0值，认为是无效按键，重新获取
    //}
    return key;
}

/* 判断当前是否有按键按下
 * 如果有，函数返回1，否则返回0
 * 此函数通常与函数get_key()配对使用
 */
int have_key(void)
{
    return !!_kbhit();
}

/* 即printf
 */
int strprint(const char *format, ...)
{
    int len;
    va_list ap;  

    va_start(ap, format);
    len = vprintf(format, ap);
    va_end(ap);

    return len;
}
