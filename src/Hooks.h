//
// Created by alex on 14.06.19.
//

#ifndef WOWCPP_HOOKS_H
#define WOWCPP_HOOKS_H

#include <string.h>
#include <GL/glx.h>
//#include <GL/glut.h>
#include <dlfcn.h>
#include <GL/gl.h>
//#include <SDL/SDL.h>

#include "third_party/subhook/subhook.c"
#include "WindowUtils.h"
#include "wow/WOWFunctions.h"
#include "DebugConsole.h"

using CallbackFunction = void (*)(void);
template <typename T = CallbackFunction>
/**
 * @brief A simple wrapper for subhook c functions. 
 * Make sure you called Install() before accessing Trampolin or Original.
 */
class Hook
{
private:
    subhook_t hook;

public:
    CallbackFunction UserCallBack;
    T Trampolin = nullptr;
    T Original = nullptr;
    T Callback = nullptr;

    template <typename U, typename V>
    void Install(U func_too_hook, V callback)
    {
        Install((void *)func_too_hook, (void *)callback);
    }

    void Install(T func_too_hook, T callback)
    {
        Install((void *)func_too_hook, (void *)callback);
    }

    void Install(void *func_too_hook, void *callback)
    {
        hook = subhook_new((void *)func_too_hook, (void *)callback, (subhook_flags_t)0);
        subhook_install(hook);
        Callback = (T)callback;
        Trampolin = (T)subhook_get_trampoline(hook);
        Original = (T)hook->src;
    }

    void Disable()
    {
        subhook_remove(hook);
    }

    void Enable()
    {
        Install(hook->src, hook->dst);
    }
};

Hook<decltype(glXSwapBuffers) *> Hook_glXSwapBuffers;
Hook<> Hook_MainLoop;
Hook<void __fastcall (*)(int)> Hook_Interact;
Hook<void __thiscall (*)(int *, float *,float*)> Hook_W2S;
Hook<void (*)(float,float)> Hook_glDepthRange;

void MainLoopCallBack()
{
    Hook_MainLoop.UserCallBack();
    Hook_MainLoop.Trampolin();
}

void FrameCallBack(Display *di, GLXDrawable dr)
{
    Hook_glXSwapBuffers.Disable();
    Hook_glXSwapBuffers.UserCallBack();
    glXSwapBuffers(di, dr);
    Hook_glXSwapBuffers.Enable();
}

void __fastcall InteractCallback(int param_1)
{
    Logs.push_back("Interact called");
}

void __thiscall W2SCallback(int *thiz, float *param, float *param2)
{
    //printf("W2SCallback %x %f %f\n",thiz, param[0],param[1]);
    printf("W2SCallback %x %f %f | %f %f\n",thiz, param[0],param[1], param2[0],param2[1]);

    Hook_W2S.Trampolin(thiz,param,param2);

}

void glDepthRangeCallback(GLclampd near, GLclampd far){
    printf("glDepthRangeCallback\n");
    Hook_glDepthRange.Disable();
    if(far == 1){
        Hook_glDepthRange.UserCallBack();
    }
    glDepthRange(near,far);
    Hook_glDepthRange.Enable();
}

void InitHook(CallbackFunction _MainLoopCallBack, CallbackFunction _FrameCallBack, CallbackFunction _GLdepthRangeCallBack)
{

    while (dlsym(RTLD_NEXT, "glXGetProcAddressARB") == 0)
    {
    }
    Hook_glXSwapBuffers.UserCallBack = _FrameCallBack;
    Hook_glXSwapBuffers.Install(glXGetProcAddressARB((GLubyte *)"glXSwapBuffers"), FrameCallBack);
    
    while(glXGetProcAddressARB((GLubyte *)"glDepthRange") == 0)
    {
    }
    Hook_glDepthRange.UserCallBack = _GLdepthRangeCallBack;
    Hook_glDepthRange.Install(glXGetProcAddressARB((GLubyte *)"glDepthRange"), glDepthRangeCallback);

    Hook_MainLoop.UserCallBack = _MainLoopCallBack;
    Hook_MainLoop.Install(MAINLOOP_FUNC, MainLoopCallBack);

    //Hook_Interact.Install(0x00600960, InteractCallback);

    //Hook_W2S.Install(0x00534010, W2SCallback);


}

#endif //WOWCPP_HOOKS_H
