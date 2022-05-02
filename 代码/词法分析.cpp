#include<bits/stdc++.h>
using namespace std;
const int MAX_NUM = 500;
struct token_{
	int row; //行数 
	string type; //类型 
	string str; //单词 
	string ex;  //映射的一个字符 
};
struct read_data{
	char A; //左边非终结符 
	char B; //终结符
	char C; //右边非终结符 
};
ofstream output("./2_output.txt");
ofstream maptxt("./maptxt.txt");
token_ token[MAX_NUM];
read_data data[MAX_NUM]; 
vector<char> final_;
vector<char> state_; //终态 
char begin_ = NULL;
vector<int> end__;
const char end_ = 'Z';
struct NFA_data{
	char t[100]; //终结符 
	int len; //终结符长度 
};
NFA_data nfa_[128][128];
NFA_data dfa_[128][128];
const char delimiter[20] = {',','(',')','{','}',';','<','>','#'}; //界符 
const char single_op[20] = {'+','-','*','/','!','%','~','&','|','^','='};   //单目运算符
const char binary_op[20][5] = {"++","--","&&","||","<=","!=","==",">=","+=","-=","*=","/="}; //双目
const string keyword[50] = {"break","case","char","continue","do","complex","double","else","float","for",
"if","int","include","long","main","return","switch","typedef","void","unsigned","while","iostream"};
const string qualifier[50] = {"const","static"};

char to_char(string t){
	if(t == keyword[0]) return 'a';  //"break"
	if(t == keyword[1]) return 'b';  //"case"
	if(t == keyword[2]) return 'c';  //"char"
	if(t == keyword[3]) return 'd';  //"continue"
	if(t == keyword[4]) return 'e';  //"do"
	if(t == keyword[5]) return 'f';  //"complex"
	if(t == keyword[6]) return 'g';  // "double"
	if(t == keyword[7]) return 'h';  //"else"
	if(t == keyword[8]) return 'i';  //"float"
	if(t == keyword[9]) return 'j';  //"for"
	if(t == keyword[10]) return 'k';  //"if"
	if(t == keyword[11]) return 'l';  //"int"
	if(t == keyword[12]) return 'm';  //"include"
	if(t == keyword[13]) return 'n';  //"long" 
	if(t == keyword[14]) return 'o';  //"main"
	if(t == keyword[15]) return 'p';  //"return"
	if(t == keyword[16]) return 'q';  //"switch"
	if(t == keyword[17]) return 'r';  //"typedef"
	if(t == keyword[18]) return 's';  //"void"
	if(t == keyword[19]) return 't';  //"unsigned"
	if(t == keyword[20]) return 'u';  //"while"
	if(t == keyword[21]) return 'v';  //"iostream"
	
	if(t == qualifier[0]) return '8';
	if(t == qualifier[1]) return '9';

}
bool isIn(char a){
	for(int i = 0; i < final_.size(); i++){
		if(final_[i] == a)return true;
	}
	return false;
}
int count_f(){
	return final_.size();
}
int read_(string file_name){ //读3型文法 生成NFA 
	ifstream srcFile(file_name, ios::in);
	if(!srcFile){
		cout<<"error opening source file."<<endl;
		return -1;
	}
	string tmp = "";
	int cnt = 0;
	while(srcFile >> tmp){
		if(!begin_){
			begin_ = tmp[0];
		}
		if(!isIn(tmp[3])){
			final_.push_back(tmp[3]);
		}
		if(tmp.size() == 4){
			data[cnt].A = tmp[0];
			data[cnt].B = tmp[3];
			nfa_[tmp[0]]['Z'].t[nfa_[tmp[0]]['Z'].len++] = tmp[3]; 
			
		}else if(tmp.size() == 5){
			data[cnt].A = tmp[0];
			data[cnt].B = tmp[3];
			data[cnt].C = tmp[4];
			nfa_[tmp[0]][tmp[4]].t[nfa_[tmp[0]][tmp[4]].len++] = tmp[3];
		}
		cnt++;
		
	}
	return cnt;
} 

void show_nfa(){
	for(int i = 'A'; i <= 'Z'; i++){
		for(int j = 'A'; j <= 'Z'; j++){
			for(int k = 0; k < nfa_[i][j].len; k++){
				cout<<char(i)<<"->"<<char(j)<<":"<<nfa_[i][j].t[k]<<endl;
			} 
		}
	}
}

string find_state(char A, char a){
	string ans = "";
	for(int i = 'A'; i <= 'Z'; i++){
		for(int j = 0; j < nfa_[A][i].len; j++){
			if(nfa_[A][i].t[j] == a ){
				ans += char(i);
				break;
				
			}
		}
	
	}
	return ans;
}

string get_closure(string tmp, char a){
	string ans = "";
	string temp = "";
	bool flag = true;
	for(int i = 0; i < tmp.size(); i++){
		temp = find_state(tmp[i], a);
		for(int j = 0; j < temp.size(); j++){
			flag = true;
			for(int k = 0; k < ans.size(); k++){
				if(ans[k] == temp[j]){
					flag = false; 
				}
			}
			if(flag){
				ans += temp[j];
			}
		
		}
	}
	sort(ans.begin(), ans.end());
	return ans;
}
 
bool isEnd(string s){
	for(int i = 0; i < s.size(); i++){
		if(s[i] == 'Z'){
			return true;
		}
	}
	return false;
} 

