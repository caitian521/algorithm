#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
using namespace std;

template<class Type> //�էרc��
struct huffnode{
    int weight;
    Type ch;
    char code[50];
    int parent,leftchild,rightchild;
public:
    huffnode():ch(Type()),weight(0),parent(-1),leftchild(-1),rightchild(-1)
    {}

};

class hufftree
{
    list<huffnode<char> > hufflist;
    huffnode<char> *nodetable;
    char file[200];
    int num;
public:
    hufftree(const char *filename);
    ~hufftree();
    void Addchar(const char s);
    void CreateTree();
    bool SelectMin(const int pos,int &minst);
    void CreateCode();
    void Compression();
    void showcode();
    void CreateConfig();
    void Decompression();
    void ReadConfig();
    int Match(const char prebuff[],const char buff[],int &endat,FILE *fout,int &cnt);
    void print(int p,FILE *fout,int &cnt);
};

hufftree::hufftree(const char *filename)
{
    strcpy(file,filename);
    FILE *fin1 = fopen(file,"rb");
    char s;
    s = fgetc(fin1);
    while(s !=EOF)
    {
        //cout<<s<<endl;
        if(s=='\r')
            s = fgetc(fin1);
        Addchar(s);
        s = fgetc(fin1);
    }
    num = hufflist.size();
    printf("%d\n",num);
    nodetable = new huffnode<char>[num*2-1];
    list<huffnode<char> >::iterator iter = hufflist.begin();
    for(int i = 0;i<num && iter!=hufflist.end();i++,iter++)
    {
        nodetable[i].ch = iter->ch;
        nodetable[i].weight = iter->weight;
    }
    //�k��d��ߧըn��
    fclose(fin1);
}

void hufftree::Addchar(const char s)
{
    list<huffnode<char> >::iterator iter = hufflist.begin();
    for(;iter!=hufflist.end();iter++)
    {
        if(iter->ch==s)
        {
            iter->weight++;
            break;
        }
    }
    if(iter==hufflist.end())
    {
        huffnode<char> node;
        node.weight = 1;
        node.ch = s;
        hufflist.push_back(node);
    }

}

void hufftree::CreateTree()
{
    int left = 0,right=0;
    for(int i = num;i<2*num-1;i++)
    {
        SelectMin(i-1,left);
        nodetable[left].parent = i;
        SelectMin(i-1,right);
        nodetable[right].parent = i;
        nodetable[i].weight = nodetable[left].weight+nodetable[right].weight;
        nodetable[i].leftchild = left;
        nodetable[i].rightchild = right;
    }
}
bool hufftree::SelectMin(const int pos,int &minst)
{
    int i=0,minnum=0;
    while(minnum<pos)
    {
        if(nodetable[i].parent==-1)
            break;
        minnum++;
    }
    if(minnum>pos)//��һ��while��Ŀ��
        return false;//ruguo false???
    while(i<=pos)
    {
        if(nodetable[i].parent == -1 && nodetable[minnum].weight>nodetable[i].weight)
            minnum = i;
        i++;
    }
    minst = minnum;
}
void hufftree::CreateCode()
{
    int i;
    int c,p;
    int start;
    char *tmpcode = new char[num];
    tmpcode[num] = '\0';
    for(i = 0;i<num;i++)
    {
        start = num;
        c = i;
        while((p=nodetable[c].parent)>0)
        {
            start--;
            tmpcode[start]=(nodetable[p].leftchild==c)?'0':'1';
            c = p;
        }
        strcpy(nodetable[i].code,&tmpcode[start]);//
    }
}

