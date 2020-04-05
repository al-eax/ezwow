
#ifndef __DBGCONSOLE_H__
#define __DBGCONSOLE_H__

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>

#include "ImGuiOglFrame.h"
#include "imgui_impl_opengl2.h"

static std::vector<std::string> Logs;
static std::map<std::string, void(*)()> Commands;

class DebugConsole
{

private:

    void ExecuteCmd(std::string cmd)
    {
        Logs.push_back(">" + cmd);
        if(Commands.count(cmd) == 1)
            Commands[cmd]();
    }

public:

    void Draw()
    {
        ImGui::Begin("Debugconsole", NULL, ImVec2(350, 200), 0.6f, ImGuiWindowFlags_NoSavedSettings);
        
        static char txtb_command[200];

        ImGui::InputText("", txtb_command, 200);
        ImGui::SameLine();
        if (ImGui::Button("execute"))
        {
            ExecuteCmd(std::string(txtb_command));
            txtb_command[0] = '\0';
        }

        ImGui::BeginChild("Scrolling");
        for (int i = Logs.size() - 1; i >= 0; i--)
            ImGui::Text(Logs[i].c_str());

        ImGui::EndChild();

        ImGui::End();

        if(Logs.size() > 200)
            Logs.erase(Logs.begin(), Logs.begin() + 100);
    }
};

#endif

