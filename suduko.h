#ifndef _SHUDUKO_H_
#define _SHUDUKO_H_

#ifdef IN_SUDUKO_C
#include "console.h"

#ifdef IN_CONSOLE_GAMES
#define main suduko_main
#else
int suduko_main(void) { return 0; };
#endif

#define ASIZE 9

#define WALL_BK_CLR  BLUE
#define WALL_CLR     LIGHT_BLUE
#define NUM_CLR      BLACK
#define NNUM_CLR     LIGHT_PURPLE
#define NUM_BK_CLR1  GREEN
#define NUM_BK_CLR2  LIGHT_GREEN
#define NUM_BK_CLR3  YELLOW
#define NUM_BK_CLR4  LIGHT_YELLOW
#define NUM_MKE_CLR  RED
#define NUM_MKC_CLR  BRIGHT_WHITE
#define NUM_MK_CLR   BLUE
#define NNUM_MK_CLR1 GRAY
#define NNUM_MK_CLR2 WHITE
#define NNUM_BK_CLR  WHITE

#define BK_CLR       LIGHT_AQUA
#define TSTR_CLR     LIGHT_RED
#define ASTR_CLR     GREEN
#define FSTR_CLR     RED
#define ISTR_CLR     BLUE
#define OSTR_CLR     RED
#define OBK_CLR      GREEN

#define WIN_WIDTH  25 
#define WIN_HEIGHT 15

typedef struct  {
    int cursor_x, cursor_y;
    int map[9][9];
    int level;
    int is_return;
    int is_first;
} Suduko;

void draw_face(Suduko *sd);
void init_data(Suduko *sd);
void resolve_key(Suduko *sd);
void draw_suduko(Suduko *sd, int i, int j, int n, int model);
void show_hint(Suduko *sd, int model);
#endif

int suduko_main(void);

#endif
