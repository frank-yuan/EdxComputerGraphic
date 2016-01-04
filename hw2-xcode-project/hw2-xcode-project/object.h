//
//  object.h
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/3/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#ifndef object_h
#define object_h

#include <glm/glm.hpp>
#include "object_transform.h"


typedef glm::mat3 mat3 ;
typedef glm::mat4 mat4 ;
typedef glm::core::type::vec3 vec3 ;
typedef glm::core::type::vec4 vec4 ;
typedef glm::core::type::ivec3 ivec3;

enum shape
{
    cube,
    sphere,
    mesh
} ;

class scene_object {
public:
    shape type ;
    GLfloat ambient[4] ;
    GLfloat diffuse[4] ;
    GLfloat specular[4] ;
    GLfloat emission[4] ;
    GLfloat shininess ;
    object_transform transform;
    
};

class sphere_object : public scene_object
{
public:
    GLfloat size ;
};

class mesh_object : public scene_object
{
public:
    void LoadVertices(int count, vec3 data[]);
    void LoadTriangles(int count, ivec3 data[]);
private:
    int vertex_count;
    vec3* vertices;
    int triangle_count;
    ivec3* triangles;
};
#endif /* object_h */
