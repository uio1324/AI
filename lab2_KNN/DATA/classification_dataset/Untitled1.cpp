#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <set>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <stdlib.h>
#include <sstream>
using namespace std;

vector <string> Emotion_train, Emotion_validation, Emotion_check;
vector <double> Emotion_anger, Emotion_disgust, Emotion_fear, Emotion_joy, Emotion_sad, Emotion_surprise;
vector <double> testData;
map < int, double > EuclideanDistance;
vector <string> ArticleSet, DataSet, singleTest, validation;
vector <string>::iterator it, it1;
double TrueNum = 0;

typedef pair<int, double> PAIR;  
  
bool cmp_by_value(const PAIR& lhs, const PAIR& rhs) {  
  return lhs.second < rhs.second;  
}  

void TF()
{
	int i, j, k;
	char SingleWord[150];
	bool isOut;
	vector <string> SingleRow;
	ofstream tf("TF.txt");
	for(i = 0; i < ArticleSet.size(); i++)
	{
		for(j = 0; j < ArticleSet[i].length(); j++)
		{
			SingleWord[j] = ArticleSet[i][j];
		}
		SingleWord[j] = '\0';
		const char *d = " ";
		char *p;
		p = strtok(SingleWord, d);
		while(p)
		{
			SingleRow.push_back(p);	
			p=strtok(NULL,d);
		}
		int numExist = 0;
		for(j = 0; j < SingleRow.size(); j++)
        {
        	if(find(DataSet.begin(), DataSet.end(), SingleRow[j]) == DataSet.end())
        	{
        		numExist ++;
			}
		}
		for(j = 0; j < DataSet.size(); j++)
		{
			isOut = 0;
			for(k = 0; k < SingleRow.size(); k++)
			{	
				if(DataSet[j] == SingleRow[k])
				{
					tf << (double)count(SingleRow.begin(), SingleRow.end(), SingleRow[k]) / ( SingleRow.size()) << " ";
					isOut = 1;
					break;
				}	
			}
			if(!isOut)
			{
				tf << "0 ";
			}
		}
		tf << "\n";
		SingleRow.clear();
	}
	tf.flush();
}

void One_Hot()
{
	int i, j, k;
	char SingleWord[150];
	bool isOut;
	vector <string> SingleRow;
	ofstream onehot("onehot.txt");
	for(i = 0; i < ArticleSet.size(); i++)
	{
		for(j = 0; j < ArticleSet[i].length(); j++)
		{
			SingleWord[j] = ArticleSet[i][j];
		}
		SingleWord[j] = '\0';
		const char *d = " ";
		char *p;
		p = strtok(SingleWord, d);
		while(p)
		{
			SingleRow.push_back(p);	
			p=strtok(NULL,d);
		}
		for(j = 0; j < DataSet.size(); j++)
		{
			isOut = 0;
			for(k = 0; k < SingleRow.size(); k++)
			{	
				if(DataSet[j] == SingleRow[k])
				{
					onehot << "1 ";
					isOut = 1;
					TrueNum++;
					break;
				}	
			}
			if(!isOut)
			{
				onehot << "0 ";
			}
		}
		onehot << "\n";
		SingleRow.clear();
	}
	onehot.flush();
}

void FileCheck_regression()
{
    fstream data("train_set.csv");
	string s;
	int i;
	getline(data,s);
	while(getline(data, s))
	{
		char s1[s.length()+1], s2[s.length()+1];
		for(i = 0; i < s.length(); i++)
		{
			s2[i] = s1[i] = s[i];
		}
		s2[i] = s1[i] = '\0';
		const char *d = ",";  
		const char *d1 =" ";
        char *p, *p1, *p2;  
        p = strtok(s1,d);  
		ArticleSet.push_back(p);
		p1 = strtok(p,d1);
		while(p1)
		{
			DataSet.push_back(p1);
			p1 = strtok(NULL,d1);  
        }
    //?????????????Emotion
        p2 = strtok(s2,d);
        p2 = strtok(NULL,d);
        Emotion_anger.push_back(atof(p2));
        p2 = strtok(NULL,d);
        Emotion_disgust.push_back(atof(p2));
        p2 = strtok(NULL,d);
        Emotion_fear.push_back(atof(p2));
        p2 = strtok(NULL,d);
        Emotion_joy.push_back(atof(p2));
        p2 = strtok(NULL,d);
        Emotion_sad.push_back(atof(p2));
        p2 = strtok(NULL,d);
        Emotion_surprise.push_back(atof(p2));
	}
    //???????????,??????????DataSet?,??????? 
	for(it=++DataSet.begin(); it != DataSet.end();)
	{
    	it1 = find(DataSet.begin(),it,*it);
    	if(it1 != it)
        	it=DataSet.erase(it);           
    	else  
        	it++;       
    }
    One_Hot();
}

