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



typedef struct TrainSet {
	vector <double> num;
	double label;
	double label_forVal;
	int id;
};

vector <TrainSet> trainSet, valSet, testSet;

double checktrainSet(vector <double> w);

void FileCheck(int modeChoose)
{
	fstream data("train.csv");
	string s;
	int i;
	//getline(data,s);
	while (getline(data, s))
	{
		vector <double> nums;
		nums.push_back(1);//ÿһ������ǰ����+1
		TrainSet singleTrain;
		char *str = (char *)s.c_str();//string --> char
		const char *split = ",";
		char *p = strtok(str, split);//���ŷָ�����ȡ��
		double a;
		while (p != NULL)
		{
			sscanf(p, "%lf", &a);//char ---> int
			nums.push_back(a);
			p = strtok(NULL, split);
		}
		for (i = 0; i < nums.size() - 1; i++)
		{
			singleTrain.num.push_back(nums[i]);
		}
		singleTrain.label = nums[nums.size() - 1];
		trainSet.push_back(singleTrain);
		//���˽�������ѵ������x����
	}
	//���˽�������ѵ�����ݴ�������trainSet
	//val���
	if (modeChoose == 0)
	{
		fstream val("val.csv");
		while (getline(val, s))
		{
			vector <double> nums;
			nums.push_back(1);//ÿһ������ǰ����+1
			TrainSet singleTrain;
			char *str = (char *)s.c_str();//string --> char
			const char *split = ",";
			char *p = strtok(str, split);//���ŷָ�����ȡ��
			double a;
			while (p != NULL)
			{
				sscanf(p, "%lf", &a);//char ---> int
				nums.push_back(a);
				p = strtok(NULL, split);
			}
			for (i = 0; i < nums.size() - 1; i++)
			{
				singleTrain.num.push_back(nums[i]);
			}
			singleTrain.label_forVal = nums[nums.size() - 1];
			valSet.push_back(singleTrain);
			//���˽�������ѵ������x����
		}
	}
	else if (modeChoose == 1)
	{
		fstream t("test.csv");
		while (getline(t, s))
		{
			vector <double> nums;
			nums.push_back(1);//ÿһ������ǰ����+1
			TrainSet singleTrain;
			char *str = (char *)s.c_str();//string --> char
			const char *split = ",";
			char *p = strtok(str, split);//���ŷָ�����ȡ��
			double a;
			while (p != NULL)
			{
				sscanf(p, "%lf", &a);//char ---> int
				nums.push_back(a);
				p = strtok(NULL, split);
			}
			nums.pop_back();
			for (i = 0; i < nums.size(); i++)
			{
				singleTrain.num.push_back(nums[i]);
			}
			//singleTrain.label = nums[nums.size()-1];
			testSet.push_back(singleTrain);
			//���˽�������ѵ������x����
		}
	}
}

int sign(double x) {
	return x > 0 ? 1 : -1;
}

void PLA(vector <TrainSet> v)
{
	//��ʼ��Ȩ������w0 = 0
	vector <double> w0;
	int i, i0, j;
	double signW;
	for (i = 0; i < trainSet[0].num.size();i++)
	{
		//ȫ����ʼ��wΪ1
		w0.push_back(1);
	}
	//���������������
	for (i0 = 0; i0 < 10; i0++)
	{
		//��������ѵ������
		for (i = 0; i < trainSet.size(); i++)
		{
			signW = 0;
			for (j = 0; j < trainSet[i].num.size(); j++)
			{
				signW += trainSet[i].num[j] * w0[j];
			}
			//��Ԥ�ⲻ��ȷ�������w��ֵ
			if (sign(signW) != trainSet[i].label)
			{
				for (j = 0; j < w0.size(); j++)
				{
					w0[j] = w0[j] + trainSet[i].label * trainSet[i].num[j];
				}
			}
		}
	}
	//����Ԥ�⣬�õ����յ�wֵ
	//ѭ���������е�label�ͱ�׼ֵ�Ĳ�ࡣ
	for (i = 0; i < valSet.size(); i++)
	{
		signW = 0;
		for (j = 0; j < w0.size(); j++)
		{
			signW += valSet[i].num[j] * w0[j];
		}
		valSet[i].label = sign(signW);
	}
}

