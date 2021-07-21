//
//  ViewController.m
//  OpenglDemo
//
//  Created by wangyahui on 2021/3/15.
//

#import "ViewController.h"

@interface ViewController ()<UITableViewDelegate, UITableViewDataSource>

@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (strong, nonatomic) NSMutableArray<NSString *> *titleArray;
@property (strong, nonatomic) NSMutableArray<NSString *> *vcArray;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.titleArray = [NSMutableArray arrayWithObjects:@"粒子动画", @"双重纹理", @"图片模糊", @"矩阵变换", @"立方体", nil];
    self.vcArray = [NSMutableArray arrayWithObjects:@"ParticleSystemVCViewController", @"DoubleSampleVC", @"BlurViewController", @"MatrixViewController", @"CubeViewController", nil];

}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return _titleArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"identifier"];
    if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"identifier"];
        cell.textLabel.text = _titleArray[indexPath.row];
    }
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    Class class = NSClassFromString(_vcArray[indexPath.row]);
    UIViewController *vc = [class new];
    vc.title = _titleArray[indexPath.row];
    [self.navigationController pushViewController:vc animated:YES];
}

@end
