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
};

typedef struct Node {
	vector <double> weight;
	vector <double> num;
	vector <double> error;
	double theta = rand() / double(RAND_MAX);
	double output = 0;
};


vector <TrainSet> trainSet, valSet, testSet;
vector <Node> i2h_weight;
Node hidden, out;
vector <double> w, minibatch;


void FileCheck(int modeChoose)
{
	fstream data("train.csv");
	string s;
	int i, j;
	getline(data,s);
	while (getline(data, s)) {
		vector <double> nums;
		TrainSet singleTrain;
		char *str = (char *)s.c_str();//string --> char
		const char *split = ",";
		char *p = strtok(str, split);//逗号分隔依次取出
		double a;
		while (p != NULL){
			sscanf(p, "%lf", &a);//char ---> int
			nums.push_back(a);
			p = strtok(NULL, split);
		}
		for (i = 2; i < nums.size(); i++) {
			singleTrain.num.push_back(nums[i]);
		}

		trainSet.push_back(singleTrain);
		//至此结束单个训练数据x处理
	}
	//至此结束所有训练数据处理，存入trainSet
	/*for (i = 0; i < trainSet.size(); i++) {
		for (j = 0; j < trainSet[0].num.size(); j++) {
			cout << trainSet[i].num[j] << " ";
		}
		cout << endl;

	}*/
	//val情况
	if (modeChoose == 0) {
		fstream val("val.csv");
		string s;
		//getline(val, s);
		while (getline(val, s)) {
			vector <double> nums;
			TrainSet singleTrain;
			char *str = (char *)s.c_str();//string --> char
			const char *split = ",";
			char *p = strtok(str, split);//逗号分隔依次取出
			double a;
			while (p != NULL) {
				sscanf(p, "%lf", &a);//char ---> int
				nums.push_back(a);
				p = strtok(NULL, split);
			}
			for (i = 2; i < nums.size(); i++) {
				singleTrain.num.push_back(nums[i]);
			}

			valSet.push_back(singleTrain);
			//至此结束单个测试数据x处理
		}
	}

	//test情况
	else if (modeChoose == 1) {
		fstream test("test.csv");
		string s;
		getline(test, s);
		while (getline(test, s)) {
			vector <double> nums;
			TrainSet singleTrain;
			char *str = (char *)s.c_str();//string --> char
			const char *split = ",";
			char *p = strtok(str, split);//逗号分隔依次取出
			double a;
			while (p != NULL) {
				sscanf(p, "%lf", &a);//char ---> int
				nums.push_back(a);
				p = strtok(NULL, split);
			}
			nums.pop_back();
			for (i = 2; i < nums.size(); i++) {
				singleTrain.num.push_back(nums[i]);
			}
			
			testSet.push_back(singleTrain);
			//至此结束单个测试数据x处理
		}
	}
	

}

void initialize_weight() {
	int i, j;
	//初始化输入到隐藏层
	int node_number = 20;
	for (i = 0; i < node_number; i++) {
		Node tempNode;
		for (j = 0; j < trainSet[0].num.size(); j++) {
			tempNode.weight.push_back(rand() / double(RAND_MAX));
			tempNode.error.push_back(0);
			tempNode.theta = rand() / double(RAND_MAX);
		}
		i2h_weight.push_back(tempNode);
	}
	//初始化隐藏到输出层
	for (i = 0; i < node_number; i++) {
		hidden.weight.push_back(rand() / double(RAND_MAX));
		hidden.num.push_back(0);
		hidden.error.push_back(0);
	}
	hidden.theta = rand() / double(RAND_MAX);
}

double sigmoid(double x)
{
	double ans = 1 / (1 + exp(-x));
	return ans;
}

void forward_pass_i2h(TrainSet t) {
	int i, j, k;
	double node_value;
	for (j = 0; j < i2h_weight.size(); ++j) {
		node_value = 0;
		for (k = 0; k < trainSet[0].num.size() - 1; k++) {
			node_value += i2h_weight[j].weight[k] * t.num[k];
		}
		hidden.num[j] = (sigmoid(node_value + i2h_weight[j].theta));//单独一个权重向量计算的结果
	}
	//得到隐藏层的结果，存于Node hidden中的num
}

