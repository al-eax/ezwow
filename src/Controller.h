//
// Created by alex on 27.06.19.
//

#ifndef WOWCPP_CONTROLLER_H
#define WOWCPP_CONTROLLER_H

#include <pthread.h>

#include "MainMenu.h"
#include "Hooks.h"
#include "Radar.h"
#include "wow/WOWClient.h"
#include "wow/WOWFunctions.h"
#include "wow/WOWCamera.h"
#include "wow/Offsets.h"
#include "PathRecorder.h"
#include "PathWalker.h"
#include "DebugConsole.h"
#include "Esp.h"

class Controller
{
    static inline MainMenu menu;
    inline static pthread_t thread;
    inline static bool hooks_installed = false;
    inline static bool is_ingame = false;
    static inline Esp esp;

public:
    /**
     * This function gets NO CALLS from the game MAIN THREAD!
     * This function gets called once per frame, when wines directx server calls glXSwapBuffers.
     * Its perfect for ui rendering and rinning other visual systems.
     */
    static void GlxSwapBufferCallback()
    {
        try
        {
            menu.Render();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    /**
     * This function gets called in the function at GET_CAM_OFFSET_FUNC.
     * This function gets multiple calls per frame.
     */
    static void InGameLoopCallback()
    {
        is_ingame = WOWClient().IsInGame();
       // menu.in_game = is_ingame;
        static bool walk = false;

        if (menu.btn_login)
        {
            menu.btn_login = false;
            WOWFunctions::Login("foo", "bar");
            Logs.push_back("logged in");
        }

        if(menu.btn_dbg1){
            menu.btn_dbg1 = false;
            menu.in_game = is_ingame;
        }
    }

    /**
     * This function gets called in a created thread in Controller::Run method.
     */
    static void ThreadLoopCallback()
    {
        if (!hooks_installed)
        {
            InitHook(InGameLoopCallback, GlxSwapBufferCallback, []() {   });
            hooks_installed = true;
        }
    }

    static void Run()
    {
        pthread_create(&thread, NULL, [](void *) -> void * { while(1) Controller::ThreadLoopCallback(); }, NULL);
    }
};

#endif //WOWCPP_CONTROLLER_H
