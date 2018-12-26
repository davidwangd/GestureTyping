//
//  getWords.hpp
//  Algorithm
//
//  Created by Jiayin Wang on 2018/12/25.
//  Copyright © 2018年 Jiayin Wang. All rights reserved.
//

#ifndef getWords_hpp
#define getWords_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

struct Unit
{
	string word;
	float pointH[60];
	float pointW[60];
    float NorH[60];
    float NorW[60];
    float minH,maxH,minW,maxW;
    float centralH,centralW;
    float shapeScore;
    float posScore;
	Unit* next;
};

class Algorithm
{
private:
	int length;
    float keyR;
	Unit dataBase[30];
	string inputFile;
    float normalizeL;
    float shapeThreshold;
    float positionA[60];
    float computeDis(float x1,float x2,float y1,float y2);
    Unit cal;
    Unit wordList;
    
    int loadData();
    int getData();
    int normalize(Unit *pointer);
    int norList(Unit *pointer);
    int templatePruning();
    int computeShape();
    
    int cal_d(Unit *a,int i,Unit *b);
    int calD(Unit *input);
    int computeLocation();
    
    int Integration();
    
public:
	Algorithm()
	{
		length=50;
        keyR=35;
        normalizeL=100;
        shapeThreshold=200;// to be tested***
        for(int i=0;i<25;i++)
        {
            // 0.02
            positionA[i]=0.02+(12-i)*0.001;
            positionA[49-i]=0.02+(12-i)*0.001;
        }
		inputFile="/Users/alice/Desktop/Junior/上学期/软件工程-白晓颖/git/Algorithm/Algorithm/out.txt";
		memset(dataBase,0,sizeof(Unit)*30);
        memset(&cal,0,sizeof(Unit));
        memset(&wordList,0,sizeof(Unit));
        loadData();
        cout<<"ini FINISHED~"<<endl;
	}
    
    void startCompute()
    {
        while(1)
        {
            cout<<"start computing"<<endl;
            getData();
            normalize(&cal);
            templatePruning();
            computeShape();
            computeLocation();
            Integration();
        }
    }

};

#endif /* getWords_hpp */