void PLA_Pocket(vector <TrainSet> v)
{
	//��ʼ��Ȩ������w0 = 0
	vector <double> w0;
	vector <double> w1;
	int i, i0, j;
	double signW;
	for (i = 0; i < trainSet[0].num.size(); i++)
	{
		//ȫ����ʼ��wΪ1
		w0.push_back(1);
		w1.push_back(1);
	}
	//���������������
	for (i0 = 0; i0 < 50; i0++)
	{
		//��������ѵ������
		for (i = 0; i < trainSet.size(); i++)
		{
			signW = 0;
			for (j = 0; j < trainSet[i].num.size(); j++)
			{
				signW += trainSet[i].num[j] * w0[j];
			}
			//��Ԥ�ⲻ��ȷ�������w��ֵ
			if (sign(signW) != trainSet[i].label)
			{
				for (j = 0; j < w0.size(); j++)
				{
					w0[j] = w0[j] + trainSet[i].label * trainSet[i].num[j];
				}
				//�����Ѿ�����w��ֵ����Ҫ�ж�w�Ĵ������Ƿ�С��ȫ������
				double c1 = checktrainSet(w0), c2 = checktrainSet(w1);
				if (c1 > c2)
				{
					for (j = 0; j < w1.size(); j++)
					{
						w1[j] = w0[j];
					}
				}
			}
		}
	}
	//����Ԥ�⣬�õ����յ�wֵ
	//ѭ���������е�label�ͱ�׼ֵ�Ĳ�ࡣ
	//���Ǽ�����֤����
	for (i = 0; i < valSet.size(); i++)
	{
		signW = 0;
		for (j = 0; j < w1.size(); j++)
		{
			signW += valSet[i].num[j] * w1[j];
		}
		valSet[i].label = sign(signW);
	}
}

double checktrainSet(vector <double> w)
{
	double count = 0;
	for (int i = 0; i < trainSet.size(); i++)
	{
		double signW = 0;
		for (int j = 0; j < w.size(); j++)
		{
			signW += trainSet[i].num[j] * w[j];
		}
		//valSet[i].label = sign(signW);
		if (sign(signW) == trainSet[i].label)
		{
			count++;
		}
	}
	return (count / (double)trainSet.size());
}

void ShowAccuracy(vector <TrainSet> v)
{
	double count = 0, TP = 0, FN = 0, TN = 0, FP = 0;
	
	for (int i = 0; i < v.size(); i++)
	{
		/*if (v[i].label == v[i].label_forVal)
		{
			count++;
		}*/
		if (v[i].label_forVal == 1 && v[i].label == 1) TP++;
		else if (v[i].label_forVal == 1 && v[i].label == -1) FN++;
		else if (v[i].label_forVal == -1 && v[i].label == -1) TN++;
		else if (v[i].label_forVal == -1 && v[i].label == 1) FP++;
	}
	//cout << (count /(double)v.size()) << endl;
	cout << "Accuracy: " << (TP + TN) / (TP + FN + TN + FP )<< endl;
	cout << "Precision: " << (TP ) / (TP + FP) << endl;
	double Precision = (TP) / (TP + FP);
	cout << "Recall: " << (TP) / (TP + FN) << endl;
	double Recall = (TP) / (TP + FN);
	cout << "F1: " << 2*(Precision * Recall) / (Precision + Recall) << endl;
}

void OutputTest()
{
	ofstream testResult("15352048_chenxiao_PLA.csv");
	//��ʼ��Ȩ������w0 = 0
	vector <double> w0;
	vector <double> w1;
	int i, i0, j;
	double signW;
	for (i = 0; i < trainSet[0].num.size(); i++)
	{
		//ȫ����ʼ��wΪ1
		w0.push_back(1);
		w1.push_back(1);
	}
	//���������������
	for (i0 = 0; i0 < 50; i0++)
	{
		//��������ѵ������
		for (i = 0; i < trainSet.size(); i++)
		{
			signW = 0;
			for (j = 0; j < trainSet[i].num.size(); j++)
			{
				signW += trainSet[i].num[j] * w0[j];
			}
			//��Ԥ�ⲻ��ȷ�������w��ֵ
			if (sign(signW) != trainSet[i].label)
			{
				for (j = 0; j < w0.size(); j++)
				{
					w0[j] = w0[j] + trainSet[i].label * trainSet[i].num[j];
				}
				//�����Ѿ�����w��ֵ����Ҫ�ж�w�Ĵ������Ƿ�С��ȫ������
				double c1 = checktrainSet(w0), c2 = checktrainSet(w1);
				if (c1 > c2)
				{
					for (j = 0; j < w1.size(); j++)
					{
						w1[j] = w0[j];
					}
				}
			}
		}
	}
	//����Ԥ�⣬�õ����յ�wֵ
	//ѭ���������е�label�ͱ�׼ֵ�Ĳ�ࡣ
	for (i = 0; i < testSet.size(); i++)
	{
		signW = 0;
		for (j = 0; j < w1.size(); j++)
		{
			signW += testSet[i].num[j] * w1[j];
		}
		testSet[i].label = sign(signW);
		testResult << testSet[i].label << endl;
	}
}

int main()
{
	FileCheck(0);
	//PLA(valSet);
	PLA_Pocket(valSet);
	ShowAccuracy(valSet);
	//FileCheck(1);
	//OutputTest();
	system("pause");
	return 0;
}
