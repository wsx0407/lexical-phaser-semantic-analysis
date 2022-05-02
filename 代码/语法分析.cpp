#include<bits/stdc++.h>
using namespace std;
const int MAX_NUM = 500;
struct project{
	char A; //文法左边 
	string str; //文法右边 
};
struct line{ //一个项目中的一个项目 
	char A;
	int next;
	string str;
	vector<char> t;
};
struct state_{ // 项目 
	vector<line> vl;
	char to;
	vector<int> from;
	int index;
	map<char, int> flag;
};
vector<state_> st; //项目集族 
vector<project> pro;
vector<char> Vn;
vector<char> Vt;
vector<vector<char>> first(MAX_NUM);
int s[MAX_NUM]; //判断是否能推出z 
string action_[MAX_NUM][MAX_NUM];
int goto_[MAX_NUM][MAX_NUM]; // -1  acc
// @代替 # ； z代替空  Z代替S_； 
int read_(string file_name){ 
	ifstream srcFile(file_name, ios::in);
	if(!srcFile){
		cout<<"error opening source file."<<endl;
		return -1;
	}
	string tmp = "";
	int cnt = 0;
	while(srcFile >> tmp){
		project p;
		p.A = tmp[0];
		for(int i = 3; i < tmp.size(); i++){
			p.str += tmp[i];
			if(tmp[i] > 'Z' || tmp[i] < 'A'){
				if(find(Vt.begin(), Vt.end(), tmp[i]) == Vt.end()){
					Vt.push_back(tmp[i]);
				}
			}
		
		}
		if(find(Vn.begin(), Vn.end(),  tmp[0]) == Vn.end()){
			Vn.push_back(tmp[0]);
		}
		pro.push_back(p);
		p.str = "";
		cnt++;
	}
//	for(int i = 0; i < Vt.size(); i++)cout<<Vt[i];
//	cout<<endl;
//	for(int i = 0; i < Vn.size(); i++)cout<<Vn[i];
//	cout<<endl;
//	for(int i = 0; i < cnt; i++){
//		cout<<pro[i].A<<" : "<<pro[i].str<<endl;
//	}
	return cnt;
} 


void isz(){
	for(int i = 0; i < MAX_NUM; i++){
		s[i] = -1;
	}	
// 扫描文中产生式 
	vector<project> pro_copy;
	vector<char> vn_1;
	bool flag[pro.size()] = {false};
	for(int i = 0; i < pro.size(); i++){
		for(int j = 0; j < pro[i].str.size(); j++){
			if(pro[i].str[j] != 'z' && pro[i].str[j] > 'Z' || pro[i].str[j] < 'A'){
				flag[i] = true;
				break;
			}
		}	
		if(!flag[i]){
			pro_copy.push_back(pro[i]);
		}
	} 
	for(int i = 0; i < pro.size(); i++){
		if(flag[i] == false){
			if(find(vn_1.begin(), vn_1.end(), pro[i].A) == vn_1.end()){
				vn_1.push_back(pro[i].A);
			}
		}else{
			bool f = true;
			for(int j = i+1; j < pro.size(); j++){
				if(flag[j] == false && pro[i].A == pro[j].A){
					f = false;
					if(find(vn_1.begin(), vn_1.end(), pro[i].A) == vn_1.end()){
						vn_1.push_back(pro[i].A);
					}	
				}
			}
			if(f){
				s[pro[i].A] = 0;
			}
		}	
	} 
	for(int i = 0; i < pro.size(); i++){
		for(int j = 0; j < pro[i].str.size(); j++){
			if(pro[i].str[j] == 'z'){
				s[pro[i].A] = 1;
			}
		}
	}
//	删除推出空的所有产生式 
	for(int i = 0; i < pro_copy.size(); i++){
		 if(s[pro_copy[i].A] == 1){
		 	pro_copy.erase(pro_copy.begin()+i);
		 	i--;
		 }
	}
// 产生式右部字符 
	int cnt = 0;
	for(int i = 0; i < Vn.size(); i++){
		if(s[Vn[i]] == -1)cnt++;
	}
	while(cnt){
		for(int i = 0; i < pro_copy.size(); i++){
			for(int j = 0; j < pro_copy[i].str.size(); j++){
				if(s[pro_copy[i].str[j]] == 1){
					pro_copy[i].str.erase(pro_copy[i].str.begin()+j);
					j--;
				}
			}
			if(pro_copy[i].str.size() == 0){
				s[pro_copy[i].A] = 1;
			} 
		}
		for(int i = 0; i < pro_copy.size(); i++){
			 if(s[pro_copy[i].A] == 1){
			 	pro_copy.erase(pro_copy.begin()+i);
			 	i--;
			 }
		}
		vector<project> pro_;
		for(int i = 0; i < pro_copy.size(); i++){
			flag[i] = false;
			for(int j = 0; j < pro_copy[i].str.size(); j++){
				if(s[pro_copy[i].str[j]] == 0){
					flag[i] = true;
					break;
				}
			}
			if(!flag[i]){
				pro_.push_back(pro_copy[i]);
			}
		}
		for(int i = 0; i < pro_copy.size(); i++){
			bool it = true;
			for(int j = 0; j < pro_.size(); j++){
				if(pro_copy[i].A == pro_[j].A && pro_copy[i].str == pro_[j].str){
					it = false;
					break;
				}
			}
			if(it){
				s[pro_copy[i].A] = 0;
			}
		}
		pro_copy.clear();
		for(int i = 0; i < pro_.size(); i++){
			pro_copy.push_back(pro_[i]);
		}
		
		cnt = 0;
		for(int i = 0; i < Vn.size(); i++){
		if(s[Vn[i]] == -1)cnt++;
	}
	}
//	for(int i = 'A'; i <= 'Z'; i++){
//		cout<<char(i)<<" "<<s[i]<<endl; 
//	} 	 
} 

