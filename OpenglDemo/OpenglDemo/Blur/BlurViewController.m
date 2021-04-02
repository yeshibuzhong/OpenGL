//
//  BlurViewController.m
//  OpenglDemo
//
//  Created by wangyahui on 2021/3/17.
//

#import "BlurViewController.h"

int esBlurMain ( ESContext *esContext, float width, float height );
void changeBlurValue(float value);

@interface BlurViewController ()

@end

@implementation BlurViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self setupGL];
    
    CGFloat statusHeight = [[UIApplication sharedApplication] statusBarFrame].size.height;
    
    CGFloat navheight = self.navigationController.navigationBar.frame.size.height;
    UISlider *slider = [[UISlider alloc] initWithFrame:CGRectMake(100, statusHeight + navheight, 200, 50)];
    slider.backgroundColor = [UIColor redColor];
    [slider addTarget:self action:@selector(slider:) forControlEvents:UIControlEventValueChanged];
    [self.view addSubview:slider];
}

- (void)slider:(UISlider *)slider {
    changeBlurValue(slider.value);
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
