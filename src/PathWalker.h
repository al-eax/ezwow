
#ifndef __PATHWALKER_H__
#define __PATHWALKER_H__

#include "Utils.h"

#include "wow/WOWObjectManager.h"
#include "wow/WOWFunctions.h"

#include <vector>

class PathWalker
{
private:
    std::vector<Vector3f> path;
    bool enabled = false;

public:
    void Start(std::vector<Vector3f> _path)
    {
        path = _path;
        enabled = true;
    }

    void Resume(){
        enabled = true;
    }

    void Schedule()
    {
        if (!enabled)
            return;

        auto current_pos = WOWObjectManager().GetLocalPlayer().GetLocation();

        if (path.size() == 0){
            enabled = false;
            WOWFunctions::ClickToMove(ClickToMoveType::Idle,0,current_pos,false);
            return;
        }
  
        auto path_point = path[0];

        if(current_pos.dist(path_point) > 2 ){
            WOWFunctions::ClickToMove(ClickToMoveType::Move,0,path_point,true);
        }else if( current_pos.dist(path_point) <= 2){
            path.erase(path.begin());
        }
    }

    void Stop()
    {
        enabled = false;
        auto current_pos = WOWObjectManager().GetLocalPlayer().GetLocation();
        WOWFunctions::ClickToMove(ClickToMoveType::Idle,0,current_pos,false);
    }

};

#endif