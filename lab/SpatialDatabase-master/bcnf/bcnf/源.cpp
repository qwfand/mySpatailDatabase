#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<stdio.h>
using namespace std;

string R; //��ϵģʽ
vector< pair<string, string> > F; // ������������FD)
vector<string>subset; //��ϵģʽ R �������Ӽ�
char *temp; //�������Ӽ��ĸ�������
vector<string>candidate_key; // ���еĺ�ѡ��
vector<string>super_key; //���еĳ���

/*********************************************************************/
bool _includes(string s1, string s2){ //�ж� s2 ��ÿ��Ԫ���Ƿ񶼴����� s1
    sort(s1.begin(), s1.end());
    sort(s2.begin(), s2.end());
    return includes(s1.begin(), s1.end(), s2.begin(), s2.end()); // includes�����ǻ������򼯺ϵģ�����������
}
string get_attribute_closure(const string &X, const vector< pair<string, string> > &F){ //�������Լ�X�ıհ�
    string ans(X); //��ʼ�� ans
    string temp;
    bool *vis = new bool[F.size()];
    fill(vis, vis + F.size(), 0);
    do{
        temp = ans;
        for (int i = 0; i != F.size(); ++i){
            if (!vis[i] && _includes(ans, F[i].first)){
                vis[i] = 1;
                ans += F[i].second;
            }
        }
    } while (temp != ans); // ans ���κθı�ʱ��ֹѭ��

    delete[]vis;
    vis = NULL;
    //ɾ���ظ���
    sort(ans.begin(), ans.end());
    ans.erase(unique(ans.begin(), ans.end()), ans.end());
    return ans;
}
void _all_subset(int pos, int cnt, int num){ // get_all_subset()�ĸ�������
    if (num <= 0){
        temp[cnt] = '\0';
        subset.push_back(temp);
        return;
    }
    temp[cnt] = R[pos];
    _all_subset(pos + 1, cnt + 1, num - 1);
    _all_subset(pos + 1, cnt, num - 1);
}
void get_all_subset(const string &R){ //���ϵģʽR�������Ӽ���������subset��
    subset.clear();
    temp = NULL;
    temp = new char[R.size()];
    _all_subset(0, 0, R.length());
    //delete[]temp;
    temp = NULL;
}
bool is_candidate_key(const string &s){ //�ж� s �Ƿ��Ǻ�ѡ��
    for (int i = 0; i != candidate_key.size(); ++i)
        if (_includes(s, candidate_key[i])) //���s��������֪�ĺ�ѡ������ôs�Ͳ��Ǻ�ѡ��
            return false;
    return true;
}

bool cmp_length(const string &s1, const string &s2){ //�� subset ���ַ�����������
    return s1.length() < s2.length();
}
void get_candidate_key(const string &R, const vector< pair<string, string> > &F){//���ϵģʽ R����F�����к�ѡ��
    get_all_subset(R);
    sort(subset.begin(), subset.end(), cmp_length);
    candidate_key.clear();
    super_key.clear();
    for (int i = 0; i != subset.size(); ++i){
        if (_includes(get_attribute_closure(subset[i], F), R)){
            super_key.push_back(subset[i]);
            if (is_candidate_key(subset[i]))
                candidate_key.push_back(subset[i]);
        }
    }
}

