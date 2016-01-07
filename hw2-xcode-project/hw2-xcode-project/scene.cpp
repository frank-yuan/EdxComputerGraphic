//
//  scene.cpp
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/5/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <vector>
#include <stdio.h>
#include "scene.h"
#include "Transform.h"
using namespace std;


// Function to read the input data values
// Use is optional, but should be very helpful in parsing.
bool readvals(stringstream &s, const int numvals, float* values)
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}



void scene::LoadData(const char* filename)
{
    string str, cmd;
    ifstream in;
    in.open(filename);
    if (in.is_open()) {
        
        // I need to implement a matrix stack to store transforms.
        // This is done using standard STL Templates
        stack <mat4> transfstack;
        transfstack.push(mat4(1.0));  // identity
        
        float ambient[4];
        float diffuse[4];
        float specular[4];
        float shininess;
        glm::ivec2 screensize;
        int maxdepth;
        
        getline (in, str);
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines
                
                stringstream s(str);
                s >> cmd;
                int i;
                float values[10]; // Position and color for light, colors for others
                // Up to 10 params for cameras.
                bool validinput; // Validity of input
                
                // Process the light, add it to database.
                // Lighting Command
                if (cmd == "point") {
                    validinput = readvals(s, 6, values);
                    if (validinput)
                    {
                        point_light* light = new point_light();
                        light->location = glm::vec3(values[0], values[1], values[2]);
                        light->color = glm::vec4(values[4], values[5], values[6], 1);
                        mPointLights.push_back(light);
                    }
                }
                else if (cmd == "directional") {
                    validinput = readvals(s, 6, values);
                    if (validinput)
                    {
                        direction_light* light = new direction_light();
                        light->direction = glm::vec3(values[0], values[1], values[2]);
                        light->color = glm::vec4(values[4], values[5], values[6], 1);
                        mDirectionalLights.push_back(light);
                    }
                }
                // Material Commands
                // Ambient, diffuse, specular, shininess properties for each object.
                // Filling this in is pretty straightforward, so I've left it in
                // the skeleton, also as a hint of how to do the more complex ones.
                // Note that no transforms/stacks are applied to the colors.
                
                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            ambient[i] = values[i];
                        }
                    }
                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            diffuse[i] = values[i];
                        }
                    }
                } else if (cmd == "specular") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            specular[i] = values[i];
                        }
                    }
//                } else if (cmd == "emission") {
//                    validinput = readvals(s, 4, values);
//                    if (validinput) {
//                        for (i = 0; i < 4; i++) {
//                            emission[i] = values[i];
//                        }
//                    }
                } else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        shininess = values[0];
                    }
                } else if (cmd == "size") {
                    validinput = readvals(s,2,values);
                    if (validinput) {
                        screensize = glm::ivec2((int)values[0], (int)values[1]);
                    }
                } else if (cmd == "maxdepth")
                {
                    validinput = readvals(s, 1, values);
                    if (validinput)
                    {
                        maxdepth = values[0];
                    }
                }
                
                else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {
                        // lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fovy
                        
                        glm::vec3 eyeinit = glm::vec3(values[0], values[1], values[2]);
                        glm::vec3 center = glm::vec3(values[3], values[4], values[5]);
                        glm::vec3 upinit = glm::vec3(values[6], values[7], values[8]);
                        //upinit = Transform::upvector(upinit, center - eye);
                        float fovy = values[9];
                        mCamera.Init(eyeinit, center, upinit, screensize, fovy, maxdepth);
                        
                    }
                }
                
                else if (cmd == "sphere")
                {
                    validinput = readvals(s, 4, values);
                    if (validinput)
                    {
                        sphere_object* obj = new sphere_object();
                        obj->size = values[3];
                        for (i = 0; i < 3; i++) {
                            (obj->ambient)[i] = ambient[i];
                            (obj->diffuse)[i] = diffuse[i];
                            (obj->specular)[i] = specular[i];
                        }
                        obj->shininess = shininess;
                        
                        // Set the object's transform
                        obj->transform.SetTransform(transfstack.top() * glm::mat4(glm::vec4(1, 0, 0, 0),
                                                                                  glm::vec4(0, 1, 0, 0),
                                                                                  glm::vec4(0, 0, 1, 0),
                                                                                  glm::vec4(values[1], values[2], values[3], 1)
                                                                                  ));
                        mObjects.push_back((scene_object*)obj);
                    }
                }
                
                // I've left the code for loading objects in the skeleton, so
                // you can get a sense of how this works.
                // Also look at demo.txt to get a sense of why things are done this way.
                /*                else if (cmd == "sphere" || cmd == "cube" || cmd == "teapot") {
                 if (numobjects == maxobjects) { // No more objects
                 cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
                 } else {
                 validinput = readvals(s, 1, values);
                 if (validinput) {
                 object * obj = &(objects[numobjects]);
                 obj->size = values[0];
                 
                 // Set the object's light properties
                 for (i = 0; i < 4; i++) {
                 (obj->ambient)[i] = ambient[i];
                 (obj->diffuse)[i] = diffuse[i];
                 (obj->specular)[i] = specular[i];
                 (obj->emission)[i] = emission[i];
                 }
                 obj->shininess = shininess;
                 
                 // Set the object's transform
                 obj->transform = transfstack.top();
                 
                 // Set the object's type
                 if (cmd == "sphere") {
                 obj->type = sphere;
                 } else if (cmd == "cube") {
                 obj->type = cube;
                 } else if (cmd == "teapot") {
                 obj->type = teapot;
                 }
                 }
                 ++numobjects;
                 }
                 }*/
                
                else if (cmd == "translate") {
                    validinput = readvals(s,3,values);
                    if (validinput) {
                        transfstack.top() = transfstack.top() * Transform::translate(values[0], values[1], values[2]);
                        //transfstack.push(Transform::translate(values[0], values[1], values[2]) * transfstack.top() );
                        
                        cout << "Translating";
                        Transform::printMat4(transfstack.top());
                        
                        // YOUR CODE FOR HW 2 HERE.
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file.
                        // Also keep in mind what order your matrix is!
                        
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values);
                    if (validinput) {
                        transfstack.top() = transfstack.top() * Transform::scale(values[0], values[1], values[2]);
                        cout << "Scaling";
                        Transform::printMat4(transfstack.top());
                        //transfstack.push(Transform::scale(values[0], values[1], values[2]) * transfstack.top());
                        
                        // YOUR CODE FOR HW 2 HERE.
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file.
                        // Also keep in mind what order your matrix is!
                        
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values);
                    if (validinput) {
                        transfstack.top() = transfstack.top()
                        * mat4(Transform::rotate(values[3], vec3(values[0], values[1], values[2])));
                        // YOUR CODE FOR HW 2 HERE.
                        // values[0..2] are the axis, values[3] is the angle.
                        // You may want to normalize the axis (or in Transform::rotate)
                        // See how the stack is affected, as above.
                        // Note that rotate returns a mat3.
                        // Also keep in mind what order your matrix is!
                        
                    }
                }
                
                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n";
                    } else {
                        transfstack.pop();
                    }
                }
                
                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
            }
            getline (in, str);
        }
        
        // Set up initial position for eye, up and amount
        // As well as booleans
        
        
    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n";
        throw 2;
    }
}