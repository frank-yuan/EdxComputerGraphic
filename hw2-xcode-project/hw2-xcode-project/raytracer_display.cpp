//
//  raytracer_display.h
//  hw2-xcode-project
//
//  Created by Xuan Yuan (Frank) on 1/3/16.
//  Copyright © 2016 frank-yuan. All rights reserved.
//

#include <GLUT/glut.h>

#ifndef raytracer_display_h
#define raytracer_display_h
void raytracer_display()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();
}

void raytracer_reshape(int width, int height)
{
}

#endif /* raytracer_display_h */