typedef vector<pair<string, string> > vpss;
vpss get_minimum_rely(const vpss &F){ //���� F ��������
    vpss G(F);
    //ʹ G ��ÿ�� FD ���ұ߾�Ϊ������
    for (int i = 0; i != G.size(); ++i){
        if (G[i].second.length() > 1){
            string f = G[i].first, s = G[i].second, temp;
            G[i].second = s[0];
            for (int j = 1; j<s.length(); ++j){
                temp = s[j];
                G.push_back(make_pair(f, temp));
            }
        }
    }

    int MAXN = 0;
    for (int i = 0; i != G.size(); ++i)
        if (G[i].first.length()>MAXN)
            MAXN = G[i].first.length();
    bool *del = new bool[MAXN];

    //�� G ��ÿ�� FD ������������������
    for (int i = 0; i != G.size(); ++i){
        if (G[i].first.length() > 1){
            fill(del, del + G[i].first.length(), 0);
            for (int j = 0; j != G[i].first.length(); ++j){ //���ڵ�i��FD,�ж��Ƿ������first�ĵ�j������
                string temp;
                del[j] = 1;
                for (int k = 0; k != G[i].first.length(); ++k)
                    if (!del[k])
                        temp += G[i].first[k];
                if (!_includes(get_attribute_closure(temp, G), G[i].second)) //����ɾ��
                    del[j] = 0;
            }
            string temp;
            for (int j = 0; j != G[i].first.length(); ++j)
                if (!del[j])
                    temp += G[i].first[j];
            G[i].first = temp;
        }
    }
    delete[]del;
    del = NULL;

    //������ȥ��
    sort(G.begin(), G.end());
    G.erase(unique(G.begin(), G.end()), G.end());

    //�� G ����������� FD
    vpss ans;
    for (int i = 0; i != G.size(); ++i){ //�жϵ�i�� FD �Ƿ�����
        vpss temp(G);
        temp.erase(temp.begin() + i);
        if (!_includes(get_attribute_closure(G[i].first, temp), G[i].second)) //�� i �� FD ��������
            ans.push_back(G[i]);
    }
    return ans;
}
string _difference(string a, string b){ //��ȥ�أ��ٷ��� a �� b �Ĳ���� a-b
    string c;
    c.resize(a.size());
    sort(a.begin(), a.end());
    a.erase(unique(a.begin(), a.end()), a.end());
    sort(b.begin(), b.end());
    string::iterator It = set_difference(a.begin(), a.end(), b.begin(), b.end(), c.begin());
    c.erase(It, c.end());
    return c;
}
/***************����ϵģʽ R  ����ֽ�� BCNF ģʽ�� *****************/
vector<string> split_to_bcnf(const string &R, const vector< pair<string, string> > &F){
    vector<string> ans;
    vector<string> temp;
    ans.push_back(R);
    vector< pair<string, string> > FF = get_minimum_rely(F); //���� F����С�������� FF
    int flag;
    do{
        flag = 0;
        temp.resize(ans.size());
        temp.assign(ans.begin(), ans.end()); //���浱ǰ�� ans

        for (int i = 0; i != ans.size(); ++i){
            vector< pair<string, string> > MC;// �� ans[i] �ϵ���С������
            for (int j = 0; j != FF.size(); ++j){
                if (_includes(ans[i], FF[j].first) && _includes(ans[i], FF[j].second))
                    MC.push_back(FF[j]);
            }
            sort(MC.begin(), MC.end());
            MC.erase(unique(MC.begin(), MC.end()), MC.end());

            get_candidate_key(ans[i], MC);
            for (int j = 0; j != MC.size(); ++j){
                int is_super_key = 0;
                for (int k = 0; k != super_key.size(); ++k){
                    if (_includes(MC[j].first, super_key[k])){
                        is_super_key = 1;
                        break;
                    }
                }
                // ans[i]�д���һ����ƽ�� FD x->y, �� x �������������Ͱ� ans[i]
                //�ֽ�� xy �� ans[i]-y ;
                if (!is_super_key){
                    ans.push_back(_difference(ans[i], MC[j].second));
                    ans[i] = MC[j].first + MC[j].second;
                    flag = 1;
                    break;
                }
            }
            if (flag)
                break;
        }
        /********�� ans û�иı�ʱ��ֹѭ��������������ѭ�� **********/
        sort(temp.begin(), temp.end());
        sort(ans.begin(), ans.end());
        temp.erase(unique(temp.begin(), temp.end()), temp.end());
        ans.erase(unique(ans.begin(), ans.end()), ans.end());
        if (equal(ans.begin(), ans.end(), temp.begin()))
            break;
    } while (flag);
    return ans;
}

/********************************************************************/

void init(){ //��ʼ��
    R = "";
    F.clear();
}
void inputR(){   //�����ϵģʽ R
    cout << "�������ϵģʽ R:" << endl;
    cin >> R;
}
void inputF(){  //���뺯�������� F
    int n;
    string temp;
    cout << "�����뺯����������Ŀ��" << endl;
    cin >> n;
    cout << "������" << n << "������������(������ʽΪ a->b ab->c) " << endl;
    for (int i = 0; i < n; ++i){
        pair<string, string>ps;
        cin >> temp;
        int j;
        for (j = 0; j != temp.length(); ++j){ //���� ps.first
            if (temp[j] != '-'){
                if (temp[j] == '>')
                    break;
                ps.first += temp[j];
            }
        }
        ps.second.assign(temp, j + 1, string::npos); //���� ps.second
        F.push_back(ps); //����ps
    }
}
/**************************************************************************/
int main(){
    //freopen("in.txt", "r", stdin);
    init();
    inputR();
    inputF();

    vector<string> ans = split_to_bcnf(R, F);
    cout << "����ϵģʽ R ����ֽ�� BCNF ģʽ�������£�" << endl;
    for (int i = 0; i != ans.size(); ++i)
        cout << ans[i] << endl;
    system("pause");
    return 0;
}