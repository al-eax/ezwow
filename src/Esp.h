

#ifndef __ESP_H__
#define __ESP_H__

#include "wow/WOWObjectManager.h"
#include "Utils.h"
#include "wow/WOWCamera.h"
#include "ImGuiOglFrame.h"
#include "wow/WOWClient.h"

class Esp
{
private:
    /* data */
    Vector2f WorldToScreen(Vector3f cam_position, Vector3f view_direction, Vector3f world_point, Vector2f screen_size, Vector2f near_far)
    {
    
        //create the other view vectors
        if (view_direction.len() == 0)
            return Vector2f{-1, -1};

        Vector3f foreward = view_direction.norm();

        if (foreward.len() == 0)
            return Vector2f{-1, -1};

        Vector3f left = foreward.cross({0, 0, 100}).norm(); //calc the left vector

        if (left.len() == 0)
            return Vector2f{-1, -1};

        Vector3f up = foreward.cross(left).norm(); //calc the up vector

        if (up.len() == 0)
            return Vector2f{-1, -1};

        printf("foreward  %s  left %s  up %s\n", foreward.str().c_str(), left.str().c_str(), up.str().c_str());

        auto rel_world_pos = world_point.sub(cam_position);

        // check if rel_world_pos is in ront of us:
        // we project the enemy on our view_direction vector. Dot gives us the factor f; so f * view_direction = projected point
        if (rel_world_pos.dot(foreward) < 0)
        {
            //return Vector2f{-1, -1};
            printf("- invisible\n");
        }
        else
        {
            printf("+ visible %f\n", rel_world_pos.dot(foreward));
        }

        Vector2f transforms_rel_world_pos{
            left.mult(rel_world_pos.dot(left)).y / (rel_world_pos.dot(foreward)  - near_far.x  ),
            -up.mult(rel_world_pos.dot(up)).z / (rel_world_pos.dot(foreward)     - near_far.x  )};

        // corecction
        if(transforms_rel_world_pos.x == 1 || transforms_rel_world_pos.y == 1)
            return {-1,-1};


 
        printf("before transforms_rel_world_pos   %s \n", transforms_rel_world_pos.str().c_str());


    //    if (transforms_rel_world_pos.x > 0)
    //        transforms_rel_world_pos.x = transforms_rel_world_pos.x / 0.67;
    //    else
    //        transforms_rel_world_pos.x = transforms_rel_world_pos.x / 0.29;

        printf("transforms_rel_world_pos normed   %s \n", transforms_rel_world_pos.str().c_str());


        transforms_rel_world_pos.x = screen_size.x * 0.5 + screen_size.x * 0.5 * transforms_rel_world_pos.x;
        transforms_rel_world_pos.y = screen_size.y * 0.5 + screen_size.y * 0.5 * transforms_rel_world_pos.y;

      

        float dist_x_mid = std::abs(transforms_rel_world_pos.x - screen_size.x * 0.5);
        //transforms_rel_world_pos.x += dist_x_mid * corecction.x;

        printf("after transforms_rel_world_pos   %s \n", transforms_rel_world_pos.str().c_str());
        return transforms_rel_world_pos;
    }
    Vector2f fov{1, 1};

void ObjectToWindowShort(float WindowOut[3])
{
    float mvx[16], px[16];
    int vp[4];

    glGetFloatv(GL_MODELVIEW_MATRIX, mvx);
    glGetFloatv(GL_PROJECTION_MATRIX, px);
    glGetIntegerv(GL_VIEWPORT, vp);

    float x2 = px[0]*mvx[12]+px[4]*mvx[13]+px[8]*mvx[14]+px[12];
    float y2 = px[1]*mvx[12]+px[5]*mvx[13]+px[9]*mvx[14]+px[13];
    float z2 = px[2]*mvx[12]+px[6]*mvx[13]+px[10]*mvx[14]+px[14];
    float w2 = px[3]*mvx[12]+px[7]*mvx[13]+px[11]*mvx[14]+px[15];

    WindowOut[0] = (float) vp[0] + vp[2] * 0.5f * (x2/w2 + 1.0f);
    WindowOut[1] = (float) vp[1] + vp[3] * 0.5f * (y2/w2 + 1.0f);
    WindowOut[2] = (float)                 0.5f * (z2/w2 + 1.0f);
    WindowOut[1] = (float) (vp[3] - WindowOut[1]);
}

public:
    void Draw()
    {
        Vector2f size = WOWClient().GetVideoResolution();

        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoSavedSettings;
        //window_flags |= ImGuiWindowFlags_NoMouseInputs;
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("ESPP", NULL, ImVec2(size.x, size.y), 0.0f, window_flags);

        auto cam_pos = WOWCamera().Position();
        auto view_direction = WOWCamera().ViewDirection();
        auto units = WOWObjectManager().GetUnitList();
        auto u = units[0];

        auto screen_pos = WorldToScreen(cam_pos, view_direction, u.GetLocation(), size,  WOWCamera().zPlane());
        float foo[3] = {u.GetLocation().x,u.GetLocation().y,u.GetLocation().z};

        ObjectToWindowShort(foo);
        if (screen_pos.x != -1 && screen_pos.y != -1)
        {
            ImDrawList *draw_list = ImGui::GetWindowDrawList();
            //printf("+ isible\n");
            draw_list->AddCircleFilled(ImVec2(screen_pos.x, screen_pos.y), 4, ImColor(200, 200, 150));
  
            draw_list->AddCircleFilled(ImVec2(foo[0],foo[1]), 4, ImColor(100, 200, 150));

        }
        else
        {
            //printf("- invisible\n");
        }

        ImGui::End();
        ImGui::Begin("ESP menu", NULL);
        ImGui::LabelText(u.GetName().c_str(), "");
        ImGui::SliderFloat("fovx", &fov.x, -500.0f, 500.0f);
        ImGui::SliderFloat("fovy", &fov.y, -500.0f, 500.0f);

        ImGui::End();
    }
};

#endif