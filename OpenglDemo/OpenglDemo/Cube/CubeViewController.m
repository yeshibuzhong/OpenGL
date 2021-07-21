//
//  CubeViewController.m
//  OpenglDemo
//
//  Created by wangyahui on 2021/7/20.
//

#import "CubeViewController.h"

int esCubeMain ( ESContext *esContext, float width, float height );
void cubeChangeTime(float value);

@interface CubeViewController ()

@property (strong, nonatomic) NSTimer *timer;

@end

@implementation CubeViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    [self setupGL];
    
    __block float value = 0;
    _timer = [NSTimer scheduledTimerWithTimeInterval:1 / 60 repeats:YES block:^(NSTimer * _Nonnull timer) {
        value += 0.01;
        cubeChangeTime(value);
    }];
}

- (void)dealloc {
    NSLog(@"dealloc");
    [_timer invalidate];
    _timer = nil;
}

- (void)setupGL
{
    [super setupGL];
    float scale = [UIScreen mainScreen].scale;
    esCubeMain(&_esContext, self.view.frame.size.width * scale, self.view.frame.size.height * scale);
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
