#ifndef __WOWUNIT_H__
#define __WOWUNIT_H__

#include "WOWObject.h"

struct WOWUnit : public  WOWObject{
    
    auto GetName(){
        auto name_ptr = Read<uint32_t >(BaseAddress + WOWUNIT_NAME_OFFSET_1);
        if(name_ptr == 0) return std::string("?");
        auto c_name = Read<char*>(name_ptr + WOWUNIT_NAME_OFFSET_2);
        return std::string(c_name);
    }

    auto GetNamePtr(){
        auto name_ptr = Read<uint32_t >(BaseAddress + WOWUNIT_NAME_OFFSET_1);
        if(name_ptr == 0) return "?";
        auto c_name = Read<const char*>(name_ptr + WOWUNIT_NAME_OFFSET_2);
        return c_name;
    }

    auto GetHealth(){
        return Read<uint32_t>(GetDescriptor() + WOWUNIT_HP_OFFSET);
    }

    auto GetLevel(){
        return Read<uint32_t >(GetDescriptor() + WOWUNIT_LEVEL_OFFSET);
    }

    void Print() {
        printf("<WOWUnit: address=0x%x  type=%d  guid=%d  name=%s>\n",BaseAddress,GetType(),GetGuid(), GetName().c_str());
    }
};



#endif