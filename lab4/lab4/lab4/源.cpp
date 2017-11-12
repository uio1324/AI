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
	vector <double> data;
	double label;
};

vector <TrainSet> trainSet, testSet, valSet;

struct node {
	double attribute = 0;//����ֵ  
	double arrived_value = 0;//���������ֵ
	vector <double> choice;
	bool isLeaf = 0;
	vector<node *> children;//���еĺ���  
};

node * final_root;//���ڵ�

void FileCheck(int modeChoose)
{
	fstream data("train.csv");
	string s;
	int i;
	while (getline(data, s))
	{
		vector <double> nums;
		TrainSet singleTrain;
		char *str = (char *)s.c_str();//string --> char
		const char *split = ",";
		char *p = strtok(str, split);//���ŷָ�����ȡ��
		double a;
		while (p != NULL)
		{
			sscanf(p, "%lf", &a);//char ---> double
			nums.push_back(a);
			p = strtok(NULL, split);
		}
		for (i = 0; i < nums.size() - 1; i++)
		{
			singleTrain.data.push_back(nums[i]);
		}
		singleTrain.label = nums[nums.size() - 1];
		trainSet.push_back(singleTrain);
	}
	//���˽�������ѵ�����ݴ�������trainSet

	if (modeChoose == 0)
	{
		fstream val("val.csv");
		while (getline(val, s))
		{
			vector <double> nums;
			TrainSet singleTrain;
			char *str = (char *)s.c_str();//string --> char
			const char *split = ",";
			char *p = strtok(str, split);//���ŷָ�����ȡ��
			double a;
			while (p != NULL)
			{
				sscanf(p, "%lf", &a);//char ---> double
				nums.push_back(a);
				p = strtok(NULL, split);
			}
			for (i = 0; i < nums.size() - 1; i++)
			{
				singleTrain.data.push_back(nums[i]);
			}
			singleTrain.label = nums[nums.size() - 1];
			valSet.push_back(singleTrain);
			//���˽�������ѵ������x����
		}
	}


}

//�����з������ݼ�������ѡ�������ֵa
vector <TrainSet> devide_data(vector <TrainSet> D, double a)
{
	vector <TrainSet> temp;
	set <double> attr;
	for(int i = 0; i < D.size(); i++)
	{
		attr.insert(D[i].data[a]);
	}
	for (set <double>::iterator it = attr.begin(); it != attr.end(); it++)
	{
		for (int j = 0; j < D.size(); j++)
		{
			if (*it == D[j].data[a])
			{
				temp.push_back(D[j]);
			}
		}
		//�õ���ǰ�����µ�һ�������ݼ�temp

	}
	return temp;
}

double get_most_value(vector <TrainSet> data)
{
	double count1 = 0, count2 = 0;
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i].label == 1) count1++;
		else count2++;
	}
	return (count1 > count2 ? 1 : -1);
}

double ID3(vector <TrainSet> data, vector <double> usedAttr)
{
	int i, i0, j, k; double tmp = 0, H_tmp = 0, min_result = 1, final_result = 0;
	for (i = 0; i < data.size(); i++)
		if (data[i].label == 1) tmp++;
	//���㾭����
	double HD = -1.0*tmp / data.size()*log2(tmp / data.size()) - (data.size() - tmp) / data.size()*log2((data.size() - tmp) / data.size());
	map <double, double> num_attr, num_label;
	vector <double> result;
	for (i = 0; i < data[0].data.size(); i++)//����ÿһά��
	{
		//������Ҫ������ά��
		if (find(usedAttr.begin(), usedAttr.end(), i) != usedAttr.end())
		{
			result.push_back(-1);//������䱻������ά��
			continue;
			
		}
		num_attr.clear();
		for (j = 0; j < data.size(); j++)
		{
			num_attr[data[j].data[i]]++;
		}
		H_tmp = 0;
		for (map <double, double>::iterator it = num_attr.begin(); it != num_attr.end(); it++)
		{
			num_label.clear();
			for (j = 0; j < data.size(); j++)
			{
				if (it->first == data[j].data[i])
				{
					num_label[data[j].label]++;
				}
			}
			for (map <double, double>::iterator it1 = num_label.begin(); it1 != num_label.end(); it1++)
			{
				H_tmp += (it->second / data.size())*(-1.0*it1->second / it->second) * log2((1.0*it1->second / it->second));
			}

		}
		//�õ�һ����ά�ȵ�H_tmp����H(|)
		result.push_back(H_tmp);
	}
	//�õ�����ά�ȵ�H�����������غ���Ϣ����
	for (i = 0; i < result.size(); i++)
	{
		//cout << i << " " << result[i] << endl;
		if (result[i] < min_result && result[i] > 0)
		{
			min_result = result[i];
			final_result = i;
		}
	}
	return final_result;

}

