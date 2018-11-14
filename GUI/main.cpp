//
//  main.cpp
//  testforQuagga
//
//  Created by Jiayin Wang on 2018/11/10.
//  Copyright © 2018年 Jiayin Wang. All rights reserved.
//
/*
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <iostream>
#include <vector>
using namespace std;
*/

#include "CImg.h"
using namespace cimg_library;
int main()
{
    CImg<unsigned char> SrcImg;
    SrcImg.load_bmp("test.bmp");
    int width = SrcImg.width();
    int height = SrcImg.height();
    // 定义一个每个颜色 8 位(bit)的 640x400 的彩色图像 横 纵
    CImg<unsigned char> img(width+200,height+200,1,3);
    //将像素值设为 0（黑色）
    img.fill(0);
    // 定义一个紫色
    unsigned char purple[] = { 255,0,255 };
    
    // 在坐标(100, 100)处画一个紫色的“Hello world”
    img.draw_text(100,100,"Hello World",purple);
    // 在一个标题为“My first CImg code”的窗口中显示这幅图像

    img.draw_image(100,100,SrcImg);
    img.display("keyboard Layout");
    //img.display("keyboard Layout");
    
    return 0;
} 
