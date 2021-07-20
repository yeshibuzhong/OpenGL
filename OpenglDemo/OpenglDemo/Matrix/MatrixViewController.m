//
//  MatrixVCViewController.m
//  OpenglDemo
//
//  Created by wangyahui on 2021/7/20.
//

#import "MatrixViewController.h"

int esMatrixMain ( ESContext *esContext, float width, float height );
void changeTime(float value);

@interface MatrixViewController ()

@end

@implementation MatrixViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    [self setupGL];
    
    __block float value = 0;
    NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:1 / 60 repeats:YES block:^(NSTimer * _Nonnull timer) {
        value += 0.01;
        changeTime(value);
    }];
}

- (void)setupGL
{
    [super setupGL];
    float scale = [UIScreen mainScreen].scale;
    esMatrixMain(&_esContext, self.view.frame.size.width * scale, self.view.frame.size.height * scale);
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
