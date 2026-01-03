#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLEANMASK(mask) (mask & ~(numlockmask | LockMask) & (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | Mod5Mask))
#define LENGTH(X) (sizeof X / sizeof X[0])
#include "config.h"  // Must be last to use types/functions

Display *dpy;
Window root;
Atom wm_protocols, wm_delete;
unsigned int numlockmask = 0;
int running = 1;

// Function implementations (defined here, prototypes in config.h)
void spawn(XKeyEvent *ev, const Arg *arg) {
    (void)ev;
    if (fork() == 0) {
        setsid();
        execvp(((char **)arg->v)[0], (char **)arg->v);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
}

void raise_win(XKeyEvent *ev, const Arg *arg) {
    (void)arg;
    if (ev->subwindow != None) {
        XRaiseWindow(dpy, ev->subwindow);
    }
}

void killclient(XKeyEvent *ev, const Arg *arg) {
    (void)arg;
    if (ev->subwindow == None) {
        return;
    }
    // Politely request close (WM_DELETE_WINDOW)
    XEvent ke;
    ke.type = ClientMessage;
    ke.xclient.window = ev->subwindow;
    ke.xclient.message_type = wm_protocols;
    ke.xclient.format = 32;
    ke.xclient.data.l[0] = wm_delete;
    ke.xclient.data.l[1] = CurrentTime;
    XSendEvent(dpy, ev->subwindow, False, NoEventMask, &ke);
    // If app ignores, you can add XKillClient(dpy, ev->subwindow); here as fallback
}

void quit(XKeyEvent *ev, const Arg *arg) {
    (void)ev;
    (void)arg;
    running = 0;
}

// Initialize display, grabs, and masks
void init(void) {
    if (!(dpy = XOpenDisplay(NULL))) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
    root = DefaultRootWindow(dpy);
    wm_protocols = XInternAtom(dpy, "WM_PROTOCOLS", False);
    wm_delete = XInternAtom(dpy, "WM_DELETE_WINDOW", False);

    // Get numlock mask
    XModifierKeymap *modmap = XGetModifierMapping(dpy);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < modmap->max_keypermod; j++) {
            if (modmap->modifiermap[i * modmap->max_keypermod + j] == XKeysymToKeycode(dpy, XK_Num_Lock)) {
                numlockmask = (1 << i);
            }
        }
    }
    XFreeModifiermap(modmap);

    // Grab mouse buttons for move/resize (Super + L/R click)
    XGrabButton(dpy, 1, MODKEY, root, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, MODKEY, root, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);

    // Grab keys (with lock variants)
    unsigned int modifiers[] = {0, LockMask, numlockmask, numlockmask | LockMask};
    for (size_t i = 0; i < LENGTH(keys); i++) {
        for (int m = 0; m < 4; m++) {
            XGrabKey(dpy, XKeysymToKeycode(dpy, keys[i].keysym), keys[i].mod | modifiers[m], root, True, GrabModeAsync, GrabModeAsync);
        }
    }
}

// Handle events
void handle_events(void) {
    XEvent ev;
    XWindowAttributes attr;
    XButtonEvent start;

    while (running) {
        XNextEvent(dpy, &ev);
        switch (ev.type) {
            case KeyPress:
                for (size_t i = 0; i < LENGTH(keys); i++) {
                    if (ev.xkey.keycode == XKeysymToKeycode(dpy, keys[i].keysym) &&
                        CLEANMASK(ev.xkey.state) == CLEANMASK(keys[i].mod)) {
                        keys[i].func(&ev.xkey, &keys[i].arg);
                    }
                }
                break;
            case ButtonPress:
                if (ev.xbutton.subwindow != None) {
                    XGrabPointer(dpy, ev.xbutton.subwindow, True, PointerMotionMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
                    XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
                    start = ev.xbutton;
                }
                break;
            case MotionNotify:
                while (XCheckTypedEvent(dpy, MotionNotify, &ev));  // Drain for smoothness
                int xdiff = ev.xbutton.x_root - start.x_root;
                int ydiff = ev.xbutton.y_root - start.y_root;
                XMoveResizeWindow(dpy, ev.xmotion.window,
                                  attr.x + (start.button == 1 ? xdiff : 0),
                                  attr.y + (start.button == 1 ? ydiff : 0),
                                  MAX(1, attr.width + (start.button == 3 ? xdiff : 0)),
                                  MAX(1, attr.height + (start.button == 3 ? ydiff : 0)));
                break;
            case ButtonRelease:
                XUngrabPointer(dpy, CurrentTime);
                break;
        }
    }
}

int main(void) {
    init();
    handle_events();
    XCloseDisplay(dpy);
    return 0;
}
