//
//  light.h
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/4/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#ifndef light_h
#define light_h

struct light
{
    glm::vec4 color;
};

struct point_light : public light
{
    glm::vec3 location;
};

struct direction_light : public light
{
    glm::vec3 direction;
};

#endif /* light_h */
