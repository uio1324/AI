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
vector <double> w;


void FileCheck(int modeChoose)
{
	fstream data("train.csv");
	string s;
	int i;
	//getline(data,s);
	while (getline(data, s))
	{
		vector <double> nums;
		nums.push_back(1);//每一个样本前加上+1
		TrainSet singleTrain;
		char *str = (char *)s.c_str();//string --> char
		const char *split = ",";
		char *p = strtok(str, split);//逗号分隔依次取出
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
		//至此结束单个训练数据x处理
	}
	//至此结束所有训练数据处理，存入trainSet
	//val情况
	if (modeChoose == 0)
	{
		fstream val("val.csv");
		while (getline(val, s))
		{
			vector <double> nums;
			nums.push_back(1);//每一个样本前加上+1
			TrainSet singleTrain;
			char *str = (char *)s.c_str();//string --> char
			const char *split = ",";
			char *p = strtok(str, split);//逗号分隔依次取出
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
			//至此结束单个训练数据x处理
		}
	}
	else if (modeChoose == 1)
	{
		fstream t("test.csv");
		while (getline(t, s))
		{
			vector <double> nums;
			nums.push_back(1);//每一个样本前加上+1
			TrainSet singleTrain;
			char *str = (char *)s.c_str();//string --> char
			const char *split = ",";
			char *p = strtok(str, split);//逗号分隔依次取出
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
			//至此结束单个训练数据x处理
		}
	}
}

int sign(double x) {
	return x > 0.5 ? 1 : 0;
}



void LR(vector <TrainSet> v)
{
	//初始化权重向量w0 = 0
	vector <double> w0;
	vector <double> cost;
	vector <double> w_tmp;
	int i, i0, j;
	double s, c;
	for (i = 0; i < trainSet[0].num.size(); i++)
	{
		//全部初始化w为1
		w.push_back(1);
		w0.push_back(1);
	}
	//设置总体迭代次数i0，学习步长n
	double n = 0.7;
	for (i0 = 0; i0 < 1000; i0++)
	{
		//遍历所有训练数据,计算每个样例的权重分数，存入w_tmp
		w_tmp.clear();
		for (i = 0; i < trainSet.size(); i++)
		{
			s = 0;
			for (j = 0; j < trainSet[i].num.size(); j++)
			{
				s += w0[j] * trainSet[i].num[j];//计算每个样例的权重分数
			}
			w_tmp.push_back(s);
		}
		//得到所有样例的权重分数
		//每一维的梯度计算
		cost.clear();
		for (i = 0; i < trainSet[0].num.size(); i++)
		{
			c = 0;
			for (j = 0; j < trainSet.size(); j++)
			{
				c += (1 / (1 + exp(-1.0*w_tmp[j])) - trainSet[j].label) * trainSet[j].num[i];//每一维的梯度计算
			}
			cost.push_back(c);
			//cout << "ΔCost " << i << ": " << c << endl;
		}
		//更新每一维的权重
		for (i = 0; i < w0.size(); i++)
		{
			w[i] = w0[i] -= (pow(n, i0) + 0.0001) * cost[i];
			//cout << "w" << i << ": " << w0[i] << endl;
		}
	}
	//结束预测，得到最终的w值
	//循环计算所有的label和标准值的差距。
	//这是计算验证集的
	for (i = 0; i < valSet.size(); i++)
	{
		double zhishu = 0, p = 0;
		for (j = 0; j < w0.size(); j++)
		{
			zhishu += valSet[i].num[j] * w0[j];
		}
		p = (1 / (1 + exp(-1.0* zhishu)));
		valSet[i].label = sign(p);
	}
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
		else if (v[i].label_forVal == 1 && v[i].label == 0) FN++;
		else if (v[i].label_forVal == 0 && v[i].label == 0) TN++;
		else if (v[i].label_forVal == 0 && v[i].label == 1) FP++;
	}
	//cout << (count /(double)v.size()) << endl;
	cout << "Accuracy: " << (TP + TN) / (TP + FN + TN + FP) << endl;
	cout << "Precision: " << (TP) / (TP + FP) << endl;
	double Precision = (TP) / (TP + FP);
	cout << "Recall: " << (TP) / (TP + FN) << endl;
	double Recall = (TP) / (TP + FN);
	cout << "F1: " << 2 * (Precision * Recall) / (Precision + Recall) << endl;
}

void OutputTest()
{
	ofstream testResult("15352048_chenxiao.txt");
	//循环计算所有的label和标准值的差距。
	int i, j;
	for (i = 0; i < testSet.size(); i++)
	{
		double zhishu = 0, p = 0;
		for (j = 0; j < w.size(); j++)
		{
			zhishu += testSet[i].num[j] * w[j];
		}
		p = (1 / (1 + exp(-1.0* zhishu)));
		testSet[i].label = sign(p);
		testResult << testSet[i].label << endl;
	}
}

int main()
{
	FileCheck(0);
	LR(trainSet);
	ShowAccuracy(valSet);
	//FileCheck(1);
	//OutputTest();
	system("pause");
	return 0;
}
