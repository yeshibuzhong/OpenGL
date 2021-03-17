//
//  BlurViewController.m
//  OpenglDemo
//
//  Created by wangyahui on 2021/3/17.
//

#import "BlurViewController.h"

int esBlurMain ( ESContext *esContext, float width, float height );

@interface BlurViewController ()

@end

@implementation BlurViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self setupGL];
}

- (void)setupGL
{
    [super setupGL];
    
    esBlurMain(&_esContext, ((GLKView *)self.view).drawableWidth, ((GLKView *)self.view).drawableWidth);

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
