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
#include "object.h"
#include "light.h"
#include "raytrace_camera.h"

class scene
{
public:
    void LoadData(const char* filename);
    inline const raytrace_camera& GetCamera() const{return mCamera;}
    inline const std::vector<scene_object*>& GetRenderableObject() const{return mObjects;}
    inline const std::vector<point_light*> GetPointLights() const{return mPointLights;}
    inline const std::vector<direction_light*> GetDirectionLights() const{return mDirectionalLights;}

    
private:
    raytrace_camera mCamera;
    std::vector<scene_object*> mObjects;
    std::vector<point_light*> mPointLights;
    std::vector<direction_light*> mDirectionalLights;
};


#endif /* scene_h */
