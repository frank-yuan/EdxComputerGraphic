//
//  object.cpp
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/7/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "defines.h"
#include "object.h"
#include "raytrace_camera.h"

void sphere_object::IntersectWithRay(glm::vec3 location, glm::vec3 direction, raycast_hit& rayhit)
{
    float a = glm::dot(direction, direction);
    vec3 v1 = location - transform.GetLocation();
    float b = 2 * glm::dot(direction, v1);
    float c = glm::dot(v1, v1) - radius * radius;
    
    float delta = b * b - 4 * a * c;
    
    if (IS_FLOAT_EQUALS(delta, 0))
    {
        float distance = -b / 2 * a;
        if (distance < rayhit.distance)
        {
            rayhit.distance = distance;
            rayhit.object = this;
            rayhit.location = location + direction * distance;
            rayhit.normal = glm::normalize(rayhit.location - transform.GetLocation());
            rayhit.color = ambient + diffuse;
        }
    }
    else if (delta > 0)
    {
        float root = sqrt(delta);
        float valueMax = 0.5 * (-b + root) / a;
        float valueMin = 0.5 * (-b - root) / a;
        float distance = -1;
        if (valueMin > 0 || valueMax > 0)
        {
            if (valueMin > 0 && valueMax > 0)
            {
                distance = valueMin < valueMax ? valueMin : valueMax;
            }
            else if (valueMin > 0)
            {
                distance = valueMin;
            }
            else
            {
                distance = valueMax;
            }
            
        }

        if (distance > 0)
        {
            rayhit.distance = distance;
            rayhit.object = this;
            rayhit.location = location + direction * distance;
            rayhit.normal = glm::normalize(rayhit.location - transform.GetLocation());
            rayhit.color = ambient + diffuse;
        }
    }
}