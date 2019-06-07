#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <stdio.h>
using namespace std;

string R;//��ϵģʽ
vector <pair<string, string>> F;//����������
vector<string> subset;//��ϵģʽR�������Ӽ�
char *temp;//�������Ӽ��ĸ�������
vector<string> candidate_key;//���еĺ�ѡ��
vector<string> super_key;//���еĳ���

bool Includes(string s1,string s2)
{
    sort(s1.begin(),s1.end());
    sort(s2.begin(),s2.end());
    return includes(s1.begin(),s1.end(),s2.begin(),s2.end());
}
string getArrClosure(const string &X, const vector<pair<string, string>> &F)
// �������Լ�X�ıհ�
{
    string ans(X);
    string temp;
    bool *vis = new bool[F.size()];
    fill(vis, vis + F.size(), 0);
    while (temp != ans)
    {
        temp = ans;
        for (int i = 0; i != F.size();i++)
        {
            if (!vis[i]&&Includes(ans,F[i].first))
            {
                vis[i]=1;
                ans+=F[i].second;
            }
        }
    }

    delete[]vis;
    vis=NULL;

    //delete the repeated
    sort(ans.begin(),ans.end());
    ans.erase(unique(ans.begin(),ans.end()),ans.end());
    return ans;

}
void allSubset(int pos,int cnt,int num,const string &R)
//getAllSubset�ĸ�������
{
    if(num<=0)
    {
        temp[cnt]='\0';
        subset.push_back(temp);
        return;
    }
    temp[cnt]=R[pos];
    allSubset(pos+1,cnt+1,num-1,R);
    allSubset(pos+1,cnt,num-1,R);
}
void getAllSubset(const string &R)
//���ϵģʽR�������Ӽ���������subset��
{
    subset.clear();
    temp=NULL;
    temp=new char[R.size()];
    allSubset(0,0,R.length(),R);
    temp=NULL;
}

bool isCandidateKey(const string &s)
{
    for (int i=0;i!=candidate_key.size();i++)
    {
        if (Includes(s,candidate_key[i]))
            return false;
    }
    return true;
}

bool cmpLength(const string &s1,const string &s2)
//��subset���ַ������Ƚ�������
{
    return s1.length()<s2.length();
}
void getCandidateKey(const string &R,const vector<pair<string,string>> &F)//���ϵģʽR����F�����к�ѡ��
{
    getAllSubset(R);
    sort(subset.begin(),subset.end(),cmpLength);
    candidate_key.clear();
    super_key.clear();
    for (int i=0;i!=subset.size();i++)
    {
        if (Includes(getArrClosure(subset[i],F),R))
        {
            super_key.push_back(subset[i]);
            if (isCandidateKey(subset[i]))
                candidate_key.push_back(subset[i]);
        }
    }
}

//����F�����򸲸�
vector<pair<string, string>> getMinRely(const vector<pair<string, string>> &F)
{
    vector<pair<string, string>> G(F);
    // ʹG��ÿһ��FD���ұ߾�Ϊ������
    for (int i = 0; i != G.size(); i++)
    {
        if (G[i].second.length() > 1)
        {
            string f = G[i].first, s = G[i].second, temp;
            G[i].second = s[0];
            for (int j = 1; j < s.length(); j++)
            {
                temp=s[j];
                G.push_back(make_pair(f, temp));
            }

        }
    }

    int MAXN = 0;
    for (int i = 0; i != G.size(); i++)
        if (G[i].first.length()>MAXN)
            MAXN = G[i].first.length();
    bool *del = new bool[MAXN];

    //��G��ÿ��FD������������������
    for (int i = 0; i != G.size();i++)
    {
        if (G[i].first.length() > 1)
        {
            fill(del, del + G[i].first.length(), 0);
            for (int j = 0; j != G[i].first.length(); j++)
            {
                string temp;
                del[j] = 1;
                for (int k = 0; k != G[i].first.length(); k++)
                    if (!del[k])
                        temp += G[i].first[k];
                if (!Includes(getArrClosure(temp, G), G[i].second))
                    del[j]=0;                
            }
            string temp;
            for (int j=0;j!=G[i].first.length();j++)
            {
                if (!del[j])
                    temp+=G[i].first[j];
            }
            G[i].first = temp;
        }
    }
    delete[]del;
    del=NULL;

    //delete the repeated
    sort(G.begin(),G.end());
    G.erase(unique(G.begin(),G.end()),G.end());

    //��G�����������FD
    vector<pair <string,string>> ans;
    for (int i=0;i!=G.size();i++)
    {
        vector<pair <string,string>> temp(G);
        temp.erase(temp.begin()+i);
        if (!Includes(getArrClosure(G[i].first,temp), G[i].second))
            ans.push_back(G[i]);
    }
    return ans;
}

