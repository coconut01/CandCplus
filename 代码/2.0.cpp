#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STU_NUM 100	//最多人数
#define TEST_NUM 35	//题目数量

struct STU
{
	char name[10];	//姓名
	int ID_num;	//学号
	char ans[TEST_NUM + 1];	//答案
	int grade;	//分数
};

//定义全局变量
STU stu[STU_NUM]; 
int stu_num = 0;	//统计总人数
int get_ans[TEST_NUM][5];	//二维数组存储每题四个选项出现的频次，其中部分答案有误如X，所以有五种答案
char get_answer[TEST_NUM];	//参考答案
int MaxGrade, MinGrade;	//	最高分、最低分

//载入数据
void LoadData()	{
	FILE *fp;
	char buffer[100];
	
	if((fp = fopen("sample.txt","r")) == NULL) {
		printf("没有找到或者不能打开文件！");
		return;
	}
	memset(buffer, 0, sizeof(buffer));
	
	while ( fgets(buffer, 100, fp ) != NULL) {
		sscanf(buffer, "%d %s %s", &stu[stu_num].ID_num, stu[stu_num].name, stu[stu_num].ans);
		memset(buffer, 0, sizeof(buffer));
		stu_num++;	//统计学生总数
	}
	fclose(fp);
}

//根据每个学生的选择得到参考答案
void GetAnswer()
{
	memset(get_ans, 0, sizeof(get_ans));	//使用memset函数使数组元素置零
	memset(get_answer, 0, sizeof(get_answer));
	
	for (int i = 0; i < stu_num; i++)	//遍历所有人的答案
		for (int j = 0; j < TEST_NUM; j++){
			int tmp = stu[i].ans[j] - 'A';	//体会一下什么意思？？？将A、B、C、D转换成1、2、3、4
			get_ans[j][tmp]++;	//二维数组存储四个选项出现的频率
		}

	for (int i = 0; i < TEST_NUM; i++) {
		int max = 0, index = 0;
		for (int j = 0; j < 5; j++)
			if( get_ans[i][j] > max ) {
				max = get_ans[i][j];
				index = j;
			}
			get_answer[i] = 'A'+ index;	//存放参考答案的数组
	}
}

//打印参考答案及频次
void PrintAnswer()
{
	GetAnswer();
	printf("参考答案以及频次如下:\n");
	printf("%s\t %s\t %s\n", "题号", "答案", "频次");
	for (int i = 0; i < TEST_NUM; i++)
		printf("%d\t %c\t %d\n", i + 1, get_answer[i], get_ans[i][get_answer[i]- 'A']);

	printf("\n");
}

//根据参考答案以及学生自己的选择得到分数
void GetGrade()
{
	GetAnswer();
	for ( int i = 0; i < stu_num; i++ ) {
		stu[i].grade = 0;
		for ( int j = 0; j < TEST_NUM; j++ )
			if (get_answer[j] == stu[i].ans[j]) {	//将学生答案与参考答案进行对比
				if (j < 15)
					stu[i].grade += 2;	//根据不同的分值得到总成绩
				else if (j<25)
					stu[i].grade += 3;
				else
					stu[i].grade += 4;
			}
	}
}

//排序
void Sort()
{
	GetGrade();
	STU temp;
	for (int i = 0; i < stu_num; i++)	//冒泡排序
		for (int j = 0; j < stu_num - i; j++)
			if (stu[j].grade < stu[j + 1].grade){
				temp = stu[j + 1];
				stu[j + 1] = stu[j];
				stu[j] =temp;
			}
	MaxGrade = stu[0].grade;	//排序后第一个即为最高分
	MinGrade = stu[stu_num - 1].grade;	//排序后最后一个即为最低分
}

//打印排序结果
void PrintSort()
{
	Sort();
	printf("%s\t %s\t %s\t\t %s\n", "排名", "学号", "姓名", "分数");
	for (int i = 0; i < stu_num; i++)
		printf("%d\t %d\t %s\t\t %d\n", i+1, stu[i].ID_num, stu[i].name, stu[i].grade);
}

