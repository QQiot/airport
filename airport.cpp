
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
    printf("������ؼ����\n");
    printf("ÿ�зֱ�Ϊ��ʼ�¼��������¼������翪ʼʱ�䡢��ٿ�ʼʱ�����ɻ��ʱ������\n");
    criticalpath(dig,nodeNumNew);
    while(maxtime>0){
        timer(dig,nodeNumNew);
    }
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
    printf("������AOE���Ķ��㣨������Ϣ����ȣ��ÿո��������\n");
    int i;
    for (i = 0; i < nodeNumNew; i++) {
        printf("��%d���������Ϣ����ȣ�", i + 1);
        cin >> dig[i].vertex >> dig[i].id;
        dig[i].link = NULL;
    }
    printf("������AOE���ıߣ�ʼ�㡢�յ��Ȩ���ÿո��������\n");
    edgenode1 *s;
    int a, b;//��¼�ߵ�ʼ����յ�
    for (i = 0; i < edgNum; i++) {
        s = (edgenode1 *) malloc(sizeof(edgenode1));
        printf("��%d���ߵ�ʼ�㡢�յ��Ȩ��", i + 1);
        cin >> a >> b >> s->dut;
        s->adjvex = b - 1;
        s->next = dig[a - 1].link;
        dig[a - 1].link = s;
    }
}

void criticalpath(vexnode1 dig[],int nodeNumNew)//dig��AOE���Ĵ�Ȩ�ڽӱ�
{
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
    int i, j, k, m;
    int front = -1, rear = -1;  //˳����е���λָ���ó�ֵ-1
    int *tport = (int *)malloc(numNew*sizeof(int));

    for (i = 0; i < numNew; i++)//ɨ�趥��������Ϊ��Ķ������
        if (dig[i].id == 0)
            tport[++rear] = i;
    edgenode1 *p;
    edgenode1 *pBack;
    while (front != rear) //�ӷǿ�
    {
        front++;
        j = tport[front];  //v(j+1)���ӣ���ɾȥv(j+1)
        m++;    //�Գ��ӵĶ����������
        p = dig[j].link;  //pָ��v(j+1)Ϊ���ĳ��߱��б�����±�
        /*
        if(dig[j].vertex != 's'){
            printf("---�¼���%c�Ѿ���ʼ---\n",dig[j].vertex);
            dig[j].vertex = 's';
        }
        */
        int pNum = 0;
        while (p)   //ɾȥ������v(j+1)Ϊ���ĳ���
        {	pBack = p;
            p->dut--;
            /*
            if(p->dut==0){
                if(pNum == 0){
                        dig[j].link = p->next;
                    }else{
                        pBack->next = p->next;
                    }

                k = p->adjvex;//k�Ǳ�(v(j+1),v(k+1))�յ�v(k+1)���±�
                dig[k].id--;   //v(k+1)��ȼ�1
                if (dig[k].id == 0){
                    //TODO:

                }
            }
            */
            p = p->next;   //��v(j+1)����һ����
            pNum++;
        }
    }
    printf("������ؼ����\n");
    printf("ÿ�зֱ�Ϊ��ʼ�¼��������¼������翪ʼʱ�䡢��ٿ�ʼʱ�����ɻ��ʱ������\n");
    criticalpath(dig,nodeNumNew);

}



void read(FILE *fp)
{
    int row=0;
    char mid;
    while(!feof(fp))
    {
        mid=fgetc(fp); //��txt�ı��ж�ȡһ���ַ���ֵ��mid
        if(mid=='\n') //�������ַ�Ϊ���з�
            row++; //��¼txt��������
    }
    row++;  //���һ��û�л��з�
    printf("����Ϊ%d\n",row);
    rewind(fp); //���ļ���ʼλ��
}

int readmain()
{
    FILE *fp;
    double data[9][2] = {0.0};   //��ά����
    int index[9] = {0};
    //��ά�������±�
    double temp;
    int i, j;
    int count = 0;  //����������¼�Ѷ����ĸ�����
    if((fp=fopen(file_name, "rb")) == NULL) {
        printf("��ȷ���ļ�(%s)�Ƿ����!\n", file_name);
        exit(1);
    }
    read(fp);     //��ȡ����
    while(1==fscanf(fp, "%d", &temp)) //lf,le�����ԣ�����Ķ������ԣ�%eҲ����
    {
        data[(index[count%2])++][count%2] = temp;
        count++;
    }
    fclose(fp);   //�رվ��
    /******��������****************/
    for(i=0;i<9;i++)
    {   printf("��%d�����ݣ�\n",i+1);
        for(j=0;j<2;j++)
        {
            printf("%d ", data[i][j]);//
        }
        printf("\n");
    }

    return 0;
}