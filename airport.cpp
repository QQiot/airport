#include<stdio.h>
#include<iostream>

using namespace std;
#define n 9  //������
#define ed 11  //����
#define maxsize 20  //�ߵ�����������
typedef char vextype;
typedef struct node1 {
    int adjvex;  //�ڽӵ����±꣩
    int dut;     //Ȩֵ
    struct node1 *next;  //����
} edgenode1;  //�߱���
typedef struct {
    vextype vertex;  //������Ϣ
    int id;      //���
    edgenode1 *link;  //�߱�ͷָ��
} vexnode1;//�������
void creatgraph(vexnode1 dig[]);//����AOE�����ڽӱ�
void criticalpath(vexnode1 dig[]);//dig��AOE���Ĵ�Ȩ�ڽӱ�
int main() {
    printf(" ����AOE���Ĺؼ�·������\n");
    vexnode1 dig[n];
    creatgraph(dig);
    printf("�����AOE�����ڽӱ�\n");
    int i;
    edgenode1 *s;
    for (i = 0; i < n; i++) {
        printf("|%c %d |", dig[i].vertex, dig[i].id);
        s = dig[i].link;
        while (s) {
            printf("����> %d %d", s->adjvex + 1, s->dut);
            s = s->next;
        }
        printf("\n");
    }
    printf("������ؼ����\n");
    printf("ÿ�зֱ�Ϊ��ʼ�¼��������¼������翪ʼʱ�䡢��ٿ�ʼʱ�����ɻ��ʱ������\n");
    criticalpath(dig);
}

void creatgraph(vexnode1 dig[])//����AOE�����ڽӱ�
{
    printf("������AOE���Ķ��㣨������Ϣ����ȣ��ÿո��������\n");
    int i;
    for (i = 0; i < n; i++) {
        printf("��%d���������Ϣ����ȣ�", i + 1);
        cin >> dig[i].vertex >> dig[i].id;
        dig[i].link = NULL;
    }
    printf("������AOE���ıߣ�ʼ�㡢�յ��Ȩ���ÿո��������\n");
    edgenode1 *s;
    int a, b;//��¼�ߵ�ʼ����յ�
    for (i = 0; i < ed; i++) {
        s = (edgenode1 *) malloc(sizeof(edgenode1));
        printf("��%d���ߵ�ʼ�㡢�յ��Ȩ��", i + 1);
        cin >> a >> b >> s->dut;
        s->adjvex = b - 1;
        s->next = dig[a - 1].link;
        dig[a - 1].link = s;
    }
}

void criticalpath(vexnode1 dig[])//dig��AOE���Ĵ�Ȩ�ڽӱ�
{
    int i, j, k, m;
    int front = -1, rear = -1;  //˳����е���λָ���ó�ֵ-1
    int tport[n], vl[n], ve[n];
    int l[maxsize], e[maxsize];
    edgenode1 *p;
    for (i = 0; i < n; i++)
        ve[i] = 0;    //��ʱ��v(i+1)�����緢��ʱ����ó�ֵ��
    for (i = 0; i < n; i++)//ɨ�趥��������Ϊ��Ķ������
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
    if (m < n)   //�����л�·����ֹ�㷨
    {
        printf("AOE���л�·\n");
        return;
    }
    for (i = 0; i < n; i++)//Ϊ���¼�v(i+1)����ٷ���ʱ��vl[i]�ó�ֵ
        vl[i] = ve[n - 1];
    for (i = n - 2; i >= 0; i--)//���������е�����ȡ����
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
    for (j = 0; j < n; j++)  //ɨ�趥�������ȡ����v(j+1)
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