#include<bits/stdc++.h>
#define PI 3.14159
using namespace std;
class Generate_weight
{
public:
    int x = 0;

    void UNIFORM(double *p)
    {
        int i, a;
        double f;
        for (i = 0; i < 2;   i++, x = x + 689)
        {
            a = rand() + x;  //加上689是因为系统产生随机数的更换频率远远不及程序调用函数的时间
            a = a % 1000;
            f = (double)a;
            f = f / 1000.0;
            *p = f;
            p++;
        }
    }
    double Gaussian(int E,int D)
    {
        int i, j;
        double A, B, C, r;
        double uni[2];
        double *p;
        srand((unsigned)time(NULL));  //随机数种子采用系统时钟
        UNIFORM(&uni[0]);  //调用UNIFORM函数产生2个均匀分布的随机数并存入数组uni[2]
        A = sqrt((-2)*log(uni[0]));
        B = 2 * PI*uni[1];
        C = A * cos(B);
        r = E + C * D;    //E,D分别是期望和方差
//        outfile << r << "   ";  //将数据C输出到被定义的文件中
        return r;
    }
    //返回归一化的vector, ans[0] 为归一化数据， ans2 为原始数据
    vector<vector<double> > Toone(int E,int D,int n)
    {
        vector<vector<double> > ans;
        vector<double> vec1,vec2;
        double tot = 0;
        for(int i = 0; i < n; i++)
        {
            vec1.push_back(Gaussian(E,D));
            tot += vec1[i];
        }
        for(int i = 0; i < vec1.size(); i++)
        {
            vec2.push_back(vec1[i] / tot);
        }
        ans.push_back(vec2);
        ans.push_back(vec1);
        return ans;
    }
    struct node
    {
        int key;
        double value;
        node(int k, int v)
        {
            key = k;
            value = v;
        }
    };
    struct cmp
    {
        bool operator()(node a, node b)
        {
            return a.value < b.value;
        }
    };
    //
    vector<vector<int> > Niagra(vector<double> weight, double Error)
    {
//        priority_queue<node, vector<node>, cmp> pri1,pri2;//pri1 存储大于weight的， pri2 存储小于weight
        vector<double> weight_train(weight.size());
//        for(int i = 0; i < weight.size(); i++)
//        {
//            weight_train.push_back(0.125);
//        }
        weight_train[0] = 1;
        vector<vector<int> > ans;
        double deviation = 0;
        for(int i = 0; i < weight.size(); i++)
        {
            deviation += abs(weight_train[i] - weight[i]);
        }
//        int k = 1;
        while(deviation > Error)
        {
            vector<int> vec(weight.size());
            double Upper = INT_MIN, lower = INT_MAX;
            int Upper_local = 0, lower_Local = 0;
            for(int i = 0; i < weight.size(); i++)
            {
                if(weight_train[i] - weight[i] > Upper)
                {
                    Upper = weight_train[i] - weight[i];
                    Upper_local = i;
                }
                if(weight_train[i] - weight[i] < lower)
                {
                    lower = weight_train[i] - weight[i];
                    lower_Local = i;
                }
            }
            deviation = 0;
            double Min_deviation = INT_MAX;
            int Min_k = 0;
            int k_temp = 0;
            while(1)
            {
//                cout<<"???"<<endl;
                weight_train[Upper_local] -= pow(2, -k_temp);
                weight_train[lower_Local] += pow(2, -k_temp);
                double deviation_temp = 0;
                for(int i = 0; i < weight.size(); i++)
                {
                    deviation_temp += abs(weight_train[i] - weight[i]);
                }
                if(deviation_temp < Min_deviation)
                {
                    Min_deviation = deviation_temp;
                    Min_k = k_temp;
                }
                weight_train[Upper_local] += pow(2, -k_temp);
                weight_train[lower_Local] -= pow(2, -k_temp);
                if(deviation_temp > Min_deviation)
                {
                    break;
//                    Min_deviation = deviation_temp;
                }
//                Min_k++;
                k_temp++;
            }
            vec[Upper_local] = -Min_k;
            vec[lower_Local] = Min_k;
            weight_train[Upper_local] -= pow(2, -Min_k);
            weight_train[lower_Local] += pow(2, -Min_k);

            ans.push_back(vec);
            for(int i = 0; i < weight.size(); i++)
            {
                deviation += abs(weight_train[i] - weight[i]);
            }
            cout<<deviation<<"\t"<<pow(2,-Min_k)<<endl;
            for(int i = 0; i < weight_train.size(); i++)
            {
                cout<<setw(10)<<weight_train[i];
            }
            cout<<endl;
//            k++;
        }
        return ans;
    }
};
int main()
{
    double t;
    t = pow(2,-1);
    cout<<t<<endl;
    Generate_weight tx;
    int n = 8;
    int E = 4, D = 1;
    double Error = 0.02;
    vector<vector<double> > data;
    data = tx.Toone(E,D,n);
    double tot = 0;
    for(int i = 0; i < data[0].size(); i++)
    {
        cout<<setw(10)<<data[0][i];
    }
    cout<<endl;
    vector<vector<int> > ans;
    ans = tx.Niagra(data[0],Error);
    cout<<"---"<<endl;
    for (int i = 0 ; i < ans.size(); i++)
    {
        for(int j = 0; j < ans[i].size(); j++)
        {
            cout<<ans[i][j]<<"\t";
        }
        cout<<endl;
    }
    return 0;
}
