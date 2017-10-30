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
using namespace std;
//set <string> DataSet;
//set<string>::iterator DataSet_single;
vector <string> ArticleSet, DataSet;
vector <string>::iterator it, it1;
map <string, double> AllWord;
double TrueNum = 0;

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
		for(j = 0; j < DataSet.size(); j++)
		{
			isOut = 0;
			for(k = 0; k < SingleRow.size(); k++)
			{	
				if(DataSet[j] == SingleRow[k])
				{
					tf << count(SingleRow.begin(), SingleRow.end(), SingleRow[k]) / (double)SingleRow.size() << " ";
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

void TF_IDF()
{
	int i, j, k;
	char SingleWord[150];
	bool isOut;
	set <string> SingleRow; 
	ofstream tf_idf("tfidf.txt");
	set<string>::iterator it;
	map <string, double>::iterator it1;
	vector <string> SingleRow_No_Repeat;
	for(i = 0; i <ArticleSet.size(); i++)
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
			SingleRow.insert(p);	
			p=strtok(NULL,d);
		}
		for(j = 0; j < DataSet.size(); j++)
		{
			for(k = 0; k < SingleRow.size(); k++)
			{
				it = SingleRow.find(DataSet[j]);
				if(it != SingleRow.end())
				{
					it1 = AllWord.find(DataSet[j]);
					if(it1!= AllWord.end())
					{
						AllWord[DataSet[j]] ++;
						break;
					}
					else
					{
						AllWord.insert(pair<string, double>(DataSet[j], 1));	
						break;	
					}
				}
			}
		}
		SingleRow.clear();
	}
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
			SingleRow_No_Repeat.push_back(p);	
			p=strtok(NULL,d);
		}
		for(j = 0; j < DataSet.size(); j++)
		{
			isOut = 0;
			for(k = 0; k < SingleRow_No_Repeat.size(); k++)
			{
				if(DataSet[j] == SingleRow_No_Repeat[k])
				{
					isOut = 1;
					break;
				}
			}
			if(isOut)
			{
				it1 = AllWord.find(SingleRow_No_Repeat[k]);
					tf_idf << log( ArticleSet.size() / (1 + AllWord[SingleRow_No_Repeat[k]])) *  (count(SingleRow_No_Repeat.begin(), SingleRow_No_Repeat.end(), SingleRow_No_Repeat[k]) / (double)SingleRow_No_Repeat.size() )<< " ";
			}
			else
			{
				tf_idf << "0 ";
			}
		}
		tf_idf << "\n";
		SingleRow_No_Repeat.clear();
	}
	tf_idf.flush();
}

void smatrix()
{
	fstream onehot("onehot.txt");
	ofstream smatrix("smatrix.txt");
	string s;
	int i, row = 0;
	smatrix << "[" << ArticleSet.size() << "]\n" << "[" << DataSet.size() << "]\n" << "[" << TrueNum << "]\n";
	while(getline(onehot, s))
	{
		for(i = 0; i < s.length(); i++)
		{
			if(s[i] == '1')
			{
				smatrix << "[" << row << "," << (i+1)/2 << ",1]\n" ;
			}
		}
		row ++;
	}
	smatrix.flush();
}

