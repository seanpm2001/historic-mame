/***************************************************************************

  vidhrdw.c

  Functions to emulate the video hardware of the machine.

  Todo:
  7 unknown attribute bits on scroll 2/3.
  Speed, speed, speed...

  OUTPUT PORTS (preliminary)
  0x00-0x01     OBJ RAM base (/256)
  0x02-0x03     Scroll1 RAM base (/256)
  0x04-0x05     Scroll2 RAM base (/256)
  0x06-0x07     Scroll3 RAM base (/256)
  0x08-0x09     "Other" RAM - Scroll distortion (/256)
  0x0a-0x0b     Palette base (/256)
  0x0c-0x0d     Scroll 1 X
  0x0e-0x0f     Scroll 1 Y
  0x10-0x11     Scroll 2 X
  0x12-0x13     Scroll 2 Y
  0x14-0x15     Scroll 3 X
  0x16-0x17     Scroll 3 Y
  0x18-0x19     ????

  0x20-0x21     start offset for the rowscroll matrix
  0x22-0x23     unknown but widely used - usually 0x0e


  Registers move from game to game.. following example strider
  0x66-0x67     Video control register (location varies according to game)
  0x6a-0x6b     Priority mask
  0x6c-0x6d     Priority mask
  0x6e-0x6f     Priority mask
  0x70-0x71     Control register (usually 0x003f)

  Fixed registers
  0x80-0x81     ????	    Sound output.
  0x88-0x89     ????	    Port thingy (sound fade???)

  Known Bug List
  ==============
  All games
  * Sprite lag.
  * Large sprites don't exit smoothly on the left side of the screen (e.g. Car
    in Mega Man attract mode, cadillac in Cadillacs and Dinosaurs)

  Magic Sword.
  * In several places (starting from floor 3, I think) you can see high priority
    scroll 3 tiles sticking up in front of scroll2.
  * Rogue scroll 2 character at end of level 1 - fixed with kludge 4
    (code e7ff attr 027f, code 3c36 attr 005f)
  * during attract mode, characters are shown with a black background. There is
    a background, but the layers are disabled. I think this IS the correct
	behaviour.

  King of Dragons.
  * Distortion effect missing on character description screen during attract
    mode. The game rapidly toggles on and off the layer enable bit.
  * Flickering text n the bottom left corner on the player selection screen.

  Captain Commando
  * The continue screen is missing the text and counter. The game is playing
    one too many frames of the star animation. The culprit seems to be here:
00D95A:   70 00                          moveq   #$0, D0
00D95C:   3b 40 28 28                    move.w  D0, ($2828,A5)
00D960:   3b 40 28 2c                    move.w  D0, ($282c,A5)
00D964:   3b 7c 91 e0 28 1c              move.w  #$91e0, ($281c,A5)
00D96A:   3b 7c 01 00 28 28              move.w  #$100, ($2828,A5)
00D970:   3b 7c 01 00 28 2c              move.w  #$100, ($282c,A5)
    replacing the #$100 at D96A with #$000 fixes the problem.

  ffight
  * continue screen has garbage character tiled in the background

  mtwins
  * in the cave, some parts of the background don't wave. That's because
    rowscroll is not supported for high priority tiles.
  * attract mode screens have garbage character tiled in the background

  cawing
  * garbage characters inbetween text in attract mode
  * at end of level 2, a big black box covers the large plane. This seems to
    be supposed to be a *mask* to hide the fuel pipes. The pipes are sprites,
	the large plane is scroll2. Therefore, the mask should block sprites but
	not tiles from the other planes.

  area 88
  * garbage characters in attract mode

  knights
  * garbage characters left on screen during boot and under INSERT COIN
    (patched out in cps1_render_scroll1()).

  qad
  * in attract mode, "Dragons" is missing from the title screen, only
    "Quiz &" is visible.
  * garbage sprites scrolling into screen after a victory

  mercs
  * a few wrong scroll 2 tiles (check part 2 of the first attract mode round)

  punisher
  * the van at the beginning of stage 2 shouldn't be there

  dino
  * in level 6, the legs of the big dino which stomps you are almost entirely
    missing.
  * in level 6, palette changes due to lightnings cause a lot of tiling effects
    on scroll2.

  1941
  * garbage tiles around the final boss


  Todo
  ====

  Implement CPS2 QSound system


  Unknown issues
  ==============

  There are often some redundant high bits in the scroll layer's attributes.


***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"
#include "drivers/cps1.h"

#define VERBOSE 1


/********************************************************************

			Configuration table:

********************************************************************/

/* Game specific data */
struct CPS1config
{
	char *name;             /* game driver name */

	/* Some games interrogate a couple of registers on bootup. */
	/* These are CPS1 board B self test checks. They wander from game to */
	/* game. */
	int cpsb_addr;        /* CPS board B test register address */
	int cpsb_value;       /* CPS board B test register expected value */

	/* some games use as a protection check the ability to do 16-bit multiplies */
	/* with a 32-bit result, by writing the factors to two ports and reading the */
	/* result from two other ports. */
	/* It looks like this feature was introduced with 3wonders (CPSB ID = 08xx) */
	int mult_factor1;
	int mult_factor2;
	int mult_result_lo;
	int mult_result_hi;

	int layer_control;
	int priority0;	/* this seems to apply only to scroll1. cawing uses it. */
	int priority1;
	int priority2;
	int priority3;
	int control_reg;  /* Control register? seems to be always 0x3f */

	/* ideally, the layer enable masks should consist of only one bit, */
	/* but in many cases it is unknown which bit is which. */
	int scrl1_enable_mask;
	int scrl2_enable_mask;
	int scrl3_enable_mask;

	int bank_scroll1;
	int bank_scroll2;
	int bank_scroll3;

	int   kludge;  /* Ghouls n Ghosts sprite kludge */
};

struct CPS1config *cps1_game_config;

