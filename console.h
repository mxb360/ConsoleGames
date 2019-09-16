/* console.h 目前有Windows版和Linux版
 * 本文件提供了对控制台/Shell的简单操作的函数
 * 包括显示位置，显示颜色，按键，窗口大小等操作
 * 目的之一是方便用C语言开发控制台游戏
 *
 * auther:  Ma Xiaobo
 * version: 1.0
 * date:    2018-8
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

/* 去除VS的安全函数的警告 */
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 函数执行成功与否 */
#define EXEC_ERROR   1
#define EXEC_SUCCESS 0

/* 颜色相关（仅支持16种颜色） */
#define BLACK         0    // 黑色
#define BLUE          1    // 蓝色
#define GREEN         2    // 绿色
#define AQUA          3    // 浅绿色
#define RED           4    // 红色
#define PURPLE        5    // 紫色
#define YELLOW        6    // 黄色
#define WHITE         7    // 白色
#define GRAY          8    // 灰色
#define LIGHT_BLUE    9    // 淡蓝色
#define LIGHT_GREEN   10   // 淡绿色
#define LIGHT_AQUA    11   // 淡浅绿色
#define LIGHT_RED     12   // 淡红色
#define LIGHT_PURPLE  13   // 淡紫色
#define LIGHT_YELLOW  14   // 淡黄色
#define BRIGHT_WHITE  15   // 亮白色

/* 按键码 */
#define KEY_DOWN  -1       // 方向键 上  
#define KEY_UP    -2       // 方向键 下
#define KEY_LEFT  -3       // 方向键 左
#define KEY_RIGHT -4       // 方向键 右
#define KEY_BACK  8        // 退格键 '\b'
#define KEY_ENTER 13       // 回车键 '\r'
#define KEY_ESC   27       // Esc键
#define KEY_TAB   9        // Tab键  '\t'
#define KEY_SPACE 32       // 空格键 ' ' 
 
int  strprint(const char *format, ...);  // 即printf
void set_origin(int x, int y);           // 设置逻辑坐标原点的绝对坐标位置
void set_axis(int x, int y);             // 设置逻辑x, y坐标轴的单位长度和方向(可以是负数)
void gotoxy(int x, int y);               // 移动光标到逻辑坐标(x, y)处
void set_color(int bcolor, int fcolor);  // 设置当前输出位置的显示颜色
void _set_color(int color);              // 设置当前输出位置的显示颜色(不推荐)
void xycolor(int x, int y, int bcolor, int fcolor); // 同时gotoxy和set_color
void fill_color(int bcolor, int fcolor); // 刷屏函数
void refill_color(void);                 // 刷屏函数
void clrscr(void);                       // 清屏
void hid_cursor(int size);               // 隐藏控制台光标
void set_title(const char *title);       // 设置控制台窗口的标题为title
void set_size(int width, int height);    // 设置控制台窗口的尺寸
void pause(const char *str);             // 使程序暂停下来，按下任意键后继续
void sleep(int ms);                      // 延时ms毫秒 
int  get_key(int Case);                  // 获取当前的按键
int  have_key(void);                     // 判断当前是否有按键按下

#endif
