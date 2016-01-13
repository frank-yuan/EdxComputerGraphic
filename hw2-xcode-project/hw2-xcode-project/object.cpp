//
//  object.cpp
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/7/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "defines.h"
#include "object.h"
#include "raytrace_camera.h"

///////////////////////////////////////////////////////////////////////////
//                          Sphere Object Implementation                 //
///////////////////////////////////////////////////////////////////////////

void sphere_object::IntersectWithRay(glm::vec3 _location, glm::vec3 _direction, raycast_hit& rayhit)
{
    mat4 invertMatrix = glm::inverse(transform.GetTransform());
    vec3 location = vec3(invertMatrix * vec4(_location, 1));
    vec3 direction = glm::normalize(vec3(invertMatrix * vec4(_direction, 0)));
    float a = glm::dot(direction, direction);
    vec3 v1 = location;
    float b = 2 * glm::dot(direction, v1);
    float c = glm::dot(v1, v1) - radius * radius;
    
    float delta = b * b - 4 * a * c;
    
    if (IS_FLOAT_EQUALS(delta, 0))
    {
        float distance = -b / 2 * a;
        if (distance < rayhit.distance)
        {
            rayhit.distance = distance;
            rayhit.object = this;
            rayhit.location = location + direction * distance;
            rayhit.normal = glm::normalize(rayhit.location - transform.GetLocation());
            rayhit.color = ambient + emission + diffuse;
        }
    }
    else if (delta > 0)
    {
        float root = sqrt(delta);
        float valueMax = 0.5 * (-b + root) / a;
        float valueMin = 0.5 * (-b - root) / a;
        float distance = -1;
        if (valueMin > 0 || valueMax > 0)
        {
            if (valueMin > 0 && valueMax > 0)
            {
                distance = valueMin < valueMax ? valueMin : valueMax;
            }
            else if (valueMin > 0)
            {
                distance = valueMin;
            }
            else
            {
                distance = valueMax;
            }
            
        }
        if (distance > 0)
        {
            vec4 intersectPos4 = transform.GetTransform() * vec4(location + distance * direction, 1);
            vec3 intersectPos = vec3(intersectPos4)/intersectPos4.w;
            float distanceInWorldCoordinate = glm::length(intersectPos - _location);
            if (distanceInWorldCoordinate < rayhit.distance)
            {
                rayhit.distance = distanceInWorldCoordinate;
                rayhit.object = this;
                rayhit.location = intersectPos;
                rayhit.normal = vec3(glm::transpose(glm::inverse(transform.GetTransform())) * vec4(_location, 1));//glm::normalize(rayhit.location - transform.GetLocation());
                rayhit.color = ambient + emission + diffuse;
            }
        }

       
    }
}

///////////////////////////////////////////////////////////////////////////
//                          Vertex Cache Implementation                  //
///////////////////////////////////////////////////////////////////////////

vertex_cache* vertex_cache::sInstance = NULL;
vertex_cache* vertex_cache::Instance()
{
    if (sInstance == NULL)
    {
        sInstance = new vertex_cache();
    }
    return sInstance;
}

vertex_data vertex_cache::AddVertices(int count, vec3* data)
{
    vertex_data result;
    result.count = count;
    result.vertices = data;
    return result;
}

///////////////////////////////////////////////////////////////////////////
//                          Mesh Object Implementation                   //
///////////////////////////////////////////////////////////////////////////
void mesh_object::AddTriangle(ivec3 data)
{
    mTriangles.push_back(data);
}

void mesh_object::IntersectWithRay(glm::vec3 location, glm::vec3 direction, raycast_hit& rayhit)
{
    // TODO: use AABB test to accelerate
    // iterate all triangles
    typedef std::vector<ivec3>::const_iterator triangleConstIter;
    for (triangleConstIter iter = mTriangles.begin(); iter != mTriangles.end(); ++iter)
    {
        ivec3 triangleIndex = *iter;
        if (LineTriangleIntersectTest(triangleIndex, location, direction, rayhit))
        {
            // need do anything?
        }
    }
}

/////////////////////////////////////////////////////////////////
// Solve linear equation with following form
// a1x + b1y = c1
// a2x + b2y = c2
// parameters should be vectors with [a1, b1, c1], [a2, b2, c2]
// result should be a float[2]
/////////////////////////////////////////////////////////////////

bool SolveTwoVarLinearEquations(std::vector<vec3>& parameterVectors, float result[])
{
    std::vector<vec3> legalVectors;
    for (int i = 0; i < parameterVectors.size(); ++i)
    {
        vec3 v = parameterVectors[i];
        float sum = ABS(v.x) + ABS(v.y) + ABS(v.z);
        if (!IS_FLOAT_EQUALS(sum, 0))
        {
            legalVectors.push_back(v);
        }
    }
    if (legalVectors.size() > 1)
    {
        vec3 v1 = legalVectors[0];
        vec3 v2 = legalVectors[1];
        float numerator = (v1.x * v2.y - v1.y * v2.x);
        if (IS_FLOAT_EQUALS(numerator, 0))
        {
            return false;
        }
        float x = (v1.z * v2.y - v1.y * v2.z) / numerator;
        float y = (v1.x * v2.z - v1.z * v2.x) / numerator;
        result[0] = x;
        result[1] = y;
        return true;
    }
    return false;
}



bool mesh_object::LineTriangleIntersectTest(ivec3 triangleIndex, glm::vec3 location, glm::vec3 direction, raycast_hit& rayhit)
{
    vec3 vertices[3];
    // TODO: cache
    for (int i = 0; i < 3; ++i)
    {
        vec4 location = transform.GetTransform() * vec4(*(mVertexData.vertices + triangleIndex[i]), 1);
        vertices[i] = vec3(location / location.w);
    }
    // Calculate normal
    vec3 normal = glm::normalize(glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
    
    // backface culling
    if (glm::dot(direction, normal) > 0)
    {
        return false;
    }
    
    // calculate intersect point on the plane
    float distance = (glm::dot(vertices[0], normal) - glm::dot(location, normal))/glm::dot(direction, normal);
    
    if (distance <= 0 || distance > rayhit.distance)
        return false;

    vec3 intersectPoint = location + direction * distance;
    
    // Test whether the intersect point is in the triangle
    vec3 P_A = intersectPoint - vertices[0];
    vec3 B_A = vertices[1] - vertices[0];
    vec3 C_A = vertices[2] - vertices[0];
    
    // (B-A)x + (C-A)y = P-A
    float result[2];
    std::vector<vec3> equationParameters;
    for (int i = 0; i < 3; ++i)
    {
        equationParameters.push_back(vec3(B_A[i], C_A[i], P_A[i]));
    }
    if (SolveTwoVarLinearEquations(equationParameters, result))
    {
        float x = result[0];
        float y = result[1];
        // Now we know the insect point is inside the triangle
        if (x <= 1 && x >= 0 && y <= 1 && y >= 0 && x + y <= 1)
        {
            rayhit.distance = distance;
            rayhit.location = intersectPoint;
            rayhit.color = emission + ambient + diffuse;
            rayhit.normal = normal;
            rayhit.object = this;
            return true;
        }
    }
    return false;
}
