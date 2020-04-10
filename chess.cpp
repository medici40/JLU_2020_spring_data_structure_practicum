//国标不香吗
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
//学生后加的
#include<vector> // 因为老师允许使用stl
#include<algorithm>
#include <cstring>
#include <memory>

#define BLACK 0
#define WHITE 1
#define EMPTY 2
#define INF INT_MAX
#define nonSenseInt -1
#define re(i,a,b) for(int i=a;i<b;++i)
#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b
using namespace std;
int currentSize;//记录每次走法生成的个数

struct Point{ //点结构
	int x,y;
};
struct Point_1{ //点结构
	int x,y;
	int score;
};
struct Step{ //步结构
	Point first,second;
	int value;
};
int Board[19][19]; //存储棋盘信息，其元素值为 BLACK, WHITE, EMPTY 之一
int evaluate(int computerSide, int simuBoard[19][19] = Board);// 第一个参数表示我方棋颜色,第二个参数为棋盘当前状态(其默认参数为全局量Board)
int mySide=BLACK; // 方便起见,增加我方


bool hasNeighbor(int x, int y, int simuBoard[19][19] = Board){//存储合法的走法，
	int direction_x[7] = {0, 1, 2, 3, -1, -2, -3};
	int direction_y[7] = {0, 1, 2, 3, -1, -2, -3};
	for(int i  = 0; i < 7; i++)
		for(int  j = 1; j < 7; j++)
			if (simuBoard[x + direction_x[i]][y + direction_y[j]] != 2)
				return true;
	for(int  i = 1; i < 7; i++)
			if (simuBoard[x + direction_x[i]][y] != 2)
				return true;
	return false;
}

int getValue(int x, int y, int computerSide, int simuBoard[19][19] = Board){ // 棋盘修改为局部变量

}

bool sortByM1( const Step &v1, const Step &v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    return v1.value < v2.value;//升序排列
}

// 两步的
vector<Step>* generateMove(int computerSide, int simuBoard[19][19] = Board){
	// std::vector<Step>::iterator it;
	std::vector<Point_1> validCondition;
	std::vector<Point_1>::iterator ite;
    // for (it = moveCondition.begin(); it != moveCondition.end(); ){
    //     it = moveCondition.erase(it);
    // }
	// for (it_1 = validCondition.begin(); it_1 != validCondition.end(); ){
    //     it_1 = validCondition.erase(it_1);
    // }
	//寻找可以下的点
	vector<Step>* toReturn = new vector<Step>;
	for(int i  = 0; i < 19; i++)
		for(int  j = 1; j < 19; j++){
				if (simuBoard[i][j] == 2){
					if(hasNeighbor(i, j, simuBoard)){
						int temp = getValue(i, j, computerSide, simuBoard);
						Point_1 temp_struct;
						temp_struct.x = i;
						temp_struct.y = j;
						temp_struct.score = temp;
						validCondition.push_back(temp_struct);
					}
				}
		}
	//将可以下的点组合，要将两步映射为一步
	for(int i  = 0; i < validCondition.size(); i++)
		for(int  j = i + 1; j < validCondition.size(); j++){
			Step temp;
			temp.first.x = validCondition[i].x;
			temp.first.y = validCondition[i].y;
			temp.second.x = validCondition[j].x;
			temp.second.y = validCondition[j].y;
			temp.value = validCondition[i].score + validCondition[j].score;
			toReturn->push_back(temp);
		}
	//将toReturn里面的元素按评估分数升序排列
	std::sort(toReturn->begin(),toReturn->end(),sortByM1);
	return toReturn;
}


void copyStep(Step to, Step from){
    to.first.x = from.first.x;
    to.first.y = from.first.y;
    to.second.x = from.second.x;
    to.second.y = from.second.y;
}

int negaMax(int whosTurn, int depth, int alpha, int beta,int simuBoard[19][19]=Board){
    // TODO:出现平局、胜负情况下的判定返回,即没有child的情况
    if(depth == 0){ // 叶节点
        return (whosTurn==mySide?1:-1)*evaluate(whosTurn,simuBoard);
    }

    int highestScore = (-1)*INF;
    int negaMaxValue = nonSenseInt;
    auto subBoard = new int[19][19];
    memcpy(subBoard,simuBoard,sizeof(int[19][19]));
    vector<Step>* toMove = generateMove(whosTurn,simuBoard);
    re(i,0,(*toMove).size()){
		// move
        subBoard[(*toMove)[i].first.x][(*toMove)[i].first.y] = whosTurn;
        subBoard[(*toMove)[i].second.x][(*toMove)[i].second.y] = whosTurn;
        negaMaxValue = (-1)*negaMax(1-whosTurn, depth-1,(-1)*alpha,(-1)*beta,subBoard);
        highestScore = max(highestScore,negaMaxValue);
        alpha = max(alpha,negaMaxValue);
        if(alpha>=beta){ // 隐式剪枝
            return alpha;
        }
		// unmove
		subBoard[(*toMove)[i].first.x][(*toMove)[i].first.y] = EMPTY;
        subBoard[(*toMove)[i].second.x][(*toMove)[i].second.y] = EMPTY;
    }
    return highestScore;
}

