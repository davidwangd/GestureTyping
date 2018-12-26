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
    Unit cal;
    Unit inputData;
    Unit wordList;
    Unit *wordEnd;
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

public:
	Algorithm()
	{
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
        inputFile="./out.txt";
        memset(dataBase,0,sizeof(Unit)*30);
        memset(&cal,0,sizeof(Unit));
        memset(&wordList,0,sizeof(Unit));
        loadData();
        cout<<"ini FINISHED~"<<endl;
	}
    
    void startCompute()
    {
        cout<<"start computing"<<endl;
        getData();
        Unit *ans=&inputData;
        while(ans->next!=NULL)
        {
            memcpy(&cal,ans,sizeof(Unit));
            memset(&wordList, 0, sizeof(Unit));
            normalize(&cal);
            templatePruning();
            computeShape();
            computeLocation();
            Integration();
            Unit *pointer=&wordList;
            string word1,word2;
            float mina=10000,minb=10000;
            float tempa=10000,tempb=10000;
            float finalS=0;
            string finalW;
            QuickSortList(&wordList,wordEnd);
            while(pointer->next!=NULL)
            {
                cout<<pointer->word<<endl;
                cout<<pointer->finalScore<<endl;
                if(pointer->shapeScore<mina||(pointer->shapeScore==mina&&tempa>pointer->posScore))
                {
                    mina=pointer->shapeScore;
                    tempa=pointer->posScore;
                    word1=pointer->word;
                }
                if(pointer->posScore<minb||(pointer->posScore==minb&&pointer->shapeScore<tempb))
                {
                    minb=pointer->posScore;
                    tempb=pointer->shapeScore;
                    word2=pointer->word;
                }
                if(finalS<pointer->finalScore)
                {
                    finalS=pointer->finalScore;
                    finalW=pointer->word;
                }
                //cout<<pointer->word<<endl;
                //cout<<pointer->shapeScore<<endl;
                //cout<<pointer->posScore<<endl;
                pointer=pointer->next;
            }
            cout<<cal.word<<endl;
            cout<<word1<<" "<<word2<<" "<<finalW<<endl;
            ans=ans->next;
        }

        
    }

};

#endif /* getWords_hpp */
