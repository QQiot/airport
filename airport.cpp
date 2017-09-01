
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
    printf("【输出关键活动】\n");
    printf("每行分别为开始事件、结束事件、最早开始时间、最迟开始时间和完成活动的时间余量\n");
    criticalpath(dig,nodeNumNew);
    while(maxtime>0){
        timer(dig,nodeNumNew);
    }
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
    printf("【输入AOE网的顶点（顶点信息和入度，用空格隔开）】\n");
    int i;
    for (i = 0; i < nodeNumNew; i++) {
        printf("第%d个顶点的信息和入度：", i + 1);
        cin >> dig[i].vertex >> dig[i].id;
        dig[i].link = NULL;
    }
    printf("【输入AOE网的边（始点、终点和权，用空格隔开）】\n");
    edgenode1 *s;
    int a, b;//记录边的始点和终点
    for (i = 0; i < edgNum; i++) {
        s = (edgenode1 *) malloc(sizeof(edgenode1));
        printf("第%d条边的始点、终点和权：", i + 1);
        cin >> a >> b >> s->dut;
        s->adjvex = b - 1;
        s->next = dig[a - 1].link;
        dig[a - 1].link = s;
    }
}

void criticalpath(vexnode1 dig[],int nodeNumNew)//dig是AOE网的带权邻接表
{
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
    int i, j, k, m;
    int front = -1, rear = -1;  //顺序队列的首位指针置初值-1
    int *tport = (int *)malloc(numNew*sizeof(int));

    for (i = 0; i < numNew; i++)//扫描顶点表，将入度为零的顶点入队
        if (dig[i].id == 0)
            tport[++rear] = i;
    edgenode1 *p;
    edgenode1 *pBack;
    while (front != rear) //队非空
    {
        front++;
        j = tport[front];  //v(j+1)出队，即删去v(j+1)
        m++;    //对出队的顶点个数计数
        p = dig[j].link;  //p指向v(j+1)为起点的出边表中表结点的下标
        /*
        if(dig[j].vertex != 's'){
            printf("---事件：%c已经开始---\n",dig[j].vertex);
            dig[j].vertex = 's';
        }
        */
        int pNum = 0;
        while (p)   //删去所有以v(j+1)为起点的出边
        {	pBack = p;
            p->dut--;
            /*
            if(p->dut==0){
                if(pNum == 0){
                        dig[j].link = p->next;
                    }else{
                        pBack->next = p->next;
                    }

                k = p->adjvex;//k是边(v(j+1),v(k+1))终点v(k+1)的下标
                dig[k].id--;   //v(k+1)入度减1
                if (dig[k].id == 0){
                    //TODO:

                }
            }
            */
            p = p->next;   //找v(j+1)的下一条边
            pNum++;
        }
    }
    printf("【输出关键活动】\n");
    printf("每行分别为开始事件、结束事件、最早开始时间、最迟开始时间和完成活动的时间余量\n");
    criticalpath(dig,nodeNumNew);

}



void read(FILE *fp)
{
    int row=0;
    char mid;
    while(!feof(fp))
    {
        mid=fgetc(fp); //从txt文本中读取一个字符赋值给mid
        if(mid=='\n') //如果这个字符为换行符
            row++; //记录txt数据行数
    }
    row++;  //最后一行没有换行符
    printf("行数为%d\n",row);
    rewind(fp); //回文件起始位置
}

int readmain()
{
    FILE *fp;
    double data[9][2] = {0.0};   //二维数组
    int index[9] = {0};
    //二维数组行下标
    double temp;
    int i, j;
    int count = 0;  //计数器，记录已读出的浮点数
    if((fp=fopen(file_name, "rb")) == NULL) {
        printf("请确认文件(%s)是否存在!\n", file_name);
        exit(1);
    }
    read(fp);     //读取行数
    while(1==fscanf(fp, "%d", &temp)) //lf,le都可以，但别的都不可以，%e也不行
    {
        data[(index[count%2])++][count%2] = temp;
        count++;
    }
    fclose(fp);   //关闭句柄
    /******处理数据****************/
    for(i=0;i<9;i++)
    {   printf("第%d行数据：\n",i+1);
        for(j=0;j<2;j++)
        {
            printf("%d ", data[i][j]);//
        }
        printf("\n");
    }

    return 0;
}