void get_first(){
	vector<project> pro_copy;
	for(int i = 0; i < pro.size(); i++){
		if(pro[i].str[0] > 'Z' || pro[i].str[0] < 'A'){
			if(find(first[pro[i].A].begin(), first[pro[i].A].end(), pro[i].str[0]) == first[pro[i].A].end()){
				first[pro[i].A].push_back(pro[i].str[0]);
			}
		}else{
			pro_copy.push_back(pro[i]);
		}
	}
	int cnt = 1;
	while(cnt){
		cnt = 0;	
		for(int i = 0; i < pro_copy.size(); i++){
			for(int j = 0; j < pro_copy[i].str.size(); j++){
				if(pro_copy[i].str[j] > 'Z' || pro_copy[i].str[j] < 'A'){
					if(find(first[pro_copy[i].A].begin(), first[pro_copy[i].A].end(), pro_copy[i].str[j]) == first[pro_copy[i].A].end()){
						first[pro_copy[i].A].push_back(pro_copy[i].str[0]);
						cnt++;
					}
					break;
				}else{
//					cout<<pro_copy[i].str[j]<<endl;
					if(s[pro_copy[i].str[j]] == 0){
					for(int k = 0; k < first[pro_copy[i].str[j]].size(); k++){
//						cout<<first[pro_copy[i].str[j]][k];
						if(find(first[pro_copy[i].A].begin(), first[pro_copy[i].A].end(), first[pro_copy[i].str[j]][k]) == first[pro_copy[i].A].end()){
							first[pro_copy[i].A].push_back(first[pro_copy[i].str[j]][k]);
							cnt++;
						}
					}
//					cout<<endl;
					break;
					}else if(s[pro_copy[i].str[j]] == 1){
						for(int k = 0; k < first[pro_copy[i].str[j]].size(); k++){
							if(find(first[pro_copy[i].A].begin(), first[pro_copy[i].A].end(), first[pro_copy[i].str[j]][k]) == first[pro_copy[i].A].end()){
								first[pro_copy[i].A].push_back(first[pro_copy[i].str[j]][k]);
								cnt++;
							}
						}
					}
				} 
			}
		}
		
	}
//	for(int i = 'A'; i <= 'Z'; i++){
//
//		if(first[i].size()!=0){
//			cout<<char(i)<<"   ";
//			for(int j = 0; j < first[i].size(); j++){
//				cout<<first[i][j];
//			}
//			cout<<endl;
//		}
//	
//	}
}

int isSame(state_ s){
	int cnt;
	int cnt_1;
	for(int i = 0; i < st.size(); i++){
		if(st[i].to == s.to){
			cnt = 0;
			for(int j = 0; j < s.vl.size(); j++){
				for(int k = 0; k < st[i].vl.size(); k++){
					if(s.vl[j].A == st[i].vl[k].A && s.vl[j].next == st[i].vl[k].next &&
					s.vl[j].str== st[i].vl[k].str){
						cnt_1 = 0;
						for(int n = 0; n < s.vl[j].t.size(); n++){
							for(int m = 0; m < st[i].vl[k].t.size(); m++){
								if(s.vl[j].t[n] == st[i].vl[k].t[m]){
									cnt_1++;
									break;
								}
							}
						}
						if(cnt_1 == s.vl[j].t.size()){
							cnt++;
						}
					}
				}
			}
			if(cnt == s.vl.size()){
				return i;
			}
		}
	}
	return -1;
}
 
