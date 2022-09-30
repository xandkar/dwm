/* vim:set noexpandtab tabstop=8 shiftwidth=8: */
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/* TODO: Consider Xresources to switch between per-DPI configs */
#include "config_dpi.h"

/* The 8 terminal colors, Zenburn */
/* #3f3f3f ends-up lokking green, so using #3a3a3a instead... */
/* The 8 bright terminal colors, Zenburn */
/* #709080 is actually green, even if it looks blackish in the remoinal, so
 * using #262626 instead */

/*                              Zenburn  */
#define  COL_NORMAL_ZB_BLACK    "#3f3f3f"
#define  COL_NORMAL_ZB_RED      "#705050"
#define  COL_NORMAL_ZB_GREEN    "#87af87"
#define  COL_NORMAL_ZB_YELLOW   "#dfaf8f"
#define  COL_NORMAL_ZB_BLUE     "#9ab8d7"
#define  COL_NORMAL_ZB_MAGENTA  "#dc8cc3"
#define  COL_NORMAL_ZB_CYAN     "#8cd0d3"
#define  COL_NORMAL_ZB_GRAY     "#dcdccc"
#define  COL_BRIGHT_ZB_BLACK    "#262626"
#define  COL_BRIGHT_ZB_RED      "#dca3a3"
#define  COL_BRIGHT_ZB_GREEN    "#72d5a3"
#define  COL_BRIGHT_ZB_YELLOW   "#f0dfaf"
#define  COL_BRIGHT_ZB_BLUE     "#94bff3"
#define  COL_BRIGHT_ZB_MAGENTA  "#ec93d3"
#define  COL_BRIGHT_ZB_CYAN     "#93e0e3"
#define  COL_BRIGHT_ZB_WHITE    "#ffffff"

static const char fg_norm [] = COL_NORMAL_ZB_GRAY;   //"#4C5153";
static const char fg_sel  [] = COL_NORMAL_ZB_YELLOW; //"#F7F6F5";
static const char bg_norm [] = COL_NORMAL_ZB_BLACK;  //"#F7F6F5";
static const char bg_sel  [] = COL_BRIGHT_ZB_BLACK;  //"#4C5153";
static const char br_norm [] = COL_NORMAL_ZB_BLACK;
static const char br_sel  [] = COL_NORMAL_ZB_YELLOW;

static const char *colors[][3] = {
	/*                 fg        bg        border  */
	[SchemeNorm]   = { fg_norm , bg_norm , br_norm },
	[SchemeSel]    = { fg_sel  , bg_sel  , br_sel  },
	/*[SchemeWarn]   = { COL_BRIGHT_ZB_YELLOW , COL_NORMAL_ZB_BLACK , COL_BRIGHT_ZB_WHITE },*/
	/*[SchemeUrgent] = { COL_BRIGHT_ZB_RED    , COL_NORMAL_ZB_BLACK , COL_BRIGHT_ZB_WHITE },*/
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/*  class          instance    title       tags mask     isfloating   monitor */
	  { "Gimp",        NULL,       NULL,       0,            1,           -1 },
	  { "screencast",  NULL,       NULL,       0,            1,           -1 },
	/*{ "qutebrowser", NULL,       NULL,       1 << 8,       0,           -1 },*/
	/*{ "Spotify",     NULL,       NULL,       1 << 8,       1,           -1 },*/

	/* Mathematica */
	{ "Preferences",   NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|||",      col },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
        { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
        { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
        { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
        { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] =
	{ "dmenu_run"
	, "-m"  , dmenumon
	, "-fn" , dmenufont
	, "-nb" , COL_NORMAL_ZB_BLACK
	, "-nf" , COL_NORMAL_ZB_GRAY
	, "-sb" , COL_NORMAL_ZB_BLACK
	, "-sf" , COL_BRIGHT_ZB_GREEN
	, NULL
	};
static const char *termcmd_st_tm[]   = { "st", "-e", "tmux", "-L", "dwm", NULL };
static const char *termcmd_st[]      = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd_st_tm } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = termcmd_st } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },

	/* Increaseing gap is easier to intuit as decreasing windows,
	 * and vis-a-vis, hence fliped +|- keys:
	 */
	{ MODKEY,                       XK_equal,  setgaps,        {.i = -10 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = +10 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },

	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd_st_tm } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
