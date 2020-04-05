

#ifndef __WOWFUNCTIONS_H__
#define __WOWFUNCTIONS_H__

#include <iostream>

#include "Offsets.h"
#include "../Utils.h"

class WOWFunctions
{

public:
    static void TargetUnit(uint64_t guid)
    {
        auto func = (decltype(TargetUnit) *)TARGETUNIT_FUNC_ADDR;
        func(guid);
    }

    static void CastSpell(uint64_t spell_id, uint64_t guid)
    {
        auto func = (void (*)(uint64_t, uint64_t))CASTSPELLBYID;
        func(spell_id, guid);
    }

    static void CastSpellByName(std::string spell)
    {
        DoLuaString("CastSpellByName(\"" + spell + "\")");
    }

    static void Login(std::string user, std::string pass)
    {
        ((void(__cdecl *)(const char *, const char *))0x0046E560)(user.c_str(), pass.c_str());
    }

    static void DoLuaString(std::string command)
    {
        asm(
            "push $0;" //0
            "push %0;" //combat.lua arg0 | push eax
            "push %1;" //text arg1 | push ebx
            "call %2;" //call arg2 | call ecx
            "add $0xC, %%esp;"
            :
            : "eax"(command.c_str()), "ebx"(command.c_str()), "ecx"(LUA_TO_STRING_FUNX)
            :);
    }

    static void SendChatMessage(std::string msg)
    {
        DoLuaString("DEFAULT_CHAT_FRAME:AddMessage(\"" + msg + "\");");
    }

    static void PrintLog(std::string text)
    {
        // DoLuaString("DEFAULT_CHAT_FRAME:AddMessage('" + text + "', 0.0, 1.0, 0.0);");
    }

    static void PrintCTMFlags()
    {
      
        auto actionType = *(uint32_t *)0xD689BC;
        auto timestamp = *(uint32_t *)0xD689B8;
        auto Precision = *(float *)0xD689B4;          //float, mapped from a named signature, but i cant tell if it actually does anything.
        auto pX = *(float *)0xD68A18;                 //float
        auto pY = *(float *)0xD68A1C;                 //float
        auto pZ = *(float *)0xD68A20;                 //float
        auto cX = *(float *)0xD68A18 + 4;             //float
        auto cY = *(float *)0xD68A1C + 4;             //float
        auto cZ = *(float *)0xD68A20 + 4;             //float
        auto ctm_target_guid = *(uint64_t *)0xD689C0; //ulong, interact guid,

        auto unknown1 = *(uint32_t *)0x00d68a14; //always 40100000
        auto unknown2 = *(uint32_t *)0x00d689cc; // always 0
        auto unknown3 = *(uint32_t *)0x00d689d0; // always 0

        auto ptr = GetLocalPlayerPtr();
        //lbl1 = 3f000000
        // LBL2 = 0
        //
        /*

          auto ctm_current_location_y = Read<float>(0x00d68a0c);
        auto ctm_current_location_z = Read<float>(0x00d68a10);
        auto lbl_ctm_precision = Read<float>(0x00d68a10);
        auto ctm_facing = Read(0x00d689a4);             //set 415f66f3 while walking | set 40490fdb while standing
        auto ctm_flag_alway3f000000 = Read(0x00d689ac); //is now 0x16d30008
        auto ctm_flag_always0 = Read(0x00d68998);
        auto ctm_timestamp = Read(0x00d689b8);
        printf("3f* %x   zero  %x   face %x   clocz %f   clocy %f  stmp %i  type %x  prcn %f  xyz (%f|%f|%f)   cxyz (%f|%f|%f)  un1 %i  un2 %x  un3 %x\n", ctm_flag_alway3f000000, ctm_flag_always0,
               ctm_facing, ctm_current_location_z, ctm_current_location_y, ctm_timestamp, actionType, Precision,
               pX, pY, pZ, cX, cY, cZ,
               unknown1, unknown2, unknown3);
        */
    }


    static void ClickToMove(ClickToMoveType type, uint64_t guid, Vector3f pos, bool walk = true)
    {
        static bool _walk = true;
        if (walk)
        {
            Write(0x00d689a4, 0x415f66f3);
           // _walk = false;
        }
        else
        {
            //Write(0x00d689a4, 0x40490fdb);
        }
        
        auto actionType = (uint32_t *)0xD689BC;
        auto timestamp = (uint32_t *)0xD689B8;
        auto Precision = (float *)0xD689B4;          //float, mapped from a named signature, but i cant tell if it actually does anything.
        auto positionX = (float *)0xD68A18;          //float
        auto positionY = (float *)0xD68A1C;          //float
        auto positionZ = (float *)0xD68A20;          //float
        auto ClickX = (float *)0xD68A18 + 4;         //float
        auto ClickY = (float *)0xD68A1C + 4;         //float
        auto ClickZ = (float *)0xD68A20 + 4;         //float
        auto ctm_target_guid = (uint64_t *)0xD689C0; //ulong, interact guid,

        *positionX = pos.x;
        *positionY = pos.y;
        *positionZ = pos.z;
 
        //*timestamp++;
        *actionType = (uint32_t)type;
        *Precision = 0.0;
        if (guid != 0)
        {
            *ctm_target_guid = guid;
        }
    }

    static uint32_t GetLocalPlayerPtr()
    {
        return ((uint32_t __cdecl(*)())0x00402F40)();
    }
};

#endif