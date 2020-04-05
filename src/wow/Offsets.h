//
// Created by alex on 14.06.19.
//
/*
 * see https://github.com/ARNFRIED/TrampolineDetours
 *
 */

#ifndef WOWCPP_OFFSETS_H
#define WOWCPP_OFFSETS_H

#include <iostream>
#include <stdio.h>

// LocalPlayer PlayerBase
uint32_t PLAYER_BASE = 0x00E29D28;

//WOW Objectmanager offset list:

uint32_t OBJ_TABLE = 0x00D43318;
uint32_t OBJ_TABLE_CLIENTCONN_OFFSET = 0x0;
uint32_t OBJ_TABLE_OBJMGR_OFFSET = 0x2218;
uint32_t OBJMGR_LP_GUID_OFFSET = 0xC0;
uint32_t OBJMGR_FIRST_OBJ = 0xAC;

//WOWObject offset list:

uint32_t WOWOBJ_NEXT_OBJ_OFFSET = 0x3C;
uint32_t WOWOBJ_GUID_OFFSET = 0x30;
uint32_t WOWOBJ_LOCATION_OFFSET = 0xBF0;
uint32_t WOWOBJ_TYPE_OFFSET = 0x14;
uint32_t WOWOBJ_DESCRIPTOR_OFFSET = 0x8;
uint32_t WOWOBJ_ROTATION = 0xBFC;

enum WOWObjectType : uint32_t
{
    Object = 0,
    Item = 1,
    Container = 2,
    Unit = 3,
    Player = 4,
    GameObject = 5,
    DynamicObject = 6,
    Corpse = 7,
    AiGroup = 8,
    AreaTrigger = 9,
    ListEnd = 205,
};

enum ClickToMoveType
{
    FaceTarget = 0x1,
    Face = 0x2,
    Stop_ThrowsException = 0x3,
    Move = 0x4,
    NpcInteract = 0x5,
    Loot = 0x6,
    ObjInteract = 0x7,
    FaceOther = 0x8,
    Skin = 0x9,
    AttackPosition = 0xA,
    AttackGuid = 0xB,
    ConstantFace = 0xC,
    _None = 0xD,
    Attack = 0x10,
    Idle = 0x13,
};

//WOWUnit offset list:

uint32_t WOWUNIT_NAME_OFFSET_1 = 0xdb8;
uint32_t WOWUNIT_NAME_OFFSET_2 = 0x40;
uint32_t WOWUNIT_HP_OFFSET = 0x70;
uint32_t WOWUNIT_LEVEL_OFFSET = 0x88;

// Camera

uint32_t CAMSTRUCT_OFFSET = 0x732c;
uint32_t CAM_ADDRESS = 0x00c6eccc;
uint32_t CAM_SOME_GUID_OFFSET = 0x88; //size 8 bytes

// Other offsets

uint32_t VIDEO_RESOLUTION_WIDTH_ADDR = 0x00D6938C;
uint32_t VIDEO_RESOLUTION_HEIGHT_ADDR = 0x00D6938C + 0x4;

uint32_t WINDOW_WIDTH_OFFSET = 0x00E17FB8;
uint32_t WINDOW_HEIGHT_OFFSET = 0x00E17FB8 + 0x4;

uint32_t TARGET_GUID_FUNC = 0x004a46a0;
uint32_t LUA_TO_STRING_FUNX = 0x00706C80;

uint32_t CLIENT_STATUS_TEXT_PTR = 0x00C07CD0;
uint32_t LOGIN_FUNC_PTR = 0x0046E560; //__cdecl (char* user,char*pass)

uint32_t GET_CAM_OFFSET_FUNC = 0x004ab5b0;
uint32_t MAINLOOP_FUNC2 = 0x00428770; //0x000000000042b42b
uint32_t MAINLOOP_FUNC = 0x00428810;

// FUNCTIONs

uint32_t TARGETUNIT_FUNC_ADDR = 0x004a6690;
uint32_t CASTSPELLBYID = 0x006fc520;
uint32_t CASTSPELL_BY_NAME = 0x004c42e0;


uint32_t MOUSE_OVER_UNIT_GUID_ADDR = 0x00DDEC78;

uint32_t TARGET_OBJECT_FUNC = 0x00600960;
#endif //WOWCPP_OFFSETS_H