void FileCheck_classification()
{
    fstream data("train_set.csv");
	string s;
	int i;
	getline(data,s);
	while(getline(data, s))
	{
		char s1[s.length()+1], s2[s.length()+1];
		for(i = 0; i < s.length(); i++)
		{
			s2[i] = s1[i] = s[i];
		}
		s2[i] = s1[i] = '\0';
		const char *d = ",";  
		const char *d1 =" ";
        char *p, *p1, *p2;  
        p = strtok(s1,d);  
		ArticleSet.push_back(p);
		p1 = strtok(p,d1);
		while(p1)
		{
			DataSet.push_back(p1);
			p1 = strtok(NULL,d1);  
        }
    //?????????????Emotion
        p2 = strtok(s2,d);
        p2 = strtok(NULL,d);
		Emotion_train.push_back(p2);
	}
    //???????????,??????????DataSet?,??????? 
	for(it=++DataSet.begin(); it != DataSet.end();)
	{
    	it1 = find(DataSet.begin(),it,*it);
    	if(it1 != it)
        	it=DataSet.erase(it);           
    	else  
        	it++;       
    }
    One_Hot();
    //TF();
}

string KNN_classification(int k)
{
    int i, j, row = 0;
    double res, d;
    string s, s1;
    vector <double> singleRow;
    //To get OneHot matrix with ArticleSet and DataSet
    fstream onehot("onehot.txt");
    while(getline(onehot, s))
    {
        for(i = 0; i < s.length(); i++)
        {
            if(s[i] == '1')
            {
                singleRow.push_back(1);
            }
            else if(s[i] == '0')
            {
                singleRow.push_back(0);
            }
        }
        d = 0;
        for(i = 0; i < singleRow.size(); i++)
        {
            //????
            d += pow(singleRow[i] - testData[i], 2);
        }
        for(i;i  < testData.size(); i++)
		{
			d += testData[i] * testData[i];
		 } 
        res = (double)sqrt(d);
        EuclideanDistance.insert(pair<int,double>(row, res));
        row++;
        singleRow.clear();
    }

//    fstream tf("TF.txt");
//    double dtemp;
//    while(getline(tf,s))
//    {
//    	stringstream ss;
//    	ss << s;
//    	while(!ss.eof())
//    	{
//    		ss >> dtemp;
//    		singleRow.push_back(dtemp);
//		}
//		singleRow.pop_back();
//		d = 0;
//		for(i = 0; i < singleRow.size(); i++)
//		{
//			d += pow(singleRow[i] - testData[i], 2);
//		}
//		for(i;i  < testData.size(); i++)
//		{
//			d += testData[i] * testData[i];
//		 } 
//		res = (double)sqrt(d);
//		EuclideanDistance.insert(pair<int,double>(row, res));
//    	row++;
//    	singleRow.clear();
//	}
    vector<PAIR> EDistance(EuclideanDistance.begin(), EuclideanDistance.end());
    sort(EDistance.begin(), EDistance.end(), cmp_by_value);
	

    
    int cnt_joy = 0,cnt_sad = 0, cnt_fear = 0, cnt_anger = 0,cnt_surprise = 0, cnt_disgust = 0, cnt_max = 0;
    string predictRes;

    for(i = 0; i < k; i++)
    {
		if(Emotion_train[EDistance[i].first] == "joy") cnt_joy++;
		else if(Emotion_train[EDistance[i].first] == "sad") cnt_sad++;
		else if(Emotion_train[EDistance[i].first] == "fear") cnt_fear++;
		else if(Emotion_train[EDistance[i].first] == "anger") cnt_anger++;
		else if(Emotion_train[EDistance[i].first] == "surprise") cnt_surprise++;
		else if(Emotion_train[EDistance[i].first] == "disgust") cnt_disgust++;
    }
    if(cnt_joy >= cnt_max)
    {
        cnt_max = cnt_joy;
        predictRes = "joy";
    }
    if(cnt_sad > cnt_max)
    {
        cnt_max = cnt_sad;
        predictRes = "sad";
    }
    if(cnt_fear > cnt_max)
    {
        cnt_max = cnt_fear;
        predictRes = "fear";
    }
    if(cnt_anger >= cnt_max)
    {
        cnt_max = cnt_anger;
        predictRes = "anger";
    }
    if(cnt_surprise >= cnt_max)
    {
        cnt_max = cnt_surprise;
        predictRes = "surprise";
    }
    if(cnt_disgust >= cnt_max)
    {
        cnt_max = cnt_disgust;
        predictRes = "disgust";
    }
    EuclideanDistance.clear();
    
//    for(i = 0; i < k; i++)
//	{
//		cout << EDistance[i].second << endl;
//	} 
//	cout << predictRes << endl;
    return predictRes;
}
 