bool add_first(line a, int x){
	if(a.str.size() <= x+1)return false;
	else{
		while(a.str.size() > x+1){
			if(a.str[x+1] >= 'A' && a.str[x+1] <= 'Z' && s[a.str[x+1]] == 1){
				x++;
			}else{
				return false;
			}
		}
	}
	return true;
}
 
vector<char> find_first(line a, int x){
	vector<char> ans;
	if(a.str.size() <= x+1){
		return ans;
	}
	while(a.str.size() > x+1){
		char next = a.str[x+1];
		if(next > 'Z' || next < 'A'){
			ans.push_back(next);
			return ans;
		}else{
		
			if(s[next] == 0){
				for(int i = 0; i < first[next].size(); i++){
					if(first[next][i] != 'z')
					ans.push_back(first[next][i]);
				}
				return ans;
			}else if(s[next] == 1){
				for(int i = 0; i < first[next].size(); i++){
					if(find(ans.begin(), ans.end(), first[next][i]) == ans.end()){
						if(first[next][i] != 'z')
						ans.push_back(first[next][i]);
					}
					
				}
				x++;
			}
		}
	}
	return ans;
}

bool find_same(state_ s, line l){
	int cnt = 0;
	for(int i = 0; i < s.vl.size(); i++){
		if(s.vl[i].A == l.A && s.vl[i].next == l.next && s.vl[i].str == l.str){
			cnt = 0;
			for(int j = 0; j < l.t.size(); j++){
				for(int n = 0; n < s.vl[i].t.size(); n++){
					if(s.vl[i].t[n] == l.t[j]){
						cnt++;
						break;
					}
				}
			}
			if(cnt == l.t.size())return true;
		}
	}
	return false;
}


