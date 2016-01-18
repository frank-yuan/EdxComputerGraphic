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
    double a = 1.0;
    vec3 v1 = location;
    double b = 2 * glm::dot(direction, v1);
    double c = glm::dot(v1, v1) - radius * radius;
    
    double delta = b * b - 4 * a * c;
    
    if (IS_FLOAT_EQUAL(delta, 0))
    {
        float distance = -b / 2 * a;
        SetRayHit(location, direction, distance, rayhit);
    }
    else if (delta > 0)
    {
        double root = sqrt(delta);
        double valueMax = 0.5 * (-b + root) / a;
        double valueMin = 0.5 * (-b - root) / a;
        double distance = -1;
        if (valueMin > 0 || valueMax > 0)
        {
            if (valueMin > FLOAT_PRECISION && valueMax > FLOAT_PRECISION)
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
            SetRayHit(location, direction, distance, rayhit);
        }

       
    }
}

void sphere_object::SetRayHit(glm::vec3 localLocation, glm::vec3 localDirection, float distance, raycast_hit& rayhit)
{
    vec3 localIntersectPos = localLocation + distance * localDirection;
    vec4 intersectPos4 = transform.GetTransform() * vec4(localIntersectPos, 1);
    vec3 intersectPos = vec3(intersectPos4)/intersectPos4.w;
    vec3 rayStartPos = vec3(transform.GetTransform() * vec4(localLocation, 1));
    float distanceInWorldCoordinate = glm::length(intersectPos - rayStartPos);
    if (distanceInWorldCoordinate < rayhit.distance)
    {
        rayhit.distance = distanceInWorldCoordinate;
        rayhit.object = this;
        rayhit.location = intersectPos;
        mat4 imat = glm::inverse(transform.GetTransform());
        
        rayhit.normal = glm::normalize(vec3(glm::transpose(imat) * vec4(localIntersectPos , 0)));
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
typedef std::vector<ivec3>::const_iterator triangleConstIter;

void mesh_object::Initialize()
{
    // create AABB
    vec3 max = vec3(-std::numeric_limits<float>::max());
    vec3 min = vec3(std::numeric_limits<float>::max());
    for (triangleConstIter iter = mTriangles.begin();
         iter != mTriangles.end();
         ++iter)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                float val = mVertexData.vertices[(*iter)[j]][i];
                if (val > max[i])
                {
                    max[i] = val;
                }
                else if (val < min[i])
                {
                    min[i] = val;
                }
            }
        }
    }
    collider.SetSize(min, max);
}

void mesh_object::AddTriangle(ivec3 data)
{
    mTriangles.push_back(data);
}
int triangleTested = 0;
void mesh_object::IntersectWithRay(glm::vec3 _location, glm::vec3 _direction, raycast_hit& rayhit)
{

    mat4 invertMatrix = glm::inverse(transform.GetTransform());
    vec3 location = vec3(invertMatrix * vec4(_location, 1));
    vec3 direction = glm::normalize(vec3(invertMatrix * vec4(_direction, 0)));
    vec3 globalLocation;
    float globalDistance;
    
    if (collider.IntersectWithRay(location, direction))
    {
        // iterate all triangles
        raycast_hit triangle_hit;
        bool anyIntersect = false;
        for (triangleConstIter iter = mTriangles.begin(); iter != mTriangles.end(); ++iter)
        {
            ++triangleTested;
            ivec3 triangleIndex = *iter;
            if (LineTriangleIntersectTest(triangleIndex, location, direction, triangle_hit))
            {
                anyIntersect = true;
            }
        }
        if (anyIntersect)
        {
            vec4 hitLocation = transform.GetTransform() * vec4(triangle_hit.location, 1);
            globalLocation = vec3(hitLocation) / hitLocation.w;
            globalDistance = glm::length(globalLocation - _location);
            if (globalDistance < rayhit.distance)
            {
                rayhit.location = globalLocation;
                rayhit.distance = globalDistance;
                rayhit.object = this;
                
                mat4 imat = glm::inverse(transform.GetTransform());
                rayhit.normal = glm::normalize(vec3(glm::transpose(imat) * vec4(triangle_hit.normal , 0)));
            }
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
vec3 cacheLegalVector[3];
bool SolveTwoVarLinearEquations(vec3 parameterVectors[], int count, float result[])
{
    int legalcount = 0;
    for (int i = 0; i < count; ++i)
    {
        vec3 v = parameterVectors[i];
        float sum = ABS(v.x) + ABS(v.y) + ABS(v.z);
        if (!IS_FLOAT_EQUAL(sum, 0))
        {
            cacheLegalVector[legalcount++] = v;
        }
    }
    if (legalcount > 1)
    {
        vec3 v1 = cacheLegalVector[0];
        vec3 v2 = cacheLegalVector[1];
        float numerator = (v1.x * v2.y - v1.y * v2.x);
        if (IS_FLOAT_EQUAL(numerator, 0))
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
        vec4 location = vec4(*(mVertexData.vertices + triangleIndex[i]), 1);
        vertices[i] = vec3(location / location.w);
    }
    // Calculate normal
    vec3 normal = glm::normalize(glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
    
//    // backface culling
//    if (glm::dot(direction, normal) > 0)
//    {
//        return false;
//    }
    
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
    for (int i = 0; i < 3; ++i)
    {
        equationParams[i] = vec3(B_A[i], C_A[i], P_A[i]);
    }
    if (SolveTwoVarLinearEquations(equationParams, 3, result))
    {
        float x = result[0];
        float y = result[1];
        // Now we know the insect point is inside the triangle
        if (x <= 1 && x >= 0 && y <= 1 && y >= 0 && x + y <= 1)
        {
            rayhit.distance = distance;
            rayhit.location = intersectPoint;
            rayhit.normal = normal;
            rayhit.object = this;
            return true;
        }
    }
    return false;
}
