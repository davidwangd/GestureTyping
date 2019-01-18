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

#include<time.h>
#include "../Listener.h"
#include "../keyboardUI/keyboardUI.h"

using namespace std;

struct Unit
{
    string word;
    int times;
    float pointH[60];
    float pointW[60];
    float NorH[60];
    float NorW[60];
    float minH,maxH,minW,maxW;
    float centralH,centralW;
    float shapeScore;
    float posScore;
    float finalScore;
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
    float shapeInt;
    float positionInt;
    float positionA[60];
    float computeDis(float x1,float x2,float y1,float y2);
    char *output[6];
    int outputNUM;
    Unit cal;
    Unit inputData;
    //Unit wordList;
    Unit wordListNEW[20000];
    int mempointer;
    Unit *wordEnd;
    // 保留LeapMotion和UI的接口。
    MyListener *listener;
    keyboard* ui;
    void swapAB(Unit* a,Unit* b);
    void QuickSortList(Unit* pHead,Unit* pEnd);
    Unit* Partition(Unit* pBegin,Unit* pEnd);
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
    
    int preState;
    int nowState;
    
public:
    /**
     * Modified by DavidWang
     * 2018/12/27
     * 增加两个默认参数，为了和之前的代码兼容加入了默认值
     * 在实际使用中必须有合法值
     */
	Algorithm(MyListener* listener = nullptr, keyboard *ui = nullptr)
	{
		this->listener = listener;
		this->ui = ui;
	}

	void init(){
		mempointer = 0;
        length=50;
        keyR=35;
        normalizeL=100;
        shapeThreshold=100;// to be tested***
        for(int i=0;i<25;i++)
        {
            // 0.02
            positionA[i]=0.02+(12-i)*0.0005;
            positionA[49-i]=0.02+(12-i)*0.0005;
        }
        shapeInt=100;
        positionInt=50;
        inputFile="out.txt";
        //memset(dataBase,0,sizeof(Unit)*30);
        memset(&cal,0,sizeof(Unit));
        //memset(&wordList,0,sizeof(Unit));
        loadData();
        preState=this->listener->getState();
        nowState=preState;
        for(int i=0;i<6;i++)
            output[i]=new char[50];
        cout<<"ini FINISHED~"<<endl;
    }
    
	~Algorithm() {
	//	delete [] dataBase;
	}
	void startCompute();
    
};

#endif /* getWords_hpp */
