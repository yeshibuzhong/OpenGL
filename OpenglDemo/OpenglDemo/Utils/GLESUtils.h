//
//  GLESUtils.h
//  OpenglDemo
//
//  Created by wangyahui on 2021/3/15.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/ES3/gl.h>

NS_ASSUME_NONNULL_BEGIN

@interface GLESUtils : NSObject

GLuint esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc );
+ (GLuint)setupTexture:(id)fileName;

@end

NS_ASSUME_NONNULL_END
