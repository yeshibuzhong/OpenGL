//
//  DoubleSampleVC.m
//  OpenglDemo
//
//  Created by wangyahui on 2021/3/15.
//

#import "DoubleSampleVC.h"

int esDoubleSampleVCMain ( ESContext *esContext, float width, float height );

@interface DoubleSampleVC ()

@end

@implementation DoubleSampleVC

- (void)viewDidLoad {
    [super viewDidLoad];

    [self setupGL];
}

- (void)setupGL
{
    [super setupGL];
    float scale = [UIScreen mainScreen].scale;
    esDoubleSampleVCMain(&_esContext, self.view.frame.size.width * scale, self.view.frame.size.height * scale);
}


- (void)update
{
    if ( _esContext.updateFunc )
    {
        _esContext.updateFunc( &_esContext, self.timeSinceLastUpdate );
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    _esContext.width = view.drawableWidth;
    _esContext.height = view.drawableHeight;
    
    if ( _esContext.drawFunc )
    {
        _esContext.drawFunc( &_esContext );
    }
}


@end