void hufftree::Compression()
{
    //freopen(file,"r",stdin);
    FILE *fin = fopen(file,"rb");
    FILE *fout = fopen("compression.txt","wb");
    char s;
    char *thiscode = new char[256];
    unsigned char inch = 0;
    int index = 0;
    s = fgetc(fin);
    int countch = 10;
    while(s != EOF)
    {
        //cout<<"hhh"<<endl;
        //thiscode[0] = '\0';
        if(s=='\r')
            s = fgetc(fin);
        for(int i = 0;i<num;i++)
        {
            if(nodetable[i].ch==s)
            {
                thiscode = nodetable[i].code;
                //cout<<nodetable[i].code<<endl;
                break;
            }
        }
        countch--;
        if(countch>0)
            cout<<s<<"  "<<thiscode<<endl;
        for(int t = 0;t<strlen(thiscode);t++)
        {

            inch <<= 1;
            if(thiscode[t]=='1')
                inch|=1;
            index++;
            //cout<<inch<<endl;
            if(index==8)
            {
                //cout<<inch<<endl;
                fputc(inch,fout);
                index = 0;
                inch = 0;
            }
        }
        s = fgetc(fin);
    }
    if(index>0)
    {
        inch =inch<<(8-index);
        fputc(inch,fout);
    }
    fclose(fin);
    fclose(fout);
}
hufftree::~hufftree()
{

}
void hufftree::showcode()
{
    for(int i = 0;i<num;i++)
    {
        cout<<nodetable[i].ch<<"--"<<nodetable[i].code<<endl;
    }
}
void hufftree::CreateConfig()
{
    char configname[200];
    strcpy(configname,"huffman.config");
    cout<<configname<<endl;
    FILE *fout = fopen(configname,"w");
    for(int i = 0;i<num;i++)
    {
        fputs(nodetable[i].code,fout);
        fputc(',',fout);
        fputc(nodetable[i].ch,fout);
        fputc('\n',fout);
    }
    fclose(fout);
}
void hufftree::print(int p,FILE *fout,int &cnt)
{
    if(cnt>0)
    {
        if(nodetable[num*2-2].weight-cnt<10)
            cout<<"yes:"<<nodetable[p].ch<<endl;
        char ch;
        ch = nodetable[p].ch;
        if(ch=='\n')
            fputc('\r',fout);
        fputc(nodetable[p].ch,fout);
    }
    cnt--;
}
int hufftree::Match(const char prebuff[],const char buff[],int &endat,FILE *fout,int &cnt)
{
    int p = num*2-2;
    int Child = nodetable[p].leftchild;
    int flag = 0;
    int start = 0;
    int sign = 0;
    //cout<<"-----"<<prebuff<<endl;
    //return;
    for(int i = 0;prebuff[i]!='\0';i++)
    {
        //cout<<"this is:"<<i<<endl;
        if(Child == -1)
        {
            print(p,fout,cnt);
            p = num*2-2;
            start = i;
        }
        if(prebuff[i]=='0')
        {
            p = nodetable[p].leftchild;
        }
        else{
            p = nodetable[p].rightchild;
        }
        Child = nodetable[p].leftchild;
    }
    if(Child == -1)//
    {
        print(p,fout,cnt);
        p = num*2-2;
        start = 0;
    }
    else
        if(prebuff[0]!='\0')
            flag = 1;
    //cout<<p<<endl;
    //return;
    for(int i = 0;i<8;i++)
    {
        //if(nodetable[num*2-2].weight-cnt<10)
            //cout<<buff[i]<<endl;
        if(Child == -1)//
        {
            print(p,fout,cnt);
            p = num*2-2;
            sign = i;
            flag = 0;
        }
        if(buff[i]=='0')
        {
            p = nodetable[p].leftchild;
        }
        else{
            p = nodetable[p].rightchild;
        }
        Child = nodetable[p].leftchild;
    }
    if(Child == -1)
    {
        print(p,fout,cnt);
        sign = 8;//
        endat = sign;
    }
    else{
            endat = sign;
        if(flag)
            return start;
    }
    return -1;
}

void hufftree::Decompression()
{
    FILE *fin = fopen("compression.txt","rb");
    FILE *fout = fopen("new.txt","wb");
    unsigned char readnum;
    readnum = fgetc(fin);
    int endat = 0;
    int sign = 0;
    char buff[9];
    char prebuff[33];
    prebuff[0] = '\0';
    buff[8] = '\0';
    for(int i = 0;i<8;i++)
        buff[i] = '0';
    cout<<"depression"<<endl;
    int cnt = nodetable[num*2-2].weight;

    while(!feof(fin))//readnum!=EOF
    {

        for(int i = 7;i>=0;i--)
        {
            buff[i]=(readnum&1)?'1':'0';
            readnum >>= 1;
        }
        sign = Match(prebuff,buff,endat,fout,cnt);
        if(sign>=0)
        {
            char temp[32];
            strcpy(temp,&prebuff[sign]);
            prebuff[0]='\0';
            strcpy(prebuff,temp);
            strcat(prebuff,buff);
            cout<<"doushengle:"<<prebuff<<endl;
        }
        else
        {
            if(endat<8)
                strcpy(prebuff,&buff[endat]);
            else
                prebuff[0] = '\0';
        }
        readnum = fgetc(fin);
        endat = 0;
        sign = 0;
    }
    fclose(fin);
    fclose(fout);
}
void hufftree::ReadConfig()
{
    FILE *fin = fopen("huffman.config","rb");
    //s = getc()
    fclose(fin);
}
int main()
{
    hufftree hf("E:\\�һ�����\\�ҵ���ϰ\\�㷨���-�о���\\������\\Aesop_Fables.txt");
    hf.CreateTree();
    hf.CreateCode();
    hf.showcode();
    hf.Compression();
    //hf.CreateConfig();
    hf.Decompression();
}
//����Ҫ˵һ���������Ǿ�����windows�£�������һ����������д�ļ�ʱ�����з��ᱻת���ɻس������з����ڴ����ļ��ϣ����������ϵ��ļ�ʱ�����ֻ�����洦�����ǰ��ļ��������Ļس������з�ת���ɻ��з���
//��ˣ��ڶ�ȡһ�������ļ�ʱ���ı���ʽ��ȡ���ļ����ݺ��п��ܻ�ȶ������ļ��̣���Ϊ�ı���ʽ��ȡҪ�ѻس������������ַ����һ���ַ����൱�ڽض����ļ�������Ϊʲô�����ǿ����أ���Ϊ���������в��������ŵ�45��42�������ֽڣ�45��CR�س���ASCII�룬42�ǻ��з�CL��ASCII�룩��Ҳ�Ͳ����ڡ��ض̡������ˣ���˶�����������һ���ġ�
//�������˵���ļ��ļ�(���ı���ʽд��)��������ı���ʽ�����������ļ�(�Զ����Ʒ�ʽд��)������Զ����Ʒ�ʽ������Ȼ���ܻ᲻��ȷ��������Ѿ������ˡ�
