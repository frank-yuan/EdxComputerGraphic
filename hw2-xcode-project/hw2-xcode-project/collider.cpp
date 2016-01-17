//
//  collider.cpp
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/16/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#include "collider.h"
#include "defines.h"
#include <math.h>

/*
 Fast Ray-AABB Intersection
 Implement according to
 http://www.twinklingstar.cn/2015/2479/programmers_computational_geometry-bounding_volumes/
 */
bool HitBoundingBox(
                    glm::vec3& boxMin,
                    glm::vec3& boxMax,
                    glm::vec3& start,
                    glm::vec3& dir,
                    glm::vec3& hit)
{
    float minDistance = -std::numeric_limits<float>::max();
    float maxDistance = std::numeric_limits<float>::max();
    for (int i = 0; i < 3; ++i)
    {
        // parallel in this axis
        if (IS_FLOAT_EQUAL(dir[i], 0))
        {
            // if the start point is out of the range between min and max value in this axis
            // the ray cannot intersect with box
            if (start[i] < boxMin[i] || start[i] > boxMax[i])
            {
                return false;
            }
        }
        else
        {
            float minD = (boxMin[i] - start[i]) / dir[i];
            float maxD = (boxMax[i] - start[i]) / dir[i];
            // swap
            if (minD > maxD)
            {
                float t = minD;
                minD = maxD;
                maxD = t;
            }
            minDistance = fmax(minDistance, minD);
            maxDistance = fmin(maxDistance, maxD);
            if (minDistance > maxDistance || maxDistance < 0)
            {
                return false;
            }
        }
    }
    
    float distance = minDistance >= 0 ? minDistance : maxDistance;
    hit = start + distance * dir;
    return true;
}


bool box_collider::IntersectWithRay(glm::vec3 location, glm::vec3 direction)
{
    glm::vec3 result;
    return HitBoundingBox(mMin, mMax, location, direction, result);
}
