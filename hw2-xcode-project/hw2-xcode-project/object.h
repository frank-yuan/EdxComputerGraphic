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
#include <vector>
#include "object_transform.h"


typedef glm::mat3 mat3 ;
typedef glm::mat4 mat4 ;
typedef glm::core::type::vec3 vec3 ;
typedef glm::core::type::vec4 vec4 ;
typedef glm::core::type::ivec3 ivec3;

struct raycast_hit;

enum shape
{
    cube,
    sphere,
    mesh
} ;

class scene_object {
public:
    shape type ;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
//    float emission[4] ;
    float shininess ;
    object_transform transform;
    
    virtual void IntersectWithRay(glm::vec3 location, glm::vec3 direction, raycast_hit& rayhit) = 0;
    
};

class sphere_object : public scene_object
{
public:
    float radius ;
    void IntersectWithRay(glm::vec3 location, glm::vec3 direction, raycast_hit& rayhit);
};

struct vertex_data
{
    int count;
    vec3* vertices;
    vertex_data():count(0), vertices(NULL){}
};

class vertex_cache
{
public:
    static vertex_cache* Instance();
    vertex_data AddVertices(int count, vec3* data);
private:
    vertex_cache(){};
    static vertex_cache* sInstance;
};

class mesh_object : public scene_object
{
public:
    mesh_object(vertex_data vertexData):mVertexData(vertexData){};
    void AddTriangle(ivec3 data);
    void IntersectWithRay(glm::vec3 location, glm::vec3 direction, raycast_hit& rayhit);
private:
    bool LineTriangleIntersectTest(ivec3 triangleIndex, glm::vec3 location, glm::vec3 direction, raycast_hit& rayhit);
private:
    vertex_data mVertexData;
    //int mTriangleCount;
    std::vector<ivec3> mTriangles;
};
#endif /* object_h */
