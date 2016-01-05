/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167x by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.
// Since all files need access to it, we define EXTERN as either blank or
// extern, depending on if included in the main program or not.

#ifdef MAINPROGRAM
#define EXTERN
#else
#define EXTERN extern
#endif
#include <vector>
EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye
EXTERN vec3 up;  // The (regularly updated) vector coordinates of the up

#ifdef MAINPROGRAM
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
int amountinit = 5; // Initial step amount for camera movement
vec3 center(0.0,0.0,0.0) ; // Center look at point
int w = 600, h = 400 ; // width and height
float fovy = 90.0 ; // For field of view

// for ray tracer
int maxdepth;
int maxverts;
int maxvertnorms;
#else
EXTERN vec3 eyeinit ;
EXTERN vec3 upinit ;
EXTERN vec3 center ;
EXTERN int amountinit;
EXTERN int w, h ;
EXTERN float fovy ;

// for ray tracer
EXTERN int maxdepth;
EXTERN int maxverts;
EXTERN int maxvertnorms;
#endif

EXTERN bool useGlu; // Toggle use of "official" opengl/glm transform vs user
EXTERN GLuint vertexshader, fragmentshader, shaderprogram ; // shaders
static enum {view, translate, scale} transop ; // which operation to transform

EXTERN float sx, sy ; // the scale in x and y
EXTERN float tx, ty ; // the translation in x and y

// Lighting parameter array, similar to that in the fragment shader
const int numLights = 10 ;
EXTERN GLfloat lightposn [4*numLights] ; // Light Positions
EXTERN GLfloat lightcolor[4*numLights] ; // Light Colors
EXTERN GLfloat lightransf[4*numLights] ; // Lights transformed by modelview
EXTERN int numused ;                     // How many lights are used

// Materials (read from file)
// With multiple objects, these are colors for each.
EXTERN GLfloat ambient[4] ;
EXTERN GLfloat diffuse[4] ;
EXTERN GLfloat specular[4] ;
EXTERN GLfloat emission[4] ;
EXTERN GLfloat shininess ;

// For multiple objects, read from a file.
extern class object;
EXTERN std::vector<object*> objects;
extern struct point_light;
EXTERN std::vector<point_light*> pointlights;
extern struct direction_light;
EXTERN std::vector<direction_light*> directionlights;
EXTERN std::string fname;


// Variables to set uniform params for lighting fragment shader
EXTERN GLuint lightcol ;
EXTERN GLuint lightpos ;
EXTERN GLuint numusedcol ;
EXTERN GLuint enablelighting ;
EXTERN GLuint ambientcol ;
EXTERN GLuint diffusecol ;
EXTERN GLuint specularcol ;
EXTERN GLuint emissioncol ;
EXTERN GLuint shininesscol ;

