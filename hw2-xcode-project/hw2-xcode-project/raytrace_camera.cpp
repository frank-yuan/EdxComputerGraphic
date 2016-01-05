//
//  raytrace_camera.cpp
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/5/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#include <stdio.h>
#include "raytrace_camera.h"


void raytrace_camera::Init(glm::vec3 location, glm::vec3 lookAt, glm::vec3 up, glm::ivec2 screenSize, float fovy, int maxdepth)
{
    
}

raycast_hit& raytrace_camera::SingleRayTracing(glm::ivec2 screenPos, scene& scene) const
{
    raycast_hit result;
    return result;
}