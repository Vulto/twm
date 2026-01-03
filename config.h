#include <X11/keysym.h>

#define MODKEY Mod4Mask  // Super key

static const char *termcmd[] = { "st", NULL };

typedef union {
    int i;
    unsigned int ui;
    float f;
    const void *v;
} Arg;

static void spawn(XKeyEvent *ev, const Arg *arg);
static void raise_win(XKeyEvent *ev, const Arg *arg);
static void killclient(XKeyEvent *ev, const Arg *arg);
static void quit(XKeyEvent *ev, const Arg *arg);

typedef struct {
    unsigned int mod;
    KeySym keysym;
    void (*func)(XKeyEvent *ev, const Arg *arg);
    const Arg arg;
} Key;

static Key keys[] = {
    /* modifier          key        function      argument */
    { MODKEY,            XK_F1,     raise_win,    {0} },
    { MODKEY,            XK_c,      killclient,   {0} },
    { MODKEY,            XK_Return, spawn,        {.v = termcmd } },
    { MODKEY,            XK_q,      quit,         {0} },
};

/*
*   TODO: 
*   Resize is laggy 
*   Handle Multiple monitors (it should duplicate screen on both monitors by default)
*   New windows should open full screen by default
*/
