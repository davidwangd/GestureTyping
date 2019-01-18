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

	//memset(dataBase,0,sizeof(Unit)*30);

	for (int i = 0;i < 26;i++) {
		pointer[i] = &(dataBase[i]);
		pointer[i]->next = NULL;
	}
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

		//printf("flag: %d\n", charFlag);

		//pointer[charFlag] = new Unit;
		pointer[charFlag]->word = temp;
        pointer[charFlag]->centralH=0;
        pointer[charFlag]->centralW=0;
        pointer[charFlag]->minH=400;
        pointer[charFlag]->maxH=0;
        pointer[charFlag]->minW=400;
        pointer[charFlag]->maxW=0;
        //pointer[charFlag]->word[pointer[charFlag]->word.size()-1]='\0';
        //cout<<pointer[charFlag]->word<<endl;
        //cout<<pointer[charFlag]->word.size()<<endl;
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
        pointer[charFlag]=pointer[charFlag]->next;

		pointer[charFlag]->next = NULL;
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
    //printf("[templatePruning]\n");
    int allNUM=0;
    int wordNUM=0;
    Unit *ans=wordListNEW;
    for(int i=0;i<26;i++)
    {
        float dis=computeDis(dataBase[i].pointH[0], cal.pointH[0], dataBase[i].pointW[0], cal.pointW[0]);
        //printf("[templatePruning] %f\n",dis );
        if(dis>shapeThreshold)
            continue;
        Unit *pointer=&dataBase[i];
        while(pointer->next!=NULL)
        {
            wordNUM++;
            dis=computeDis(pointer->pointH[49], cal.pointH[49], pointer->pointW[49], cal.pointW[49]);
            if(dis>shapeThreshold)
            {
                pointer=pointer->next;
                continue;
            }
            allNUM++;
            memcpy(ans, pointer, sizeof(Unit));
            wordEnd=ans;
            mempointer++;
            ans->next=&wordListNEW[mempointer];
            memset(ans->next,0,sizeof(Unit));
            ans=ans->next;
            pointer=pointer->next;
        }
    }
    printf("[templatePruning] num: %d %d\n",allNUM,wordNUM );
    return allNUM;
}

int Algorithm::computeShape()
{
    printf("[computeShape]\n");
    Unit *pointer=wordListNEW;
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
    printf("[computeLocationn]\n");
    Unit *pointer=wordListNEW;
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
    printf("[Integration]\n");
    float head;
    float position,shape;
    Unit *pointer=wordListNEW;
    while(pointer->next!=NULL)
    {
        head=-1.0/2.0*pow(pointer->posScore/positionInt,2);
        position=1.0/(positionInt*sqrt(2*3.1415926))*exp(head);
        head=-1.0/2.0*pow(pointer->shapeScore/shapeInt,2);
        shape=1.0/(shapeInt*sqrt(2*3.1415926))*exp(head);
        pointer->finalScore=shape*position;
        //pointer->finalScore=shape*position*pointer->times;
        //cout<<shape<<" "<<position<<" "<<pointer->finalScore<<endl;
        pointer=pointer->next;
    }
    
	printf("Done int\n");
    return 0;
}