Step aGoodStep(int depth){
    Step move;
    int highestScore = (-1)*INT_MAX;
    int alpha = (-1)*INF;
    int beta = INF;
    int possibleScore = nonSenseInt;
    Step candidateMove;
    auto simuBoard = new int[19][19];
    memcpy(simuBoard,Board,sizeof(int[19][19]));

    vector<Step>* toMove = generateMove(mySide,simuBoard);
    memcpy(simuBoard,Board,sizeof(Board));


    re(i,0,(*toMove).size()){
        // move
        simuBoard[(*toMove)[i].first.x][(*toMove)[i].first.y] = mySide;
        simuBoard[(*toMove)[i].second.x][(*toMove)[i].second.y] = mySide;
        possibleScore = (-1)*negaMax(1-mySide,depth,alpha,beta,simuBoard);
        if(possibleScore>highestScore){
            highestScore = possibleScore;
            copyStep(candidateMove,(*toMove)[i]);
        }
        // unmove
        simuBoard[(*toMove)[i].first.x][(*toMove)[i].first.y] = EMPTY;
        simuBoard[(*toMove)[i].second.x][(*toMove)[i].second.y] = EMPTY;
    }

    free(simuBoard);

}

int main()
{
	Step step;//临时步结构
	char message[256];//通信消息缓冲
    int computerSide;//己方执棋颜色
    int start=0;//对局开始标记
	srand(int(time(0)));
	//此处放置初始化代码
    //...
	while (1)	//程序主循环
	{
		fflush(stdout);//不要删除此语句，否则程序会出问题
		scanf("%s", message);//获取平台命令消息
        //分析命令
		if (strcmp(message, "name?") == 0)//向对战平台发送队名
		{
			fflush(stdin);
			/***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
			/*******/		printf("name Eureka\n");		/**只修改令狐冲，不要删除name空格****/
			/***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
		}
		else if (strcmp(message, "new") == 0)//建立新棋局,下先手棋
		{
			int i, j;
			scanf("%s", message);//获取己方执棋颜色
			fflush(stdin);
			if (strcmp(message, "black") == 0)	computerSide = BLACK;  //执黑
			else  computerSide = WHITE;   //执白

			for (i = 0; i<19; ++i)   //初始化棋局
				for (j = 0; j<19; ++j)
					Board[i][j] = EMPTY;
			start = 1;

			if (computerSide == BLACK)
			{
				/**********生成第一手着法，并保存在step结构中，落子坐标为(step.first.x,step.first.y)**********/
				/****************************在下方填充代码，并替换我的示例代码******************************/

				// 第一步落子其实没啥好换的,直接手动优化
				step.first.x = 9;
				step.first.y = 9;


				/******************************在上面填充第一步行棋代码*******************************************/

				Board[step.first.x][step.first.y] = computerSide;//处理己方行棋
				printf("move %c%c@@\n", step.first.x + 'A', step.first.y + 'A');//输出着法
			}
		}
		else if(strcmp(message,"move")==0)//行棋,本程序核心,正常下棋
		{
			scanf("%s", message);//获取对手行棋着法
            fflush(stdin); 
			step.first.x=message[0]-'A';		step.first.y=message[1]-'A';
			step.second.x=message[2]-'A';		step.second.y=message[3]-'A';
            //处理对手行棋
			Board[step.first.x][step.first.y] = 1 - computerSide;
			if(!(step.second.x==-1 && step.second.y==-1)) Board[step.second.x][step.second.y] = 1 - computerSide;

            /**********************************************************************************************************/
			/***生成落子的坐标，保存在step结构中，第1子下在(step.first.x,step.first.y)，第2子下在(step.first.x,step.first.y)*****/
			/**************************************在下方填充代码，并替换我的示例代码*****************************************/
			
			mySide = computerSide;
			//生成第1子落子位置step.first.x和step.first.y
			int x, y;
			x = rand() % 19; y = rand() % 19;
			while (Board[x][y] != EMPTY)
			{
				x = rand() % 19;	y = rand() % 19;
			}
			step.first.x = x;
			step.first.y = y;
			Board[step.first.x][step.first.y] = computerSide;

			//生成第2子落子位置step.second.x和step.second.y	
			x = rand() % 19;	y = rand() % 19;
			while (Board[x][y] != EMPTY)
			{
				x = rand() % 19;	y = rand() % 19;
			}
			step.second.x = x;
			step.second.y = y;
			Board[step.second.x][step.second.y] = computerSide;
			
			/*****************************************在上面填充代码******************************************************/
			/**********************************************************************************************************/

			printf("move %c%c%c%c\n",step.first.x+'A',step.first.y+'A',step.second.x+'A',step.second.y+'A');//输出着法
		}
        else if (strcmp(message, "error") == 0)//着法错误
        {
            fflush(stdin);
        }
		else if (strcmp(message, "end") == 0)//对局结束
		{
            fflush(stdin);
			start = 0;
		}
		else if (strcmp(message, "quit") == 0)//退出引擎
		{
            fflush(stdin);
			printf("Quit!\n");
			break;
		}
	}
	return 0;
}

 