static struct CPS1config cps1_config_table[]=
{
	/* name       CPSB ID    multiply protection  ctrl    priority masks  unknwn  layer enable   banks   kludge */
	{"forgottn",0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x06,0x0e,0x0e, 0,0,0 },
	{"ghouls",  0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x04,0x08, 0,0,0 , 1},
	{"ghoulsj", 0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x04,0x08, 0,0,0 , 1},
	{"strider", 0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 1,0,1 },
	{"striderj",0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 1,0,1 },
	{"dwj",     0x00,0x0000, 0x00,0x00,0x00,0x00, 0x6c,0x6a,0x68,0x66,0x64,0x62, 0x02,0x04,0x08, 0,1,1 },
	{"willow",  0x00,0x0000, 0x00,0x00,0x00,0x00, 0x70,0x6e,0x6c,0x6a,0x68,0x66, 0x20,0x10,0x08, 0,1,0 },
	{"willowj", 0x00,0x0000, 0x00,0x00,0x00,0x00, 0x70,0x6e,0x6c,0x6a,0x68,0x66, 0x20,0x10,0x08, 0,1,0 },
	{"unsquad", 0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x38,0x38,0x38, 0,0,0 },
	{"area88",  0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x38,0x38,0x38, 0,0,0 },
	{"ffight",  0x60,0x0004, 0x00,0x00,0x00,0x00, 0x6e,0x66,0x70,0x68,0x72,0x6a, 0x02,0x0c,0x0c, 0,0,0 },
	{"ffightu", 0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 0,0,0 },
	{"ffightj", 0x60,0x0004, 0x00,0x00,0x00,0x00, 0x6e,0x66,0x70,0x68,0x72,0x6a, 0x02,0x0c,0x0c, 0,0,0 },
	{"1941",    0x60,0x0005, 0x00,0x00,0x00,0x00, 0x68,0x6a,0x6c,0x6e,0x70,0x72, 0x02,0x08,0x20, 0,0,0 },
	{"1941j",   0x60,0x0005, 0x00,0x00,0x00,0x00, 0x68,0x6a,0x6c,0x6e,0x70,0x72, 0x02,0x08,0x20, 0,0,0 },
	{"mercs",   0x60,0x0402, 0x00,0x00,0x00,0x00, 0x6c,0x00,0x00,0x00,0x00,0x62, 0x02,0x04,0x08, 0,0,0 },	/* (uses port 74) */
	{"mercsu",  0x60,0x0402, 0x00,0x00,0x00,0x00, 0x6c,0x00,0x00,0x00,0x00,0x62, 0x02,0x04,0x08, 0,0,0 },	/* (uses port 74) */
	{"mercsj",  0x60,0x0402, 0x00,0x00,0x00,0x00, 0x6c,0x00,0x00,0x00,0x00,0x62, 0x02,0x04,0x08, 0,0,0 },	/* (uses port 74) */
	{"mtwins",  0x5e,0x0404, 0x00,0x00,0x00,0x00, 0x52,0x54,0x56,0x58,0x5a,0x5c, 0x38,0x38,0x38, 0,0,0 },
	{"chikij",  0x5e,0x0404, 0x00,0x00,0x00,0x00, 0x52,0x54,0x56,0x58,0x5a,0x5c, 0x38,0x38,0x38, 0,0,0 },
	{"msword",  0x00,0x0000, 0x00,0x00,0x00,0x00, 0x62,0x64,0x66,0x68,0x6a,0x6c, 0x20,0x06,0x06, 0,0,0 , 4},
	{"mswordu", 0x00,0x0000, 0x00,0x00,0x00,0x00, 0x62,0x64,0x66,0x68,0x6a,0x6c, 0x20,0x06,0x06, 0,0,0 , 4},
	{"mswordj", 0x00,0x0000, 0x00,0x00,0x00,0x00, 0x62,0x64,0x66,0x68,0x6a,0x6c, 0x20,0x06,0x06, 0,0,0 , 4},
	{"cawing",  0x40,0x0406, 0x00,0x00,0x00,0x00, 0x4c,0x4a,0x48,0x46,0x44,0x42, 0x10,0x0a,0x0a, 0,0,0 },	/* row scroll used at the beginning of mission 8, put 07 at ff8501 to jump there */
	{"cawingj", 0x40,0x0406, 0x00,0x00,0x00,0x00, 0x4c,0x4a,0x48,0x46,0x44,0x42, 0x10,0x0a,0x0a, 0,0,0 },	/* row scroll used at the beginning of mission 8, put 07 at ff8501 to jump there */
	{"nemo",    0x4e,0x0405, 0x00,0x00,0x00,0x00, 0x42,0x44,0x46,0x48,0x4a,0x4c, 0x04,0x22,0x22, 0,0,0 },
	{"nemoj",   0x4e,0x0405, 0x00,0x00,0x00,0x00, 0x42,0x44,0x46,0x48,0x4a,0x4c, 0x04,0x22,0x22, 0,0,0 },
	{"sf2",     0x48,0x0407, 0x00,0x00,0x00,0x00, 0x54,0x52,0x50,0x4e,0x4c,0x4a, 0x08,0x12,0x12, 2,2,2 , 3},
	{"sf2a",    0x48,0x0407, 0x00,0x00,0x00,0x00, 0x54,0x52,0x50,0x4e,0x4c,0x4a, 0x08,0x12,0x12, 2,2,2 , 3},
	{"sf2b",    0x48,0x0407, 0x00,0x00,0x00,0x00, 0x54,0x52,0x50,0x4e,0x4c,0x4a, 0x08,0x12,0x12, 2,2,2 , 3},
	{"sf2e",    0xd0,0x0408, 0x00,0x00,0x00,0x00, 0xdc,0xda,0xd8,0xd6,0xd4,0xd2, 0x10,0x0a,0x0a, 2,2,2 , 3},
	{"sf2j",    0x6e,0x0403, 0x00,0x00,0x00,0x00, 0x62,0x64,0x66,0x68,0x6a,0x6c, 0x20,0x06,0x06, 2,2,2 , 3},
	{"3wonders",0x72,0x0800, 0x4e,0x4c,0x4a,0x48, 0x68,0x66,0x64,0x62,0x60,0x70, 0x20,0x04,0x08, 0,1,1 , 2},
	{"3wonderj",0x72,0x0800, 0x4e,0x4c,0x4a,0x48, 0x68,0x66,0x64,0x62,0x60,0x70, 0x20,0x04,0x08, 0,1,1 , 2},
	{"kod",     0x00,0x0000, 0x5e,0x5c,0x5a,0x58, 0x60,0x6e,0x6c,0x6a,0x68,0x70, 0x30,0x08,0x30, 0,0,0 },
	{"kodj",    0x00,0x0000, 0x5e,0x5c,0x5a,0x58, 0x60,0x6e,0x6c,0x6a,0x68,0x70, 0x30,0x08,0x30, 0,0,0 },
	{"kodb",    0x00,0x0000, 0x00,0x00,0x00,0x00, 0x60,0x6e,0x6c,0x6a,0x68,0x70, 0x30,0x08,0x30, 0,0,0 },
	{"captcomm",0x00,0x0000, 0x46,0x44,0x42,0x40, 0x60,0x6e,0x6c,0x6a,0x68,0x70, 0x20,0x12,0x12, 0,0,0 },	/* multiply is used only to center the startup text */
	{"captcomu",0x00,0x0000, 0x46,0x44,0x42,0x40, 0x60,0x6e,0x6c,0x6a,0x68,0x70, 0x20,0x12,0x12, 0,0,0 },	/* multiply is used only to center the startup text */
	{"captcomj",0x00,0x0000, 0x46,0x44,0x42,0x40, 0x60,0x6e,0x6c,0x6a,0x68,0x70, 0x20,0x12,0x12, 0,0,0 },	/* multiply is used only to center the startup text */
	{"knights", 0x00,0x0000, 0x46,0x44,0x42,0x40, 0x68,0x66,0x64,0x62,0x60,0x70, 0x20,0x10,0x02, 0,0,0 },
	{"knightsj",0x00,0x0000, 0x46,0x44,0x42,0x40, 0x68,0x66,0x64,0x62,0x60,0x70, 0x20,0x10,0x02, 0,0,0 },
	{"sf2ce",   0x00,0x0000, 0x40,0x42,0x44,0x46, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 2,2,2 , 3},
	{"sf2cea",  0x00,0x0000, 0x40,0x42,0x44,0x46, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 2,2,2 , 3},
	{"sf2ceb",  0x00,0x0000, 0x40,0x42,0x44,0x46, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 2,2,2 , 3},
	{"sf2cej",  0x00,0x0000, 0x40,0x42,0x44,0x46, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 2,2,2 , 3},
	{"sf2red",  0x00,0x0000, 0x40,0x42,0x44,0x46, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 2,2,2 , 3},
	{"sf2rb",   0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 2,2,2 , 3},
	{"varth",   0x00,0x0000, 0x00,0x00,0x00,0x00, 0x6e,0x66,0x70,0x68,0x72,0x6a, 0x02,0x0c,0x0c, 0,0,0 },	/* CPSB test has been patched out (60=0008) */
	{"varthj",  0x00,0x0000, 0x4e,0x4c,0x4a,0x48, 0x60,0x6e,0x6c,0x6a,0x68,0x70, 0x20,0x06,0x06, 0,0,0 },	/* CPSB test has been patched out (72=0001) */
	{"cworld2j",0x00,0x0000, 0x00,0x00,0x00,0x00, 0x60,0x00,0x00,0x00,0x00,0x70, 0x20,0x14,0x14, 0,0,0 },
	{"wof",     0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x04,0x08, 0,0,0 },
	{"wofj",    0x00,0x0000, 0x00,0x00,0x00,0x00, 0x62,0x64,0x66,0x68,0x6a,0x6c, 0x10,0x08,0x04, 0,0,0 },
	{"dino",    0x00,0x0000, 0x00,0x00,0x00,0x00, 0x4a,0x4c,0x4e,0x40,0x42,0x44, 0x16,0x16,0x16, 0,0,0 },	/* layer enable never used */
	{"punisher",0x4e,0x0c00, 0x00,0x00,0x00,0x00, 0x52,0x54,0x56,0x48,0x4a,0x4c, 0x04,0x02,0x20, 0,0,0 },
	{"punishrj",0x4e,0x0c00, 0x00,0x00,0x00,0x00, 0x52,0x54,0x56,0x48,0x4a,0x4c, 0x04,0x02,0x20, 0,0,0 },
	{"slammast",0x6e,0x0c01, 0x00,0x00,0x00,0x00, 0x56,0x40,0x42,0x68,0x6a,0x6c, 0x10,0x0c,0x0c, 0,0,0 },
	{"mbomber", 0x5e,0x0c02, 0x00,0x00,0x00,0x00, 0x6a,0x6c,0x6e,0x70,0x72,0x5c, 0x10,0x0c,0x0c, 0,0,0 },
	{"mbomberj",0x5e,0x0c02, 0x00,0x00,0x00,0x00, 0x6a,0x6c,0x6e,0x70,0x72,0x5c, 0x10,0x0c,0x0c, 0,0,0 },
	{"sf2t",    0x00,0x0000, 0x40,0x42,0x44,0x46, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 2,2,2 , 3},
	{"sf2tj",   0x00,0x0000, 0x40,0x42,0x44,0x46, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 2,2,2 , 3},
	{"pnickj",  0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x00,0x00,0x00,0x00,0x70, 0x0e,0x0e,0x0e, 0,0,0 },
	{"qad",     0x00,0x0000, 0x00,0x00,0x00,0x00, 0x6c,0x00,0x00,0x00,0x00,0x52, 0x16,0x16,0x16, 0,0,0 },
	{"qadj",    0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x0e,0x0e,0x0e, 0,0,0 },
	{"megaman", 0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 0,0,0 },
	{"rockmanj",0x00,0x0000, 0x00,0x00,0x00,0x00, 0x66,0x68,0x6a,0x6c,0x6e,0x70, 0x02,0x0c,0x0c, 0,0,0 },
	{0}		/* End of table */
};

static void cps1_init_machine(void)
{
	const char *gamename = Machine->gamedrv->name;
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	struct CPS1config *pCFG=&cps1_config_table[0];
	while(pCFG->name)
	{
		if (strcmp(pCFG->name, gamename) == 0)
		{
			break;
		}
		pCFG++;
	}
	cps1_game_config=pCFG;

	if (strcmp(gamename, "ghouls" )==0)
	{
		/* Patch out self-test... it takes forever */
		WRITE_WORD(&RAM[0x61964+0], 0x4ef9);
		WRITE_WORD(&RAM[0x61964+2], 0x0000);
		WRITE_WORD(&RAM[0x61964+4], 0x0400);
	}
	else if (strcmp(gamename, "ghoulsj" )==0)
	{
		/* Patch out self-test... it takes forever */
		WRITE_WORD(&RAM[0x61930+0], 0x4ef9);
		WRITE_WORD(&RAM[0x61930+2], 0x0000);
		WRITE_WORD(&RAM[0x61930+4], 0x0400);
	}
	else if (strcmp(gamename, "slammast" )==0)
	{
		/* temporary patch until the protection is understood */
		WRITE_WORD(&RAM[0x0fbe], 0x4e75);
	}
	else if (strcmp(gamename, "mbomber" )==0 || strcmp(gamename, "mbomberj" )==0)
	{
		/* temporary patch until the protection is understood */
		WRITE_WORD(&RAM[0x0f1a], 0x4e75);
	}
}



