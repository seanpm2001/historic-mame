/*********************************************************************

  usrintrf.h

  Functions used to handle MAME's crude user interface.

*********************************************************************/

#ifndef USRINTRF_H
#define USRINTRF_H

struct DisplayText
{
	const char *text;	/* 0 marks the end of the array */
	int color;	/* see #defines below */
	int x;
	int y;
};

#define DT_COLOR_WHITE 0
#define DT_COLOR_YELLOW 1
#define DT_COLOR_RED 2


struct GfxElement *builduifont(int totalcolors,const unsigned char *palette,const unsigned char *pens);
void displaytext(const struct DisplayText *dt,int erase);
int showcharset(void);
int setup_menu(void);

#endif
