// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// ParticleSystem.c
//
//    This is an example that demonstrates rendering a particle system
//    using a vertex shader and point sprites.
//
#include <stdlib.h>
#include <math.h>
#include "esUtil.h"

#define NUM_PARTICLES   1000
#define PARTICLE_SIZE   7

#define ATTRIBUTE_LIFETIME_LOCATION       0
#define ATTRIBUTE_STARTPOSITION_LOCATION  1
#define ATTRIBUTE_ENDPOSITION_LOCATION    2


float cube_vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

typedef struct VEC3 {
    float x;
    float y;
    float z;
} VEC3;

float cubePositions[] = {
   0.0f,  0.0f,  0.0f,
   0.5f,  0.5f, -3.0f,
  -0.5f, -2.2f, -2.5f,
  -1.8f, -2.0f, -2.3f,
   0.4f, -0.4f, -3.5f,
  -1.7f,  3.0f, -7.5f,
   1.3f, -2.0f, -2.5f,
   1.5f,  2.0f, -2.5f,
   1.5f,  0.2f, -1.5f,
  -1.3f,  1.0f, -1.5f
};

typedef struct
{
    // Handle to a program object
    GLuint programObject;
    
    // Uniform location
    GLint texture[2];
    GLint  mvpLoc;
    GLint samplerLoc[2];
    ESMatrix projectionMatrix;
    ESMatrix modelViewMatrix;
    ESMatrix mvpMatrix;
    
} UserData;

float cubeTimeValue = 0;
unsigned int vbo;

///
// Load texture from disk
//
static GLuint LoadTexture ( void *ioContext, char *fileName )
{
    int width,
    height;
    char *buffer = esLoadTGA ( ioContext, fileName, &width, &height );
    GLuint texId;
    
    if ( buffer == NULL )
    {
        esLogMessage ( "Error loading (%s) image.\n", fileName );
        return 0;
    }
    
    glGenTextures ( 1, &texId );
    glBindTexture ( GL_TEXTURE_2D, texId );
    
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    
    free ( buffer );
    
    return texId;
}


///
// Initialize the shader and program object
//
static int Init ( ESContext *esContext )
{
    char vShaderStr[] =
    "#version 300 es                                          \n"
    "layout(location=0) in vec4 a_position;                   \n"
    "layout(location=1) in vec2 a_texCoord;                   \n"
    "layout(location=2) in vec4 a_color;                      \n"
    "uniform mat4 u_mvpMatrix;                                \n"
    "out vec2 v_texCoord;                                     \n"
    "out vec4 v_color;                                        \n"
    "void main()                                              \n"
    "{                                                        \n"
    "  gl_Position = u_mvpMatrix * a_position;                \n"
    "  v_texCoord = a_texCoord;                               \n"
    "  v_color = a_color;                                     \n"
    "}";

    char fShaderStr[] =
    "#version 300 es                                          \n"
    "precision mediump float;                                 \n"
    "in vec2 v_texCoord;                                      \n"
    "in vec4 v_color;                                         \n"
    "uniform sampler2D u_texture;                             \n"
    "uniform sampler2D u_texture1;                            \n"
    "layout(location = 0) out vec4 outColor;                  \n"
    "void main() {                                            \n"
    "   vec4 baseColor;                                       \n"
    "   vec4 baseColor1;                                      \n"
    "   baseColor = texture(u_texture, v_texCoord);           \n"
    "   baseColor1 = texture(u_texture1, v_texCoord);         \n"
    "   outColor = baseColor * (baseColor1 + 0.25) * v_color; \n"
    "}                                                        \n";
    
    UserData *userData = esContext->userData;

    userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );
    
    userData->mvpLoc = glGetUniformLocation(userData->programObject, "u_mvpMatrix");
    userData->samplerLoc[0] = glGetUniformLocation(userData->programObject, "u_texture");
    
    userData->texture[0] = LoadTexture ( esContext->platformData, "lightmap.tga" );
    
    glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
    
    // Fill in particle data array
    srand ( 0 );
    
    if ( userData->texture[0] <= 0)
    {
        return FALSE;
    }
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    
    return TRUE;
}

ESMatrix cube_matrix1;
ESMatrix cube_matrix2;
ESMatrix cube_matrix3;
ESMatrix cube_matrix4;

///
//  Update time-based variables
//
static void Update ( ESContext *esContext, float deltaTime )
{
    
}

///
// Draw a triangle using the shader pair created in Init()
//
static void Draw ( ESContext *esContext )
{
    UserData *userData = esContext->userData;
    
    glViewport ( 0, 0, esContext->width, esContext->height );
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram ( userData->programObject );
    glEnable(GL_DEPTH_TEST);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, userData->texture[0]);
    glUniform1i(userData->samplerLoc[0], 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(0 * sizeof(float)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    
    glVertexAttrib4f(2, 1, 1, 1, 1);
    
    float aspect = (float)esContext->width / (float)esContext->height;
    esMatrixLoadIdentity(&userData->projectionMatrix);
    esPerspective(&userData->projectionMatrix, 90.0f, aspect, 1.0f, 20.0f);
    
    for (int i = 0; i < 10; i++) {
        esMatrixLoadIdentity(&userData->modelViewMatrix);

        esTranslate(&userData->modelViewMatrix, cubePositions[i * 3], cubePositions[i * 3 + 1], cubePositions[i * 3 + 2] + - 2);
        //旋转
        esRotate(&userData->modelViewMatrix, cubeTimeValue, 0.5 + sin(i * i), sin(i * i), 0);
        //缩小0.5倍
        esScale(&userData->modelViewMatrix, 0.3, 0.3, 0.3);
        esMatrixMultiply(&userData->mvpMatrix, &userData->modelViewMatrix, &userData->projectionMatrix);
        glUniformMatrix4fv(userData->mvpLoc, 1, GL_FALSE, (GLfloat *)&userData->mvpMatrix.m[0][0]);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

}

///
// Cleanup
//
static void ShutDown ( ESContext *esContext )
{
    UserData *userData = esContext->userData;
    
    // Delete texture object
    glDeleteTextures ( 1, &userData->texture );
    
    // Delete program object
    glDeleteProgram ( userData->programObject );
}


int esCubeMain ( ESContext *esContext, float width, float height )
{
    esContext->userData = malloc ( sizeof ( UserData ) );
    
    esCreateWindow ( esContext, "ParticleSystem", width, height, ES_WINDOW_RGB );
    
    if ( !Init ( esContext ) )
    {
        return GL_FALSE;
    }
    
    esRegisterDrawFunc ( esContext, Draw );
    esRegisterUpdateFunc ( esContext, Update );
    esRegisterShutdownFunc ( esContext, ShutDown );
    
    return GL_TRUE;
}

void cubeChangeTime(float value) {
    cubeTimeValue = value;
}