INLINE int cps1_port(int offset)
{
	return READ_WORD(&cps1_output[offset]);
}

INLINE unsigned char * cps1_base(int offset)
{
	int base=cps1_port(offset)*256;
	return &cps1_gfxram[base&0x3ffff];
}

int cps1_output_r(int offset)
{
#if VERBOSE
if (errorlog && offset >= 0x18) fprintf(errorlog,"PC %06x: read output port %02x\n",cpu_get_pc(),offset);
#endif

	/* Some games interrogate a couple of registers on bootup. */
	/* These are CPS1 board B self test checks. They wander from game to */
	/* game. */
	if (offset && offset == cps1_game_config->cpsb_addr)
		return cps1_game_config->cpsb_value;

	/* some games use as a protection check the ability to do 16-bit multiplies */
	/* with a 32-bit result, by writing the factors to two ports and reading the */
	/* result from two other ports. */
	if (offset && offset == cps1_game_config->mult_result_lo)
		return (READ_WORD(&cps1_output[cps1_game_config->mult_factor1]) *
				READ_WORD(&cps1_output[cps1_game_config->mult_factor2])) & 0xffff;
	if (offset && offset == cps1_game_config->mult_result_hi)
		return (READ_WORD(&cps1_output[cps1_game_config->mult_factor1]) *
				READ_WORD(&cps1_output[cps1_game_config->mult_factor2])) >> 16;

	return READ_WORD(&cps1_output[offset]);
}

void cps1_output_w(int offset, int data)
{
	int value;

#if VERBOSE
if (errorlog && offset >= 0x18 && //offset != 0x22 &&
		offset != cps1_game_config->layer_control &&
		offset != cps1_game_config->priority0 &&
		offset != cps1_game_config->priority1 &&
		offset != cps1_game_config->priority2 &&
		offset != cps1_game_config->priority3 &&
		offset != cps1_game_config->control_reg)
	fprintf(errorlog,"PC %06x: write %02x to output port %02x\n",cpu_get_pc(),data,offset);

#ifdef MAME_DEBUG
#if 0
if (offset == 0x22 && data != 0x0e)
{
	char baf[40];
	sprintf(baf,"port 22 = %02x",data);
	usrintf_showmessage(baf);
}
#endif
if (cps1_game_config->priority0 && offset == cps1_game_config->priority0 && data != 0x00)
{
	char baf[40];
	sprintf(baf,"priority0 %04x",data);
	usrintf_showmessage(baf);
}
if (cps1_game_config->control_reg && offset == cps1_game_config->control_reg && data != 0x3f)
{
	char baf[40];
	sprintf(baf,"control_reg %02x",data);
	usrintf_showmessage(baf);
}
#endif
#endif
	COMBINE_WORD_MEM(&cps1_output[offset],data);
}



/* Public variables */
unsigned char *cps1_gfxram;
unsigned char *cps1_output;
int cps1_gfxram_size;
int cps1_output_size;

/* Private */

/* Offset of each palette entry */
const int cps1_obj_palette    =0;
const int cps1_scroll1_palette=32;
const int cps1_scroll2_palette=32+32;
const int cps1_scroll3_palette=32+32+32;
#define cps1_palette_entries (32*4)  /* Number colour schemes in palette */

const int cps1_scroll1_size=0x4000;
const int cps1_scroll2_size=0x4000;
const int cps1_scroll3_size=0x4000;
const int cps1_obj_size    =0x4000;
const int cps1_other_size  =0x4000;
const int cps1_palette_size=cps1_palette_entries*32; /* Size of palette RAM */
static int cps1_flip_screen;    /* Flip screen on / off */

static unsigned char *cps1_scroll1;
static unsigned char *cps1_scroll2;
static unsigned char *cps1_scroll3;
static unsigned char *cps1_obj;
static unsigned char *cps1_palette;
static unsigned char *cps1_other;
static unsigned char *cps1_old_palette;

/* Working variables */
static int cps1_last_sprite_offset;     /* Offset of the last sprite */
static int cps1_layer_enabled[4];       /* Layer enabled [Y/N] */

int scroll1x, scroll1y, scroll2x, scroll2y, scroll3x, scroll3y;
static unsigned char *cps1_scroll2_old;
static struct osd_bitmap *cps1_scroll2_bitmap;


/* Output ports */
#define CPS1_OBJ_BASE	   0x00    /* Base address of objects */
#define CPS1_SCROLL1_BASE       0x02    /* Base address of scroll 1 */
#define CPS1_SCROLL2_BASE       0x04    /* Base address of scroll 2 */
#define CPS1_SCROLL3_BASE       0x06    /* Base address of scroll 3 */
#define CPS1_OTHER_BASE	 0x08    /* Base address of other video */
#define CPS1_PALETTE_BASE       0x0a    /* Base address of palette */
#define CPS1_SCROLL1_SCROLLX    0x0c    /* Scroll 1 X */
#define CPS1_SCROLL1_SCROLLY    0x0e    /* Scroll 1 Y */
#define CPS1_SCROLL2_SCROLLX    0x10    /* Scroll 2 X */
#define CPS1_SCROLL2_SCROLLY    0x12    /* Scroll 2 Y */
#define CPS1_SCROLL3_SCROLLX    0x14    /* Scroll 3 X */
#define CPS1_SCROLL3_SCROLLY    0x16    /* Scroll 3 Y */
#define CPS1_ROWSCROLL_OFFS     0x20    /* base of row scroll offsets in other RAM */

#define CPS1_SCROLL2_WIDTH      0x40
#define CPS1_SCROLL2_HEIGHT     0x40


/*
CPS1 VIDEO RENDERER

*/
const int cps1_gfx_region=1;	    /* MAME memory region to draw from */
static UINT32 *cps1_gfx;		 /* Converted GFX memory */
static int *cps1_char_pen_usage;	/* pen usage array */
static int *cps1_tile16_pen_usage;      /* pen usage array */
static int *cps1_tile32_pen_usage;      /* pen usage array */
static int cps1_max_char;	       /* Maximum number of 8x8 chars */
static int cps1_max_tile16;	     /* Maximum number of 16x16 tiles */
static int cps1_max_tile32;	     /* Maximum number of 32x32 tiles */

int cps1_gfx_start(void)
{
	UINT32 dwval;
	int size=Machine->memory_region_length[cps1_gfx_region];
	unsigned char *data = Machine->memory_region[cps1_gfx_region];
	int i,j,nchar,penusage,gfxsize;

	gfxsize=size/4;

	/* Set up maximum values */
	cps1_max_char  =(gfxsize/2)/8;
	cps1_max_tile16=(gfxsize/4)/8;
	cps1_max_tile32=(gfxsize/16)/8;

	cps1_gfx=malloc(gfxsize*sizeof(UINT32));
	if (!cps1_gfx)
	{
		return -1;
	}

	cps1_char_pen_usage=malloc(cps1_max_char*sizeof(int));
	if (!cps1_char_pen_usage)
	{
		return -1;
	}
	memset(cps1_char_pen_usage, 0, cps1_max_char*sizeof(int));

	cps1_tile16_pen_usage=malloc(cps1_max_tile16*sizeof(int));
	if (!cps1_tile16_pen_usage)
		return -1;
	memset(cps1_tile16_pen_usage, 0, cps1_max_tile16*sizeof(int));

	cps1_tile32_pen_usage=malloc(cps1_max_tile32*sizeof(int));
	if (!cps1_tile32_pen_usage)
	{
		return -1;
	}
	memset(cps1_tile32_pen_usage, 0, cps1_max_tile32*sizeof(int));

#if 0
	if (Machine->orientation & ORIENTATION_SWAP_XY)
	{
		unsigned char *p=data;
		for (i=0; i<gfxsize/2; i++)
		{
			int x;
		       nchar=i/8;  /* 8x8 char number */
		       dwval=0;

		       for (x=0; x<8; x++)
		       {
				int y=i%8;
				int n,mask;
				n=0;
				mask=0x80>>y;
				p=data+nchar*16+x*2;
				if (*(p+size/4)&mask)	   n|=1;
				if (*(p+size/4+1)&mask)	 n|=2;
				if (*(p+size/2+size/4)&mask)    n|=4;
				if (*(p+size/2+size/4+1)&mask)  n|=8;
				dwval|=n<<(28-x*4);
				penusage=1<<n;
				cps1_char_pen_usage[nchar]|=penusage;
				cps1_tile16_pen_usage[nchar/2]|=penusage;
				cps1_tile32_pen_usage[nchar/8]|=penusage;
		       }
		       cps1_gfx[2*i]=dwval;
		       dwval=0;
		       for (x=0; x<8; x++)
		       {
				int y=i%8;
				int n,mask;
				n=0;
				mask=0x80>>y;
				p=data+nchar*16+x*2;
				if (*(p)&mask)	   n|=1;
				if (*(p+1)&mask)	 n|=2;
				if (*(p+size/2)&mask)    n|=4;
				if (*(p+size/2+1)&mask)  n|=8;
				dwval|=n<<(28-x*4);
				penusage=1<<n;
				cps1_char_pen_usage[nchar]|=penusage;
				cps1_tile16_pen_usage[nchar/2]|=penusage;
				cps1_tile32_pen_usage[nchar/8]|=penusage;
		       }

		       cps1_gfx[2*i+1]=dwval;
		}

	}
	else
#endif
	{
		for (i=0; i<gfxsize/2; i++)
		{
		       nchar=i/8;  /* 8x8 char number */
		       dwval=0;
		       for (j=0; j<8; j++)
		       {
				int n,mask;
				n=0;
				mask=0x80>>j;
				if (*(data+size/4)&mask)	   n|=1;
				if (*(data+size/4+1)&mask)	 n|=2;
				if (*(data+size/2+size/4)&mask)    n|=4;
				if (*(data+size/2+size/4+1)&mask)  n|=8;
				dwval|=n<<(28-j*4);
				penusage=1<<n;
				cps1_char_pen_usage[nchar]|=penusage;
				cps1_tile16_pen_usage[nchar/2]|=penusage;
				cps1_tile32_pen_usage[nchar/8]|=penusage;
		       }
		       cps1_gfx[2*i]=dwval;
		       dwval=0;
		       for (j=0; j<8; j++)
		       {
				int n,mask;
				n=0;
				mask=0x80>>j;
				if (*(data)&mask)	  n|=1;
				if (*(data+1)&mask)	n|=2;
				if (*(data+size/2)&mask)   n|=4;
				if (*(data+size/2+1)&mask) n|=8;
				dwval|=n<<(28-j*4);
				penusage=1<<n;
				cps1_char_pen_usage[nchar]|=penusage;
				cps1_tile16_pen_usage[nchar/2]|=penusage;
				cps1_tile32_pen_usage[nchar/8]|=penusage;
		       }
		       cps1_gfx[2*i+1]=dwval;
		       data+=2;
		}
	}
	return 0;
}

