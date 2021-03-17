//
//  ParticleSystemVCViewController.m
//  OpenglDemo
//
//  Created by wangyahui on 2021/3/16.
//

#import "ParticleSystemVCViewController.h"

 extern  void esParticleSystemMain( ESContext *esContext );

@interface ParticleSystemVCViewController ()

@end

@implementation ParticleSystemVCViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    [self setupGL];
}

- (void)setupGL
{
    [super setupGL];
    
    esParticleSystemMain( &_esContext );
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