//����F�ıհ�
vector<pair<string, string>> getFunClosure(const string &R,const vector < pair<string, string>> &F)
{
    string temp;
    vector <pair<string, string>> G;
    getAllSubset(R);
    for (int i = 0; i < subset.size(); i++)
    {
        temp = getArrClosure(subset[i], F);
        for (int j = 0; j < temp.length(); j++)
        {
            string s;
            s = temp[j];
            if (!Includes(subset[i], s))
                G.push_back(make_pair(subset[i], s));
        }
    }
    return G;
}

//���غ��������رհ�
vector<pair<string, string>> getFunClosure(const vector < pair<string, string>> &F)
{
    string temp;
    vector<pair<string, string>> G;
    for (int i = 0; i < F.size(); i++)
    {
        temp = getArrClosure(F[i].first, F);
        for (int j = 0; j < temp.length(); j++)
        {
            string s;
            s = temp[j];
            if (!Includes(F[i].first, s))
                G.push_back(make_pair(F[i].first, s));
        }
    }
    return G;
}

string difference(string a,string b)
//��a-b
{
    string c;
    c.resize(a.size());
    sort(a.begin(),a.end());
    a.erase(unique(a.begin(),a.end()),a.end());
    sort(b.begin(),b.end());
    string::iterator it = set_difference(a.begin(),a.end(),b.begin(),b.end(),c.begin());
    c.erase(it,c.end());
    return c;
}
vector<string> splitToBCNF(const string &R, const vector<pair<string, string>> &F)
{
    vector<string> ans;
    vector<string> temp;
    ans.push_back(R);
    vector<pair<string,string>> FF=getFunClosure(getMinRely(F));//����F�ıհ���FF

    int flag=1;
    while (flag)
    {
        flag=0;
        temp.resize(ans.size());
        temp.assign(ans.begin(),ans.end());

        for (int i=0;i!=ans.size();i++)
        {
            vector<pair<string,string>> MC;
            for (int j=0;j!=FF.size();j++)
            {
                if (Includes(ans[i],FF[j].first) && Includes(ans[i],FF[j].second))
                    MC.push_back(FF[j]); 
            }
            sort(MC.begin(),MC.end());
            MC.erase(unique(MC.begin(),MC.end()),MC.end());

            getCandidateKey(ans[i],MC);
            for (int j=0;j!=MC.size();j++)
            {
                int is_super_key=0;
                for (int k=0;k!=super_key.size();k++)
                    if (Includes(MC[j].first,super_key[k]))
                    {
                        is_super_key=1;
                        break;
                    }
                //��ans[i]�д���һ����ƽ����x->y,��x��������������ans[i]�ֽ�Ϊxy��ans[i]-y
                if (!is_super_key)
                {
                    ans.push_back(difference(ans[i],MC[j].second));
                    ans[i]=MC[j].first+MC[j].second;
                    flag=1;
                    break;
                }
            }
            if (flag)
                break;       
        }
    }

    for (int i = 0; i < ans.size(); i++)
        for (int j = i + 1; j < ans.size(); j++)
            if (Includes(ans[i], ans[j]))
                ans.erase(ans.begin()+j);
    return ans;
}

void inputF()
{
    int n;
    string temp;
    cin >> n;
    getline(cin,temp);
    for (int i = 0; i < n; i++)
    {
        pair<string, string> ps;
        getline(cin,temp);
        int j;
        for (j = 0; j != temp.length();j++)
            if (temp[j] != '-' && temp[j]!=' ')
            {
            if (temp[j] == '>' && temp[j + 1] == ' ')
            {
                j++;
                break;
            }               
            ps.first += temp[j];
            }       
        ps.second.assign(temp, j + 1, string::npos);
        F.push_back(ps);
    }
}

int main(void)
{
    FILE* stream;
    freopen_s(&stream, "1.in", "r", stdin);
    freopen_s(&stream, "1.out", "w", stdout);
    R = "";
    F.clear();

    int n;
    char ch='A';
    cin >> n;
    for (int i=0;i<n;i++)
    {
        R += ch;
        ch++;
    }
    inputF();

    vector<string> ans = splitToBCNF(R, F);
    for (int i = 0; i != ans.size(); i++)
        cout << ans[i] << endl;
    return 0;
}