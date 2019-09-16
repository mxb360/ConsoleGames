#define IN_CONSOLE_GAMES

#define IN_SUDUKO_C
#include "suduko.h"
#include <ctype.h>

#define NUM_QU(i, j) ((i) / 3 * 3 + (j) / 3)
#define CNUM_QU(i, j) (((i) - (i) / 3 * 3) * 3 + ((j) - (j) / 3 * 3))
#define IS_SAME(i1, j1, i2, j2) ((i1) == (i2) || (j1) == (j2) || NUM_QU(i1, j1) == NUM_QU( i2, j2))

const char *num[] = {"　","１","２","３","４","５","６","７","８","９"};
const int data[][ASIZE][ASIZE] = {
    {{5,0,9,0,0,4,0,1,0},{6,0,0,8,0,2,0,0,5},{0,4,0,3,0,0,6,0,7},
     {0,0,4,0,0,7,9,0,0},{0,6,0,9,2,0,0,3,0},{9,0,0,0,4,0,0,7,0},
     {0,5,0,0,1,0,0,0,3},{0,9,0,0,8,5,0,2,0},{0,0,1,2,0,0,4,0,0}},
};
const int max_level = sizeof(data) / sizeof(data[0]);

int main(void)
{
    Suduko sd;
    sd.is_first = 1;

    while (1) {
        init_data(&sd);
        draw_face(&sd);
        while (!sd.is_return) {
            resolve_key(&sd);
        }

        if (sd.is_return)
            break;
    }

    return 0;
}

void init_data(Suduko *sd)
{
    int i, j;

    sd->cursor_x = ASIZE / 2;
    sd->cursor_y = ASIZE / 2;
    sd->is_return = 0;

    if (sd->is_first) {
        sd->level = 0;
    }

    for (i = 0; i < ASIZE; i++)
        for (j = 0; j < ASIZE; j++)
            sd->map[i][j] = data[sd->level][i][j];
}

/* 绘制游戏界面 */
void draw_face(Suduko *sd)
{
    int i, j;

    set_title("控制台小游戏 - 数独");
    set_origin(4, 2);
    set_axis(2, 1);
    set_size(WIN_WIDTH, WIN_HEIGHT);
    fill_color(BK_CLR, TSTR_CLR);

    for (i = -1; i <= ASIZE; i++)
        for (j = -1; j <= ASIZE; j++) {
            
            if (i == -1 || i == ASIZE || j == -1 || j == ASIZE) {
                xycolor(j, i, WALL_BK_CLR, WALL_CLR);
                printf("▓");
            } else 
                draw_suduko(sd, i, j, 0, 0);
        }

    
    show_hint(sd, 1);
    gotoxy(sd->cursor_x, sd->cursor_y);
}

/* 绘制一个数字 */
void draw_suduko(Suduko *sd, int i, int j, int n, int model)
{
    int bclr, fclr;

    if (n && !data[sd->level][i][j])
        sd->map[i][j] = n - '0';

    if (NUM_QU(i, j) % 2) {
        if (CNUM_QU(i, j) % 2)  bclr = model != 3 ? NUM_BK_CLR1 : NNUM_MK_CLR1;
        else                    bclr = model != 3 ? NUM_BK_CLR3 : NNUM_MK_CLR2;
    } else {
        if (CNUM_QU(i, j) % 2)  bclr = model != 3 ? NUM_BK_CLR2 : NNUM_MK_CLR1;
        else                    bclr = model != 3 ? NUM_BK_CLR4 : NNUM_MK_CLR2;
    }
    if (data[sd->level][i][j])       fclr = NUM_CLR;
    else                             fclr = NNUM_CLR;
    if (model == 1 && sd->map[i][j]) bclr = NUM_MK_CLR;   
    if (model == 2)                  bclr = NUM_MKE_CLR;
    if (model == 4)                  bclr = NUM_MKC_CLR;
    xycolor(j, i, bclr, fclr);
    printf(num[sd->map[i][j]]);
}

/* 显示提示 */
void show_hint(Suduko *sd, int model)
{
    int i, j;

    for (i = 0; i < ASIZE; i++)
        for (j = 0; j < ASIZE; j++) {
            if (i == sd->cursor_y && j == sd->cursor_x)
                draw_suduko(sd, i, j, 0, model ? 4 : 0);  
            else if (data[sd->level][sd->cursor_y][sd->cursor_x]) {
                if (sd->map[i][j] == sd->map[sd->cursor_y][sd->cursor_x])
                    if (IS_SAME(i, j, sd->cursor_y, sd->cursor_x))
                        draw_suduko(sd, i, j, 0, model ? 2 : 0);
                    else
                        draw_suduko(sd, i, j, 0, model ? 1 : 0);
            } else {
                if (IS_SAME(i, j, sd->cursor_y, sd->cursor_x)) {
                    draw_suduko(sd, i, j, 0, model ? 3 : 0);
                    if (sd->map[i][j] &&
                        sd->map[i][j] == sd->map[sd->cursor_y][sd->cursor_x])
                        draw_suduko(sd, i, j, 0, model ? 2 : 0);
                } else if (sd->map[i][j] &&
                    sd->map[i][j] == sd->map[sd->cursor_y][sd->cursor_x])
                    draw_suduko(sd, i, j, 0, model ? 1 : 0);
            }
        }
}


/* 按键处理 */
void resolve_key(Suduko *sd)
{
    int ch = get_key(1);

    show_hint(sd, 0);
    switch (ch) {
    case KEY_UP:
    case 'W': sd->cursor_y = (sd->cursor_y + ASIZE - 1) % ASIZE; break;
    case KEY_DOWN:
    case 'S': sd->cursor_y = (sd->cursor_y + 1) % ASIZE; break;
    case KEY_LEFT:
    case 'A': sd->cursor_x = (sd->cursor_x + ASIZE - 1) % ASIZE; break;
    case KEY_RIGHT:
    case 'D': sd->cursor_x = (sd->cursor_x + 1) % ASIZE; break;
        // 功能键
    case 'Q': sd->is_return = 1; return;       // 返回主菜单
    case 'Z':  break;// 悔棋
    case KEY_SPACE:          // 落子
        //if (player_down(gb, gb->cursor_x, gb->cursor_y) && gb->game_model)
        //    computer_down(gb);
        return;
    default:
        if (isdigit(ch)) {
            draw_suduko(sd, sd->cursor_y, sd->cursor_x, ch, 0);
        }
    }
    show_hint(sd, 1);
    gotoxy(sd->cursor_x, sd->cursor_y);
}
