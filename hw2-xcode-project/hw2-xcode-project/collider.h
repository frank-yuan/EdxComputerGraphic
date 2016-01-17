//
//  collider.hpp
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/16/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#ifndef collider_hpp
#define collider_hpp

#include <stdio.h>
#include <glm/glm.hpp>

class ICollider
{
public:
    virtual ~ICollider() {}
    bool virtual IntersectWithRay(glm::vec3 location, glm::vec3 direction) = 0;
};

class box_collider : public ICollider
{
public:
    box_collider() : mMin(glm::vec3(0)), mMax(glm::vec3(0)){};
    box_collider(glm::vec3 min, glm::vec3 max) : mMin(min), mMax(max){};
    bool IntersectWithRay(glm::vec3 location, glm::vec3 direction);
    inline void SetSize(glm::vec3 min, glm::vec3 max){mMin = min; mMax = max;}
private:
    glm::vec3 mMin;
    glm::vec3 mMax;
};
#endif /* collider_hpp */
