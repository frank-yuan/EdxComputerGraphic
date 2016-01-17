/******************************************************************************/
/* This is the program skeleton for homework 2 in CSE167x by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects    */
/******************************************************************************/

// You shouldn't have to edit this file at all!

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <cmath>
#include <GLUT/glut.h>
#include <FreeImage.h>
#include "shaders.h"
#include "Transform.h"
#include "scene.h"
#include "myutil.h"
#include "progress_recorder.h"
#include "time_helper.h"

using namespace std;

// Main variables in the program.
#define MAINPROGRAM
void display(void);  // prototype for display function.
void raytracer_display(void);
void raytracer_reshape(int width, int height);

void printProgress(int progress)
{
    std::cout << progress << "%" << std::endl;
}
extern int triangleTested;
int main(int argc, char* argv[])
{
  
    uint64 startTime = GetTimeMs64();
    if (argc < 2) {
        cerr << "Usage: transforms scenefile [opengl]\n";
        exit(-1);
    }
    
    FreeImage_Initialise();
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("Raytracer: Scene Viewer");
    scene myscene;
    myscene.LoadData(argv[1]);
    glm::ivec2 screenSize = myscene.GetCamera().GetScreenSize();
    vec3 vcolor;
    //myscene.GetCamera().GetColorFromRaytracing(screenSize / 2, myscene, vcolor);
    
    int totalPixels = screenSize.x * screenSize.y;

    BYTE* pixels = (BYTE*)malloc(sizeof(BYTE) * totalPixels * 3);

    progress_recorder recorder(totalPixels - 1);
    recorder.RegisterChangedEvent(printProgress);
    
    for (int y = 0; y < screenSize.y; ++y)
    {
        for (int x = 0; x < screenSize.x; ++x)
        {
            
            if (myscene.GetCamera().GetColorFromRaytracing(glm::ivec2(x, y), myscene, vcolor))
            {
                int color = GetColorInt(vcolor);
                pixels[(x+(y * screenSize.x)) * 3 + 0] = (color) & 0xFF;
                pixels[(x+(y * screenSize.x)) * 3 + 1] = (color >> 8) & 0xFF;
                pixels[(x+(y * screenSize.x)) * 3 + 2] = (color >> 16) & 0xFF;
                
            }
            else
            {
                for (int i = 0; i < 3; ++i)
                {
                    pixels[(x+(y * screenSize.x)) * 3 + i] = 0;
                }
                // customed clear color for test
//                pixels[(x+(y * screenSize.x)) * 3 + 0] = 121;
//                pixels[(x+(y * screenSize.x)) * 3 + 1] = 77;
//                pixels[(x+(y * screenSize.x)) * 3 + 2] = 49;
            }
                        
            recorder.UpdateProgress(x+(y * screenSize.x));
        }
    }
    FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, screenSize.x, screenSize.y, screenSize.x * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
    //TODO: output file name
    FreeImage_Save(FIF_PNG, img, myscene.GetOutputFilename(), 0);
    FreeImage_DeInitialise();
    uint64 endTime = GetTimeMs64();
    cout << "Scene file:" << myscene.GetOutputFilename() << " finish after " << (endTime - startTime) / 1000.0f << " seconds." << "tested triangles:" << triangleTested << endl;
//    glutDisplayFunc(raytracer_display);
//    glutReshapeFunc(raytracer_reshape);
//    
//    
//    glutReshapeWindow(screenSize.x, screenSize.y);
//    
//    
//    //printHelp();
//    glutMainLoop();

    return 0;
}
