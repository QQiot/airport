
#include<stdio.h>
#include<iostream>

using namespace std;
#define nodeNum 9  //顶点数
#define edgNum 11  //边数
#define maxsize 20  //边的最大可能条数


const char file_name[50] = "d:\\node.txt";

int maxtime = 0;

typedef struct node1 {
    int adjvex;  //邻接点域（下标）
    int dut;     //权值
    struct node1 *next;  //链域
} edgenode1;  //边表结点

typedef struct {
    char vertex;  //顶点信息
    int id;      //入度
    edgenode1 *link;  //边表头指针
} vexnode1;//顶点表结点

void creatgraph(vexnode1 dig[],int nodeNumNew);//建立AOE网的邻接表
void criticalpath(vexnode1 dig[],int nodeNumNew);//dig是AOE网的带权邻接表
void printGraph(vexnode1 dig[],int nodeNumNew);
void read(FILE *fp);
int readmain();
void timer(vexnode1 dig[],int nodeNumNew);

void main() {
    printf(" ——AOE网的关键路径——\n");
    vexnode1 dig[nodeNum];
    int nodeNumNew = sizeof(dig)/sizeof(dig[0]);
    creatgraph(dig,nodeNumNew);
    printGraph(dig,nodeNumNew);
    criticalpath(dig,nodeNumNew);

    timer(dig,nodeNumNew);

    system("pause");
}

void printGraph(vexnode1 dig[],int nodeNumNew){
    printf("【输出AOE网的邻接表】\n");
    int i;
    edgenode1 *s;
    for (i = 0; i < nodeNumNew; i++) {
        printf("|%c %d |", dig[i].vertex, dig[i].id);
        s = dig[i].link;
        while (s) {
            printf("——> %d %d", s->adjvex + 1, s->dut);
            s = s->next;
        }
        printf("\n");
    }
}

void creatgraph(vexnode1 dig[],int nodeNumNew)//建立AOE网的邻接表
{
    int i = 0;
    printf("【输入AOE网的顶点node.txt（顶点信息和入度，用空格隔开）】\n");
    char buffer[256]; //每行最多255个字符
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
        printf("D:\\node.txt不存在\n");
    }

    printf("【输入AOE网的边（始点、终点和权，用空格隔开）】\n");
    edgenode1 *s;
    int a, b, c;//记录边的始点和终点
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
        printf("D:\\edge.txt不存在\n");
    }
}

void criticalpath(vexnode1 dig[],int nodeNumNew)//dig是AOE网的带权邻接表
{

    printf("【输出关键活动】\n");
    printf("每行分别为开始事件、结束事件、最早开始时间、最迟开始时间和完成活动的时间余量\n");
    int numNew = nodeNumNew;
    int i, j, k, m;
    int front = -1, rear = -1;  //顺序队列的首位指针置初值-1
    int *tport = (int *)malloc(numNew*sizeof(int));
    int *vl = (int *)malloc(numNew*sizeof(int));
    int *ve = (int *)malloc(numNew*sizeof(int));
    int l[maxsize], e[maxsize];
    edgenode1 *p;
    for (i = 0; i < numNew; i++)
        ve[i] = 0;    //各时间v(i+1)的最早发生时间均置初值零
    for (i = 0; i < numNew; i++)//扫描顶点表，将入度为零的顶点入队
        if (dig[i].id == 0)
            tport[++rear] = i;
    m = 0;  //计数器初始化
    while (front != rear) //队非空
    {
        front++;
        j = tport[front];  //v(j+1)出队，即删去v(j+1)
        m++;    //对出队的顶点个数计数
        p = dig[j].link;  //p指向v(j+1)为起点的出边表中表结点的下标
        while (p)   //删去所有以v(j+1)为起点的出边
        {
            k = p->adjvex;//k是边(v(j+1),v(k+1))终点v(k+1)的下标
            dig[k].id--;   //v(k+1)入度减1
            if (ve[j] + p->dut > ve[k])
                ve[k] = ve[j] + p->dut;   //修改ve[k]
            if (dig[k].id == 0)
                tport[++rear] = k;  //新的入度为零的顶点v(k+1)入队
            p = p->next;   //找v(j+1)的下一条边
        }
    }
    if (m < numNew)   //网中有回路，终止算法
    {
        printf("AOE网有回路\n");
        return;
    }
    maxtime = ve[numNew - 1];
    for (i = 0; i < numNew; i++)//为各事件v(i+1)的最迟发生时间vl[i]置初值
        vl[i] = ve[numNew - 1];
    for (i = numNew - 2; i >= 0; i--)//按拓扑序列的逆序取顶点
    {
        j = tport[i];
        p = dig[j].link;  //取v(j+1)的出边表上第一个表结点
        while (p) {
            k = p->adjvex;  //k为(v(j+1),v(k+1))的终点v(k+1)的下标
            if ((vl[k] - p->dut) < vl[j])
                vl[j] = vl[k] - p->dut;  //修改vl[j]
            p = p->next;   //找v(j+1)的下一条边
        }
    }
    i = 0;//边计数器置初值
    for (j = 0; j < numNew; j++)  //扫描顶点表，依次取顶点v(j+1)
    {
        p = dig[j].link;
        while (p)   //扫描顶点的v(j+1)的出边表
        {//计算各边(v(j+1),v(k+1))所代表的活动a(i+1)的e[i]和l[i]
            k = p->adjvex;
            e[++i] = ve[j];
            l[i] = vl[k] - p->dut;
            printf("%c\t%c\t%d\t%d\t%d\t",//输出活动a(i+1)的有关信息
                   dig[j].vertex, dig[k].vertex, e[i], l[i], l[i] - e[i]);
            if (l[i] == e[i])//关键活动
                printf("关键活动");
            printf("\n");
            p = p->next;
        }
    }
}



void timer(vexnode1 dig[],int nodeNumNew)//dig是AOE网的带权邻接表
{
    int numNew = nodeNumNew;
    int k;
    int firstNode = 0;
    edgenode1 *p;
    edgenode1 *pBack;

    p = dig[firstNode].link;

    if(dig[firstNode].vertex != 's'){
        printf("---事件：%c已经开始---\n",dig[firstNode].vertex);
        dig[firstNode].vertex = 's';
    }

    int pNum = 0;//用于标示是不是第一个边节点
    while (p)
    {
        p->dut--;
        if(p->dut==firstNode){
            if(pNum == firstNode){
                dig[firstNode].link = p->next;
            }else{
                pBack->next = p->next;
            }
            k = p->adjvex;//k是边(v(j+1),v(k+1))终点v(k+1)的下标
            dig[k].id--;   //v(k+1)入度减1
        }
        pBack = p;
        p = p->next;
        pNum++;
    }

    printGraph(dig,nodeNumNew);
    criticalpath(dig,nodeNumNew);

}


