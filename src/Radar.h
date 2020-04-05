//
// Created by alex on 28.06.19.
//

#ifndef WOWCPP_RADAR_H
#define WOWCPP_RADAR_H

#include <string>

#include "ImGuiOglFrame.h"
#include "wow/WOWObjectManager.h"
#include "wow/WOWCamera.h"
#include "DebugConsole.h"

class Radar
{
private:

    Vector2f WorldToRadar(Vector3f world_pos, Vector2f camera_pos,Vector2f radar_pos , Vector2f radar_size, float camera_yaw){

    }

    void DrawUnits()
    {
        std::string text_filter(txtb_textfilter);
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        ImVec2 winpos = ImGui::GetWindowPos();
        ImVec2 winsize = ImGui::GetWindowSize();

        float scale_x = winsize.x / 200;
        float scale_y = winsize.y / 200;

        auto Mgr = WOWObjectManager();
        if(Mgr.GetLocalPlayer().BaseAddress == 0) return;
        auto my_loc = Mgr.GetLocalPlayer().GetLocation();
        int arrow_size = 10;
        float arrow_strech = 0.5;
        
        draw_list->AddTriangleFilled(ImVec2(winpos.x + winsize.x / 2 - arrow_size * arrow_strech, winpos.y + winsize.y / 2 ),
            ImVec2(winpos.x + winsize.x / 2 + arrow_size * arrow_strech, winpos.y + winsize.y / 2 ),
            ImVec2(winpos.x + winsize.x / 2, winpos.y + winsize.y / 2 - arrow_size),
             ImColor(255, 0, 0, 255)
        );
       
        for (auto &unit : Mgr.GetUnitList())
        {
            if (unit.GetHealth() == 0)
                continue;

            if (text_filter != "" && str_toupper(unit.GetName()).find(text_filter) == std::string::npos)
                continue;
           
            Vector3f rel_pos = unit.GetLocation().sub(my_loc);
            rel_pos = rel_pos.mult(sldr_zoom);

            Vector2f rel2d{rel_pos.x, rel_pos.y};

            
            //printf("yaw %f\n",yaw);
            try
            {

                float yaw = WOWObjectManager().GetLocalPlayer().GerRotation();

                rel2d = rel2d.rot(-yaw + deg2rad(90));
                rel2d.y *= -1;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                printf("%s\n",e.what());
                continue;
            }
            
           

            Vector2f pos2d{
                (winpos.x + winsize.x / 2 + rel2d.x * scale_x),
                (winpos.y + winsize.y / 2 + rel2d.y * scale_y)};

            draw_list->AddCircleFilled(ImVec2(pos2d.x, pos2d.y), 2, ImColor(255, 100, 255, 255));
            if(chxb_draw_names)
                draw_list->AddText(ImVec2(pos2d.x, pos2d.y), ImColor(255, 100, 255, 255), unit.GetName().c_str());
        }
    }

public:
    float sldr_zoom = 0.5;
    char txtb_textfilter[200];
    bool chxb_draw_names;

    void Draw()
    {
        ImGui::Begin("Radar", NULL, ImVec2(200, 200), 0.2f);
        ImGui::InputText("Filter", txtb_textfilter, 200);
        ImGui::SameLine();
        if(ImGui::Button("clear")) txtb_textfilter[0] = '\0';
        ImGui::SliderFloat("scale", &sldr_zoom, 0.01f, 2.0f);
        ImGui::SameLine();
        ImGui::Checkbox("draw names",&chxb_draw_names);

        DrawUnits();

        ImGui::End();
    }
};

#endif //WOWCPP_RADAR_H