void cps1_gfx_stop(void)
{
	if (cps1_gfx)
	{
		free(cps1_gfx);
	}
	if (cps1_char_pen_usage)
	{
		free(cps1_char_pen_usage);
	}
	if (cps1_tile16_pen_usage)
	{
		free(cps1_tile16_pen_usage);
	}
	if (cps1_tile32_pen_usage)
	{
		free(cps1_tile32_pen_usage);
	}
}

INLINE void cps1_draw_gfx(
	struct osd_bitmap *dest,
	const struct GfxElement *gfx,
	unsigned int code,
	int color,
	int flipx,
	int flipy,
	int sx,
	int sy,
	int tpens,
	int *pusage,
	int size,
	int max,
	int delta,
	int srcdelta)
{
	int i, j, ex, ey;
	UINT32 dwval,n;
	UINT32 *src;
	const unsigned short *paldata;

    tpens=(~tpens) & 0xffff;

    if ( code > max || !(tpens & pusage[code]))
	{
		/* No pens to draw (object is empty) */
		return;
	}

	if (Machine->orientation & ORIENTATION_SWAP_XY)
	{
		int temp;
		temp=sx;
		sx=sy;
	sy=dest->height-temp-size;
		temp=flipx;
		flipx=flipy;
	flipy=!temp;
	}

	if (cps1_flip_screen)
	{
		/* Handle flipped screen */
		flipx=!flipx;
		flipy=!flipy;
		sx=dest->width-sx-size;
		sy=dest->height-sy-size;
	}

	if (sx<0 || sx > dest->width-size || sy<0 || sy>dest->height-size )
	{
		return;
	}


	src = cps1_gfx+code*delta;
	paldata = &gfx->colortable[gfx->color_granularity * color];

	if (Machine->orientation & ORIENTATION_SWAP_XY)
	{
		if (flipy)
		{
		      sy+=size;
		      if (flipx)
		      {
				sx+=size;
				for (i=0; i<size; i++)
				{
					unsigned char *bm;
					int ny=sy;
					for (j=0; j<size/8; j++)
					{
					      dwval=*src;
					      n=(dwval>>28)&0x0f;
					      bm=dest->line[ny-1]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>24)&0x0f;
					      bm=dest->line[ny-2]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>20)&0x0f;
					      bm=dest->line[ny-3]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>16)&0x0f;
					      bm=dest->line[ny-4]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>12)&0x0f;
					      bm=dest->line[ny-5]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>8)&0x0f;
					      bm=dest->line[ny-6]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>4)&0x0f;
					      bm=dest->line[ny-7]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=dwval&0x0f;
					      bm=dest->line[ny-8]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      ny-=8;
					      src++;
					}
					sx--;
					src+=srcdelta;
				}
		      }
		      else
		      {
				for (i=0; i<size; i++)
				{
					unsigned char *bm;
					int ny=sy;
					for (j=0; j<size/8; j++)
					{
					      dwval=*src;
					      n=(dwval>>28)&0x0f;
					      bm=dest->line[ny-1]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>24)&0x0f;
					      bm=dest->line[ny-2]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>20)&0x0f;
					      bm=dest->line[ny-3]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>16)&0x0f;
					      bm=dest->line[ny-4]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>12)&0x0f;
					      bm=dest->line[ny-5]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>8)&0x0f;
					      bm=dest->line[ny-6]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>4)&0x0f;
					      bm=dest->line[ny-7]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=dwval&0x0f;
					      bm=dest->line[ny-8]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      ny-=8;
					      src++;
					}
					sx++;
					src+=srcdelta;
				}
		       }
		}
		else
		{
		       if (flipx)
		       {
				sx+=size;
				for (i=0; i<size; i++)
				{
					unsigned char *bm;
					int ny=sy;
					for (j=0; j<size/8; j++)
					{
					      dwval=*src;
					      n=(dwval>>28)&0x0f;
					      bm=dest->line[ny+0]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>24)&0x0f;
					      bm=dest->line[ny+1]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>20)&0x0f;
					      bm=dest->line[ny+2]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>16)&0x0f;
					      bm=dest->line[ny+3]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>12)&0x0f;
					      bm=dest->line[ny+4]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>8)&0x0f;
					      bm=dest->line[ny+5]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>4)&0x0f;
					      bm=dest->line[ny+6]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=dwval&0x0f;
					      bm=dest->line[ny+7]+sx;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      ny+=8;
					      src++;
					}
					sx--;
					src+=srcdelta;
				}
			}
			else
			{
				for (i=0; i<size; i++)
				{
					unsigned char *bm;
					int ny=sy;
					for (j=0; j<size/8; j++)
					{
					      dwval=*src;
					      n=(dwval>>28)&0x0f;
					      bm=dest->line[ny+0]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>24)&0x0f;
					      bm=dest->line[ny+1]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>20)&0x0f;
					      bm=dest->line[ny+2]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>16)&0x0f;
					      bm=dest->line[ny+3]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>12)&0x0f;
					      bm=dest->line[ny+4]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>8)&0x0f;
					      bm=dest->line[ny+5]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>4)&0x0f;
					      bm=dest->line[ny+6]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=dwval&0x0f;
					      bm=dest->line[ny+7]+sx;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      src++;
					      ny+=8;
					}
					sx++;
					src+=srcdelta;
				 }
			}
		}

	}
	else
	{
		if (flipy)
		{
		      sy+=size-1;
		      if (flipx)
		      {
				sx+=size;
				for (i=0; i<size; i++)
				{
					unsigned char *bm=dest->line[sy-i]+sx;
					for (j=0; j<size/8; j++)
					{
					      dwval=*src;
					      n=(dwval>>28)&0x0f;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>24)&0x0f;
					      if (tpens & (0x01 << n)) bm[-2]=paldata[n];
					      n=(dwval>>20)&0x0f;
					      if (tpens & (0x01 << n)) bm[-3]=paldata[n];
					      n=(dwval>>16)&0x0f;
					      if (tpens & (0x01 << n)) bm[-4]=paldata[n];
					      n=(dwval>>12)&0x0f;
					      if (tpens & (0x01 << n)) bm[-5]=paldata[n];
					      n=(dwval>>8)&0x0f;
					      if (tpens & (0x01 << n)) bm[-6]=paldata[n];
					      n=(dwval>>4)&0x0f;
					      if (tpens & (0x01 << n)) bm[-7]=paldata[n];
					      n=dwval&0x0f;
					      if (tpens & (0x01 << n)) bm[-8]=paldata[n];
					      src++;
					      bm-=8;
					}
					src+=srcdelta;
				}
		      }
		      else
		      {
				for (i=0; i<size; i++)
				{
					unsigned char *bm=dest->line[sy-i]+sx;
					for (j=0; j<size/8; j++)
					{
					      dwval=*src;
					      n=(dwval>>28)&0x0f;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>24)&0x0f;
					      if (tpens & (0x01 << n)) bm[1]=paldata[n];
					      n=(dwval>>20)&0x0f;
					      if (tpens & (0x01 << n)) bm[2]=paldata[n];
					      n=(dwval>>16)&0x0f;
					      if (tpens & (0x01 << n)) bm[3]=paldata[n];
					      n=(dwval>>12)&0x0f;
					      if (tpens & (0x01 << n)) bm[4]=paldata[n];
					      n=(dwval>>8)&0x0f;
					      if (tpens & (0x01 << n)) bm[5]=paldata[n];
					      n=(dwval>>4)&0x0f;
					      if (tpens & (0x01 << n)) bm[6]=paldata[n];
					      n=dwval&0x0f;
					      if (tpens & (0x01 << n)) bm[7]=paldata[n];
					      bm+=8;
					      src++;
					}
					src+=srcdelta;
				}
		       }
		}
		else
		{
		       if (flipx)
		       {
				sx+=size;
				for (i=0; i<size; i++)
				{
					unsigned char *bm=dest->line[sy+i]+sx;
					for (j=0; j<size/8; j++)
					{
					      dwval=*src;
					      n=(dwval>>28)&0x0f;
					      if (tpens & (0x01 << n)) bm[-1]=paldata[n];
					      n=(dwval>>24)&0x0f;
					      if (tpens & (0x01 << n)) bm[-2]=paldata[n];
					      n=(dwval>>20)&0x0f;
					      if (tpens & (0x01 << n)) bm[-3]=paldata[n];
					      n=(dwval>>16)&0x0f;
					      if (tpens & (0x01 << n)) bm[-4]=paldata[n];
					      n=(dwval>>12)&0x0f;
					      if (tpens & (0x01 << n)) bm[-5]=paldata[n];
					      n=(dwval>>8)&0x0f;
					      if (tpens & (0x01 << n)) bm[-6]=paldata[n];
					      n=(dwval>>4)&0x0f;
					      if (tpens & (0x01 << n)) bm[-7]=paldata[n];
					      n=dwval&0x0f;
					      if (tpens & (0x01 << n)) bm[-8]=paldata[n];
					      bm-=8;
					      src++;
					}
					src+=srcdelta;
				}
			}
			else
			{
				for (i=0; i<size; i++)
				{
					unsigned char *bm=dest->line[sy+i]+sx;
					for (j=0; j<size/8; j++)
					{
					      dwval=*src;
					      n=(dwval>>28)&0x0f;
					      if (tpens & (0x01 << n)) bm[0]=paldata[n];
					      n=(dwval>>24)&0x0f;
					      if (tpens & (0x01 << n)) bm[1]=paldata[n];
					      n=(dwval>>20)&0x0f;
					      if (tpens & (0x01 << n)) bm[2]=paldata[n];
					      n=(dwval>>16)&0x0f;
					      if (tpens & (0x01 << n)) bm[3]=paldata[n];
					      n=(dwval>>12)&0x0f;
					      if (tpens & (0x01 << n)) bm[4]=paldata[n];
					      n=(dwval>>8)&0x0f;
					      if (tpens & (0x01 << n)) bm[5]=paldata[n];
					      n=(dwval>>4)&0x0f;
					      if (tpens & (0x01 << n)) bm[6]=paldata[n];
					      n=dwval&0x0f;
					      if (tpens & (0x01 << n)) bm[7]=paldata[n];
					      src++;
					      bm+=8;
					}
					src+=srcdelta;
				 }
			}
		}
       }
}

