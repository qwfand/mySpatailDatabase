typedef vector< pair<string, string> > yilai;
class Database
{
public:
    int N; //���Լ�������ϵ�ĸ���
    string R1; //���Լ�
    string R2; //��������Լ�
    yilai R3; //���Լ�������ϵ
    void R1shuru();
    void R2shuru();
    void R3shuru();
    bool ZFxiangdeng(char a, string b);
    bool ZFCbaohan(string a, string b);
    string bibao(string R2, yilai R3);
    string shuchuchuli(string X);
};