void forward_pass_h2o() {
	int i, j, k;
	double temp_output = 0;
	for (i = 0; i < hidden.num.size(); i++) {
		temp_output += hidden.num[i] * hidden.weight[i];
	}
	hidden.output = temp_output + hidden.theta;
	
}

void backward_pass(TrainSet t) {
	int i, j, k;
	double n = 0.0000001;
	//更新输出误差
	double output_error =  (t.num[t.num.size() - 1] - hidden.output);
	//更新隐藏层误差
	for (i = 0; i < hidden.num.size(); i++) {
		hidden.error[i] = hidden.num[i] * (1 - hidden.num[i]) * hidden.weight[i] * output_error;
	}
	//在不使用minibatch的情况下使用以下代码，每一条训练数据就更新一次权重向量
	//更新权重向量
	for (i = 0; i < hidden.weight.size(); i++) {
		hidden.weight[i] = hidden.weight[i] + n * output_error * hidden.num[i];
	}
	hidden.theta = hidden.theta + n * hidden.output;
	for (i = 0; i < i2h_weight.size(); i++) {
		for (j = 0; j < i2h_weight[0].weight.size(); j++) {
			i2h_weight[i].weight[j] = i2h_weight[i].weight[j] + n * hidden.error[i] * (t.num[j]);
		}
		//更新theta
		i2h_weight[i].theta = i2h_weight[i].theta + n * hidden.error[i];
	}

}

void OutputTest() {
	ofstream test_result("test_result.txt");
	for (int i = 0; i < testSet.size(); i++) {
		forward_pass_i2h(testSet[i]);//前向传递
		forward_pass_h2o();//前向传递
		test_result << hidden.output << endl;
	}
}



int main()
{
	FileCheck(0);//数据读取, 0 = val, 1 = test
	initialize_weight();//初始化权重
	ofstream for_matlab("for_matlab.txt");
	int cnt = 10000, id = 0;
	for (int i = 0; i < cnt; i++) {

		for (int j = 0; j < trainSet.size(); j++) {//对于每一个训练数据，算一次权重更新的迭代
			forward_pass_i2h(trainSet[j]);//前向传递
			forward_pass_h2o();//前向传递
			backward_pass(trainSet[j]);
		}
		//if (i % 10 == 0) {
		//	id++;
		//	double O_average = 0, E = 0;
		//	for (int k = 0; k < trainSet.size(); k++) {//对于每一个训练数据，算一次权重更新的迭代
		//		forward_pass_i2h(trainSet[k]);//前向传递
		//		forward_pass_h2o();//前向传递
		//		E += 0.5 * pow((trainSet[k].num[trainSet[k].num.size() - 1] - hidden.output), 2);
		//	}
		//	printf("id: %d, E = %lf\n", id, E / trainSet.size());
		//	for_matlab << E / trainSet.size() << ",";
		//}
		if (i % 10 == 0) {
			id++;
			double O_average = 0, E = 0;
			for (int k = 0; k < valSet.size(); k++) {//对于每一个训练数据，算一次权重更新的迭代
				forward_pass_i2h(valSet[k]);//前向传递
				forward_pass_h2o();//前向传递
				E += 0.5 * pow((valSet[k].num[valSet[k].num.size() - 1] - hidden.output), 2);
			}
			printf("id: %d, E = %lf\n", id, E / valSet.size());
			for_matlab << E / valSet.size() << ",";
		}
		
	
		
	}
	OutputTest();
	//ofstream for_matlab_standard("for_matlab_standard.txt");
	//double E = 0;
	//for (int i = 0; i < valSet.size(); i++) {
	//	forward_pass_i2h(valSet[i]);//前向传递
	//	forward_pass_h2o();//前向传递
	//	for_matlab << hidden.output << ",";
	//	for_matlab_standard << valSet[i].num[valSet[i].num.size() - 1] << ",";
	//}
	


	system("pause");
	return 0;
}