void create_(){
	line l_tmp;
	vector<char> t_tmp;
	state_ s_tmp;
	l_tmp.A = 'Z';
	l_tmp.next = 0;
	l_tmp.str = 'S';
	l_tmp.t.push_back('@');
	s_tmp.vl.push_back(l_tmp);
	queue<line> q;
	q.push(l_tmp); 
	//I0项目集 
	while(!q.empty()){
		line now = q.front();
		q.pop();
		l_tmp.t.clear();
		for(int i = 0; i < now.t.size(); i++){
			l_tmp.t.push_back(now.t[i]);
		} 
		if(now.next < now.str.size() && now.str[now.next] >= 'A' && now.str[now.next] <= 'Z'){
			l_tmp.A = now.str[now.next];
			t_tmp = find_first(now, now.next);
			if(t_tmp.size() != 0){ // first
				if(!add_first(now, now.next))l_tmp.t.clear();
				for(int i = 0; i < t_tmp.size(); i++){
					if(find(l_tmp.t.begin(), l_tmp.t.end(), t_tmp[i]) == l_tmp.t.end()){
						l_tmp.t.push_back(t_tmp[i]);
					}
				}
			}
			l_tmp.next = 0;
			
			for(int i = 0; i < pro.size(); i++){
				if(pro[i].A == now.str[now.next]){
					l_tmp.str = pro[i].str;
					if(l_tmp.str == "z")l_tmp.next++;
					if(!find_same(s_tmp, l_tmp)){
						s_tmp.vl.push_back(l_tmp);
						q.push(l_tmp);
					}
				
				}
			}
		}	
	} 
	s_tmp.index = 0;
	st.push_back(s_tmp);
	//产生项目集 
	queue<state_> qs;
	qs.push(s_tmp);
	int cnt = 1;
	while(!qs.empty()){
		s_tmp.vl.clear();
		s_tmp.from.clear();
		state_ now_s = qs.front();
		qs.pop();
		vector<char> used;
	    while(!q.empty())q.pop();
		for(int i = 0; i < now_s.vl.size(); i++){
			if(now_s.vl[i].next < now_s.vl[i].str.size() &&  // 移进 
			find(used.begin(), used.end(), now_s.vl[i].str[now_s.vl[i].next]) == used.end()){
				s_tmp.vl.clear();
				s_tmp.from.clear();
				while(!q.empty())q.pop();
				s_tmp.to = now_s.vl[i].str[now_s.vl[i].next];
				s_tmp.from.push_back(now_s.index);
				s_tmp.index = cnt;
				used.push_back(now_s.vl[i].str[now_s.vl[i].next]);
	
				if(now_s.vl[i].next < now_s.vl[i].str.size()){
					l_tmp.next = now_s.vl[i].next + 1; 
					l_tmp.A = now_s.vl[i].A;
					l_tmp.str = now_s.vl[i].str;
					l_tmp.t.clear();
					for(int j = 0; j < now_s.vl[i].t.size(); j++){
						l_tmp.t.push_back(now_s.vl[i].t[j]);
					}
					s_tmp.vl.push_back(l_tmp);
					q.push(l_tmp);
					for(int j = i+1; j < now_s.vl.size(); j++){
						if(now_s.vl[j].str[now_s.vl[j].next] == now_s.vl[i].str[now_s.vl[i].next]){
							if(now_s.vl[j].next < now_s.vl[j].str.size()){
								l_tmp.next = now_s.vl[j].next + 1;
								l_tmp.A = now_s.vl[j].A;
								l_tmp.str = now_s.vl[j].str;
								l_tmp.t.clear();
								for(int k = 0; k < now_s.vl[j].t.size(); k++){
									l_tmp.t.push_back(now_s.vl[j].t[k]);
								}
								s_tmp.vl.push_back(l_tmp);
								q.push(l_tmp);
							}
						}
					}
				}
				
				// 生成该项目集中的其他项目 
				while(!q.empty()){
					line now = q.front();
					q.pop();
					l_tmp.t.clear();
					for(int j = 0; j < now.t.size(); j++){
						l_tmp.t.push_back(now.t[j]);
					} 
					if(now.next < now.str.size() && now.str[now.next] >= 'A' && now.str[now.next] <= 'Z'){
						l_tmp.A = now.str[now.next];
						t_tmp = find_first(now, now.next);
						if(t_tmp.size() != 0){ // first
							if(!add_first(now, now.next))l_tmp.t.clear();
							for(int j = 0; j < t_tmp.size(); j++){
								if(find(l_tmp.t.begin(), l_tmp.t.end(), t_tmp[j]) == l_tmp.t.end()){
									l_tmp.t.push_back(t_tmp[j]);
								}
							}
						}
						l_tmp.next = 0;
						for(int j = 0; j < pro.size(); j++){
							l_tmp.next = 0;
							if(pro[j].A == now.str[now.next]){
								l_tmp.str = pro[j].str;
								if(l_tmp.str == "z")l_tmp.next++;
								if(!find_same(s_tmp, l_tmp)){
									s_tmp.vl.push_back(l_tmp);
									q.push(l_tmp);
								}
							}
						}
					}	
					
				} 
				int same = isSame(s_tmp);
				if(same == -1){
					cnt++;
					st.push_back(s_tmp);
					qs.push(s_tmp);
				}else{
					if(find(st[same].from.begin(), st[same].from.end(), s_tmp.from[0]) == st[same].from.end()){
						st[same].from.push_back(s_tmp.from[0]);
					}
					
				}
			}	
		}	
	}
	
} 

void show_create(){
	cout<<"LR(1)项目集族："<<endl;
	for(int i = 0; i < st.size(); i++){
		for(int i = 0; i < 28; i++)cout<<"_";
	    cout<<endl;
		cout<<"I"<<i<<": ";
		for(int j = 0; j < st[i].vl.size(); j++){
			if(j != 0)cout<<"    ";
			cout<<st[i].vl[j].A<<"->";
			for(int k = 0; k < st[i].vl[j].str.size(); k++){
				if(st[i].vl[j].str[st[i].vl[j].next] == st[i].vl[j].str[k]){
					cout<<"·"<<st[i].vl[j].str[k];
				}else cout<<st[i].vl[j].str[k];
			}
			cout<<" , ";
			for(int k = 0; k < st[i].vl[j].t.size(); k++){
				cout<<st[i].vl[j].t[k];
			}
			cout<<endl;
		}
	}
	for(int i = 0; i < 28; i++)cout<<"_";
	cout<<endl<<endl;
}

