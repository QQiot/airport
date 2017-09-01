
#include<stdio.h>
#include<iostream>

using namespace std;
#define nodeNum 9  //������
#define edgNum 11  //����
#define maxsize 20  //�ߵ�����������


const char file_name[50] = "d:\\node.txt";

int maxtime = 0;

typedef struct node1 {
    int adjvex;  //�ڽӵ����±꣩
    int dut;     //Ȩֵ
    struct node1 *next;  //����
} edgenode1;  //�߱���

typedef struct {
    char vertex;  //������Ϣ
    int id;      //���
    edgenode1 *link;  //�߱�ͷָ��
} vexnode1;//�������

void creatgraph(vexnode1 dig[],int nodeNumNew);//����AOE�����ڽӱ�
void criticalpath(vexnode1 dig[],int nodeNumNew);//dig��AOE���Ĵ�Ȩ�ڽӱ�
void printGraph(vexnode1 dig[],int nodeNumNew);
void read(FILE *fp);
int readmain();
void timer(vexnode1 dig[],int nodeNumNew);

void main() {
    printf(" ����AOE���Ĺؼ�·������\n");
    vexnode1 dig[nodeNum];
    int nodeNumNew = sizeof(dig)/sizeof(dig[0]);
    creatgraph(dig,nodeNumNew);
    printGraph(dig,nodeNumNew);
    criticalpath(dig,nodeNumNew);

    timer(dig,nodeNumNew);

    system("pause");
}

void printGraph(vexnode1 dig[],int nodeNumNew){
    printf("�����AOE�����ڽӱ�\n");
    int i;
    edgenode1 *s;
    for (i = 0; i < nodeNumNew; i++) {
        printf("|%c %d |", dig[i].vertex, dig[i].id);
        s = dig[i].link;
        while (s) {
            printf("����> %d %d", s->adjvex + 1, s->dut);
            s = s->next;
        }
        printf("\n");
    }
}

void creatgraph(vexnode1 dig[],int nodeNumNew)//����AOE�����ڽӱ�
{
    int i = 0;
    printf("������AOE���Ķ���node.txt��������Ϣ����ȣ��ÿո��������\n");
    char buffer[256]; //ÿ�����255���ַ�
    FILE *fp;
    if ( fp=fopen("D:\\node.txt","rb") )
    {
        while ( !feof(fp)&&i<nodeNumNew )
        { fgets(buffer,255,fp);
            sscanf(buffer,"%c %d",&dig[i].vertex , &dig[i].id);
            dig[i].link = NULL;
            i++;
        }
    }else{
        printf("D:\\node.txt������\n");
    }

    printf("������AOE���ıߣ�ʼ�㡢�յ��Ȩ���ÿո��������\n");
    edgenode1 *s;
    int a, b, c;//��¼�ߵ�ʼ����յ�
    if ( fp=fopen("D:\\edge.txt","rb") )
    {
        for (i = 0; !feof(fp)&&i < edgNum; i++) {
            s = (edgenode1 *) malloc(sizeof(edgenode1));
            fgets(buffer,255,fp);
            sscanf(buffer,"%d %d %d",&a , &b, &c);
            s->dut = c;
            s->adjvex = b - 1;
            s->next = dig[a - 1].link;
            dig[a - 1].link = s;
        }
    }else{
        printf("D:\\edge.txt������\n");
    }
}

