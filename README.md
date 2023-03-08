# twm
Ridiculously minimal window manager written in 42 loc

This is a simple program written in C that uses the Xlib library
to interact with the X Window System, which is the underlying system
used to create and manage windows on Linux and Unix-like operating systems.

The code starts by including the Xlib library and defining a macro function
MAX(a, b) which returns the larger of the two values passed to it.

In the main() function, the program first opens a connection to the X server
using the XOpenDisplay() function, and exits if it fails. Then it gets the
root window of the display using DefaultRootWindow() and grabs the F1 key and
the left and right mouse buttons using XGrabKey() and XGrabButton() functions,
this means that the program will receive events for these inputs.

The program enters an infinite loop, inside the loop it waits for an XEvent
using XNextEvent(). It then checks the type of the event and performs actions
based on it:

If the event is a KeyPress and the subwindow is not None, the program raises
the window using XRaiseWindow().
If the event is a ButtonPress and the subwindow is not None, the program grabs
the pointer using XGrabPointer() and gets the window attributes
using XGetWindowAttributes().
If the event is a MotionNotify, the program moves and resizes the window using
XMoveResizeWindow() based on the button pressed, left button
