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
	double attribute = 0;//属性值  
	double arrived_value = 0;//到达的属性值
	vector <double> choice;
	bool isLeaf = 0;
	vector<node *> children;//所有的孩子  
};

node * final_root;//根节点

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
		char *p = strtok(str, split);//逗号分隔依次取出
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
	//至此结束所有训练数据处理，存入trainSet

	if (modeChoose == 0)
	{
		fstream val("val.csv");
		while (getline(val, s))
		{
			vector <double> nums;
			TrainSet singleTrain;
			char *str = (char *)s.c_str();//string --> char
			const char *split = ",";
			char *p = strtok(str, split);//逗号分隔依次取出
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
			//至此结束单个训练数据x处理
		}
	}


}

//用于切分子数据集，根据选择的属性值a
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
		//得到当前特征下的一个子数据集temp

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
	//计算经验熵
	double HD = -1.0*tmp / data.size()*log2(tmp / data.size()) - (data.size() - tmp) / data.size()*log2((data.size() - tmp) / data.size());
	map <double, double> num_attr, num_label;
	vector <double> result;
	for (i = 0; i < data[0].data.size(); i++)//对于每一维度
	{
		//跳过需要跳过的维度
		if (find(usedAttr.begin(), usedAttr.end(), i) != usedAttr.end())
		{
			result.push_back(-1);//用于填充被跳过的维度
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
		//得到一整个维度的H_tmp，即H(|)
		result.push_back(H_tmp);
	}
	//得到所有维度的H，继续计算熵和信息增益
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
	//边界条件1：判断当前数据集内的标签是否全为一致的
	for (i = 1; i < data.size(); i++)
	{
		if (data[i].label != data[i - 1].label) isAllSame = 0;
	}
	if (isAllSame)
	{
		node * temp_node = new node;
		temp_node->arrived_value = data[0].label;
		temp_node->isLeaf = 1;//是叶子节点了
		return temp_node;
	}

	//边界条件2：所有的属性attr都用过，返回标签的众数
	if(usedAttr.size() == data[0].data.size())
	{
		node * temp_node = new node;
		temp_node->arrived_value = get_most_value(data);
		temp_node->isLeaf = 1;//是叶子节点了
		return temp_node;
	}

	double a = ID3(data, usedAttr);
	usedAttr.push_back(a);
	root->attribute = a;
	set <double> tempset;
	//开始递归，在每一个子属性下新建一棵树
	for (i = 0; i < data.size(); i++)
	{
		//插入到待用set中
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
		//获得当前属性a下的某一个子数据集
		if (tempTrainSet.empty())
		{
			node * temp_node = new node;
			temp_node->arrived_value = get_most_value(data);
			temp_node->isLeaf = 1;//是叶子节点了
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

double getLabel(TrainSet T, node* root)//确定单个测试集/验证集的最终标签
{
	node * root_for_travel = root;
	int i, j;
	double temp;
	bool flag;
	while (!root_for_travel->isLeaf)//没有达到叶子节点前需要一直进行遍历
	{
		temp = T.data[root_for_travel->attribute];//当前选择的属性下的值
		flag = 1;
		for (i = 0; i < root_for_travel->choice.size(); i++)
		{
			if (temp == root_for_travel->choice[i])
			{
				flag = 0;
				break;
			}
		}//i = 需要前往的属性的序号
		if(!flag) root_for_travel = root_for_travel->children[i];
		else
		{
			return root_for_travel->arrived_value;
			break;
		}
	}
	//到达叶子节点，返回结果
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
