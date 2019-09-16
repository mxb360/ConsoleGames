#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "console.h"

#define WINDOW_WIDTH         60
#define WINDOW_HEIGHT        16

#define BK_CLR               LIGHT_AQUA
#define ISTR_CLR             LIGHT_BLUE
#define TSTR_CLR             LIGHT_RED
#define ASTR_CLR             GREEN

#define OPTIONS_CHOOSE_COLOR LIGHT_PURPLE
#define OPTIONS_BK_COLOR     WHITE
#define OPTIONS_LINES        5
#define MAX_OPTIONS_LINES    100
#define OPTIONS_SHOWLINES    1

void set_options(const char *title, const char *opt[], int len);
int  get_options(void);

#endif
