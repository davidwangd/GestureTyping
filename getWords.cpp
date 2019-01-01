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
    float x=sqrt(pow(x1-x2,2)+pow(y1-y2,2));
    return x;
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
        if(!getline(myfile,temp))
        {
            cout<<"inputFile has sth wrong!"<<endl;
            return -1;
        }
        pointer[charFlag]->times=atoi(temp.c_str());
        pointer[charFlag]->centralH/=50;
        pointer[charFlag]->centralW/=50;
        
        //cout<<pointer[charFlag]->word<<endl;
        //cout<<pointer[charFlag]->centralH<<" "<<pointer[charFlag]->centralW<<endl;
        //cout<<pointer[charFlag]->minW<<" "<<pointer[charFlag]->maxW<<endl;
        
        pointer[charFlag]->next=new Unit;
        memset(pointer[charFlag]->next,0,sizeof(Unit));
        pointer[charFlag]=pointer[charFlag]->next;
        getline(myfile,temp);
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
        float dis=computeDis(dataBase[i].pointH[0], cal.pointH[0], dataBase[i].pointW[0], cal.pointW[0]);
        if(dis>shapeThreshold)
            continue;
        Unit *pointer=&dataBase[i];
        while(pointer->next!=NULL)
        {
            dis=computeDis(pointer->pointH[49], cal.pointH[49], pointer->pointW[49], cal.pointW[49]);
            if(dis>shapeThreshold)
            {
                pointer=pointer->next;
                continue;
            }
            memcpy(ans, pointer, sizeof(Unit));
            wordEnd=ans;
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
        float temp=computeDis(a->pointH[input],b->pointH[i],a->pointW[input],b->pointW[i]);
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
                pointer->posScore+=positionA[i]*computeDis(pointer->pointH[i],cal.pointH[i], pointer->pointW[i],cal.pointW[i]);
        }
        pointer=pointer->next;
    }
    return 0;
}

int Algorithm::Integration()
{
    float head;
    float position,shape;
    Unit *pointer=&wordList;
    while(pointer->next!=NULL)
    {
        head=-1.0/2.0*pow(pointer->posScore/positionInt,2);
        position=1.0/(positionInt*sqrt(2*3.1415926))*exp(head);
        head=-1.0/2.0*pow(pointer->shapeScore/shapeInt,2);
        shape=1.0/(shapeInt*sqrt(2*3.1415926))*exp(head);
        pointer->finalScore=shape*position*pointer->times;
        //cout<<shape<<" "<<position<<" "<<pointer->finalScore<<endl;
        pointer=pointer->next;
    }
    
    return 0;
}

int Algorithm::getData()
{
    Unit* pointer=&inputData;
    memset(pointer, 0, sizeof(Unit));
    
	/**
	 * 为了和之前的兼容
	 * 实际上只调用else分支的数据处理
	 */
	if (this->listener == nullptr) {

		ifstream myfile("./Samples.txt");
		string temp;
		if (!myfile.is_open())
		{
			cout << "can not open this file" << endl;
		}
		while (getline(myfile, temp))
		{

			for (int i = 0;i < 50;i++)
			{
				if (!getline(myfile, temp, ' '))
				{
					cout << "inputFile has sth wrong!" << endl;
					return -1;
				}
				pointer->pointW[i] = atof(temp.c_str());
				if (!getline(myfile, temp))
				{
					cout << "inputFile has sth wrong!" << endl;
					return -1;
				}
				pointer->pointH[i] = atof(temp.c_str());
				pointer->centralW += pointer->pointW[i];
				pointer->centralH += pointer->pointH[i];

				if (pointer->minH > pointer->pointH[i])
					pointer->minH = pointer->pointH[i];
				if (pointer->maxH < pointer->pointH[i])
					pointer->maxH = pointer->pointH[i];
				if (pointer->minW > pointer->pointW[i])
					pointer->minW = pointer->pointW[i];
				if (pointer->maxW < pointer->pointW[i])
					pointer->maxW = pointer->pointW[i];

			}
			pointer->centralH /= 50;
			pointer->centralW /= 50;

			//cout<<pointer[charFlag]->word<<endl;
			//cout<<pointer[charFlag]->centralH<<" "<<pointer[charFlag]->centralW<<endl;
			//cout<<pointer[charFlag]->minW<<" "<<pointer[charFlag]->maxW<<endl;

			pointer->next = new Unit;
			memset(pointer->next, 0, sizeof(Unit));
			pointer = pointer->next;
			getline(myfile, temp);
		}
		myfile.close();
	}
	else {
		// TODO: 
		vector<Point> points = this->listener->getSamplePoints();
		
		//points: 保证长度为50的vector数组。
		//Point: double x,y; 分别是给出的坐标的第一维和第二维。
		//需要使用这种方式来得到所有的数据。
		for (int i = 0;i < 50;i++)
		{
			pointer->pointW[i] = (float)points[i].x;
			pointer->pointH[i] = (float)points[i].y;
			pointer->centralW += pointer->pointW[i];
			pointer->centralH += pointer->pointH[i];
			if (pointer->minH > pointer->pointH[i])
				pointer->minH = pointer->pointH[i];
			if (pointer->maxH < pointer->pointH[i])
				pointer->maxH = pointer->pointH[i];
			if (pointer->minW > pointer->pointW[i])
				pointer->minW = pointer->pointW[i];
			if (pointer->maxW < pointer->pointW[i])
				pointer->maxW = pointer->pointW[i];
		}
		// 有且仅有一个数据点，根据这个数据点进行后续的计算。
		pointer->centralH /= 50;
		pointer->centralW /= 50;
		pointer->next = new Unit;
		pointer->next->next = NULL;
	}
	return 0;
}



void Algorithm::QuickSortList(Unit* pHead,Unit* pEnd)
{
    if(pHead != pEnd)
    {
        Unit* part = Partition(pHead,pEnd);
        QuickSortList(pHead,part);
        QuickSortList(part->next,pEnd);
        
    }
}

void Algorithm::swapAB(Unit* a,Unit* b)
{
    Unit temp;
    Unit *oldA=a->next;
    Unit *oldB=b->next;
    memcpy(&temp,a,sizeof(Unit));
    memcpy(a,b,sizeof(Unit));
    memcpy(b,&temp,sizeof(Unit));
    a->next=oldA;
    b->next=oldB;
}

Unit* Algorithm::Partition(Unit* pBegin,Unit* pEnd)
{
    
    float key = pBegin->finalScore;
    Unit* pSlow = pBegin;
    Unit* pFast = pSlow->next;
    Unit* temp = pBegin;
    
    while(pFast!=NULL&&pFast!=pEnd->next)
    {
        if(pFast->finalScore>key)
        {
            temp = pSlow;
            pSlow = pSlow->next;
            swapAB(pSlow,pFast);
        }
        
        pFast = pFast->next;
    }
    swapAB(pSlow,pBegin);
    return temp;//返回的结点为支点节点的前一个结点
}

