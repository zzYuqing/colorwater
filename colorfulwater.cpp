using namespace std;
#include<iostream>
#include<stack>
#include<vector>
#include<string>

//定义全局变量colorlist，只用于输出对应颜色，不参与算法实际的运算
vector<string> colorlist = { "青绿","褐绿","墨绿","粉色","红色","紫色","蓝色","浅蓝","橘色","黄色","褐色","灰蓝" };
class Bottle {
public:
	stack<int> bottle;
public:
	Bottle(vector<int> num) {
		for (int i : num) {
			if (i == 0) break;
			bottle.push(i);
		}

	}
	~Bottle() {

	}
	int gettop() {
		if (getsize() == 0) return 0;
		return bottle.top();
	}
	void dopush(int num) {
		bottle.push(num);
	}
	void dopop() {
		bottle.pop();
	}
	int getsize() {
		return bottle.size();
	}
	bool iscomplete(){
		if (getsize() == 0) return true;
		if (getsize() < 4) return false;
		stack<int> temp = bottle;
		int color = temp.top();
		while (temp.size() > 0) {
			if (temp.top() != color) return false;
			temp.pop();
		}
		return true;
	}
	bool isonecolor() {
		if (getsize() == 0) return true;
		if (getsize() == 4) return false;
		stack<int> temp = bottle;
		int color = temp.top();
		while (temp.size() > 0) {
			if (temp.top() != color) return false;
			temp.pop();
		}
		return true;

	}
};
ostream& operator<<(ostream& cout, Bottle b) {
	stack<int> s = b.bottle;
	if (s.size() == 0) {
		cout << "空";
	}
	while (s.size() > 0) {
		cout << colorlist[s.top()-1]<<" ";
		s.pop();
	}
	return cout;
}
void initialize(int num, vector<Bottle> &bottles)
{
	//要动这里的话，必须修改全局变量colorlist
	cout << "所有颜色：青绿、褐绿、墨绿、粉色、红色、紫色、蓝色、浅蓝、橘色、黄色、褐色、灰蓝" << endl;
	cout << "对应编号：01、  02、  03、  04、  05、  06、  07、  08、  09、  10、  11、  12  " << endl;
	cout << "输入每个试管的颜色，若空输0" << endl;
	//初始化
	for (int i = 1; i <= num; i++) {
		vector<int> temp = {};
		cout << "输入第" << i << "个试管的颜色编号（从下往上,空格隔开）： ";
		int j = 0;
		while (j < 4) {
			int temp_num = 0;
			cin >> temp_num;
			if (temp_num == 0) {
				temp.push_back(temp_num);
				break;
			}
			temp.push_back(temp_num);
			j++;
		}
		//可以增加一步输入检查:1.是否有四个数字，每个数字是否合法
		Bottle *bi = new Bottle(temp);
		bottles.push_back(*bi);
	}
	cout << "每个试管的颜色为（从上往下）：" << endl;
	int j = 1;
	for (auto i : bottles) {
		cout << "第" << j << "个试管的颜色： ";
		cout << i << endl;
		j++;
	}
}
vector<int> movecount(vector<Bottle> bottles, int num1,int num2) {
	//num1的试管移到num2
	int color = bottles[num1].gettop();
	vector<int> onestep = { -1,-1,-1,0 };
	if (bottles[num2].gettop() != color&& bottles[num2].getsize()!=0) return onestep;
	bottles[num1].dopop();
	onestep[0] = num1;
	onestep[1] = num2;
	onestep[2] = color;
	onestep[3]++;
	while (bottles[num1].gettop() == color&& bottles[num2].getsize()+ onestep[3]<=4) {
		onestep[3]++;
		bottles[num1].dopop();
	}
	return onestep;
}
void moveonestep(vector<Bottle> &bottles, vector<int> onestep) {
	for (int i = 0; i < onestep[3]; i++) {
		bottles[onestep[1]].dopush(onestep[2]);
		bottles[onestep[0]].dopop();
	}
}
void backonestep(vector<Bottle> &bottles, vector<int> onestep) {
	for (int i = 0; i < onestep[3]; i++) {
		bottles[onestep[0]].dopush(onestep[2]);
		bottles[onestep[1]].dopop();
	}
}
bool doneall(vector<Bottle> bottles) {
	for (auto i : bottles) {
		if (!i.iscomplete()) return false;
	}
	return true;
}
void solution(vector<Bottle> &bottles, stack<vector<int>> &solu,int num,int laststep) {
	if (doneall(bottles)) return;
	vector<int> onestep;//共4位，第一位记录选中试管，第二位记录目标试管，第三位记录颜色编号，第四位记录移动数目
	//从i管移到j管
	for (int i = 0; i < num;i++) {
		if (bottles[i].iscomplete()) continue;//i若已完成，则不能再移动
		if (i == laststep) continue;//上一次接受的管子，这次不能再移出
		for (int j = 0; j < num; j++) {
			if (i == j) continue;//不做自己对自己移动的操作
			if (bottles[j].getsize()==4) continue;//j管为4则不能接受
			if (bottles[j].getsize() == 0 && bottles[i].isonecolor()) continue;//纯色不允许移到空管
			onestep = movecount(bottles, i, j);
			if (onestep[3] == 0) continue;//移动数目为0，跳过
			solu.push(onestep);
			moveonestep(bottles, onestep);
			if (doneall(bottles)) return;
			solution(bottles, solu,num,j);
			if (doneall(bottles)) return;
			backonestep(bottles, onestep);
			solu.pop();
		}
	}
}
int main(int argc, char *argv[])
{
	int num = 0;
	cout << "输入总试管数：";
	cin >> num;
	vector<Bottle> bottles;
	initialize(num, bottles);
	stack<vector<int>> solu;
	solution(bottles, solu,num,-1);
	stack<vector<int>> solu2;
	while (solu.size()!=0) {
		solu2.push(solu.top());
		solu.pop();
	}
	while (solu2.size() != 0) {
		auto i = solu2.top();
		cout <<"从"<< i[0]+1 <<"移动到"<< i[1]+1 <<"，移动颜色为："<< colorlist[i[2]-1] <<"，移动数目为："<< i[3] << endl;
		solu2.pop();
	}
	cout << "最终结果，每个试管的颜色为（从上往下）：" << endl;
	int j = 1;
	for (auto i : bottles) {
		cout << "第" << j << "个试管的颜色： ";
		cout << i << endl;
		j++;
	}
	return 0;
}