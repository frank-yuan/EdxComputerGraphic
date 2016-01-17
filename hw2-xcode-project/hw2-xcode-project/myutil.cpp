//
//  myutil.cpp
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/13/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#include <stdio.h>
#include "myutil.h"

float Clamp(float value, float min, float max)
{
    return value < min ? min : (value > max ? max : value);
}

int GetColorInt(glm::vec3& color)
{
    int result = Clamp(color.b, 0, 1) * 255;
    result += (int)(Clamp(color.g, 0, 1) * 255) << 8;
    result += (int)(Clamp(color.r, 0, 1) * 255) << 16;
    return result;
}

