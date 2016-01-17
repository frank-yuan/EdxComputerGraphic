//
//  scene.h
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/5/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#ifndef scene_h
#define scene_h
#include <vector>
#include <string>
#include "object.h"
#include "light.h"
#include "raytrace_camera.h"

class scene
{
public:
    void LoadData(const char* filename);
    inline const raytrace_camera& GetCamera() const{return mCamera;}
    inline const std::vector<scene_object*>& GetRenderableObject() const{return mObjects;}
    inline const std::vector<light*>& GetLights() const{return mLights;}
    inline const char* GetOutputFilename() const{return mOutputFilename.c_str();}

    
private:
    raytrace_camera mCamera;
    std::vector<scene_object*> mObjects;
    std::vector<light*> mLights;
    std::string mOutputFilename = "default_output.png";
};


#endif /* scene_h */