INLINE void cps1_draw_scroll1(
	struct osd_bitmap *dest,
	unsigned int code, int color,
	int flipx, int flipy,int sx, int sy, int tpens)
{
	cps1_draw_gfx(dest,
		Machine->gfx[0],
		code,color,flipx,flipy,sx,sy,
		tpens,cps1_char_pen_usage,8, cps1_max_char, 16, 1);
}


INLINE void cps1_draw_tile16(struct osd_bitmap *dest,
	const struct GfxElement *gfx,
	unsigned int code, int color,
	int flipx, int flipy,int sx, int sy, int tpens)
{
	cps1_draw_gfx(dest,
		gfx,
		code,color,flipx,flipy,sx,sy,
		tpens,cps1_tile16_pen_usage,16, cps1_max_tile16, 16*2,0);
}

INLINE void cps1_draw_tile32(struct osd_bitmap *dest,
	const struct GfxElement *gfx,
	unsigned int code, int color,
	int flipx, int flipy,int sx, int sy, int tpens)
{
	cps1_draw_gfx(dest,
		gfx,
		code,color,flipx,flipy,sx,sy,
		tpens,cps1_tile32_pen_usage,32, cps1_max_tile32, 16*2*4,0);
}





static int cps1_transparency_scroll[4];



#ifdef MAME_DEBUG
void cps1_dump_video(void)
{
	FILE *fp;
	fp=fopen("SCROLL1.DMP", "w+b");
	if (fp)
	{
		fwrite(cps1_scroll1, cps1_scroll1_size, 1, fp);
		fclose(fp);
	}
	fp=fopen("SCROLL2.DMP", "w+b");
	if (fp)
	{
		fwrite(cps1_scroll2, cps1_scroll2_size, 1, fp);
		fclose(fp);
	}
	fp=fopen("SCROLL3.DMP", "w+b");
	if (fp)
	{
		fwrite(cps1_scroll3, cps1_scroll3_size, 1, fp);
		fclose(fp);
	}
	fp=fopen("OBJ.DMP", "w+b");
	if (fp)
	{
		fwrite(cps1_obj, cps1_obj_size, 1, fp);
		fclose(fp);
	}

	fp=fopen("OTHER.DMP", "w+b");
	if (fp)
	{
		fwrite(cps1_other, cps1_other_size, 1, fp);
		fclose(fp);
	}

	fp=fopen("PALETTE.DMP", "w+b");
	if (fp)
	{
		fwrite(cps1_palette, cps1_palette_size, 1, fp);
		fclose(fp);
	}

	fp=fopen("OUTPUT.DMP", "w+b");
	if (fp)
	{
		fwrite(cps1_output, cps1_output_size, 1, fp);
		fclose(fp);
	}
	fp=fopen("VIDEO.DMP", "w+b");
	if (fp)
	{
		fwrite(cps1_gfxram, cps1_gfxram_size, 1, fp);
		fclose(fp);
	}

}
#endif


INLINE void cps1_get_video_base(void )
{
	int layercontrol;


	/* Re-calculate the VIDEO RAM base */
	cps1_scroll1=cps1_base(CPS1_SCROLL1_BASE);
	cps1_scroll2=cps1_base(CPS1_SCROLL2_BASE);
	cps1_scroll3=cps1_base(CPS1_SCROLL3_BASE);
	cps1_obj=cps1_base(CPS1_OBJ_BASE);
	cps1_palette=cps1_base(CPS1_PALETTE_BASE);
	cps1_other=cps1_base(CPS1_OTHER_BASE);

	/* Get scroll values */
	scroll1x=cps1_port(CPS1_SCROLL1_SCROLLX);
	scroll1y=cps1_port(CPS1_SCROLL1_SCROLLY);
	scroll2x=cps1_port(CPS1_SCROLL2_SCROLLX);
	scroll2y=cps1_port(CPS1_SCROLL2_SCROLLY);
	scroll3x=cps1_port(CPS1_SCROLL3_SCROLLX);
	scroll3y=cps1_port(CPS1_SCROLL3_SCROLLY);

	/* Get transparency registers */
	if (cps1_game_config->priority1)
	{
		cps1_transparency_scroll[0]=~cps1_port(cps1_game_config->priority0);
		cps1_transparency_scroll[1]=~cps1_port(cps1_game_config->priority1);
		cps1_transparency_scroll[2]=~cps1_port(cps1_game_config->priority2);
		cps1_transparency_scroll[3]=~cps1_port(cps1_game_config->priority3);
	 }

	/* Get layer enable bits */
	layercontrol=READ_WORD(&cps1_output[cps1_game_config->layer_control]);
	cps1_layer_enabled[0]=1;
	cps1_layer_enabled[1]=layercontrol & cps1_game_config->scrl1_enable_mask;
	cps1_layer_enabled[2]=layercontrol & cps1_game_config->scrl2_enable_mask;
	cps1_layer_enabled[3]=layercontrol & cps1_game_config->scrl3_enable_mask;
#ifdef MAME_DEBUG
{
	char baf[40];
	int enablemask;

if (osd_key_pressed(OSD_KEY_Z))
{
	if (osd_key_pressed(OSD_KEY_Q)) cps1_layer_enabled[3]=0;
	if (osd_key_pressed(OSD_KEY_W)) cps1_layer_enabled[2]=0;
	if (osd_key_pressed(OSD_KEY_E)) cps1_layer_enabled[1]=0;
	if (osd_key_pressed(OSD_KEY_R)) cps1_layer_enabled[0]=0;
	if (osd_key_pressed(OSD_KEY_T))
	{
		sprintf(baf,"layer %02x",layercontrol&0xc03f);
		usrintf_showmessage(baf);
	}
}

	enablemask = cps1_game_config->scrl1_enable_mask | cps1_game_config->scrl2_enable_mask | cps1_game_config->scrl3_enable_mask;

	if (cps1_game_config->scrl1_enable_mask == cps1_game_config->scrl2_enable_mask ||
			cps1_game_config->scrl1_enable_mask == cps1_game_config->scrl3_enable_mask ||
			cps1_game_config->scrl2_enable_mask == cps1_game_config->scrl3_enable_mask)
	{
		if (((layercontrol & enablemask) && (layercontrol & enablemask) != enablemask))
		{
			sprintf(baf,"layer %02x",layercontrol&0xc03f);
			usrintf_showmessage(baf);
		}
	}

	if (((layercontrol & ~enablemask) & 0xc03e) != 0)
	{
		sprintf(baf,"layer %02x",layercontrol&0xc03f);
		usrintf_showmessage(baf);
	}
}
#endif
}


/***************************************************************************

  Start the video hardware emulation.

***************************************************************************/

