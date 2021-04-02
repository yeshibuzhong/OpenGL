//
//  BaseViewController.h
//  OpenglDemo
//
//  Created by wangyahui on 2021/3/16.
//

#import <GLKit/GLKit.h>
#import <OpenGLES/ES3/gl.h>
#include "esUtil.h"

NS_ASSUME_NONNULL_BEGIN

@interface BaseViewController : GLKViewController {
    ESContext _esContext;
}

@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;

@end




NS_ASSUME_NONNULL_END