int Algorithm::getData()
{
    Unit* pointer=&cal;
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
            //memset(pointer->next, 0, sizeof(Unit));

            pointer = pointer->next;
			pointer->next = NULL;
            getline(myfile, temp);
        }
        myfile.close();
    }
    else {
        // TODO:
        //cout<<"[getData] leap motion\n";
        vector<Point> points = this->listener->getSamplePoints();
        int length=points.size();
        if(length<50)
            return -1;
        printf("[getData] size ok\n");
        //points: 保证长度为50的vector数组。
        //Point: double x,y; 分别是给出的坐标的第一维和第二维。
        //需要使用这种方式来得到所有的数据。
        for (int i = 0;i < 50;i++)
        {
            pointer->pointW[i]=points[i].x;
            pointer->pointH[i]=points[i].y;
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

//	printf("555555555\n");
    memcpy(&temp,a,sizeof(Unit));
    memcpy(a,b,sizeof(Unit));
    memcpy(b,&temp,sizeof(Unit));
//	printf("6666666\n");
    a->next=oldA;
    b->next=oldB;
//	printf("7777777\n");
}

Unit* Algorithm::Partition(Unit* pBegin,Unit* pEnd)
{
	//printf("%x %x\n", pBegin, pEnd);
    float key = pBegin->finalScore;
    Unit* pSlow = pBegin;
    Unit* pFast = pSlow->next;
    Unit* temp = pBegin;

	//printf("000000000\n");

	/*for (Unit * tmp = pFast; ; tmp = tmp->next) {
		printf("%s\n", tmp->word);
		if (tmp == pEnd->next || tmp == NULL) break;

	}*/
	//printf("#############\n");
    while(pFast!=NULL&&pFast!=pEnd->next)
    {
		//printf("%s\n", pFast->word);
        if(pFast->finalScore>key)
        {
            temp = pSlow;
            pSlow = pSlow->next;
			//printf("!!!!!!\n");
            //swapAB(pSlow,pFast);


			Unit tempp;
			Unit *oldA = pSlow->next;
			Unit *oldB = pFast->next;

			//printf("555555555\n");
			memcpy(&tempp, pSlow, sizeof(Unit));
			memcpy(pSlow, pFast, sizeof(Unit));
			memcpy(pFast, &tempp, sizeof(Unit));
			//printf("6666666\n");
			pSlow->next = oldA;
			pFast->next = oldB;
			//printf("7777777\n");



			//printf("??????\n");
        }
        
        pFast = pFast->next;
    }
	//printf("111111111\n");
    //swapAB(pSlow,pBegin);

	Unit tempp;
	Unit *oldA = pSlow->next;
	Unit *oldB = pFast->next;

	//printf("555555555\n");
	memcpy(&tempp, pSlow, sizeof(Unit));
	memcpy(pSlow, pFast, sizeof(Unit));
	memcpy(pFast, &tempp, sizeof(Unit));
	//printf("6666666\n");
	pSlow->next = oldA;
	pFast->next = oldB;
	//printf("7777777\n");

	//printf("2222222\n");
    return temp;//返回的结点为支点节点的前一个结点
}

void Algorithm::startCompute(){

	//cout<<"start computing"<<endl;
	int temp = this->listener->getState();
	if (temp != -1)
		nowState = temp;
	else
		return;
	// 0 type  1 select  2 waiting
	// 2 2   2 0  0 0   0 1   1 1  1 2
	// unnormal 2 1  0 2  1 0
	//       waiting  select   type waiting  select  type
	// final
	if (preState == -1)
	{
		preState = nowState;
	}
	if (nowState == 2) //waiting
	{
		preState = nowState;
		return;
	}
	if (preState == 1 && nowState == 1)
		return;

	if (getData() == -1)
	{
		cout << "getData return\n";
		preState = nowState;
		return;
	}
	//Unit *ans=&inputData;
	//memcpy(&cal,ans,sizeof(Unit));
	cout << "start computing" << endl;
	mempointer = 0;
	//memset(&wordList, 0, sizeof(Unit));
	normalize(&cal);
	if (templatePruning() == 0)
	{
		preState = nowState;
		return;
	}
	computeShape();
	computeLocation();
	Integration();
	Unit *pointer = wordListNEW;
		//printf("%s\n", wordEnd->word);

	printf("---------------");
	outputNUM = 0;
	/*
	QuickSortList(wordListNEW,wordEnd);
	outputNUM=0;
	cout << "Done quicksort" << endl;
	cout<<"\n\n";
	while(pointer->next!=NULL&&outputNUM<6)
	{
	cout<<"[Computing] word: "<<pointer->word<<endl;
		cout<<"[Computing] score: "<<pointer->finalScore<<endl;
		memset(output[outputNUM],0,50);
		pointer->word.copy(output[outputNUM],pointer->word.size(),0);
		pointer=pointer->next;
		outputNUM++;
		}
		printf("[Computing State] preState %d nowState %d\n",preState,nowState);
		*/
		// 不使用QuickSort
	float score[6];
	for (int i = 0;i < 6;i++) score[i] = -1e8;
	int cnt = 0;
	while (pointer->next != NULL) {
	//	printf("%f , ", pointer->finalScore);
		for (int i = 0; i < 5; i++) {
			if (pointer->finalScore > score[i]) {
				for (int j = 4;j >= i;j--)
					memcpy(output[j + 1], output[j], 50*sizeof(char));
				pointer->word.copy(output[i], pointer->word.size(), 0);
				output[i][pointer->word.size()] = 0;
				if (outputNUM < 5) outputNUM++;
				score[i] = pointer->finalScore;
				break;
			}
		}
		pointer = pointer->next;
		cnt++;
	}

	//printf("\ntotal Words:%d\n ", cnt);

	if (preState != 1 && nowState == 1) {
		this->ui->setwords(output, outputNUM, true);
		for (int i = 0;i < 5;i++) {
			printf("%s %f\n", output[i], score[i]);
		}
	}
	else {
		this->ui->setwords(output, outputNUM, false);
	}
	preState = nowState;
	return;

}