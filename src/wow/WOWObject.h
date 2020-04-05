#ifndef WOWOBJECT_H
#define WOWOBJECT_H

#include <iostream>

#include "Offsets.h"
#include "../Utils.h"

struct WOWObject{
    uint32_t BaseAddress;

    auto GetNextObjectPtr() const{
        return Read<uint32_t>(BaseAddress + WOWOBJ_NEXT_OBJ_OFFSET);
    }

    auto GetGuid() const{
        return Read<uint64_t>(BaseAddress + WOWOBJ_GUID_OFFSET);
    }

    auto GetLocation() const{
        return Read<Vector3f>(BaseAddress + WOWOBJ_LOCATION_OFFSET);
    }

    auto GetType() const{
        return Read<WOWObjectType>(BaseAddress + WOWOBJ_TYPE_OFFSET);
    }

    auto GetDescriptor() {
        return Read<uint32_t >(BaseAddress + WOWOBJ_DESCRIPTOR_OFFSET);
    }

    void Print(){
        printf("<WOWObject: address=0x%x  type=%d  guid=%d>\n",BaseAddress,GetType(),GetGuid());
    }

    auto GerRotation() const{
        return Read<float>(BaseAddress + WOWOBJ_ROTATION);
    }
};


#endif