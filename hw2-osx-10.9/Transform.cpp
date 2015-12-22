// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"
#include <iostream>

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& v)
{
    vec3 axis = glm::normalize(v);
    float c = cosf(degrees * pi / 180);
    float s = sinf(degrees * pi / 180);
    float omc = 1 - c;
    mat3 result = mat3(
                       axis.x * axis.x * omc + c,
                       axis.x * axis.y * omc - axis.z * s,
                       axis.x * axis.z * omc + axis.y * s,
                       
                       axis.x * axis.y * omc + axis.z * s,
                       axis.y * axis.y * omc + c,
                       axis.y * axis.z * omc - axis.x * s,
                       
                       axis.x * axis.z * omc - axis.y * s,
                       axis.y * axis.z * omc + axis.x * s,
                       axis.z * axis.z * omc + c
                       
                       );
    return glm::transpose(result);

}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    vec3 lookAt = glm::normalize(eye);
    vec3 axis = glm::cross(lookAt, up);
    
    eye = rotate(-degrees, up) * eye;
    up = glm::normalize(glm::cross(axis, eye));

}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    vec3 lookAt = glm::normalize(eye);
    vec3 axis = glm::cross(lookAt, up);
    
    eye = rotate(-degrees, axis) * eye;
    up = glm::normalize(glm::cross(axis, eye));
}

mat4 Transform::lookAt(const vec3 &eyepos, const vec3 &center, const vec3 &up)
{
    vec3 eye = eyepos - center;
    vec3 n = glm::normalize(eye);
    vec3 u = glm::normalize(glm::cross(n, up));
    vec3 v = glm::cross(n, u);
    glm::mat4 m = glm::mat4(vec4(-u, -glm::dot(eye, u)),
                            vec4(-v, -glm::dot(eye, v)),
                            vec4(n, -glm::dot(eye, n)),
                            vec4(0, 0, 0, 1));
    // You will change this return call
    return glm::transpose(m);
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.  
  return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    return mat4(sx, 0, 0, 0,
              0, sy, 0, 0,
              0, 0, sz, 0,
              0, 0, 0, 1);
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
 
  return mat4(1, 0, 0, 0,
              0, 1, 0, 0,
              0, 0, 1, 0,
              tx, ty, tz, 1);
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}


void Transform::printMat4(const mat4 &mat)
{
    std::cout << "=========================" <<std::endl;
    for (int i = 0; i < mat.row_size(); ++i)
    {
        for (int j = 0; j < mat.col_size(); ++j)
        {
            std::cout << mat[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}
 
