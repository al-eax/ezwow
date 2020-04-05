//
// Created by alex on 25.06.19.
//

#ifndef IMGUIOGLFRAME_H
#define IMGUIOGLFRAME_H

#include <map>
#include <GL/gl.h>
#include <GL/glx.h>
#include <dlfcn.h> // dlopen/RTLD_LAZY
#include <math.h>  // M_PI

#include "wow/WOWObjectManager.h"
#include "wow/WOWCamera.h"

#include "WindowUtils.h"

#include "third_party/imgui/imgui.h"
#include "imgui_impl_opengl2.h"

class ImGuiOglFrame
{

private:
    static inline GLXContext my_context;
    static inline GLXContext game_context;
    static inline Display *display;
    static inline GLXDrawable current_drawable;

    static void CreateOwnContext()
    {
        int screen = -1;
        glXQueryContext(display, game_context, GLX_SCREEN, &screen);
        int attribs[] = {GLX_FBCONFIG_ID, -1, None};
        int dummy;
        glXQueryContext(display, game_context, GLX_FBCONFIG_ID, &attribs[1]);
        GLXFBConfig *fb = glXChooseFBConfig(display, screen, attribs, &dummy);
        XVisualInfo *vis = glXGetVisualFromFBConfig(display, *fb);
        my_context = glXCreateContext(display, vis, 0, True);
    }

    /**
     * @see https://www.ownedcore.com/forums/world-of-warcraft/world-of-warcraft-bots-programs/wow-memory-editing/302991-opengl-howto-about-drawing-3d-geometry-ig-perfectly-zbuffer-no-ui-overlay.html#post1933915
     */
    static bool SwitchOglContext()
    {
        auto window = GetWindowByName("World of Warcraft");
        if (window == -1)
            return false;

        if (display == nullptr)
            display = glXGetCurrentDisplay();
        game_context = glXGetCurrentContext();
        current_drawable = glXGetCurrentDrawable();
        if (my_context == 0)
            CreateOwnContext();

        glXMakeCurrent(display, current_drawable, my_context);
        glEnable(GL_DEPTH_TEST);

        uint32_t w, h;
        GetWindowSize(window, w, h);
        glXQueryDrawable(display, current_drawable, GLX_WIDTH, &w);
        glXQueryDrawable(display, current_drawable, GLX_HEIGHT, &h);
        glViewport(0, 0, w, h);

        return true;
    }

    static void UnSwitchGlContext()
    {
        glXMakeCurrent(display, current_drawable, game_context);
    }

    /**
     * @see https://www.gamedev.net/forums/topic/421529-manual-alternative-to-glulookat-/
     * @see https://www.gamedev.net/forums/topic/421529-manual-alternative-to-glulookat-/
     * @param pos
     * @param dir
     * @param up
     */
    /*
    static void LookAt(const CVector3& pos, const CVector3& dir, const CVector3& up)
    {
        CVector3 dirN;
        CVector3 upN;
        CVector3 rightN;

        dirN = dir;
        dirN.Normalize();

        upN = up;
        upN.Normalize();

        rightN = dirN.Cross(upN);
        rightN.Normalize();

        upN = rightN.Cross(dirN);
        upN.Normalize();

        float mat[16];
        mat[ 0] = rightN.x;
        mat[ 1] = upN.x;
        mat[ 2] = -dirN.x;
        mat[ 3] = 0.0;

        mat[ 4] = rightN.y;
        mat[ 5] = upN.y;
        mat[ 6] = -dirN.y;
        mat[ 7] = 0.0;

        mat[ 8] = rightN.z;
        mat[ 9] = upN.z;
        mat[10] = -dirN.z;
        mat[11] = 0.0;

        mat[12] = -(rightN.Dot(pos));
        mat[13] = -(upN.Dot(pos));
        mat[14] = (dirN.Dot(pos));
        mat[15] = 1.0;

        glMultMatrixf(&mat[0]);
    }
*/

    void SetupGl3D()
    {
        // dlopen("glut.so", RTLD_LAZY);
        float view_mat[9];
        WOWCamera cam;
        cam.ViewMatrix(view_mat);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        printf("fov %f , Ratio %f  near %f   far %f\n", cam.Fov(), cam.Ratio(), cam.zNear(), cam.zFar());
        auto _gluPerspective = (void (*)(GLdouble, GLdouble, GLdouble, GLdouble))glXGetProcAddressARB((GLubyte *)"gluPerspective");

        _gluPerspective(cam.Fov() * 180.0f / M_PI, cam.Ratio(), cam.zNear(), cam.zFar());

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glScalef(2.0f, 2.0f, 1.0f);
        auto f = cam.ViewDirection();
        float forward[3] = {
            f.x,
            f.y,
            f.z};
        float fPos[3] = {
            cam.Position().x,
            cam.Position().y,
            cam.Position().z};
        printf("POS %f  %f  %f\n", fPos[0], fPos[1], fPos[2]);

        auto _gluLookAt = (void (*)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble))glXGetProcAddressARB((GLubyte *)"gluLookAt");
        _gluLookAt(fPos[0], fPos[1], fPos[2],
                   fPos[0] + forward[0], fPos[1] + forward[1], fPos[2] + forward[2],
                   0, 0, 1);
    }

