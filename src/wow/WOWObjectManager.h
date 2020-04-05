//
// Created by alex on 14.06.19.
//

#ifndef WOWCPP_OBJECTMANAGER_H
#define WOWCPP_OBJECTMANAGER_H

#include <memory.h>
#include <stdio.h>

#include "Offsets.h"
#include "WOWObject.h"
#include "WOWPlayer.h"
#include "WOWUnit.h"
#include "../Utils.h"

struct WOWObjectManager
{

    uint32_t BaseAddress;

    WOWObjectManager()
    {
        BaseAddress = Read<uint32_t>((Read<uint32_t>(OBJ_TABLE) + OBJ_TABLE_OBJMGR_OFFSET));
    }

    auto GetLocalPlayerGUID() const
    {
        return Read<ulong>(BaseAddress + OBJMGR_LP_GUID_OFFSET);
    }

    auto GetFirstObject() const
    {
        return WOWObject{Read<uint32_t>(BaseAddress + OBJMGR_FIRST_OBJ)};
    }

    auto GetObjectList() const
    {
        std::vector<WOWObject> list;
        WOWObject currentObj = GetFirstObject();

        while (currentObj.BaseAddress != 0 && (currentObj.BaseAddress & 1) == 0)
        {
            list.push_back(currentObj);
            currentObj = WOWObject{currentObj.GetNextObjectPtr()};

        }
        return list;
    }

    auto GetObjectList(WOWObjectType type) const
    {
        return filter(GetObjectList(), [type](WOWObject a) { return a.GetType() == type; });
    }

    auto GetUnitList() const
    {
        std::vector<WOWUnit> units;
        for (auto &unit : GetObjectList(WOWObjectType::Unit))
            units.push_back((WOWUnit &)(unit));
        return units;
    }

    auto GetPlayerList() const
    {
        std::vector<WOWPlayer> units;
        for (auto &unit : GetObjectList(WOWObjectType::Player))
            units.push_back((WOWPlayer &)(unit));
        return units;
    }

    auto GetLocalPlayer() const
    {
        auto local_guid = GetLocalPlayerGUID();
        printf("local_guid %x\n",local_guid);
        auto items = filter(GetObjectList(), [local_guid](WOWObject a) { return a.GetGuid() == local_guid; });

        return (WOWPlayer&)items[0];
    }

    auto GetDynamicObjectList() const
    {
        std::vector<WOWObject> objs;
        for (auto &obj : GetObjectList(WOWObjectType::DynamicObject))
            objs.push_back((WOWObject &)(obj));
        return objs;
    }

    auto GetGameObjectList() const
    {
        std::vector<WOWObject> objs;
        for (auto &obj : GetObjectList(WOWObjectType::GameObject))
            objs.push_back((WOWObject &)(obj));
        return objs;
    }

    bool GetObjectByGuid(uint64_t guid, WOWObject& obj){
        auto objs = filter(GetObjectList(), [guid](auto obj) -> bool{return obj.GetGuid() == guid;});
        if(objs.size() > 0){
            obj = objs[0];
            return true;
        }
        return false;
    }
};

#endif //WOWCPP_OBJECTMANAGER_H
