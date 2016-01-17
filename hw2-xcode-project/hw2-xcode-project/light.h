//
//  light.h
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/4/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#ifndef light_h
#define light_h

#include <glm/glm.hpp>

class light
{
public:
    static float atten_const;
    static float atten_linear;
    static float atten_quad;
    light(glm::vec3& color, glm::vec4& location);
    glm::vec3 GetDirectionToLight(glm::vec3& location);
    double GetDistanceSqToLight(glm::vec3& location);
    glm::vec3 GetLightColor(glm::vec3& location);
private:
    float GetAttenuation(glm::vec3& location);
public:
    
private:
    glm::vec4 mColor;
    glm::vec4 mLocation;
    bool isPointLight = false;

};

#endif /* light_h */
