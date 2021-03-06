//
//  transform.h
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/4/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#ifndef object_transform_h
#define object_transform_h

#include <glm/glm.hpp>

struct object_transform
{
    void SetTranslation(glm::vec3& location);
    inline void SetTransform(const glm::mat4& mat){trans = mat;}
    glm::vec3 GetLocation() const{return glm::vec3(trans[3]) / trans[3][3];}
    inline const glm::mat4& GetTransform() const {return trans;}
private:
    glm::mat4 trans;
};

#endif /* transform_h */
