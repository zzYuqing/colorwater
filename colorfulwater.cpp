using namespace std;
#include<iostream>
#include<stack>
#include<vector>
#include<string>

//����ȫ�ֱ���colorlist��ֻ���������Ӧ��ɫ���������㷨ʵ�ʵ�����
vector<string> colorlist = { "����","����","ī��","��ɫ","��ɫ","��ɫ","��ɫ","ǳ��","��ɫ","��ɫ","��ɫ","����" };
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
		cout << "��";
	}
	while (s.size() > 0) {
		cout << colorlist[s.top()-1]<<" ";
		s.pop();
	}
	return cout;
}
void initialize(int num, vector<Bottle> &bottles)
{
	//Ҫ������Ļ��������޸�ȫ�ֱ���colorlist
	cout << "������ɫ�����̡����̡�ī�̡���ɫ����ɫ����ɫ����ɫ��ǳ������ɫ����ɫ����ɫ������" << endl;
	cout << "��Ӧ��ţ�01��  02��  03��  04��  05��  06��  07��  08��  09��  10��  11��  12  " << endl;
	cout << "����ÿ���Թܵ���ɫ��������0" << endl;
	//��ʼ��
	for (int i = 1; i <= num; i++) {
		vector<int> temp = {};
		cout << "�����" << i << "���Թܵ���ɫ��ţ���������,�ո�������� ";
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
		//��������һ��������:1.�Ƿ����ĸ����֣�ÿ�������Ƿ�Ϸ�
		Bottle *bi = new Bottle(temp);
		bottles.push_back(*bi);
	}
	cout << "ÿ���Թܵ���ɫΪ���������£���" << endl;
	int j = 1;
	for (auto i : bottles) {
		cout << "��" << j << "���Թܵ���ɫ�� ";
		cout << i << endl;
		j++;
	}
}
vector<int> movecount(vector<Bottle> bottles, int num1,int num2) {
	//num1���Թ��Ƶ�num2
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
	vector<int> onestep;//��4λ����һλ��¼ѡ���Թܣ��ڶ�λ��¼Ŀ���Թܣ�����λ��¼��ɫ��ţ�����λ��¼�ƶ���Ŀ
	//��i���Ƶ�j��
	for (int i = 0; i < num;i++) {
		if (bottles[i].iscomplete()) continue;//i������ɣ��������ƶ�
		if (i == laststep) continue;//��һ�ν��ܵĹ��ӣ���β������Ƴ�
		for (int j = 0; j < num; j++) {
			if (i == j) continue;//�����Լ����Լ��ƶ��Ĳ���
			if (bottles[j].getsize()==4) continue;//j��Ϊ4���ܽ���
			if (bottles[j].getsize() == 0 && bottles[i].isonecolor()) continue;//��ɫ�������Ƶ��չ�
			onestep = movecount(bottles, i, j);
			if (onestep[3] == 0) continue;//�ƶ���ĿΪ0������
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
	cout << "�������Թ�����";
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
		cout <<"��"<< i[0]+1 <<"�ƶ���"<< i[1]+1 <<"���ƶ���ɫΪ��"<< colorlist[i[2]-1] <<"���ƶ���ĿΪ��"<< i[3] << endl;
		solu2.pop();
	}
	cout << "���ս����ÿ���Թܵ���ɫΪ���������£���" << endl;
	int j = 1;
	for (auto i : bottles) {
		cout << "��" << j << "���Թܵ���ɫ�� ";
		cout << i << endl;
		j++;
	}
	return 0;
}