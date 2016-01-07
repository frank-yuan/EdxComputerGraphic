//
//  raytrace_camera.h
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/5/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#ifndef raytrace_camera_h
#define raytrace_camera_h

#include <glm/glm.hpp>
#include "object.h"
class scene;

struct raycast_hit
{
    scene_object* object;
    glm::vec3 location;
    glm::vec3 normal;
    glm::vec3 color;
    float distance;
    raycast_hit(): distance(100000), object(NULL)
    {
    }
};

class raytrace_camera
{
public:
    
    void Init(glm::vec3 location, glm::vec3 lookAt, glm::vec3 up, glm::ivec2 screenSize, float fovy,  int maxdepth);
    glm::ivec2 GetScreenSize() const{return mScreenSize;}
    raycast_hit SingleRayTracing(glm::ivec2 screenPos, scene& scene) const;
    inline glm::mat4 GetViewMatrix() const {return mCachedViewMatrix;}
    
private:
    glm::vec3 ScreenPosToDirection(glm::ivec2& pos) const;
private:
    glm::vec3 mLocation;
    glm::vec3 mLookAt;
    glm::vec3 mUp;
    glm::ivec2 mScreenSize;
    glm::mat4 mCachedViewMatrix;
    float mFovy;
    int mMaxDepth;
};

#endif /* raytrace_camera_h */
