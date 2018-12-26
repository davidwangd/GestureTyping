//
//  getWords.cpp
//  Algorithm
//
//  Created by Jiayin Wang on 2018/12/25.
//  Copyright © 2018年 Jiayin Wang. All rights reserved.
//

#include "getWords.hpp"

float Algorithm::computeDis(float x1,float x2,float y1,float y2)
{
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

int Algorithm::loadData()
{
    Unit* pointer[30];
    for(int i=0;i<26;i++)
        pointer[i]=&(dataBase[i]);
    int charFlag=0;
    ifstream myfile(inputFile);
    string temp;
    if (!myfile.is_open())
    {
        cout << "can not open this file" << endl;
    }
    while(getline(myfile,temp))
    {
        charFlag=temp[0]-'a';
        pointer[charFlag]->word=temp;
        pointer[charFlag]->centralH=0;
        pointer[charFlag]->centralW=0;
        pointer[charFlag]->minH=400;
        pointer[charFlag]->maxH=0;
        pointer[charFlag]->minW=400;
        pointer[charFlag]->maxW=0;
        for(int i=0;i<50;i++)
        {
            if(!getline(myfile,temp,' '))
            {
                cout<<"inputFile has sth wrong!"<<endl;
                return -1;
            }
            pointer[charFlag]->pointH[i]=atof(temp.c_str());
            pointer[charFlag]->centralH+=pointer[charFlag]->pointH[i];
            if(!getline(myfile,temp))
            {
                cout<<"inputFile has sth wrong!"<<endl;
                return -1;
            }
            pointer[charFlag]->pointW[i]=atof(temp.c_str());
            pointer[charFlag]->centralW+=pointer[charFlag]->pointW[i];
            
            if(pointer[charFlag]->minH > pointer[charFlag]->pointH[i])
                pointer[charFlag]->minH=pointer[charFlag]->pointH[i];
            if(pointer[charFlag]->maxH < pointer[charFlag]->pointH[i])
                pointer[charFlag]->maxH=pointer[charFlag]->pointH[i];
            if(pointer[charFlag]->minW > pointer[charFlag]->pointW[i])
                pointer[charFlag]->minW=pointer[charFlag]->pointW[i];
            if(pointer[charFlag]->maxW < pointer[charFlag]->pointW[i])
                pointer[charFlag]->maxW=pointer[charFlag]->pointW[i];
            
        }
        pointer[charFlag]->centralH/=50;
        pointer[charFlag]->centralW/=50;
        
        cout<<pointer[charFlag]->word<<endl;
        cout<<pointer[charFlag]->centralH<<" "<<pointer[charFlag]->centralW<<endl;
        cout<<pointer[charFlag]->minW<<" "<<pointer[charFlag]->maxW<<endl;
        
        pointer[charFlag]->next=new Unit;
        memset(pointer[charFlag]->next,0,sizeof(Unit));
        pointer[charFlag]=pointer[charFlag]->next;
        getline(myfile,temp);
    }
    myfile.close();
    
    for(int i=0;i<26;i++)
        norList(&dataBase[i]);
    return 0;
}

int Algorithm::normalize(Unit *pointer)
{
    float MaxHW=0;
    if((pointer->maxH-pointer->minH)>(pointer->maxW-pointer->minW))
        MaxHW=pointer->maxH-pointer->minH;
    else
        MaxHW=pointer->maxW-pointer->minW;
    float s=normalizeL/MaxHW;
    for(int i=0;i<50;i++)
    {
        pointer->NorH[i]=(pointer->pointH[i]-pointer->centralH)*s;
        pointer->NorW[i]=(pointer->pointW[i]-pointer->centralW)*s;
    }
    return 0;
}

int Algorithm::norList(Unit *pointer)
{
    while(pointer->next!=NULL)
    {
        normalize(pointer);
        pointer=pointer->next;
    }
    return 0;
}


int Algorithm::templatePruning()
{
    Unit *ans=&wordList;
    for(int i=0;i<26;i++)
    {
        float dis=computeDis(dataBase[i].NorH[0], cal.NorH[0], dataBase[i].NorW[0], cal.NorW[0]);
        if(dis>shapeThreshold)
            continue;
        Unit *pointer=&dataBase[i];
        while(pointer->next!=NULL)
        {
            dis=computeDis(pointer->NorH[49], cal.NorH[49], pointer->NorW[49], cal.NorW[49]);
            if(dis>shapeThreshold)
            {
                pointer=pointer->next;
                continue;
            }
            memcpy(ans, pointer, sizeof(Unit));
            ans->next=new Unit;
            memset(ans->next,0,sizeof(Unit));
            ans=ans->next;
            pointer=pointer->next;
        }
    }
    return 0;
}

int Algorithm::computeShape()
{
    Unit *pointer=&wordList;
    while(pointer->next!=NULL)
    {
        pointer->shapeScore=0;
        for(int i=0;i<50;i++)
        {
            // to be done
            pointer->shapeScore+=computeDis(pointer->NorH[i],cal.NorH[i],pointer->NorW[i],cal.NorW[i]);
        }
        pointer->shapeScore/=50;
        pointer=pointer->next;
    }
    return 0;
}

int Algorithm::cal_d(Unit *a,int input,Unit *b)
{
    // if any <= keyR, min<=KeyR then d(ai,b)-r<=0
    for(int i=0;i<50;i++)
    {
        float temp=computeDis(a->NorH[input],b->NorH[i],a->NorW[input],b->NorW[i]);
        if(temp<=keyR)
            return 0;
    }
    return 1;
    
}
int Algorithm::calD(Unit *input)
{
    // if any return ==1 then D(wordList, cal)||D(cal, wordList) !=0
    for(int i=0;i<50;i++)
    {
        if(cal_d(input, i, &cal)==1)
            return 1;
        if(cal_d(&cal, i, input)==1)
            return 1;
    }
    return 0;
}

int Algorithm::computeLocation()
{
    Unit *pointer=&wordList;
    while(pointer->next!=NULL)
    {
        pointer->posScore=0;
        for(int i=0;i<50;i++)
        {
            int flag=calD(pointer);
            if(flag!=0)
                pointer->posScore+=positionA[i]*computeDis(pointer->NorH[i],cal.NorH[i], pointer->NorW[i],cal.NorW[i]);
        }
        pointer=pointer->next;
    }
    return 0;
}