int cps1_vh_start(void)
{
	int i;

	cps1_init_machine();

	if (cps1_gfx_start())
	{
		return -1;
	}

	if (Machine->orientation & ORIENTATION_SWAP_XY)
	{
		cps1_scroll2_bitmap=osd_new_bitmap(CPS1_SCROLL2_HEIGHT*16,
			CPS1_SCROLL2_WIDTH*16, Machine->scrbitmap->depth );
	}
	else
	{
		cps1_scroll2_bitmap=osd_new_bitmap(CPS1_SCROLL2_WIDTH*16,
			CPS1_SCROLL2_HEIGHT*16, Machine->scrbitmap->depth );
	}
	if (!cps1_scroll2_bitmap)
	{
		return -1;
	}
	cps1_scroll2_old=malloc(cps1_scroll2_size);
	if (!cps1_scroll2_old)
	{
		return -1;
	}
	memset(cps1_scroll2_old, 0xff, cps1_scroll2_size);


	cps1_old_palette=(unsigned char *)malloc(cps1_palette_size);
	if (!cps1_old_palette)
	{
		return -1;
	}
	memset(cps1_old_palette, 0x00, cps1_palette_size);
	for (i = 0;i < cps1_palette_entries*16;i++)
	{
	   palette_change_color(i,0,0,0);
	}

	memset(cps1_gfxram, 0, cps1_gfxram_size);   /* Clear GFX RAM */
	memset(cps1_output, 0, cps1_output_size);   /* Clear output ports */

	/* Put in some defaults */
	WRITE_WORD(&cps1_output[0x00], 0x9200);
	WRITE_WORD(&cps1_output[0x02], 0x9000);
	WRITE_WORD(&cps1_output[0x04], 0x9040);
	WRITE_WORD(&cps1_output[0x06], 0x9080);
	WRITE_WORD(&cps1_output[0x08], 0x9100);
	WRITE_WORD(&cps1_output[0x0a], 0x90c0);

	if (!cps1_game_config)
	{
		if (errorlog)
		{
			fprintf(errorlog, "cps1_game_config hasn't been set up yet");
		}
		return -1;
	}


	/* Set up old base */
	cps1_get_video_base();   /* Calculate base pointers */
	cps1_get_video_base();   /* Calculate old base pointers */


	for (i=0; i<4; i++)
	{
		cps1_transparency_scroll[i]=0xffff;
	}
	return 0;
}

/***************************************************************************

  Stop the video hardware emulation.

***************************************************************************/
void cps1_vh_stop(void)
{
	if (cps1_old_palette)
		free(cps1_old_palette);
	if (cps1_scroll2_bitmap)
		osd_free_bitmap(cps1_scroll2_bitmap);
	if (cps1_scroll2_old)
		free(cps1_scroll2_old);
	cps1_gfx_stop();
}

/***************************************************************************

  Build palette from palette RAM

  12 bit RGB with a 4 bit brightness value.

***************************************************************************/

void cps1_build_palette(void)
{
	int offset;

	for (offset = 0; offset < cps1_palette_entries*16; offset++)
	{
		int palette = READ_WORD (&cps1_palette[offset * 2]);

		if (palette != READ_WORD (&cps1_old_palette[offset * 2]) )
		{
		   int red, green, blue, bright;

		   bright= (palette>>12);
		   if (bright) bright += 2;

		   red   = ((palette>>8)&0x0f) * bright;
		   green = ((palette>>4)&0x0f) * bright;
		   blue  = (palette&0x0f) * bright;

		   palette_change_color (offset, red, green, blue);
		   WRITE_WORD(&cps1_old_palette[offset * 2], palette);
		}
	}
}

/***************************************************************************

  Scroll 1 (8x8)

  Attribute word layout:
  0x0001	colour
  0x0002	colour
  0x0004	colour
  0x0008	colour
  0x0010	colour
  0x0020	X Flip
  0x0040	Y Flip
  0x0080
  0x0100
  0x0200
  0x0400
  0x0800
  0x1000
  0x2000
  0x4000
  0x8000


***************************************************************************/

void cps1_palette_scroll1(unsigned short *base)
{
	int x,y, offs, offsx;

	int scrlxrough=(scroll1x>>3)+8;
	int scrlyrough=(scroll1y>>3);

	int elements = Machine->gfx[0]->total_elements;

	for (x=0; x<0x36; x++)
	{
		 offsx=(scrlxrough+x)*0x80;
		 offsx&=0x1fff;

		 for (y=0; y<0x20; y++)
		 {
			int code, colour, offsy;
			int n=scrlyrough+y;
			offsy=( (n&0x1f)*4 | ((n&0x20)*0x100)) & 0x3fff;
			offs=offsy+offsx;
			offs &= 0x3fff;
			code=READ_WORD(&cps1_scroll1[offs]);
			colour=READ_WORD(&cps1_scroll1[offs+2]);
			code+=cps1_game_config->bank_scroll1*0x08000;
			base[colour&0x1f] |=
			      cps1_char_pen_usage[code % cps1_max_char]&0x7fff;
		}
	}
}

void cps1_render_scroll1(struct osd_bitmap *bitmap, int priority)
{
	int x,y, offs, offsx, sx, sy, ytop;

	int scrlxrough=(scroll1x>>3)+4;
	int scrlyrough=(scroll1y>>3);

	sx=-(scroll1x&0x07);
	ytop=-(scroll1y&0x07)+32;

	for (x=0; x<0x35; x++)
	{
		 sy=ytop;
		 offsx=(scrlxrough+x)*0x80;
		 offsx&=0x1fff;

		 for (y=0; y<0x20; y++)
		 {
			int code, offsy, colour;
			int n=scrlyrough+y;
			offsy=( (n&0x1f)*4 | ((n&0x20)*0x100)) & 0x3fff;
			offs=offsy+offsx;
			offs &= 0x3fff;

			code=READ_WORD(&cps1_scroll1[offs]);
			colour=READ_WORD(&cps1_scroll1[offs+2]);

			if (code != 0x20 &&
					code != 0xf020)	/* knights */
			{
				/* 0x0020 appears to never be drawn */
				code+=cps1_game_config->bank_scroll1*0x08000;
				if (!priority)
				{
					cps1_draw_scroll1(bitmap,
					     code,
					     colour&0x1f,
					     colour&0x20,
					     colour&0x40,
					     sx,sy,0x8000);
				}
				else
				{
					int transp;
					transp=cps1_transparency_scroll[(colour & 0x0180)>>7];
					if (transp != 0xffff)
					{
						cps1_draw_scroll1(bitmap,
							 code,
							 colour&0x1f,
							 colour&0x20,
							 colour&0x40,
							 sx,sy,transp);
					 }
				}
			 }
			 sy+=8;
		 }
		 sx+=8;
	}
}




/***************************************************************************

								Sprites
								=======

  Sprites are represented by a number of 8 byte values

  xx xx yy yy nn nn aa aa

  where xxxx = x position
		yyyy = y position
		nnnn = tile number
		aaaa = attribute word
					0x0001	colour
					0x0002	colour
					0x0004	colour
					0x0008	colour
					0x0010	colour
					0x0020	X Flip
					0x0040	Y Flip
					0x0080	unknown
					0x0100	X block size (in sprites)
					0x0200	X block size
					0x0400	X block size
					0x0800	X block size
					0x1000	Y block size (in sprites)
					0x2000	Y block size
					0x4000	Y block size
					0x8000	Y block size

  The end of the table (may) be marked by an attribute value of 0xff00.

***************************************************************************/

void cps1_find_last_sprite(void)    /* Find the offset of last sprite */
{
	int offset=6;
	/* Locate the end of table marker */
	while (offset < cps1_obj_size)
	{
		int colour=READ_WORD(&cps1_obj[offset]);
		if (colour == 0xff00)
		{
			/* Marker found. This is the last sprite. */
			cps1_last_sprite_offset=offset-6-8;
			return;
		}
		offset+=8;
	}
	/* Sprites must use full sprite RAM */
	cps1_last_sprite_offset=cps1_obj_size-8;
}

/* Find used colours */
void cps1_palette_sprites(unsigned short *base)
{
	int i;

	for (i=cps1_last_sprite_offset; i>=0; i-=8)
	{
		int x=READ_WORD(&cps1_obj[i]);
		int y=READ_WORD(&cps1_obj[i+2]);
		if (x && y)
		{
	    int colour=READ_WORD(&cps1_obj[i+6]);
	    int col=colour&0x1f;
			unsigned int code=READ_WORD(&cps1_obj[i+4]);
			if (cps1_game_config->kludge == 1 && code >= 0x01000)
			{
			       code += 0x4000;
			}
			if (cps1_game_config->kludge == 2 && code >= 0x02a00)
			{
			       code += 0x4000;
			}

	    if ( colour & 0xff00 )
			{
		int nys, nxs;
		int nx=(colour & 0x0f00) >> 8;
		int ny=(colour & 0xf000) >> 12;
		nx++;
		ny++;

		if (colour & 0x40)   /* Y Flip */					      /* Y flip */
		{
		    if (colour &0x20)
		    {
			for (nys=0; nys<ny; nys++)
			{
			    for (nxs=0; nxs<nx; nxs++)
			    {
				int cod=code+(nx-1)-nxs+0x10*(ny-1-nys);
				base[col] |=
				    cps1_tile16_pen_usage[cod % cps1_max_tile16];
			    }
			}
		    }
		    else
		    {
			for (nys=0; nys<ny; nys++)
			{
			    for (nxs=0; nxs<nx; nxs++)
			    {
				int cod=code+nxs+0x10*(ny-1-nys);
				base[col] |=
					cps1_tile16_pen_usage[cod % cps1_max_tile16];
			    }
			}
					}
		}
		else
		{
		    if (colour &0x20)
		    {
			 for (nys=0; nys<ny; nys++)
			 {
			      for (nxs=0; nxs<nx; nxs++)
			      {
				    int cod=code+(nx-1)-nxs+0x10*nys;
				    base[col] |=
					cps1_tile16_pen_usage[cod % cps1_max_tile16];

								}
			  }
		     }
		     else
		     {
							for (nys=0; nys<ny; nys++)
							{
								for (nxs=0; nxs<nx; nxs++)
								{
				    int cod=code+nxs+0x10*nys;
				    base[col] |=
					cps1_tile16_pen_usage[cod % cps1_max_tile16];
								}
							}
		      }
		  }
		  base[col]&=0x7fff;
	      }
	      else
	      {
			base[col] |=
			    cps1_tile16_pen_usage[code % cps1_max_tile16]&0x7fff;
	      }
	 }
	}
}



