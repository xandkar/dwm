/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int gappx     = 15;       /* gap pixel between windows */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
/*static const char *fonts[]          = { "terminus:size=10:weight:bold" };*/
static const char *fonts[]          = { "terminus:size=10" };
static const char dmenufont[]       =   "terminus:size=10";

/* The 8 terminal colors, Zenburn */
static const char col_normal_zb_black[]   = "#2c2c2c";
static const char col_normal_zb_red[]     = "#705050";
static const char col_normal_zb_green[]   = "#60b48a";
static const char col_normal_zb_yellow[]  = "#dfaf8f";
static const char col_normal_zb_blue[]    = "#9ab8d7";
static const char col_normal_zb_magenta[] = "#dc8cc3";
static const char col_normal_zb_cyan[]    = "#8cd0d3";
static const char col_normal_zb_gray[]    = "#dcdccc";
/* The 8 bright terminal colors, Zenburn */
static const char col_bright_zb_black[]   = "#3f3f3f";
static const char col_bright_zb_red[]     = "#dca3a3";
static const char col_bright_zb_green[]   = "#72d5a3";
static const char col_bright_zb_yellow[]  = "#f0dfaf";
static const char col_bright_zb_blue[]    = "#94bff3";
static const char col_bright_zb_magenta[] = "#ec93d3";
static const char col_bright_zb_cyan[]    = "#93e0e3";
static const char col_bright_zb_white[]   = "#ffffff";

static const char *colors[][3] = {
	/*                 fg                     bg                    border   */
	[SchemeNorm]   = { col_normal_zb_gray   , col_normal_zb_black , col_normal_zb_black },
	[SchemeSel]    = { col_normal_zb_yellow , col_bright_zb_black , col_normal_zb_gray  },
	[SchemeWarn]   = { col_bright_zb_yellow , col_normal_zb_black , col_bright_zb_white },
	[SchemeUrgent] = { col_bright_zb_red    , col_normal_zb_black , col_bright_zb_white },
};
static const unsigned int alphas[][3] = {
	/*                 fg       bg    border   */
	[SchemeNorm]   = { OPAQUE , 100 , OPAQUE },
	[SchemeSel]    = { OPAQUE , 100 , OPAQUE },
	[SchemeWarn]   = { OPAQUE , 100 , OPAQUE },
	[SchemeUrgent] = { OPAQUE , 100 , OPAQUE },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
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
    , "-nb" , col_normal_zb_black
    , "-nf" , col_normal_zb_gray
    , "-sb" , col_normal_zb_black
    , "-sf" , col_bright_zb_green
    , NULL
    };
static const char *termcmd_st_tm[]   = { "st", "-e", "tmux", NULL };
static const char *termcmd_st[]      = { "st", NULL };
static const char *termcmd_konsole[] = { "konsole", NULL };
static const char *termcmd_mlterm[]  = { "mlterm", NULL };
static const char *file_browser[]    = { "thunar", NULL };
static const char *web_browser[]     = { "qutebrowser", NULL };
static const char *editor[]          = { "gvim", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd_st_tm } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = termcmd_st } },
	{ MODKEY|ShiftMask,             XK_k,      spawn,          {.v = termcmd_konsole } },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          {.v = termcmd_mlterm } },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          {.v = file_browser } },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          {.v = web_browser } },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          {.v = editor } },
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