protected:
    /**
     * @brief Override this function to draw your imgui 2d stuff
     */
    virtual void Draw(){};

    /**
     * @brief Checks wether a key was pressed or not and sets this information to the imgui io object.
     * 
     * @param key the key to heck
     */
    static void CheckKeyInput(int key)
    {
        auto &io = ImGui::GetIO();
        static std::map<int, int> KeyWasPressed;

        io.KeysDown[io.KeyMap[ImGuiKey_Backspace]] = (key == XK_BackSpace || key == XK_KP_Delete);

        if (GetKeyState(key))
        {
            KeyWasPressed[key]++;
        }
        else if (KeyWasPressed[key] > 0)
        {
            KeyWasPressed[key] = 0;
            if (key >= XK_A && key <= XK_Z)
                io.AddInputCharacter((unsigned int)key);
            if (key == XK_space || key == XK_KP_Space)
            {
                io.AddInputCharacter(' ');
            }
        }
    }

    /**
     * @brief updates imguis io object for mouse position, clicks and pressed keys.
     * 
     */
    static void UpdateIO()
    {
        auto window = GetWindowByName("World of Warcraft");

        if (window == -1)
            return;

        auto &io = ImGui::GetIO();

        int x, y;
        unsigned int w, h;

        GetRelativeMousePosition(window, x, y);
        GetWindowSize(window, w, h);
        GetMouseBtnStatus(io.MouseDown[0], io.MouseDown[1], io.MouseDown[2]);

        io.MousePos = ImVec2(x, y);
        io.DisplaySize = ImVec2(w, h);

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos | ImGuiBackendFlags_HasSetMousePos;

        //check keys a..z
        for (int key = XK_A; key <= XK_Z; key++)
            CheckKeyInput(key);
        //check space key
        CheckKeyInput(XK_space);
    }

    void tetrahedron(float zbuffer /* 0.94f : 0.1f */, float x, float y, float z)
    {
        glDepthRange(0.0f, 1);

        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(3.0f, 3.0f, 3.0f);
        glBegin(GL_TRIANGLE_FAN);
        glColor3ub(255, 0, 255);
        glVertex3f(0.0f, 0.0f, 10.0f);

        glColor3ub(255, 0, 0);
        glVertex3f(10.0f, 0.0f, 0.0f);

        glColor3ub(0, 255, 0);
        glVertex3f(cos(2 * M_PI / 3), sin(2 * M_PI / 3), 0);

        glColor3ub(0, 0, 255);
        glVertex3f(cos(-2 * M_PI / 3), sin(-2 * M_PI / 3), 0);

        glColor3ub(255, 0, 0);
        glVertex3f(10.0f, 0.0f, 0.0f);
        glEnd();
        glPopMatrix();
    }

    void ObjectToWindowShort(float WindowOut[3])
    {
        float mvx[16], px[16];
        int vp[4];

        glGetFloatv(GL_MODELVIEW_MATRIX, mvx);
        glGetFloatv(GL_PROJECTION_MATRIX, px);
        glGetIntegerv(GL_VIEWPORT, vp);

        float x2 = px[0] * mvx[12] + px[4] * mvx[13] + px[8] * mvx[14] + px[12];
        float y2 = px[1] * mvx[12] + px[5] * mvx[13] + px[9] * mvx[14] + px[13];
        float z2 = px[2] * mvx[12] + px[6] * mvx[13] + px[10] * mvx[14] + px[14];
        float w2 = px[3] * mvx[12] + px[7] * mvx[13] + px[11] * mvx[14] + px[15];

        WindowOut[0] = (float)vp[0] + vp[2] * 0.5f * (x2 / w2 + 1.0f);
        WindowOut[1] = (float)vp[1] + vp[3] * 0.5f * (y2 / w2 + 1.0f);
        WindowOut[2] = (float)0.5f * (z2 / w2 + 1.0f);
        WindowOut[1] = (float)(vp[3] - WindowOut[1]);
    }

public:
    bool in_game = false;
    void Render()
    {
        if (!SwitchOglContext())
            return;

        static bool _init = true;

        if (_init)
        {
            _init = false;
            ImGui::CreateContext();
            ImGui_ImplOpenGL2_Init();
        }

        UpdateIO();
        ImGui_ImplOpenGL2_NewFrame();

        ImGui::NewFrame();

        Draw();

        ImGui::Render();
        ImGui::EndFrame();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        if (in_game)
        {
            printf("ingame\n");
            auto u = WOWObjectManager().GetUnitList()[0];
            float foo[3] = {u.GetLocation().x, u.GetLocation().y, u.GetLocation().z};
            auto pos = u.GetLocation();

            SetupGl3D();
            //   auto pos = WOWObjectManager().GetUnitList()[0].GetLocation();
            //   auto pos2 = WOWObjectManager().GetLocalPlayer().GetLocation();
            //   glPushMatrix();
            //   glTranslatef(0.1,.1,0.1);
            //   //glScalef(3.0f, 3.0f, 3.0f);
            //   glBegin(GL_LINES);
            //   glVertex3f(0.4, 0.4, 0.4);
            //   //glVertex3f(pos.x, pos.y, pos.z);
            //   glVertex3f(0.3, 0.3, 0.3);
            //   glEnd();
            //   glPopMatrix();
            //
            //for (float f = -10.00; f < 100; f += 0.01)
            tetrahedron(0, pos.x, pos.y, pos.z);
        }

        UnSwitchGlContext();
    }
};

#endif //IMGUIOGLFRAME_H