void cps1_render_sprites(struct osd_bitmap *bitmap)
{
	int i;
	int base_obj=0;

	/* Draw the sprites */
	for (i=cps1_last_sprite_offset; i>=0; i-=8)
	{
	int x=READ_WORD(&cps1_obj[i]);
	int y=READ_WORD(&cps1_obj[i+2]);
		if (x && y )
		{
			unsigned int code=READ_WORD(&cps1_obj[i+4]);
			int colour=READ_WORD(&cps1_obj[i+6]);
			int col=colour&0x1f;

			y &= 0x1ff;
			if (y > 450) y -= 0x200;

			/* in cawing, skyscrapers parts on level 2 have all the top bits of the */
			/* x coordinate set. Does this have a special meaning? */
			x &= 0x1ff;
			if (x > 450) x -= 0x200;

			x-=0x20;
			y+=0x20;

			if (cps1_game_config->kludge == 1 && code >= 0x01000)
			{
			       code += 0x4000;
			}
			if (cps1_game_config->kludge == 2 && code >= 0x02a00)
			{
			       code += 0x4000;
			}

			if (colour & 0xff00 )
			{
					/* handle blocked sprites */
					int nx=(colour & 0x0f00) >> 8;
					int ny=(colour & 0xf000) >> 12;
					int nxs,nys,sx,sy;
					nx++;
					ny++;

					if (colour & 0x40)
					{
						/* Y flip */
						if (colour &0x20)
						{
							for (nys=0; nys<ny; nys++)
							{
								for (nxs=0; nxs<nx; nxs++)
								{
									sx = x+nxs*16;
									sy = y+nys*16;
									if (sx > 450) sx -= 0x200;
									if (sy > 450) sy -= 0x200;

									cps1_draw_tile16(bitmap,Machine->gfx[1],
										code+(nx-1)-nxs+0x10*(ny-1-nys),
										col&0x1f,
										1,1,
										sx,sy,0x8000);
								}
							}
						}
						else
						{
							for (nys=0; nys<ny; nys++)
							{
								for (nxs=0; nxs<nx; nxs++)
								{
									sx = x+nxs*16;
									sy = y+nys*16;
									if (sx > 450) sx -= 0x200;
									if (sy > 450) sy -= 0x200;

									cps1_draw_tile16(bitmap,Machine->gfx[1],
										code+nxs+0x10*(ny-1-nys),
										col&0x1f,
										0,1,
										sx,sy,0x8000 );
								}
							}
						}
					}
					else
					{
						if (colour &0x20)
						{
							for (nys=0; nys<ny; nys++)
							{
								for (nxs=0; nxs<nx; nxs++)
								{
									sx = x+nxs*16;
									sy = y+nys*16;
									if (sx > 450) sx -= 0x200;
									if (sy > 450) sy -= 0x200;

									cps1_draw_tile16(bitmap,Machine->gfx[1],
										code+(nx-1)-nxs+0x10*nys,
										col&0x1f,
										1,0,
										sx,sy,0x8000
										);
								}
							}
						}
						else
						{
							for (nys=0; nys<ny; nys++)
							{
								for (nxs=0; nxs<nx; nxs++)
								{
									sx = x+nxs*16;
									sy = y+nys*16;
									if (sx > 450) sx -= 0x200;
									if (sy > 450) sy -= 0x200;

									cps1_draw_tile16(bitmap,Machine->gfx[1],
										code+nxs+0x10*nys,
										col&0x1f,
										0,0,
										sx,sy, 0x8000);
								}
							}
						}
					}
				}
				else
				{
					/* Simple case... 1 sprite */
					cps1_draw_tile16(bitmap,Machine->gfx[1],
						   code,
						   col&0x1f,
						   colour&0x20,colour&0x40,
						   x,y,0x8000);
				}
		}
	}
}




/***************************************************************************

  Scroll 2 (16x16 layer)

  Attribute word layout:
  0x0001	colour
  0x0002	colour
  0x0004	colour
  0x0008	colour
  0x0010	colour
  0x0020	X Flip
  0x0040	Y Flip
  0x0080	??? Priority
  0x0100	??? Priority
  0x0200
  0x0400
  0x0800
  0x1000
  0x2000
  0x4000
  0x8000


***************************************************************************/

void cps1_palette_scroll2(unsigned short *base)
{
	int elements = Machine->gfx[2]->total_elements;
	int offs, code, colour;
	for (offs=cps1_scroll2_size-4; offs>=0; offs-=4)
	{
		code=READ_WORD(&cps1_scroll2[offs]);
		colour=READ_WORD(&cps1_scroll2[offs+2])&0x1f;
		code+=cps1_game_config->bank_scroll2*0x04000;
		if (cps1_game_config->kludge == 4 && (code < 0x2800 || code > 0x37ff))
			code = 0x2800;
		base[colour] |= cps1_tile16_pen_usage[code % cps1_max_tile16];
	}
}

void cps1_render_scroll2_bitmap(struct osd_bitmap *bitmap, int priority)
{
	int sx, sy, scrly;
	int ny=(scroll2y>>4);	  /* Rough Y */

	for (sx=0; sx<CPS1_SCROLL2_WIDTH; sx++)
	{
		int n=ny;
		for (sy=0; sy<0x09*2; sy++)
		{
			long newvalue;
			int offsy, offsx, offs, colour, code;

			n&=0x3f;
			offsy  = ((n&0x0f)*4 | ((n&0x30)*0x100))&0x3fff;
			offsx=(sx*0x040)&0xfff;
			offs=offsy+offsx;

			colour=READ_WORD(&cps1_scroll2[offs+2]);

			newvalue=*(long*)(&cps1_scroll2[offs]);
			if ( newvalue != *(long*)(&cps1_scroll2_old[offs]) )
			{
				*(long*)(&cps1_scroll2_old[offs])=newvalue;
				code=READ_WORD(&cps1_scroll2[offs]);
				code+=cps1_game_config->bank_scroll2*0x04000;
				if (cps1_game_config->kludge == 4 && (code < 0x2800 || code > 0x37ff))
					code = 0x2800;
				/* Draw entire tile */
				cps1_draw_tile16(bitmap,
						Machine->gfx[2],
						code,
						colour&0x1f,
						colour&0x20,colour&0x40,
						16*sx,16*n,
						0x0000);
			}
			n++;
		}
	}
}


void cps1_render_scroll2_high(struct osd_bitmap *bitmap, int priority)
{
#ifdef LAYER_DEBUG
	static int s=0;
#endif
	int sx, sy;
	int nxoffset=(scroll2x&0x0f)+32;    /* Smooth X */
	int nyoffset=(scroll2y&0x0f);    /* Smooth Y */
	int nx=(scroll2x>>4);	  /* Rough X */
	int ny=(scroll2y>>4)-4;	/* Rough Y */

	for (sx=0; sx<0x32/2+4; sx++)
	{
		for (sy=0; sy<0x09*2; sy++)
		{
			int offsy, offsx, offs, colour, code, mask;
			int n;
			n=ny+sy+2;
			offsy  = ((n&0x0f)*4 | ((n&0x30)*0x100))&0x3fff;
			offsx=((nx+sx)*0x040)&0xfff;
			offs=offsy+offsx;
			offs &= 0x3fff;

			code=READ_WORD(&cps1_scroll2[offs]);
			colour=READ_WORD(&cps1_scroll2[offs+2]);
			mask=colour & 0x0180;
			if (priority && mask)
			{
				int transp;
				transp=cps1_transparency_scroll[(mask>>7)&3];
				code+=cps1_game_config->bank_scroll2*0x04000;
				if (cps1_game_config->kludge == 4 && (code < 0x2800 || code > 0x37ff))
					code = 0x2800;
				cps1_draw_tile16(bitmap,
							Machine->gfx[2],
							code,
							colour&0x1f,
							colour&0x20,colour&0x40,
							16*sx-nxoffset,
							16*sy-nyoffset,
							transp);
			}
		}
	}
}

void cps1_render_scroll2_low(struct osd_bitmap *bitmap, int priority)
{
      int scrly=-(scroll2y-0x20);
      int scrlx=-(scroll2x+0x40-0x20);

      if (cps1_flip_screen)
      {
	    scrly=(CPS1_SCROLL2_HEIGHT*16)-scrly;
      }

      cps1_render_scroll2_bitmap(cps1_scroll2_bitmap, priority);

      copyscrollbitmap(bitmap,cps1_scroll2_bitmap,1,&scrlx,1,&scrly,&Machine->drv->visible_area,TRANSPARENCY_PEN,palette_transparent_pen);
}


