//
//  light.cpp
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/13/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#include <stdio.h>
#include "light.h"
#include "defines.h"
#include "myutil.h"

float light::atten_const = 1;
float light::atten_linear = 0;
float light::atten_quad = 0;

light::light(glm::vec3& color, glm::vec4& location)
: mColor(glm::vec4(color, 1)), mLocation(location)
{
    isPointLight = !IS_FLOAT_EQUAL(location.w, 0);
}

double light::GetDistanceSqToLight(glm::vec3 &location)
{
    if (isPointLight)
    {
        glm::vec3 v = glm::vec3(mLocation) - location;
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }
    return std::numeric_limits<double>::max();
}

glm::vec3 light::GetDirectionToLight(glm::vec3 &location)
{
    if (isPointLight)
    {
        return glm::normalize(glm::vec3(mLocation - glm::vec4(location, 1)));
    }
    return glm::normalize(glm::vec3(mLocation));
}

glm::vec3 light::GetLightColor(glm::vec3& location)
{
    if (isPointLight)
    {
        float atten = Clamp(GetAttenuation(location), 0, 1);
        return atten * glm::vec3(mColor);
    }
    return glm::vec3(mColor);
}

float light::GetAttenuation(glm::vec3 &location)
{
    float atten = 0;
    glm::vec3 direction = glm::vec3(mLocation) - location;
    if (atten_quad != 0)
    {
        atten += atten_quad * (direction.x * direction.x + direction.y + direction.y + direction.z * direction.z);
    }
    if (atten_linear != 0)
    {
        atten += atten_linear * glm::length(direction);
    }
    if (atten_const != 1)
    {
        atten += atten_const;
    }
    if (IS_FLOAT_EQUAL(atten, 0))
    {
        atten = 1;
    }
    else
    {
        atten = 1 / atten;
    }
    return atten;
}