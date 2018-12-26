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
using namespace std;
struct Unit
{
	string word;
	float pointH[60];
	float pointW[60];
	Unit* next;
};

class Algorithm
{
private:
	int length;
	Unit dataBase[30];
	string inputFile;
	int loadData()
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
		    for(int i=0;i<50;i++)
		    {
		    	if(!getline(myfile,temp,' '))
		    	{
		    		cout<<"inputFile has sth wrong!"<<endl;
		    		return -1;
		    	}
		    	pointer[charFlag]->pointH[i]=atof(temp.c_str());
		    	if(!getline(myfile,temp))
		    	{
		    		cout<<"inputFile has sth wrong!"<<endl;
		    		return -1;
		    	}
		    	pointer[charFlag]->pointW[i]=atof(temp.c_str());
                
		    }
			cout<<pointer[charFlag]->word<<endl;
			cout<<pointer[charFlag]->pointH[1]<<" "<<pointer[charFlag]->pointW[1]<<endl;
		    pointer[charFlag]->next=new Unit;
		    pointer[charFlag]=pointer[charFlag]->next;
		    getline(myfile,temp);
		} 
		myfile.close();
		return 0; 
	}
    
    
    
    int getData();
    int normalize();
    int computeShape();
    int computeLocation();
    int Integration();
    
public:
	Algorithm()
	{
		length=50;
		inputFile="./out.txt";
		memset(dataBase,0,sizeof(Unit)*30);
		loadData();
	}
    
    int startCompute()
    {
        while(1)
        {
            getData();
            normalize();
            computeShape();
            computeLocation();
            Integration();
        }
    }

};

#endif /* getWords_hpp */