void cps1_render_scroll2_distort(struct osd_bitmap *bitmap, int priority)
{
	int other=0;
	int scrly=-scroll2y;
	int i,scrollx[1024];
	int otheroffs;

/*
	Games known to use row scrolling:

	SF2
	Mega Twins (underwater, cave)
	Carrier Air Wing (hazy background at beginning of mission 8)
	Magic Sword (fire on floor 3; screen distort after continue)
	Varth (title screen)
*/

	if (cps1_flip_screen)
		scrly=(CPS1_SCROLL2_HEIGHT*16)-scrly;

	cps1_render_scroll2_bitmap(cps1_scroll2_bitmap, priority);

	otheroffs = cps1_port(CPS1_ROWSCROLL_OFFS);

	for (i = 0;i < 256;i++)
		scrollx[(i - scrly) & 0x3ff] = -(scroll2x+0x40-0x20) - READ_WORD(&cps1_other[(2*(i + otheroffs)) & 0x7ff]);

	scrly+=0x20;

	copyscrollbitmap(bitmap,cps1_scroll2_bitmap,1024,scrollx,1,&scrly,&Machine->drv->visible_area,TRANSPARENCY_PEN,palette_transparent_pen);
}


/***************************************************************************

  Scroll 3 (32x32 layer)

  Attribute word layout:
  0x0001	colour
  0x0002	colour
  0x0004	colour
  0x0008	colour
  0x0010	colour
  0x0020	X Flip
  0x0040	Y Flip
  0x0080
  0x0100
  0x0200
  0x0400
  0x0800
  0x1000
  0x2000
  0x4000
  0x8000

***************************************************************************/

void cps1_palette_scroll3(unsigned short *base)
{
	int sx,sy;
	int nx=(scroll3x>>5)+1;
	int ny=(scroll3y>>5)-1;

	int elements = Machine->gfx[3]->total_elements;

	for (sx=0; sx<0x32/4+2; sx++)
	{
		for (sy=0; sy<0x20/4+2; sy++)
		{
			int offsy, offsx, offs, colour, code;
			int n;
			n=ny+sy;
			offsy  = ((n&0x07)*4 | ((n&0xf8)*0x0100))&0x3fff;
			offsx=((nx+sx)*0x020)&0x7ff;
			offs=offsy+offsx;
			offs &= 0x3fff;
			code=READ_WORD(&cps1_scroll3[offs]);
			code+=cps1_game_config->bank_scroll3*0x01000;
			if (cps1_game_config->kludge == 2 && code >= 0x01500)
			{
			       code -= 0x1000;
			}
			colour=READ_WORD(&cps1_scroll3[offs+2]);
			base[colour&0x1f] |=
				cps1_tile32_pen_usage[code % cps1_max_tile32];
		}
	}
}


void cps1_render_scroll3(struct osd_bitmap *bitmap, int priority)
{
	int sx,sy;
	int nxoffset=scroll3x&0x1f;
	int nyoffset=(scroll3y&0x1f); //+0x10;
	int nx=(scroll3x>>5)+1;
	int ny=(scroll3y>>5)-1;  /* -1 */

	for (sx=1; sx<0x32/4+2; sx++)
	{
		for (sy=1; sy<0x20/4+2; sy++)
		{
			int offsy, offsx, offs, colour, code, transp;
			int n;
			n=ny+sy;
			offsy  = ((n&0x07)*4 | ((n&0xf8)*0x0100))&0x3fff;
			offsx=((nx+sx)*0x020)&0x7ff;
			offs=offsy+offsx;
			offs &= 0x3fff;
			code=READ_WORD(&cps1_scroll3[offs]);
			code+=cps1_game_config->bank_scroll3*0x01000;
			if (cps1_game_config->kludge == 2 && code >= 0x01500)
			{
			       code -= 0x1000;
			}
			colour=READ_WORD(&cps1_scroll3[offs+2]);
			if (priority)
			{
					int mask=colour & 0x0180;
					if (mask && !(colour & 0xf000))
					{
						transp=cps1_transparency_scroll[mask>>7];
						cps1_draw_tile32(bitmap,
								Machine->gfx[3],
								code,
								colour&0x1f,
								colour&0x20,
								colour&0x40,
								32*sx-nxoffset,
								32*sy-nyoffset,
								transp);
					}
			 }
			 else
			 {
				cps1_draw_tile32(bitmap,Machine->gfx[3],
						code,
						colour&0x1f,
						colour&0x20,colour&0x40,
						32*sx-nxoffset,32*sy-nyoffset,
						0x8000);

			 }
		}
	}
}


void cps1_render_layer(struct osd_bitmap *bitmap, int layer, int distort)
{
	if (cps1_layer_enabled[layer])
	{
		switch (layer)
		{
			case 0:
				cps1_render_sprites(bitmap);
				break;
			case 1:
				cps1_render_scroll1(bitmap, 0);
				break;
			case 2:
				if (distort)
					cps1_render_scroll2_distort(bitmap, 0);
				else
					cps1_render_scroll2_low(bitmap, 0);
				break;
			case 3:
				cps1_render_scroll3(bitmap, 0);
				break;
		}
	}
}

void cps1_render_high_layer(struct osd_bitmap *bitmap, int layer)
{
	if (cps1_layer_enabled[layer])
	{
		switch (layer)
		{
			case 0:
				/* there are no high priority sprites */
				break;
			case 1:
				cps1_render_scroll1(bitmap, 1);
				break;
			case 2:
				cps1_render_scroll2_high(bitmap, 1);
				break;
			case 3:
				cps1_render_scroll3(bitmap, 1);
				break;
		}
	}
}


/***************************************************************************

	Refresh screen

***************************************************************************/

void cps1_vh_screenrefresh(struct osd_bitmap *bitmap,int full_refresh)
{
	unsigned short palette_usage[cps1_palette_entries];
	int layercontrol;
	int i,offset;
	int distort_scroll2=0;
	int layer;
	int videocontrol=cps1_port(0x22);
	int old_flip;


	old_flip=cps1_flip_screen;
	cps1_flip_screen=videocontrol&0x8000;
	if (old_flip != cps1_flip_screen)
	{
		 /* Mark all of scroll 2 as dirty */
		memset(cps1_scroll2_old, 0xff, cps1_scroll2_size);
	}

	layercontrol=READ_WORD(&cps1_output[cps1_game_config->layer_control]);

	distort_scroll2 = layercontrol & 0x01;

	/* row scroll is always on in the SF2 series */
	if (cps1_game_config->kludge == 3) distort_scroll2 = 1;

	/* Get video memory base registers */
	cps1_get_video_base();

	/* Find the offset of the last sprite in the sprite table */
	cps1_find_last_sprite();

	/* Build palette */
	cps1_build_palette();

	/* Compute the used portion of the palette */
	memset (palette_usage, 0, sizeof (palette_usage));
	cps1_palette_sprites (&palette_usage[cps1_obj_palette]);
	if (cps1_layer_enabled[1])
		cps1_palette_scroll1 (&palette_usage[cps1_scroll1_palette]);
	if (cps1_layer_enabled[2])
		cps1_palette_scroll2 (&palette_usage[cps1_scroll2_palette]);
	else
		memset(cps1_scroll2_old, 0xff, cps1_scroll2_size);
	if (cps1_layer_enabled[3])
		cps1_palette_scroll3 (&palette_usage[cps1_scroll3_palette]);

	for (i = offset = 0; i < cps1_palette_entries; i++)
	{
		int usage = palette_usage[i];
		if (usage)
		{
			int j;
			for (j = 0; j < 15; j++)
			{
				if (usage & (1 << j))
					palette_used_colors[offset++] = PALETTE_COLOR_USED;
				else
					palette_used_colors[offset++] = PALETTE_COLOR_UNUSED;
			}
			palette_used_colors[offset++] = PALETTE_COLOR_TRANSPARENT;
		}
		else
		{
			memset (&palette_used_colors[offset], PALETTE_COLOR_UNUSED, 16);
			offset += 16;
		}
	}

	if (palette_recalc ())
	{
		 /* Mark all of scroll 2 as dirty */
		memset(cps1_scroll2_old, 0xff, cps1_scroll2_size);
	}

	/* Blank screen */
	fillbitmap(bitmap,palette_transparent_pen,&Machine->drv->visible_area);


	/* Draw layers */
	cps1_render_layer(bitmap, (layercontrol>>0x06)&03, distort_scroll2);
	cps1_render_layer(bitmap, (layercontrol>>0x08)&03, distort_scroll2);
	cps1_render_layer(bitmap, (layercontrol>>0x0a)&03, distort_scroll2);
	layer=(layercontrol>>0x0c)&03;
	if (layer != 1)
	{
		/*
		Don't draw layer 1 if it is the highest priority layer
		We will draw it later.
		*/
		cps1_render_layer(bitmap, (layercontrol>>0x0c)&03, distort_scroll2);
	}

	/* Draw high priority layers */
	cps1_render_high_layer(bitmap, (layercontrol>>0x06)&03);
	cps1_render_high_layer(bitmap, (layercontrol>>0x08)&03);
	cps1_render_high_layer(bitmap, (layercontrol>>0x0a)&03);
	if (layer==1)
	{
		/*
		Scroll 1 is highest priority. Must draw it over high
		priority scroll parts of 2 and 3. This is correct
		behaviour since Magic Sword interludes do not
		look correct unless we do this.
		*/
		if (cps1_layer_enabled[1])
			cps1_render_scroll1(bitmap, 0);
	}
	else
	{
		cps1_render_high_layer(bitmap, (layercontrol>>0x0c)&03);
	}
}