double* KNN_regression(int k)
{
    int i, j, d, row = 0;
    double res, res_final;
    string s, s1;
    vector <double> singleRow;
    //To get OneHot matrix with ArticleSet and DataSet
    fstream onehot("onehot.txt");
    while(getline(onehot, s))
    {
        for(i = 0; i < s.length(); i++)
        {
            if(s[i] == '1')
            {
                singleRow.push_back(1);
            }
            else if(s[i] == '0')
            {
                singleRow.push_back(0);
            }
        }
        d = 0;
        for(i = 0; i < singleRow.size(); i++)
        {
            //????
            d += pow(singleRow[i] - testData[i], 2);
        }
        res = (double)sqrt(d);
        EuclideanDistance.insert(pair<int,double>(row, res));
        row++;
        singleRow.clear();
    }
//    fstream tf("TF.txt");
//    stringstream ss;
//    double dtemp;
//    while(getline(tf,s))
//    {
//    	ss << s;
//    	while(!ss.eof())
//    	{
//    		ss >> dtemp;
//    		singleRow.push_back(dtemp);
//		}
//		d = 0;
//		for(i = 0; i < singleRow.size(); i++)
//		{
//			d += pow(singleRow[i] - testData[i], 2);
//		}
//		res = (double)sqrt(d);
//		EuclideanDistance.insert(pair<int,double>(row, res));
//    	row++;
//    	singleRow.clear();
//	}
    
    //sort(EuclideanDistance.begin(), EuclideanDistance.end());
    vector<PAIR> EDistance(EuclideanDistance.begin(), EuclideanDistance.end());
    sort(EDistance.begin(), EDistance.end(), cmp_by_value); 
    
    double cnt_joy = 0, cnt_sad = 0, cnt_fear = 0, cnt_anger = 0,cnt_surprise = 0, cnt_disgust = 0, cnt_max = 0;
    res_final = 0;
    for(i = 0; i < k; i++)
    {
        cnt_anger +=  (Emotion_anger[EDistance[i].first] / (EDistance[i].second + 1) );
        cnt_disgust +=  (Emotion_disgust[EDistance[i].first] / (EDistance[i].second + 1) );  
        cnt_fear +=  (Emotion_fear[EDistance[i].first] / (EDistance[i].second + 1)); 
        cnt_joy +=  (Emotion_joy[EDistance[i].first] / (EDistance[i].second + 1)); 
        cnt_sad +=  (Emotion_sad[EDistance[i].first] / (EDistance[i].second + 1)); 
        cnt_surprise +=  (Emotion_surprise[EDistance[i].first] / (EDistance[i].second + 1)); 
    }
    res_final = cnt_anger + cnt_disgust + cnt_fear + cnt_joy + cnt_sad + cnt_surprise;
    cnt_anger = cnt_anger / res_final;
    cnt_disgust = cnt_disgust / res_final;
    cnt_fear = cnt_fear / res_final;
    cnt_joy = cnt_joy / res_final;
    cnt_sad = cnt_sad / res_final;
    cnt_surprise = cnt_surprise / res_final;
    
    
    double* temp = new double[6];
    temp[0] = cnt_anger;
    temp[1] = cnt_disgust;
    temp[2] = cnt_fear;
    temp[3] = cnt_joy;
    temp[4] = cnt_sad;
    temp[5] = cnt_surprise;

    EuclideanDistance.clear();
    return temp;
}

