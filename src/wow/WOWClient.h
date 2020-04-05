#ifndef __WOWCLIENT_H__
#define __WOWCLIENT_H__

#include "Offsets.h"
#include "../Utils.h"

#include "WOWCamera.h"
#include "WOWObjectManager.h"
#include "WOWFunctions.h"

struct WOWClient{
      
    Vector2f GetVideoResolution(){
        return Vector2f{
            1.0f * Read<uint32_t>(VIDEO_RESOLUTION_WIDTH_ADDR),
            1.0f * Read<uint32_t>(VIDEO_RESOLUTION_HEIGHT_ADDR)
        };
    }

    Vector2f GetWindowSize(){
        auto size = Vector2f{
                1.0f * Read<uint32_t>(WINDOW_WIDTH_OFFSET),
                1.0f * Read<uint32_t>(WINDOW_HEIGHT_OFFSET)
        };
        return size;
    }

    auto GetCurrentStatus(){
        if(IsInGame())  return std::string("in_game");
        return std::string( (char*)CLIENT_STATUS_TEXT_PTR);
    }

    bool IsInGame(){
        return WOWFunctions::GetLocalPlayerPtr() != 0;
    }

};
#endif