void criticalpath(vexnode1 dig[],int nodeNumNew)//dig��AOE���Ĵ�Ȩ�ڽӱ�
{

    printf("������ؼ����\n");
    printf("ÿ�зֱ�Ϊ��ʼ�¼��������¼������翪ʼʱ�䡢��ٿ�ʼʱ�����ɻ��ʱ������\n");
    int numNew = nodeNumNew;
    int i, j, k, m;
    int front = -1, rear = -1;  //˳����е���λָ���ó�ֵ-1
    int *tport = (int *)malloc(numNew*sizeof(int));
    int *vl = (int *)malloc(numNew*sizeof(int));
    int *ve = (int *)malloc(numNew*sizeof(int));
    int l[maxsize], e[maxsize];
    edgenode1 *p;
    for (i = 0; i < numNew; i++)
        ve[i] = 0;    //��ʱ��v(i+1)�����緢��ʱ����ó�ֵ��
    for (i = 0; i < numNew; i++)//ɨ�趥��������Ϊ��Ķ������
        if (dig[i].id == 0)
            tport[++rear] = i;
    m = 0;  //��������ʼ��
    while (front != rear) //�ӷǿ�
    {
        front++;
        j = tport[front];  //v(j+1)���ӣ���ɾȥv(j+1)
        m++;    //�Գ��ӵĶ����������
        p = dig[j].link;  //pָ��v(j+1)Ϊ���ĳ��߱��б�����±�
        while (p)   //ɾȥ������v(j+1)Ϊ���ĳ���
        {
            k = p->adjvex;//k�Ǳ�(v(j+1),v(k+1))�յ�v(k+1)���±�
            dig[k].id--;   //v(k+1)��ȼ�1
            if (ve[j] + p->dut > ve[k])
                ve[k] = ve[j] + p->dut;   //�޸�ve[k]
            if (dig[k].id == 0)
                tport[++rear] = k;  //�µ����Ϊ��Ķ���v(k+1)���
            p = p->next;   //��v(j+1)����һ����
        }
    }
    if (m < numNew)   //�����л�·����ֹ�㷨
    {
        printf("AOE���л�·\n");
        return;
    }
    maxtime = ve[numNew - 1];
    for (i = 0; i < numNew; i++)//Ϊ���¼�v(i+1)����ٷ���ʱ��vl[i]�ó�ֵ
        vl[i] = ve[numNew - 1];
    for (i = numNew - 2; i >= 0; i--)//���������е�����ȡ����
    {
        j = tport[i];
        p = dig[j].link;  //ȡv(j+1)�ĳ��߱��ϵ�һ������
        while (p) {
            k = p->adjvex;  //kΪ(v(j+1),v(k+1))���յ�v(k+1)���±�
            if ((vl[k] - p->dut) < vl[j])
                vl[j] = vl[k] - p->dut;  //�޸�vl[j]
            p = p->next;   //��v(j+1)����һ����
        }
    }
    i = 0;//�߼������ó�ֵ
    for (j = 0; j < numNew; j++)  //ɨ�趥�������ȡ����v(j+1)
    {
        p = dig[j].link;
        while (p)   //ɨ�趥���v(j+1)�ĳ��߱�
        {//�������(v(j+1),v(k+1))������Ļa(i+1)��e[i]��l[i]
            k = p->adjvex;
            e[++i] = ve[j];
            l[i] = vl[k] - p->dut;
            printf("%c\t%c\t%d\t%d\t%d\t",//����a(i+1)���й���Ϣ
                   dig[j].vertex, dig[k].vertex, e[i], l[i], l[i] - e[i]);
            if (l[i] == e[i])//�ؼ��
                printf("�ؼ��");
            printf("\n");
            p = p->next;
        }
    }
}



void timer(vexnode1 dig[],int nodeNumNew)//dig��AOE���Ĵ�Ȩ�ڽӱ�
{
    int numNew = nodeNumNew;
    int k;
    int firstNode = 0;
    edgenode1 *p;
    edgenode1 *pBack;

    p = dig[firstNode].link;

    if(dig[firstNode].vertex != 's'){
        printf("---�¼���%c�Ѿ���ʼ---\n",dig[firstNode].vertex);
        dig[firstNode].vertex = 's';
    }

    int pNum = 0;//���ڱ�ʾ�ǲ��ǵ�һ���߽ڵ�
    while (p)
    {
        p->dut--;
        if(p->dut==firstNode){
            if(pNum == firstNode){
                dig[firstNode].link = p->next;
            }else{
                pBack->next = p->next;
            }
            k = p->adjvex;//k�Ǳ�(v(j+1),v(k+1))�յ�v(k+1)���±�
            dig[k].id--;   //v(k+1)��ȼ�1
        }
        pBack = p;
        p = p->next;
        pNum++;
    }

    printGraph(dig,nodeNumNew);
    criticalpath(dig,nodeNumNew);

}