void Input_test_classification()
{
    fstream test_set("test_set.csv");
	ofstream test("15352048_ChenXiao_KNN_classification.csv");
	test << "textid,label\n";
    string s;
    int i, j, row =1;
    bool isOut;
    getline(test_set,s);
    while(getline(test_set, s))
    {
        char s1[s.length()+1];
		for(i = 0; i < s.length(); i++)
		{
			s1[i] = s[i];
		}
        s1[i] = '\0';
        const char *d = ",";  
		const char *d1 =" ";
        char *p, *p1;  
        p = strtok(s1,d);  
        p = strtok(NULL,d);
        p1 = strtok(p,d1);
        while(p1)
		{
			singleTest.push_back(p1);
			p1 = strtok(NULL,d1);  
        }
        int numExist = 0;
		for(i = 0; i < singleTest.size(); i++)
        {
        	if(find(DataSet.begin(), DataSet.end(), singleTest[i]) == DataSet.end())
        	{
        		numExist ++;
        		DataSet.push_back(singleTest[i]);
        	}
		}

        for(i = 0; i < DataSet.size(); i++)
        {
            isOut = 0;
            for(j = 0; j < singleTest.size(); j++)
            {
                if(DataSet[i] == singleTest[j])
                {	
                    //testData.push_back((double)count(singleTest.begin(), singleTest.end(), singleTest[j]) / (singleTest.size()));
                    //testData.push_back((double)count(singleTest.begin(), singleTest.end(), singleTest[j]) / (singleTest.size()));
                    testData.push_back(1); 
                    isOut = 1;
                    break;
                }
            }
            if(!isOut)
            {
                testData.push_back(0);
            }
        }
    //??,????????????????????OneHot????testData(??0?1??????)
        string res;
    //??k??
        res = KNN_classification(14);
    //    cout << res << endl;
    	test << row << ",";
    	row++;
    	test << res << "\n";
        singleTest.clear();
        testData.clear();
    }
    test.close();
}

void Input_test_regression()
{
    fstream test_set("test_set.csv");
	ofstream test("test.txt");
    string s;
    int i, j;
    bool isOut;
    getline(test_set,s);
    while(getline(test_set, s))
    {
        char s1[s.length()+1];
		for(i = 0; i < s.length(); i++)
		{
			s1[i] = s[i];
		}
        s1[i] = '\0';
        const char *d = ",";  
		const char *d1 =" ";
        char *p, *p1;  
        p = strtok(s1,d);  
        p = strtok(NULL,d);
        p1 = strtok(p,d1);
        while(p1)
		{
			singleTest.push_back(p1);
			p1 = strtok(NULL,d1);  
        }
        int numExist = 0;
        for(i = 0; i < singleTest.size(); i++)
        {
        	if(find(DataSet.begin(), DataSet.end(), singleTest[i]) == DataSet.end())
        	{
        		numExist ++;
			}
		}
        for(i = 0; i < DataSet.size(); i++)
        {
            isOut = 0;
            for(j = 0; j < singleTest.size(); j++)
            {
                if(DataSet[i] == singleTest[j])
                {	
                    testData.push_back(count(singleTest.begin(), singleTest.end(), singleTest[j]) / singleTest.size() );
                    isOut = 1;
                    break;
                }
            }
            if(!isOut)
            {
                testData.push_back(0);
            }
        }
    //??,????????????????????OneHot????testData(??0?1??????)
        double *temp;
        temp = KNN_regression(22);
        for(i = 0; i < 5; i++)
        {
            test << temp[i] << ","; 
        }
        test << temp[5] << "\n";	
        singleTest.clear();
        testData.clear();
        delete temp;
    }
    test.close();
}

