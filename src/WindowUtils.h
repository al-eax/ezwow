//
// Created by alex on 24.06.19.
//
// ref https://gist.github.com/kui/2622504

#ifndef WOWCPP_WINDOWUTILS_H
#define WOWCPP_WINDOWUTILS_H

#include <X11/X.h>
#include <X11/Xmu/WinUtil.h>
#include <X11/extensions/XInput.h>

#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/XInput.h>
#include <iostream>
#include <vector>
#include <X11/Xutil.h>


Window GetFocusWindow(){
    static Display* d = XOpenDisplay(NULL);
    int revert_to;
    Window w;
    XGetInputFocus(d, &w, &revert_to);
    return w;
}

std::string GetWindowName(Window window){
    static Display* d = XOpenDisplay(NULL);
    static int screen = DefaultScreen(d);
    static Window root_win = RootWindow(d, screen);

    XTextProperty prop;
    if(root_win == window)
        return "";
    if(XGetWMName(d, window, &prop)) {
        int count = 0, result;
        char **list = NULL;
        result = XmbTextPropertyToTextList(d, &prop, &list, &count); // see man
        if(result == Success){
            return std::string(list[0]);
        }
    }
    return "";
}

void GetRelativeMousePosition(Window window, int& x, int& y){
    static Display* d = XOpenDisplay(NULL);
    static int screen = DefaultScreen(d);

    static Window root_win = RootWindow(d, screen);

    int root_x,root_y;
    unsigned int mask;
    if(window == root_win)
        return;
    Window w1, w2;
    XQueryPointer(d, window,&w1, &w2,  &root_x, &root_y,&x, &y, &mask);
}

void GetWindowSize(Window win,unsigned int& width, unsigned int& height){
    static Display* d = XOpenDisplay(NULL);
    static int screen = DefaultScreen(d);
    int loc_x, loc_y;
    //static Window root_win = RootWindow(d, screen);
    Window win_return;
    unsigned int border_width_return;
    unsigned int depth_return;
    XGetGeometry(d,win,&win_return,&loc_x,&loc_y, &width,&height, &border_width_return, &depth_return);
}

struct ButtonMask{
    bool MouseLeftDown;
    bool MouseRightDown;
    bool MouseRightLeftDOwn;
};


std::vector<Window> GetWindowList() {
    //https://stackoverflow.com/a/30233345/4520565
    static Display *disp = XOpenDisplay(NULL);
    std::vector<Window> windows;
    Atom prop = XInternAtom(disp, "_NET_CLIENT_LIST", False), type;
    int form;
    unsigned long remain;
    unsigned char *list;
    unsigned long len;
    if (XGetWindowProperty(disp, XDefaultRootWindow(disp), prop, 0, 1024, False, 33,
                           &type, &form, &len, &remain, &list) == Success) {  // XA_WINDOW
        for (int i = 0; i < len; i++)
            windows.push_back(reinterpret_cast<Window*>(list)[i]);
    }
    return windows;
}

Window GetWindowByName(const std::string& name){
    for(auto w : GetWindowList() )
        if(GetWindowName(w) ==  name)
            return w;
    return -1;
}

/**
 * @see https://stackoverflow.com/a/52801588/4520565
 * @param ks
 * @return
 */
bool GetKeyState(KeySym ks) {
    static Display *dpy = XOpenDisplay(NULL);
    char keys_return[32];
    XQueryKeymap(dpy, keys_return);
    KeyCode kc2 = XKeysymToKeycode(dpy, ks);
    bool isPressed = !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));

    return isPressed;
}



void GetMouseBtnStatus(bool& btn1, bool& btn2, bool& btn3){
    static Display *display = XOpenDisplay(NULL);
    static int screen = DefaultScreen(display);
    static Window root_win = RootWindow(display, screen);

    Window root_return, child_return;
    int root_x_return, root_y_return;
    int win_x_return, win_y_return;
    unsigned int mask_return;

    XQueryPointer(
            display,
            root_win,
            &root_return,
            &child_return,
            &root_x_return,
            &root_y_return,
            &win_x_return,
            &win_y_return,
            &mask_return
    );

    btn1 = (mask_return & Button1Mask);
    btn2 = (mask_return & Button2Mask);
    btn3 = (mask_return & Button3Mask);
}


#endif //WOWCPP_WINDOWUTILS_H
