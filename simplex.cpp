#include <iostream>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <climits>
#define N 100

using namespace std;

int n,m;
double matrix[N][N];
vector<int> base;//为什么用vector？
double b[N],c[N],judge[N];
double z;
double ans[N];

void input_standard()
{
    for(int i = 0;i<m;i++)
    {
        for(int j = 0;j<n;j++)
        {
            cin>>matrix[i][j];
        }
    }
    for(int i = 0;i<m;i++)
        cin>>b[i];
    for(int i= 0;i<n;i++)
    {
        cin>>c[i];
        if(c[i]==0)
            base.push_back(i);
    }
        //////
}
int find_index()
{
//    double tmp = -1*maxint;
//    int tag = -1;
//    for(int i = 0;i<n;i++)
//    {
//        if(c[i]<0)
//        {
//            if(tmp<c[i])
//            {
//                tmp = c[i];
//                tag = i;
//            }
//        }
//    }
//    return tag;

    for(int i = 0;i<n;i++)
    {
        if(c[i]<0)
        {
            return i;
        }
    }
    return -1;
}

int cal_answer()
{
//    set<int>::iterator it=base.begin();
//    for(;it!=base.end();it++)
//        cout<<*it<<endl;
    for(int j = 0;j<n;j++)
    {
        vector<int>::iterator it = find(base.begin(),base.end(),j);
        //cout<<*it<<endl;
        if(it==base.end())
        {
            ans[j] = 0;
        }
        else
        {
            for(int i = 0;i<m;i++)
            {
                if(matrix[i][j]==1)
                    ans[j] = b[i];
            }
        }

    }
    return 1;
}

void alter(int e,int l)
{
    cout<<"当前基为："<<endl;
    vector<int>::iterator it=base.begin();
    base.erase(it+l);
    base.insert(it+l,e);
    it=base.begin();
    for(;it!=base.end();it++)
        cout<<*it<<"--";
    cout<<endl;
}
void change_base(int e,int l)
{
    cout<<"change:"<<e<<"  "<<l<<endl;
    b[l] = b[l]/matrix[l][e];
    double le = matrix[l][e];
    for(int i = 0;i<n;i++)
    {
        matrix[l][i] = matrix[l][i]/le*1.0;
    }
    for(int i = 0;i<m;i++)
    {
        if(i!=l)
        {
            b[i] = b[i]-matrix[i][e]*b[l];
            double ie = matrix[i][e];
            for(int j = 0;j<n;j++)
            {
                matrix[i][j] = matrix[i][j]-ie*matrix[l][j];
            }
        }
    }
    z = z-c[e]*b[l];
    //cout<<"jdjfk"<<c[e]<<l<<endl;
    double ce = c[e];
    for(int i = 0;i<n;i++)
        c[i] = c[i]-ce*matrix[l][i];
    alter(e,l);
}
void show_matrix()
{
    cout<<endl;
    for(int i = 0;i<n;i++)
        cout<<c[i]<<"\t";
    cout<<z<<"\t";
    cout<<endl;
    for(int i = 0;i<m;i++)
    {
        for(int j = 0;j<n;j++)
        {
            cout<<matrix[i][j]<<"\t";
        }
        cout<<b[i]<<" ";
        cout<<endl;
    }
    }
int simplex()
{
    int cnt = 4;
    while(1)
    {
        show_matrix();
        int index = find_index();
        if(index == -1)
        {
            cal_answer();
            return 1;
        }
        else
        {
            //cout<<"index: "<<index<<endl;
            memset(judge,0,sizeof(judge));
            double minum = INT_MAX;
            int new_index = -1;
            for(int i = 0;i<m;i++)
            {
                if(matrix[i][index]>0)  //b[i]!=0
                    judge[i] = b[i]*1.0/matrix[i][index];
                else
                    judge[i] = INT_MAX;
            }
            for(int i = 0;i<m;i++)
            {
                if(judge[i]<minum)
                {
                    minum = judge[i];
                    new_index = i;
                }
            }
            //cout<<"index: "<<new_index<<endl;
            if(new_index == -1)
                return -1;////////fanhuizhi
            change_base(index,new_index);
        }
    }
    return 0;
}
int main()
{
    freopen("caicai.txt","r",stdin);
    cin>>n>>m;
    input_standard();
    if(simplex())
        cout<<"done!"<<endl;
    else
        cout<<"failed!"<<endl;
    for(int i = 0;i<n;i++)
        cout<<ans[i]<<" ";
    cout<<"最优值为："<<-z<<endl;
    return 0;
}
