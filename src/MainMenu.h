//
// Created by alex on 28.06.19.
//

#ifndef WOWCPP_MAINMENU_H
#define WOWCPP_MAINMENU_H

#include "ImGuiOglFrame.h"
#include "Radar.h"
#include "wow/WOWClient.h"
#include "DebugConsole.h"
#include "Esp.h"

class MainMenu : public ImGuiOglFrame
{
public:
    // ui controls:
    bool chbx_draw_esp = false;
    bool chbx_draw_radar = false;
    bool btn_login = false;
    bool chbx_draw_console = false;
    bool chbx_esp = false;
    bool btn_dbg1, btn_dbg2, btn_dbg3,btn_dbg4;

    char txtbx_1[200];
    Radar radar;
    DebugConsole console;
    Esp esp;

protected:
    virtual void Draw() override
    {
        ImGui::Begin("EzWoW", NULL, ImVec2(200, 200), -1 ); //, ImGuiWindowFlags_NoSavedSettings);
        ImGui::Checkbox("Radar", &chbx_draw_radar);
        ImGui::Checkbox("Debugconsole", &chbx_draw_console);
        ImGui::Checkbox("Esp", &chbx_esp);

        btn_login |= ImGui::Button("login");

        ImGui::InputText("foooo", txtbx_1, 200);
        ImGui::LabelText("current Status", "%s", WOWClient().GetCurrentStatus().c_str());

        btn_dbg1 |= ImGui::Button("dbg1");
        btn_dbg2 |= ImGui::Button("dbg2");
        btn_dbg3 |= ImGui::Button("dbg3");
        btn_dbg4 |= ImGui::Button("dbg4");

        ImGui::End();

        if (chbx_draw_radar)
        {
            radar.Draw();
        }

        if(chbx_draw_console){
            console.Draw();
        }

        if(chbx_esp){
            esp.Draw();
        }
    }

    void Draw3d()
    {
    }
};

#endif //WOWCPP_MAINMENU_H
