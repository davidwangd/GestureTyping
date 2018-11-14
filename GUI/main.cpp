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
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<net/route.h>
#include <iostream>
#include <errno.h>
#include <vector>
#include <sstream>
using namespace std;

void split(const string& s,vector<string>& sv,const char flag = ' ') {
    sv.clear();
    istringstream iss(s);
    string temp;
    
    while (getline(iss, temp, flag)) {
        sv.push_back(temp);
    }
    return;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    string input="no ip route 192.168.1.0/24 192.168.2.254\n";
    printf("%s",input.c_str());
    vector<string> sv;
    split(input, sv, ' ');
    for (const auto& s : sv) {
        cout << s << endl;
    }
    vector<string> second;
    split(sv[3],second,'/');
    for (const auto& s : second) {
        cout << s << endl;
    }
    stringstream sstr1(second[1]);
    int x;
    sstr1 >> x;
    printf("%d",x);
    return 0;
}
 */
#include "CImg.h"
using namespace cimg_library;
int main()
{
    // 定义一个每个颜色 8 位(bit)的 640x400 的彩色图像
    CImg<unsigned char> img(640,400,1,3);
    //将像素值设为 0（黑色）
    img.fill(0);
    // 定义一个紫色
    unsigned char purple[] = { 255,0,255 };
    
    // 在坐标(100, 100)处画一个紫色的“Hello world”
    img.draw_text(100,100,"Hello World",purple);
    // 在一个标题为“My first CImg code”的窗口中显示这幅图像
    img.display("My first CImg code");
    
    return 0;
} 