//void set_flag(){
//	for(int i = 0; i < st.size(); i++){
//		for(int j = 0; j < st[i].vl.size(); j++){
//			if(st[i].vl[j].next == st[i].vl[j].str.size()){
//				for(int k = 0; k < pro.size(); k++){
//					if(pro[k].A == st[i].vl[j].A && pro[k].str == st[i].vl[j].str){
//						for(int n = 0; n < st[i].vl[j].t.size(); n++){
//							st[i].flag[st[i].vl[j].t[n]] = k;
//						}
//						
//					}
//				}
//			}
//		}
//	}
//}

void create_actiongoto(){
	for(int i = 0; i < MAX_NUM; i++){
		for(int j = 0; j < MAX_NUM; j++){
			action_[i][j] = "-2";
			goto_[i][j] = -2;
		}
	}
	for(int i = 1; i < st.size(); i++){
		for(int j = 0; j < st[i].from.size(); j++){
			if(st[i].to >= 'A' && st[i].to <= 'Z'){
				goto_[st[i].from[j]][st[i].to] = st[i].index;
			}else{
				action_[st[i].from[j]][st[i].to] = "S";
				string t_str; 
				int tmp = st[i].index;
				if(tmp == 0){
					t_str += '0' + tmp;
				}else{
					while(tmp > 0){
						t_str += '0' + (tmp % 10);
						tmp /= 10;
					}
				}
				reverse(t_str.begin(), t_str.end());
				action_[st[i].from[j]][st[i].to] += t_str;
				
			}
		}
	
		for(int j = 0; j < st[i].vl.size(); j++){
			if(st[i].vl[j].next == st[i].vl[j].str.size()){
				for(int k = 0; k < st[i].vl[j].t.size(); k++){
				
					action_[st[i].index][st[i].vl[j].t[k]] = "r";
					int tmp = -1;
					for(int n = 0; n < pro.size(); n++){
						if(pro[n].A == st[i].vl[j].A && pro[n].str == st[i].vl[j].str){
							tmp = n;
							break;
						}
					}
					string t_str; 
					if(tmp == 0){
						t_str += '0' + tmp;
					}else{
						while(tmp > 0){
							t_str += '0' + (tmp % 10);
							tmp /= 10;
						}
					}
					reverse(t_str.begin(), t_str.end());
					action_[st[i].index][st[i].vl[j].t[k]] += t_str;
				}
			}
		}
					
	}
	action_[1]['@'] = "acc"; 
	Vt.push_back('@');
	cout.setf(ios::left);
	cout<<"LR(1)分析表："<<endl;
	for(int i = 0; i < 187; i++)cout<<"_";
	cout<<endl;
	cout<<"|"<<setw(4)<<"状态"<<"|"<<setw(129)<<"action"<<"|"<<setw(55)<<"goto"<<"|"<<endl;
	cout<<"|    ";
	for(int i = 0; i < Vt.size(); i++){
		cout<<"|"<<setw(4)<<Vt[i];
	}
	for(int i = 0; i < Vn.size(); i++){
		cout<<"|"<<setw(3)<<Vn[i];
	}cout<<"|"<<endl;
	for(int i = 0; i < 187; i++)cout<<"_";
	cout<<endl;
	for(int i = 0; i < st.size(); i++){
		cout<<"|"<<setw(4)<<i;
		for(int j = 0; j < Vt.size(); j++){
			if(action_[i][Vt[j]] != "-2"){
				cout<<"|"<<setw(4)<<action_[i][Vt[j]];
			}else cout<<"|    ";
		}
		for(int j = 0; j < Vn.size(); j++){
			if(goto_[i][Vn[j]] != -2){
				cout<<"|"<<setw(3)<<goto_[i][Vn[j]];
			}else cout<<"|   ";
		}
		cout<<"|"<<endl;
	}
	for(int i = 0; i < 187; i++)cout<<"_";
	cout<<endl;
//	for(int i = 0; i < st.size(); i++){
//		for(int j = 'A'; j <= 'Z'; j++){
//			if(goto_[i][j] != -2)
//			cout<<i<<" "<<char(j)<<" "<<goto_[i][j]<<endl;
//		}
//	}
//	
//	for(int i = 0; i < st.size(); i++){
//		for(int j = 0; j < MAX_NUM; j++){
//			if(action_[i][j] != "-2")
//			cout<<i<<" "<<char(j)<<" "<<action_[i][j]<<endl;
//		}
//	}
}




