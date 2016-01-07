//
//  raytrace_camera.cpp
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/5/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#include <stdio.h>
#include <cmath>
#include <vector>
#include "raytrace_camera.h"
#include "scene.h"

using namespace std;

void RayTracingRecursive(glm::vec3 location, glm::vec3 direction, scene& scene, raycast_hit& hit)
{
    const std::vector<scene_object*> objects = scene.GetRenderableObject();
    for (int i = 0; i < objects.size(); ++i)
    {
        objects[i]->IntersectWithRay(location, direction, hit);
    }
}

void raytrace_camera::Init(glm::vec3 location, glm::vec3 lookAt, glm::vec3 up, glm::ivec2 screenSize, float fovy, int maxdepth)
{
    mLocation = location;
    mLookAt = lookAt;
    mUp  = up;
    mScreenSize = screenSize;
    mFovy = fovy;
    mMaxDepth = maxdepth;
    
    vec3 eye = location - lookAt;
    vec3 n = glm::normalize(eye);
    vec3 u = glm::normalize(glm::cross(up, n));
    vec3 v = glm::cross(n,u);
    glm::mat4 m = glm::mat4(vec4(u, -glm::dot(eye, u)),
                            vec4(v, -glm::dot(eye, v)),
                            vec4(n, -glm::dot(eye, n)),
                            vec4(0, 0, 0, 1));
    // You will change this return call
    mCachedViewMatrix = glm::transpose(m);
}

raycast_hit raytrace_camera::SingleRayTracing(glm::ivec2 screenPos, scene& scene) const
{
    glm::vec3 dir = ScreenPosToDirection(screenPos);
    dir = glm::vec3(glm::inverse(GetViewMatrix()) * glm::vec4(dir, 0));
    raycast_hit hit;
    RayTracingRecursive(mLocation, dir, scene, hit);
    return hit;
}

glm::vec3 raytrace_camera::ScreenPosToDirection(glm::ivec2& pos) const
{
    // first calculate fovx
    float tan_half_fovy = tan(mFovy * 3.1415926f / 360);
    float tan_half_fovx = tan_half_fovy * mScreenSize.x / mScreenSize.y;
    float horizon = tan_half_fovx * (2 * pos.x - mScreenSize.x) / mScreenSize.x;
    float vertical = tan_half_fovy * (mScreenSize.y - 2 * pos.y) / mScreenSize.y;
    return glm::normalize(glm::vec3(horizon, vertical, -1));
}