void Input_validation_classification()
{
    fstream test_set("validation_set.csv");
    string s;
    int i, j;
    bool isOut;
    getline(test_set,s);
    while(getline(test_set, s))
    {
        char s1[s.length()+1], s2[s.length()+1];
		for(i = 0; i < s.length(); i++)
		{
			s2[i] = s1[i] = s[i];
		}
        s2[i] = s1[i] = '\0';
        const char *d = ",";  
		const char *d1 =" ";
        char *p, *p1, *p2;  
        p = strtok(s1,d);  
        p1 = strtok(p,d1);
        while(p1)
		{
			singleTest.push_back(p1);
			p1 = strtok(NULL,d1);  
        }
        p2 = strtok(s2,d);
        p2 = strtok(NULL,d);
		Emotion_validation.push_back(p2);
        for(i = 0; i < DataSet.size(); i++)
        {
            isOut = 0;
            for(j = 0; j < singleTest.size(); j++)
            {
                if(DataSet[i] == singleTest[j])
                {
                    testData.push_back(1);
                    isOut = 1;
                    break;
                }
            }
            if(!isOut)
            {
                testData.push_back(0);
            }
        }
    //??,????????????????????OneHot????testData(??0?1??????)
        string res;
    //??k??
        res = KNN_classification(14);
    //    cout << res << endl;
    	Emotion_check.push_back(res);
        singleTest.clear();
        testData.clear();
    }
    int right_num = 0; 
    for(i = 0; i < Emotion_check.size(); i++)
    {
    	if(Emotion_check[i] == Emotion_validation[i])
    	{
    		cout << "id: " << i << " RIGHT for " << Emotion_validation[i] << " / " << Emotion_check[i] << endl;
    		right_num++;
    		cout << right_num << "/"  << Emotion_check.size() << endl;
		}
		else
		{
    		cout << "id: " << i << " WRONG for " << Emotion_validation[i] << " / " << Emotion_check[i] << endl;
		}
	}
	cout << "Final num: " << (double)right_num / Emotion_check.size() << endl;
}

void Input_validation_regression()
{
    fstream test_set("validation_set.csv");
    ofstream test_regression("test_regression.txt");
    string s;
    int i, j;
    bool isOut;
    getline(test_set,s);
    while(getline(test_set, s))
    {
        char s1[s.length()+1], s2[s.length()+1];
		for(i = 0; i < s.length(); i++)
		{
			s2[i] = s1[i] = s[i];
		}
        s2[i] = s1[i] = '\0';
        const char *d = ",";  
		const char *d1 =" ";
        char *p, *p1, *p2;  
        p = strtok(s1,d);  
        p1 = strtok(p,d1);
        while(p1)
		{
			singleTest.push_back(p1);
			p1 = strtok(NULL,d1);  
        }

        
        for(i = 0; i < DataSet.size(); i++)
        {
            isOut = 0;
            for(j = 0; j < singleTest.size(); j++)
            {
                if(DataSet[i] == singleTest[j])
                {
                    testData.push_back(1);
                    isOut = 1;
                    break;
                }
            }
            if(!isOut)
            {
                testData.push_back(0);
            }
        }
        double *temp;
        temp = KNN_regression(22);
        for(i = 0; i < 5; i++)
        {
            test_regression << temp[i] << "\t"; 
        }
        test_regression << temp[5] << "\n";
        singleTest.clear();
        testData.clear();
        delete temp;
    }
    
}


int main()
{
    FileCheck_classification();
    //FileCheck_regression();
    Input_test_classification();
    //Input_test_regression();
    //Input_validation_classification();
    //Input_validation_regression();
    return 0;
}

