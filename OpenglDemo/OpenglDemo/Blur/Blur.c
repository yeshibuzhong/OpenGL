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


GLfloat blur_vertices[] = {
    -0.5,  -0.5, 0.0f,
    0.5, -0.5, 0.0f,
    0.5,  0.5, 0.0f,

    -0.5,  -0.5, 0.0f,
    0.5,  0.5, 0.0f,
    -0.5,  0.5, 0.0f
};

//GLfloat blur_vertices[] = {
//    -0.5,  -0.25, 0.0f,
//    0.5, -0.25, 0.0f,
//    0,  0.5, 0.0f,
//};

GLfloat blur_texture[] = {
    0,0,
    1,0,
    1,1,
    0,0,
    1,1,
    0,1
};

typedef struct
{
    // Handle to a program object
    GLuint programObject;
    
    // Uniform location
    GLint texture;
    GLint  mvpLoc;
    GLint samplerLoc;
    GLint blurStep;
    ESMatrix projectionMatrix;
    ESMatrix modelViewMatrix;
    ESMatrix mvpMatrix;
    
} UserData;

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
    "#version 300 es                                                                     \n"
    "layout(location=0) in vec4 a_position;                                              \n"
    "layout(location=1) in vec2 a_texCoord;                                              \n"
    "uniform mat4 u_mvpMatrix;                                                           \n"
    "out vec2 v_texCoord;                                                                \n"
    "void main()                                                                         \n"
    "{                                                                                   \n"
    "  gl_Position = u_mvpMatrix * a_position;                                           \n"
    "  v_texCoord = a_texCoord;                                                          \n"
    "}";
    
    char fShaderStr[] =
    "#version 300 es                                                                     \n"
    "precision mediump float;                                                            \n"
    "uniform sampler2D s_texture;                                                        \n"
    "uniform float u_blurStep;                                                           \n"
    "in vec2 v_texCoord;                                                                 \n"
    "layout(location = 0) out vec4 outColor;                                             \n"
    "void main(void) {                                                                   \n"
    "   vec4 sample0, sample1, sample2, sample3;                                         \n"
    "   float fStep = u_blurStep / 100.0;                                                \n"
    "   sample0 = texture(s_texture, vec2(v_texCoord.x - fStep, v_texCoord.y - fStep));  \n"
    "   sample1 = texture(s_texture, vec2(v_texCoord.x + fStep, v_texCoord.y + fStep));  \n"
    "   sample2 = texture(s_texture, vec2(v_texCoord.x + fStep, v_texCoord.y - fStep));  \n"
    "   sample3 = texture(s_texture, vec2(v_texCoord.x - fStep, v_texCoord.y + fStep));  \n"
    "   outColor = (sample0 + sample1 + sample2 + sample3) / 4.0;                        \n"
    "}                                                                                   \n";
    
    UserData *userData = esContext->userData;
    
    userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );
    
    userData->mvpLoc = glGetUniformLocation(userData->programObject, "u_mvpMatrix");
    userData->samplerLoc = glGetUniformLocation(userData->programObject, "u_texture");
    userData->blurStep = glGetUniformLocation(userData->programObject, "u_blurStep");
    
    userData->texture = LoadTexture ( esContext->platformData, "lightmap.tga" );
    
    glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
    
    // Fill in particle data array
    srand ( 0 );
    
    if ( userData->texture <= 0)
    {
        return FALSE;
    }
    
    return TRUE;
}

///
//  Update time-based variables
//
static void Update ( ESContext *esContext, float deltaTime )
{
    UserData *userData = esContext->userData;
    
    float aspect = (float)esContext->width / ((float)esContext->height / 2.0);
    esMatrixLoadIdentity(&userData->projectionMatrix);
    esPerspective(&userData->projectionMatrix, 60.0f, aspect, 1.0f, 20.0f);
    
    esMatrixLoadIdentity(&userData->modelViewMatrix);
    esTranslate(&userData->modelViewMatrix, 0, 0, -2);
    esScale(&userData->modelViewMatrix, 2, 2, 2);
    
    esMatrixMultiply(&userData->mvpMatrix, &userData->modelViewMatrix, &userData->projectionMatrix);
}


float blurValue = 0;

void changeBlurValue(float value) {
    blurValue = value;
}

///
// Draw a triangle using the shader pair created in Init()
//
static void Draw ( ESContext *esContext )
{
    UserData *userData = esContext->userData;
    
    //模糊画面
    glViewport ( 0, 0, esContext->width, esContext->height / 2);
    glClear ( GL_COLOR_BUFFER_BIT );
    glUseProgram ( userData->programObject );
    
    glUniformMatrix4fv(userData->mvpLoc, 1, GL_FALSE, (GLfloat *)&userData->mvpMatrix.m[0][0]);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, userData->texture);
    glUniform1i(userData->samplerLoc, 0);
    
    glUniform1f(userData->blurStep, blurValue);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, blur_vertices);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, blur_texture);
        
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
    //原始画面
    glViewport ( 0, esContext->height / 2, esContext->width , esContext->height / 2 );
    glUniform1f(userData->blurStep, 0);
    esTranslate(&userData->mvpMatrix, 0, -0.3, 0);
    glUniformMatrix4fv(userData->mvpLoc, 1, GL_FALSE, (GLfloat *)&userData->mvpMatrix.m[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
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


int esBlurMain ( ESContext *esContext, float width, float height )
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