void nfa_dfa(){
	queue<string> st;
	st.push("S");
	map<string, int> mp;
	int cnt = 0;
	string former = "";
	string current = ""; 
	mp["S"] = cnt;  //初始状态 
	while(!st.empty()){  //遍历所有产生的状态且不产生新状态 
		former = st.front();
		for(int i = 0; i < final_.size(); i++){
			current = get_closure(former, final_[i]); //寻找当前状态经过某终结符的e-闭包 
			if(!mp.count(current) && current != ""){ //产生新状态 
				cnt++;
				mp[current] = cnt;
				st.push(current);
		    //	cout<<current<<endl;
				if(isEnd(current)){ // 判断是否为终态 
					//cout<<cnt<<endl;
					end__.push_back(cnt);
				}
			}
		
			if(current != ""){ //构建DFA 
				dfa_[mp[former]][mp[current]].t[dfa_[mp[former]][mp[current]].len++] = final_[i];
			}
		}
		st.pop();		
	}
}

void show_dfa(){
	for(int i = 0; i < 128; i++){
		for(int j = 0; j < 128; j++){
			 
			for(int k = 0; k < dfa_[i][j].len; k++){
				cout<<i<<"->"<<j<<":"<<dfa_[i][j].t[k]<<endl;
			}
		}
	}
}

bool isInt(char a){
	if(a >= '0' && a <= '9'){
		return true;
	}
	return false;
}
bool isLetter(char a){
	if(a >= 'a' && a <= 'z'){
		return true;
	}
	if(a >= 'A' && a <= 'Z'){
		return true;
	}
	return false;
}
bool isKeyword(string tmp){
//	char a [50];
////	vector<char> a(tmp.size());
//	for(int i = 0; i < tmp.size(); i++)a[i] = tmp[i];
//	for(int i = 0; i < 23; i++){
//		cout<<tmp<<"  "<< keyword[i]<<endl;
//		if(strcmp(a, keyword[i]) == 0)return true;
//	}
//	return false;
	for(int i = 0; i < 30; i ++) {
//		cout << tmp << " " << keyword[i] << endl;
		if(tmp == keyword[i]) return 1;
	} 
	return false;
}
bool isQualifier(string tmp){
	for(int i = 0; i < 30; i++){
		if(tmp == qualifier[i])return true;
	}
	return false;
}



/******************************************/
//关键字    1
//标识符    2
//常数		3
//运算符	4
//界符		5
//限定符    6
/******************************************/
int scan( ){
	string str;
	ifstream fin("code_add.txt", ios::in);
	int cnt_line = 1;
	int cnt_token = 0;
	int tmp_index = 0;
	while(getline(fin, str)){
		str += '@'; 
		string token_str = "";
		int index = 0;
		bool flag_end = true; //判断这一行是否读结束 
		bool flag_token = true; //判断当前单词是否截止 
		int cur = 0;
		int cur_past = 0;
		while(flag_end){
			cur = -1;
			cur_past = 0;
			flag_token = true; 
			while(flag_token){ 
				cur = -1;
				for(int i = 0; i < 100; i++){
					for(int j = 0; j < dfa_[cur_past][i].len; j++){
						if(dfa_[cur_past][i].t[j] == str[index]){
							cur = i;
						}
					}
				}
				if(str[index] == ' ' || str[index] == '@'){
					cur = -1;
				}
				if(cur == -1){
					flag_token = false;
					if(find(end__.begin(), end__.end(), cur_past) != end__.end()){
						token[cnt_token].row = cnt_line;
						if(isInt(token_str[0])){
							token[cnt_token].type = "常数";
							token[cnt_token].ex = '3';
							token[cnt_token++].str = token_str;
							output<<3;
						}else if(isLetter(token_str[0])){
							if(isKeyword(token_str)){
								token[cnt_token].type = "关键字";
								token[cnt_token].ex = to_char(token_str);
								token[cnt_token++].str = token_str;
								output<<to_char(token_str);
							}else if(isQualifier(token_str)){
								token[cnt_token].type = "限定符";
								token[cnt_token].ex = to_char(token_str);
								token[cnt_token++].str = token_str;
								output<<to_char(token_str);
							}else{
								token[cnt_token].type = "标识符";
								token[cnt_token].ex = '2';
								token[cnt_token++].str = token_str;
								output<<2;
							}
						}else{
							token[cnt_token].type = "界符";
							token[cnt_token].ex = token_str;
							token[cnt_token++].str = token_str;
							output<<token_str;
						}
						token_str = "";
						index--; 
					}
					token_str = "";
				}else{
					cur_past = cur;
					cur = -1;
					token_str += str[index];
				}
				if(str[index] == '@'){
					flag_end = false;
				}
				index++;
			
			}
		}
		cnt_line++;
	}
	output<<'@';
	return cnt_token;
} 

void show_ans(int n){
	for(int i = 0; i < 49; i++)cout<<"_";
	cout<<endl;
	cout<<"|"<<setw(15)<<"行号"<<"|"<<setw(15)<<"类型"<<"|"<<setw(15)<<"内容"<<"|"<<endl; 
//	for(int i = 0; i < 49; i++)cout<<"_";
//	cout<<endl;
	for(int i = 0; i < n; i++ ){
		cout<<"|"<<setw(15)<<token[i].row<<"|"<<setw(15)<<token[i].type
		<<"|"<<setw(15)<<token[i].str<<"|"<<endl;
//		for(int j = 0; j < 49; j++)cout<<"-";
//		cout<<endl;
		maptxt<<i<<" "<<token[i].ex<<" "<<token[i].str<<endl;
	}
	for(int i = 0; i < 49; i++)cout<<"-";
	cout<<endl;
}

int main(){
	int cnt = read_("3type.txt");
	nfa_dfa();
//	show_nfa(); 
//	show_dfa();
	int n = scan();
	output.close();
	show_ans(n);
	maptxt.close();

}
