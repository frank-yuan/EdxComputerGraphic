//
//  Transform.h
//  ray_tracer
//
//  Created by Xuan Yuan (Frank) on 1/2/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#ifndef Transform_h
#define Transform_h

#include "math3d.h"
struct Transform
{
    vec3 GetLocation() const
    {
        return location;
    }
    inline void SetLocation(vec3& l)
    {
        location = l;
    }
    vec3 GetRotation() const
    {
        return rotation;
    }
    inline void SetRotation(vec3& r)
    {
        rotation = r;
    }
    vec3 GetScale() const
    {
        return scale;
    }
    inline void SetScale(vec3& s)
    {
        scale = s;
    }
    Transform* GetParent() const
    {
        return parent;
    }
    void SetParent(Transform* trans);
private:
    vec3 location;
    vec3 rotation;
    vec3 scale;
    
    Transform* parent;
    mat4 worldMatrix;
};

#endif /* Transform_h */