bool scan_(){
	cout.setf(ios::left);
	cout<<"LR(1)分析过程："<<endl;
	for(int i = 0; i < 180; i++)cout<<"_";
	cout<<endl;
	cout<<"|"<<setw(4)<<"步骤"<<"|"<<setw(20)<<"状态栈"<<"|"<<setw(70)<<"符号栈"<<"|"<<setw(70)<<"输入串"<<"|"<<setw(6)<<"action"<<"|"
	<<setw(6)<<"goto"<<"|"<<endl;
	for(int i = 0; i < 180; i++)cout<<"_";
	cout<<endl;
	ifstream fi("2_output.txt");
	string str;
	string read_str;
	while(getline(fi, str)){
		read_str += str;
	}
	stack<int> zt;
	stack<char> input_str;
	stack<char> label_str;
	vector<int> zt_i;
	string input_str_i;
	string label_str_i;
	zt.push(0);
	int cnt = read_str.size() - 1;
	while(cnt >= 0){
		input_str.push(read_str[cnt]);
		cnt--;
	}
	bool flag = true;
	int now = 0;
	char now_char;
	int count = 0;
	int bz = 0;
	while(flag){
		bz++;
		now = zt.top();
		now_char = input_str.top();
		string change = action_[now][now_char];
		cout<<"|"<<setw(4)<<bz<<"|";
		while(!zt.empty()){
			zt_i.push_back(zt.top());
			zt.pop();
		}
		for(int i = zt_i.size()-1; i >= 0; i--){
			zt.push(zt_i[i]);
		}
		while(!input_str.empty()){
			input_str_i += input_str.top();
			input_str.pop();
		}
		for(int i = input_str_i.size()-1; i >= 0; i--){
			input_str.push(input_str_i[i]);
		}
		while(!label_str.empty()){
			label_str_i += label_str.top();
			label_str.pop();
		}
		for(int i = label_str_i.size()-1; i >= 0; i--){
			label_str.push(label_str_i[i]);
		}
		string ztt;
		for(int i = zt_i.size()-1; i >= 0; i--){
			ztt += to_string(zt_i[i]);
		}
	    reverse(label_str_i.begin(), 	label_str_i.end());
		cout<<setw(20)<<ztt<<"|"<<setw(70)<<label_str_i<<"|"<<setw(70)<<input_str_i<<"|";
		zt_i.clear();
		label_str_i = "";
		input_str_i = "";
		if(change == "-2"){
			flag = false;
			cout<<"-2项;"<<" "<<now_char<<"出错"<<endl; 
			break;
		}else if(change == "acc"){
			cout<<setw(6)<<"acc"<<"|      "<<endl;
			return true;
		}else if(change[0] == 'S'){
			int change_zt = 0;
			for(int i = 1; i < change.size(); i++){
				change_zt = change_zt * 10;
				change_zt += change[i] - '0';
			}
			zt.push(change_zt);
			input_str.pop();
			label_str.push(now_char);
			cout<<setw(6)<<change<<"|      "<<endl;
		}else if(change[0] == 'r'){
			int change_zt = 0;
			for(int i = 1; i < change.size(); i++){
				change_zt = change_zt * 10;
				change_zt += change[i] - '0';
			}
			char t = label_str.top();
			count = 0;
			for(int i = pro[change_zt].str.size()-1; i >= 0; i--){
				if(pro[change_zt ].str[i] == t ){
					label_str.pop();
					t = label_str.top();
					count++;
				}else if( pro[change_zt ].str[i] == 'z'){
			
					break;
				}else{
					cout<<"缺少"<<pro[change_zt ].str[i]<<endl;
					flag = false;
					break; 
				}
			}
			if(flag){
				while(count--){
					zt.pop();
				}		
				now = zt.top();
				cout<<setw(6)<<change<<"|"<<setw(6)<<goto_[now][pro[change_zt].A]<<"|"<<endl;
				zt.push(goto_[now][pro[change_zt].A]);
				label_str.push(pro[change_zt].A);	
			}		
		}	
	}
	return flag;
}

int main(){
	read_("2type.txt");
	isz();
	get_first();
//	cout << 1 << endl;
	create_();
//	cout << 1 << endl;
	show_create();
//	cout << 1 << endl;
//	set_flag();
//	cout << 1 << endl;
	create_actiongoto();
//	cout << 1 << endl;
	bool flag = scan_();

	if(flag){
		cout<<"YES"<<endl;
	}else{
		cout<<"NO"<<endl;
	}
}
