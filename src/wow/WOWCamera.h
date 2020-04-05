#ifndef __WOW_CAMERA_H__
#define __WOW_CAMERA_H__

#include "../Utils.h"
#include <memory.h>

struct WOWCamera{
    uint32_t BaseAddress;

    WOWCamera(){
        BaseAddress = Read<uint32_t>(Read<uint>(CAM_ADDRESS) + CAMSTRUCT_OFFSET);
    }

    Vector3f Position(){
        Vector3f pos;
        pos = Read<Vector3f>(BaseAddress + 0x8);
        return pos;
    }

    Vector3f ViewDirection(){
        Vector3f pos;
        pos = Read<Vector3f>(BaseAddress + 0x8 + 3 * sizeof(float));
        return pos;
    }

    void ViewMatrix(float *mat3x3){
        memcpy((void*)mat3x3,(void*)(BaseAddress + 0x14), 9 * sizeof(float));
    }



    float zNear(){
        return Read<float>(BaseAddress + 0x40 -8);
    }

    float zFar(){
        return Read<float>(BaseAddress + 0x40 - 4);
    }

    Vector2f zPlane()
    {
        return Vector2f{ zNear(), zFar() };
    }


    float Fov(){
        return Read<float>(BaseAddress + 0x40);
    }

    float Ratio(){
        return Read<float>(BaseAddress + 0x44);
    }

    Vector2f FovRatio(){
        return Vector2f{ Fov(), Ratio() };
    }
    
    void Print(){
        auto pos = Position();
        auto angle = ViewDirection();
        float M[9];
        ViewMatrix(M);
        printf("<Camera addr=%0xd  position=(%f|%f|%f)  direction=(%f|%f|%f)\nview mat:\n%f\t%f\t%f\n%f\t%f\t%f\n%f\t%f\t%f\n>\n", 
                BaseAddress, pos.x, pos.y, pos.z, angle.x, angle.y, angle.z,
               M[0],M[1],M[2],M[3],M[4],M[5],M[6],M[7],M[8] );
    }
};

#endif