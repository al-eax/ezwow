
#ifndef __PATHRECORDER_H__
#define __PATHRECORDER_H__

#include <vector>
#include <pthread.h>

#include "Utils.h"
#include "wow/WOWObjectManager.h"

class PathRecorder
{
private:
    std::vector<Vector3f> path;
    bool is_recording = false;

public:
    int distance = 5;

    void Start()
    {
        path.clear();
        is_recording = true;
    }

    void Schedule()
    {
        if (!is_recording)
            return;
        auto curr_pos = WOWObjectManager().GetLocalPlayer().GetLocation();

        if (path.size() > 0)
        {
            auto last_pos = path[path.size() -1];
            if (last_pos.dist(curr_pos) > distance)
            {
                path.push_back(curr_pos);
                printf("added new waypoint (%f|%f|%f)\n", curr_pos.x, curr_pos.y, curr_pos.z);
            }
        }
        else
        {
            path.push_back(curr_pos);
            printf("added initial waypoint (%f|%f|%f)\n", curr_pos.x, curr_pos.y, curr_pos.z);
        }
    }

    auto Stop()
    {   
        is_recording = false;
        return path;
    }
};

#endif