void ApuluB()
{
	string s1, s2, stmp;
	double rowA = 0, rowB = 0, colA = 0, colB = 0, numA = 0, numB = 0, x, y, v; 
	int i , j, k, tmp, tmp1;
	char ma[100], mb[100];
	map < pair<double, double>, double > mapA;
	map < pair<double, double>, double > ::iterator mapA_it;
	cout << "Input matrix A(txt):";
	cin >> ma;
	fstream matrixA(ma);
	cout << "Input matrix B(txt):";
	cin >> mb;
	fstream matrixB(mb);
	getline(matrixA, s1);
	tmp = 0;
	for(i = 1; i < s1.length(); i++)
	{
		if(s1[i] != ']')
		{
			tmp++;
		 } 
	}
	stmp = s1.substr(1, tmp);
	rowA = atof(stmp.c_str());
	
	getline(matrixA, s1);
	tmp = 0;
	for(i = 1; i < s1.length(); i++)
	{
		if(s1[i] != ']')
		{
			tmp++;
		 } 
	}
	stmp = s1.substr(1, tmp);
	colA = atof(stmp.c_str());	
	
	getline(matrixA, s1);
	tmp = 0;
	for(i = 1; i < s1.length(); i++)
	{
		if(s1[i] != ']')
		{
			tmp++;
		 } 
	}
	stmp = s1.substr(1, tmp);
	numA = atof(stmp.c_str());	
	
	getline(matrixB, s1);
	tmp = 0;
	for(i = 1; i < s1.length(); i++)
	{
		if(s1[i] != ']')
		{
			tmp++;
		 } 
	}
	stmp = s1.substr(1, tmp);
	rowB = atof(stmp.c_str());
	
	getline(matrixB, s1);
	tmp = 0;
	for(i = 1; i < s1.length(); i++)
	{
		if(s1[i] != ']')
		{
			tmp++;
		 } 
	}
	stmp = s1.substr(1, tmp);
	colB = atof(stmp.c_str());
	
	getline(matrixB, s1);
	tmp = 0;
	for(i = 1; i < s1.length(); i++)
	{
		if(s1[i] != ']')
		{
			tmp++;
		 } 
	}
	stmp = s1.substr(1, tmp);
	numB = atof(stmp.c_str());
	//cout << rowA  << " " << colA << " " << numA << "\n" << rowB << " " << colB << " " << numB << endl;
	
	while(getline(matrixA, s1))
	{
		tmp = 0;
		for(i = 1; i < s1.length(); i++)
		{
			if(s1[i] == ',') break; 
			tmp++;
		}
		stmp = s1.substr(1, tmp);
		x = atof(stmp.c_str());
		i++;
		tmp1 = i;
		tmp = 0;
		for(i; i < s1.length(); i++)
		{
			if(s1[i] == ',') break;
			tmp++;
		}
		stmp = s1.substr(tmp1, tmp);
		y = atof(stmp.c_str());

		stmp = s1.substr(i+1, s1.length()-1-i);
		v = atof(stmp.c_str());
		mapA.insert( pair<pair<double, double>, double>(pair<double, double>(x, y) , v) );
	}
	while(getline(matrixB, s1))
	{
		tmp = 0;
		for(i = 1; i < s1.length(); i++)
		{
			if(s1[i] == ',') break; 
			tmp++;
		}
		stmp = s1.substr(1, tmp);
		x = atof(stmp.c_str());
		i++;
		tmp1 = i;
		tmp = 0;
		for(i; i < s1.length(); i++)
		{
			if(s1[i] == ',') break;
			tmp++;
		}
		stmp = s1.substr(tmp1, tmp);
		y = atof(stmp.c_str());

		stmp = s1.substr(i+1, s1.length()-1-i);
		v = atof(stmp.c_str());
		mapA_it = mapA.find(pair<double, double>(x, y));
		if(mapA_it != mapA.end())
		{
			mapA[pair<double, double>(x, y)] += v;
		}
		else
		{
			mapA.insert( pair<pair<double, double>, double>(pair<double, double>(x, y) , v) );
		}
		
	}
	cout << rowA << endl << colA << endl << mapA.size() << endl; 
	for(mapA_it = mapA.begin(); mapA_it!= mapA.end(); mapA_it++)
	{
		cout << (mapA_it->first).first << " " << (mapA_it->first).second << " " << mapA_it->second << endl;
	}
}

int main()
{
	fstream data("text.txt");
	string s;
	int i;
	while(getline(data, s))
	{
		char s1[s.length()+1];
		for(i = 0; i < s.length(); i++)
		{
			s1[i] = s[i];
		}
		s1[i] = '\0';
		const char *d = "\t";  
		const char *d1 =" ";
        char *p, *p1;  
        p = strtok(s1,d);  
// 分割两次以得到原数据中以/t分割开的第三段字符串p 
		p=strtok(NULL,d);
		p=strtok(NULL,d);
		ArticleSet.push_back(p);
		p1 = strtok(p,d1);
		while(p1)
		{
			DataSet.push_back(p1);
			p1 = strtok(NULL,d1);  
		}
		
	}
//不能用set,因为set在调用插入的时候会进行排序（红黑树），得到一个按字母排序的集合，考虑使用vector 
//		for(DataSet_single = DataSet.begin(); DataSet_single != DataSet.end(); ++DataSet_single)
//		{
//			cout << *DataSet_single << " ";
//		}

//消除数据集中的重复元素，所有得到的元素存放在DataSet中，按出现顺序排序 
	for(it=++DataSet.begin(); it != DataSet.end();)
	{
    	it1 = find(DataSet.begin(),it,*it);
    	if(it1 != it)
        	it=DataSet.erase(it);           
    	else  
        	it++;       
	}
	//cout << "Article Size:" << ArticleSet.size() << endl; 
	//cout << "Data Size: " << DataSet.size() << endl;
	One_Hot();
	TF();
	TF_IDF();
	smatrix();
	ApuluB();
	return  0;
 } 
