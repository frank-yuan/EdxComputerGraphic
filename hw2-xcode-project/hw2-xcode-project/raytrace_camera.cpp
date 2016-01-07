//
//  raytrace_camera.cpp
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/5/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#include <stdio.h>
#include "raytrace_camera.h"

void RayTracingRecursive(glm::vec3 location, glm::vec3 direction, scene& scene, raycast_hit& hit)
{
    
}

void raytrace_camera::Init(glm::vec3 location, glm::vec3 lookAt, glm::vec3 up, glm::ivec2 screenSize, float fovy, int maxdepth)
{
    mScreenSize = screenSize;
    mFovy = fovy;
    mMaxDepth = maxdepth;
    
    vec3 eye = lookAt - location;
    vec3 n = glm::normalize(eye);
    vec3 u = glm::normalize(glm::cross(n, up));
    vec3 v = glm::cross(n, u);
    glm::mat4 m = glm::mat4(vec4(-u, -glm::dot(eye, u)),
                            vec4(-v, -glm::dot(eye, v)),
                            vec4(n, -glm::dot(eye, n)),
                            vec4(0, 0, 0, 1));
    // You will change this return call
    mCachedViewMatrix = glm::transpose(m);
}

raycast_hit raytrace_camera::SingleRayTracing(glm::ivec2 screenPos, scene& scene) const
{
    glm::vec3 dir = ScreenPosToDirection(screenPos);
    raycast_hit hit;
    RayTracingRecursive(mLocation, dir, scene, hit);
    return hit;
}

glm::vec3 raytrace_camera::ScreenPosToDirection(glm::ivec2& pos) const
{
    return glm::vec3(0);
}