vector <double> Attr;

node * create_tree(vector <TrainSet> data, vector <double> usedAttr) 
{
	node * root = new node;
	bool isAllSame = 1;
	int i, j, k;
	//�߽�����1���жϵ�ǰ���ݼ��ڵı�ǩ�Ƿ�ȫΪһ�µ�
	for (i = 1; i < data.size(); i++)
	{
		if (data[i].label != data[i - 1].label) isAllSame = 0;
	}
	if (isAllSame)
	{
		node * temp_node = new node;
		temp_node->arrived_value = data[0].label;
		temp_node->isLeaf = 1;//��Ҷ�ӽڵ���
		return temp_node;
	}

	//�߽�����2�����е�����attr���ù������ر�ǩ������
	if(usedAttr.size() == data[0].data.size())
	{
		node * temp_node = new node;
		temp_node->arrived_value = get_most_value(data);
		temp_node->isLeaf = 1;//��Ҷ�ӽڵ���
		return temp_node;
	}

	double a = ID3(data, usedAttr);
	usedAttr.push_back(a);
	root->attribute = a;
	set <double> tempset;
	//��ʼ�ݹ飬��ÿһ�����������½�һ����
	for (i = 0; i < data.size(); i++)
	{
		//���뵽����set��
		tempset.insert(data[i].data[a]);
	}
	for (set <double>::iterator it = tempset.begin(); it != tempset.end(); it++)
	{
		vector <TrainSet> tempTrainSet;
		for (j = 0; j < data.size(); j++)
		{
			if (data[j].data[a] == *it)
			{
				tempTrainSet.push_back(data[j]);
			}
		}
		//��õ�ǰ����a�µ�ĳһ�������ݼ�
		if (tempTrainSet.empty())
		{
			node * temp_node = new node;
			temp_node->arrived_value = get_most_value(data);
			temp_node->isLeaf = 1;//��Ҷ�ӽڵ���
			root->children.push_back(temp_node);
		}
		else
		{
			root->choice.push_back(*it);
			root->arrived_value = get_most_value(tempTrainSet);
			root->children.push_back(create_tree(tempTrainSet, usedAttr));
		}
	}
	return root;
}

double getLabel(TrainSet T, node* root)//ȷ���������Լ�/��֤�������ձ�ǩ
{
	node * root_for_travel = root;
	int i, j;
	double temp;
	bool flag;
	while (!root_for_travel->isLeaf)//û�дﵽҶ�ӽڵ�ǰ��Ҫһֱ���б���
	{
		temp = T.data[root_for_travel->attribute];//��ǰѡ��������µ�ֵ
		flag = 1;
		for (i = 0; i < root_for_travel->choice.size(); i++)
		{
			if (temp == root_for_travel->choice[i])
			{
				flag = 0;
				break;
			}
		}//i = ��Ҫǰ�������Ե����
		if(!flag) root_for_travel = root_for_travel->children[i];
		else
		{
			return root_for_travel->arrived_value;
			break;
		}
	}
	//����Ҷ�ӽڵ㣬���ؽ��
	return root_for_travel->arrived_value;

}

void ShowAccuracy()
{
	double count = 0;
	for (int i = 0; i < valSet.size(); i++)
	{
		if (getLabel(valSet[i], final_root) == valSet[i].label)
		{
			count++;
		}
	}
	cout << count / (double)valSet.size() << endl;
}



int main()
{
	FileCheck(0);
	//cout << "my label choose: " << ID3() << endl;
	final_root = create_tree(trainSet, Attr);
	ShowAccuracy();
	system("pause");
	return 0;
}
