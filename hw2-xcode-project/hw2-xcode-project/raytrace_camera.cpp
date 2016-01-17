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

void raytrace_camera::RayTracingRenderObjects(glm::vec3 location, glm::vec3 direction, scene& scene, raycast_hit& hit) const
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

bool raytrace_camera::GetColorFromRaytracing(glm::ivec2 screenPos, scene &scene, vec3& color) const
{
    glm::vec3 dir = ScreenPosToDirection(screenPos);
    dir = glm::vec3(glm::inverse(GetViewMatrix()) * glm::vec4(dir, 0));
    raycast_hit hit;
    // check whether the ray hits any object
    RayTracingRenderObjects(mLocation, dir, scene, hit);
    
    if (hit.object == NULL)
    {
        return false;
    }
    else
    {
        color = ShadingRaycastHit(scene, hit, dir);
    }
    return true;
    
}

glm::vec3 raytrace_camera::ScreenPosToDirection(glm::ivec2& pos) const
{
    glm::vec2 floatPos = glm::vec2(pos.x + 0.5f, pos.y + 0.5f);
    // first calculate fovx
    float tan_half_fovy = tan(mFovy * 3.1415926f / 360);
    float tan_half_fovx = tan_half_fovy * mScreenSize.x / mScreenSize.y;
    float horizon = tan_half_fovx * (2 * floatPos.x - mScreenSize.x) / mScreenSize.x;
    float vertical = tan_half_fovy * (mScreenSize.y - 2 * floatPos.y) / mScreenSize.y;
    return glm::normalize(glm::vec3(horizon, vertical, -1));
}


glm::vec3 raytrace_camera::ShadingRaycastHit(scene &myscene, raycast_hit &hit, vec3 inDirection, int depth) const
{
    if (hit.object == NULL || depth == mMaxDepth)
        return glm::vec3(0);
    //return hit.normal;
    glm::vec3 vcolor = hit.object->ambient + hit.object->emission;
    if (myscene.GetLights().size() == 0)
    {
        vcolor += hit.object->diffuse;
    }
    else
    {
        for (vector<light*>::const_iterator iter = myscene.GetLights().begin();
             iter != myscene.GetLights().end();
             ++iter)
        {
            light* li = *iter;
            glm::vec3 lightDir = li->GetDirectionToLight(hit.location);
            // Get a little higher hit point
            glm::vec3 raystart = hit.location + lightDir * 0.0001f;
            // Start raytracing to check light visible
            raycast_hit light_rayhit;
            myscene.GetCamera().RayTracingRenderObjects(raystart, lightDir, myscene, light_rayhit);
            // not intersect with any object or closer to light than hit point
            if (light_rayhit.object == NULL || li->GetDistanceSqToLight(raystart) < light_rayhit.distance * light_rayhit.distance)
            {
                glm::vec3 lightcolor = li->GetLightColor(raystart);
                // Calculate diffuse
                vcolor += hit.object->diffuse * lightcolor * max(glm::dot(lightDir, hit.normal), .0f);
                
                // Calculate specular
                glm::vec3 half = glm::normalize(-inDirection + lightDir);
                
                vcolor += hit.object->specular * lightcolor * pow(max(glm::dot(hit.normal, half), .0f), hit.object->shininess);
            }
        }
    }
    
    if (hit.object->specular != vec3(0))
    {
        // Calculate reflection
        vec3 eyeDir = inDirection;
        vec3 reflect = 2 * glm::dot(hit.normal, -eyeDir) * hit.normal + eyeDir;
        // check whether the ray hits any object
        raycast_hit newhit;
        RayTracingRenderObjects(hit.location + reflect * 0.0001f, reflect, myscene, newhit);
        if (newhit.object != NULL)
        {
            vcolor += hit.object->specular * ShadingRaycastHit(myscene, newhit, reflect, depth+1);
        }
    }
    return vcolor;
}