//按姓名查找
void SearchByName()
{
	GetGrade();
	char name[10];
	printf("请输入你要查询的名字\n");
	scanf("%s", name);
	
	for (int i = 0; i <stu_num; i++)
		if(strcmp(name, stu[i].name) == 0){
			printf("%s成绩为:%d\n\n", name, stu[i].grade);
			return;
		}
	printf("该名字不在名单中！\n\n");
}

//输出分析到文件
void OutputData()
{
	Sort();
	FILE *fp;
	int stu_Grade[5];	//用于存放每个分段的人数
	int TotalScore = 0;	//每个学生分数的累计和
	memset(&stu_Grade, 0, sizeof(stu_Grade));
	
	fp = fopen("ExamResult.txt", "w");
	
	fprintf(fp, "%s\t %s\t\t %s\n", "排名","姓名","分数");
	for (int i = 0; i < stu_num; i++) {
		fprintf(fp, "%d\t %s\t\t %d\n", i+1, stu[i].name, stu[i].grade);
		TotalScore += stu[i].grade;	//统计总分数
		
		if (stu[i].grade < 60)		//统计各个分数段人数
			stu_Grade[0]++;
		else if (stu[i].grade < 70)
			stu_Grade[1]++;
		else if	(stu[i].grade < 80)
			stu_Grade[2]++;
		else if (stu[i].grade < 90)
			stu_Grade[3]++;
		else
			stu_Grade[4]++;
	}
	
	fprintf( fp, "\n各分数段人数如下：\n"
		"0~59分的人数为:%d\n"
		"60~69分的人数为:%d\n"
		"70~79分的人数为:%d\n"
		"80~89分的人数为:%d\n"
		"90~100分的人数为:%d\n", stu_Grade[0], stu_Grade[1], stu_Grade[2], stu_Grade[3], stu_Grade[4]);
	fprintf( fp,
		"最高分为:%d\n"
		"最低分为:%d\n" 
		"平均分为:%.2f\n"
		"及格率为:%.2f\n" ,
		MaxGrade, MinGrade, (double)TotalScore / stu_num, (double)(stu_Grade[1] + stu_Grade[2] + stu_Grade[3] + stu_Grade[4]) / stu_num );
	printf("\n输出完成!\n\n");
	fclose(fp);
}

//程序主菜单
void ShowMenu()
{
	int choise, flag = 1;
	
	while(flag){
		puts("********************************************************************************");
		puts("〓〓〓〓〓〓〓〓〓〓  ☆    欢 迎 使 用 改 卷 系 统     ☆  〓〓〓〓〓〓〓〓〓〓");
		puts("〓〓〓〓〓〓〓★★★★★        ★★★★★★★★        ★★★★★〓〓〓〓〓〓〓");
		puts("〓〓〓〓〓〓〓〓〓★  ☆         1.显示参考答案         ☆  ★〓〓〓〓〓〓〓〓〓");
		puts("〓〓〓〓〓〓〓〓〓★  ☆         2.显示学生排名         ☆  ★〓〓〓〓〓〓〓〓〓");
		puts("〓〓〓〓〓〓〓〓〓★  ☆         3.查询学生成绩         ☆  ★〓〓〓〓〓〓〓〓〓");
		puts("〓〓〓〓〓〓〓〓〓★  ☆         4.输出试卷分析         ☆  ★〓〓〓〓〓〓〓〓〓");
		puts("〓〓〓〓〓〓〓〓〓★  ☆         5.退出改卷系统         ☆  ★〓〓〓〓〓〓〓〓〓");
		
		puts("\n请选择功能（输入1-5）：");
		scanf("%d", &choise);

		if( 0 < choise && choise < 6 ){
			switch(choise) {
			case 1 :
				PrintAnswer();
				break;
			case 2:
				PrintSort();
				break;
			case 3:											       
				SearchByName();
				break;
			case 4:
				OutputData();
				break;
			case 5:
				flag = 0;
				break;
			}
		}
		else
			printf("输入有误，请重新输入1-5之间的整数！\n");
	}	
}

int main()
{
	LoadData();
	ShowMenu();
	return 0;
}
