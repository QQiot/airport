#include<stdio.h>
#include<iostream>

using namespace std;
#define n 9  //顶点数
#define ed 11  //边数
#define maxsize 20  //边的最大可能条数
typedef char vextype;
typedef struct node1 {
    int adjvex;  //邻接点域（下标）
    int dut;     //权值
    struct node1 *next;  //链域
} edgenode1;  //边表结点
typedef struct {
    vextype vertex;  //顶点信息
    int id;      //入度
    edgenode1 *link;  //边表头指针
} vexnode1;//顶点表结点
void creatgraph(vexnode1 dig[]);//建立AOE网的邻接表
void criticalpath(vexnode1 dig[]);//dig是AOE网的带权邻接表
int main() {
    printf(" ——AOE网的关键路径——\n");
    vexnode1 dig[n];
    creatgraph(dig);
    printf("【输出AOE网的邻接表】\n");
    int i;
    edgenode1 *s;
    for (i = 0; i < n; i++) {
        printf("|%c %d |", dig[i].vertex, dig[i].id);
        s = dig[i].link;
        while (s) {
            printf("——> %d %d", s->adjvex + 1, s->dut);
            s = s->next;
        }
        printf("\n");
    }
    printf("【输出关键活动】\n");
    printf("每行分别为开始事件、结束事件、最早开始时间、最迟开始时间和完成活动的时间余量\n");
    criticalpath(dig);
}

void creatgraph(vexnode1 dig[])//建立AOE网的邻接表
{
    printf("【输入AOE网的顶点（顶点信息和入度，用空格隔开）】\n");
    int i;
    for (i = 0; i < n; i++) {
        printf("第%d个顶点的信息和入度：", i + 1);
        cin >> dig[i].vertex >> dig[i].id;
        dig[i].link = NULL;
    }
    printf("【输入AOE网的边（始点、终点和权，用空格隔开）】\n");
    edgenode1 *s;
    int a, b;//记录边的始点和终点
    for (i = 0; i < ed; i++) {
        s = (edgenode1 *) malloc(sizeof(edgenode1));
        printf("第%d条边的始点、终点和权：", i + 1);
        cin >> a >> b >> s->dut;
        s->adjvex = b - 1;
        s->next = dig[a - 1].link;
        dig[a - 1].link = s;
    }
}

void criticalpath(vexnode1 dig[])//dig是AOE网的带权邻接表
{
    int i, j, k, m;
    int front = -1, rear = -1;  //顺序队列的首位指针置初值-1
    int tport[n], vl[n], ve[n];
    int l[maxsize], e[maxsize];
    edgenode1 *p;
    for (i = 0; i < n; i++)
        ve[i] = 0;    //各时间v(i+1)的最早发生时间均置初值零
    for (i = 0; i < n; i++)//扫描顶点表，将入度为零的顶点入队
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
    if (m < n)   //网中有回路，终止算法
    {
        printf("AOE网有回路\n");
        return;
    }
    for (i = 0; i < n; i++)//为各事件v(i+1)的最迟发生时间vl[i]置初值
        vl[i] = ve[n - 1];
    for (i = n - 2; i >= 0; i--)//按拓扑序列的逆序取顶点
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
    for (j = 0; j < n; j++)  //扫描顶点表，依次取顶点v(j+1)
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