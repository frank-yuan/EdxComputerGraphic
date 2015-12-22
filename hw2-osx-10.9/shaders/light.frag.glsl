# version 120 

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Mine is an old machine.  For version 130 or higher, do 
// in vec4 color;  
// in vec3 mynormal; 
// in vec4 myvertex;
// That is certainly more modern

varying vec4 color;
varying vec3 mynormal; 
varying vec4 myvertex; 

const int numLights = 10; 
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights 
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.  These could be varying and/or bound to 
// a buffer.  But for now, I'll just make them uniform.  
// I use ambient, diffuse, specular, shininess as in OpenGL.  
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess; 

void main (void) 
{       
    if (enablelighting) {
        vec4 finalcolor = vec4(0);

        // YOUR CODE FOR HW 2 HERE
        // A key part is implementation of the fragment shader
        vec4 viewVertex = gl_ModelViewMatrix * myvertex;

        vec3 eyedir = normalize(-viewVertex.xyz);
        // WHY I CANNOT USE NORMALIZE BEFORE MODEL VIEW INVERSE TRANSPOSE   ????????
        vec3 normal = (gl_ModelViewMatrixInverseTranspose * vec4(mynormal, 0)).xyz;
        
        for (int i = 0; i < numused; ++i)
        {
            vec3 lightpos = lightposn[i].xyz;
            vec3 lightdir = normalize(lightpos - viewVertex.xyz);
            float lightdiffuse = max(0, dot(lightdir, normal));
            // diffuse
            finalcolor += lightdiffuse * lightcolor[i] * diffuse;
            //finalcolor = diffuse;//vec4(lightdiffuse,lightdiffuse,lightdiffuse, 1);
            
            // specular
            vec3 reflect = normalize(2 * normal * dot(lightdir, normal) - lightdir);
            
            finalcolor += pow(max(0, dot(reflect, eyedir)), shininess) * lightcolor[i] * specular;
        }
        // Color all pixels black for now, remove this in your implementation!
 
        finalcolor += ambient + emission;

        gl_FragColor = finalcolor; 
    } else {
        gl_FragColor = color;
    }
}
