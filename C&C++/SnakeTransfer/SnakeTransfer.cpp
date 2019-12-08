#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define N 25
#include <math.h>
//背景色11
//前景色：蛇—— 14；苹果——12
 
int apple_current[3];
int apple_new[2];
char score[3];
char tail[3]; 
int nextsectionwarning=1;
int Va=1;//设置第二关中苹果的速度 
int lastone=2;

//函数声明
void gotoxy(int , int );
void color(int );
void BootAnimation();
int Block(char) ;
int HitSelf(char **,int);
int Eat(char,int);
void Draw(char **, int,int);
char** Move(char **, char *,char, int *,int *);
void init(char , char ***, int *);
void Manual();
int File_in();
int File_out();
void Free(char **, int);
char SnakeDirection(char,char,char);




 
void gotoxy(int x, int y)    //输出坐标 
{
        COORD pos; 
        pos.X = x; 
        pos.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void color(int b)         //颜色函数 
{
    HANDLE hConsole = GetStdHandle((STD_OUTPUT_HANDLE)) ; 
    SetConsoleTextAttribute(hConsole,b) ;
} 

void BootAnimation(){
	int i,j;
	PlaySound("Initial.wav", NULL, SND_FILENAME | SND_ASYNC);
	color(14);
	for(i=0;i<N;i++){
		//画上下两边 
		gotoxy(i*2,i);
		for(j=0;j<N-i*2;j++)
			printf("■");
		gotoxy(i*2,N-1-i);
		for(j=0;j<N-i*2;j++)
			printf("■");
		Sleep(100);
	}
	Sleep(12000);
} 

int Block(char head[2])   //判断出界 
{
        if ((head[0] < 1) || (head[0] > N) || (head[1] < 1) || (head[1] > N))
                return 1;
        return 0;
}

int HitSelf(char **snake,int len)	//判断撞到自己 
{	int hit=0;
    if(len<5){
    	return hit;
	}   
    else
    {
	int i;
    for(i=4;i<len;i++){
    	if(snake[0][0]==snake[i][0]&&snake[0][1]==snake[i][1])
    	{
    		hit=1;
    		system("pause");
    		break;
		}
	}
	return hit;
}
}
	
                      
int Eat(char snake[2],int section2)   //吃了苹果 
{		if(section2){
			if((snake[0] == apple_new[0]) && (snake[1] == apple_new[1])){
				PlaySound("eat3.wav",NULL,SND_FILENAME | SND_ASYNC |SND_NOSTOP);
				gotoxy(N+2, N+2);
                puts("你输了");
                File_out();
                getche();
                exit(0); 
			}
		}
		else{
			if ((snake[0] == apple_current[0]) && (snake[1] == apple_current[1])){
				PlaySound("eat3.wav",NULL,SND_FILENAME | SND_ASYNC |SND_NOSTOP);
                apple_current[2] = 0;
                gotoxy((N+12)*2,12);
                color(13);
                ++score[0];
                printf("%d",score[0]*10);
                color(11);
                return 1;
        	}
		}
        return 0;
}

void Draw(char **snake, int len,int section2)    //蛇移动 
{
        if(section2){
        	gotoxy(apple_current[0]*2,apple_current[1]);//将之前的位置上的苹果擦除
        	color(11); 
			printf("■"); 
			gotoxy(apple_new[0]*2,apple_new[1]);//在新的位置画出苹果
			color(12); 
			printf("●");
			color(11);
			apple_current[0]=apple_new[0];//更新苹果的当前位置 
			apple_current[1]=apple_new[1];
		}
		else{
			if (apple_current[2]) {
                gotoxy(apple_current[0] * 2, apple_current[1]);
                color(12);
                printf("●");
                color(11);
        	}
    	}
        gotoxy(tail[0] * 2, tail[1]);
        if (tail[2]) //保留尾巴 
         {  
		 	color(14);
        	printf("★");
            color(11);
         }
    	else 
    	if(tail[0]&&tail[1])
        printf("■");//丢掉尾巴 
        gotoxy(snake[0][0] * 2, snake[0][1]);
        color(14);
        printf("★");//画出头 
        color(11);
        putchar('\n');
}

char** Move(char **snake, char *dir_current,char dir_new, int *len,int *section2)   //控制方向 
{		int Section2=*section2; 
        int i, eat = Eat(snake[0],Section2);
        char CurrentDir=*dir_current;
        memcpy(tail, snake[(*len)-1], 2);
        for (i=(*len)-1;i>0;--i) 
                memcpy(snake[i], snake[i-1], 2);
        switch (dir_new) 	//根据输入更改运动方向，注意：新的运动方向不可以当前运动方向相反，即若当前为上，则新的方向不可以为下，否则保持原运动方向 
          { 
           case 'w': case 'W':
		   		if(!(CurrentDir=='s'||CurrentDir=='S')){
           			--snake[0][1];
           			if(dir_new!=CurrentDir)
			   		PlaySound("move.wav",NULL,SND_FILENAME | SND_ASYNC) ;
					*dir_current=dir_new;
		   		}
				else {
					switch(*dir_current){
						case 'w': case 'W':--snake[0][1];break;
						case 's': case 'S':++snake[0][1];break;
						case 'a': case 'A':--snake[0][0];break;
						case 'd': case 'D':++snake[0][0];break;
						default:;
					}
				}break;
        	case 's': case 'S':
				if(!(CurrentDir=='w'||CurrentDir=='W')){
        			++snake[0][1];
					if(dir_new!=CurrentDir)
					PlaySound("move.wav",NULL,SND_FILENAME | SND_ASYNC) ;
					*dir_current=dir_new;
				}
				else {
					switch(*dir_current){
						case 'w': case 'W':--snake[0][1];break;
						case 's': case 'S':++snake[0][1];break;
						case 'a': case 'A':--snake[0][0];break;
						case 'd': case 'D':++snake[0][0];break; 
						default:;
					}
				}break;
           case 'a': case 'A':
		   		if(!(CurrentDir=='d'||CurrentDir=='D')){
		   			--snake[0][0];
					if(dir_new!=CurrentDir)
					PlaySound("move.wav",NULL,SND_FILENAME | SND_ASYNC) ;
					*dir_current=dir_new;
				}
				else {
					switch(*dir_current){
						case 'w': case 'W':--snake[0][1];break;
						case 's': case 'S':++snake[0][1];break;
						case 'a': case 'A':--snake[0][0];break;
						case 'd': case 'D':++snake[0][0];break;
						default:;
					}
				}break;
           case 'd': case 'D':
		   		if(!(CurrentDir=='a'||CurrentDir=='A')){
		   			++snake[0][0];
					if(dir_new!=CurrentDir)
					PlaySound("move.wav",NULL,SND_FILENAME | SND_ASYNC) ;
					*dir_current=dir_new;
				}
				else {
					switch(*dir_current){
						case 'w': case 'W':--snake[0][1];break;
						case 's': case 'S':++snake[0][1];break;
						case 'a': case 'A':--snake[0][0];break;
						case 'd': case 'D':++snake[0][0];break;
						default:;
					}
				}break;
           default: switch(*dir_current){	//如果输入非 “W A S D "或非 ”w a s d“，则保持原运动方向 
			case 'w': case 'W':--snake[0][1];break;
			case 's': case 'S':++snake[0][1];break;
			case 'a': case 'A':--snake[0][0];break;
			case 'd': case 'D':++snake[0][0];break;
			default:;
		   };
         }  
        if (eat)   
           { 
                snake = (char **)realloc(snake, sizeof(char *) * ((*len) + 1));
                snake[(*len)] = (char *)malloc(sizeof(char) * 2);
                memcpy(snake[(*len)], tail, 2);
                ++(*len);
                //在蛇长度达到18，即快要进入第二回合时，播放警告声音 
                if(*len==2+lastone){
                	if(nextsectionwarning){
                		PlaySound("NextSectionWarning.wav",NULL,SND_FILENAME | SND_ASYNC | SND_NOSTOP);
                		nextsectionwarning=0;
					}
				}
				//当蛇的长度达到19，即吃了16个苹果时，则进入第二回合 
                if(*len==3+lastone){
                	*section2=1;
                	//在第二回合中，为了避免蛇在自动运动过程中撞到自己，将蛇的长度缩短到4
					//需要将多余的蛇节擦除
					for(i=(*len)-1;i>3;i--) {
						gotoxy(snake[i][0]*2,snake[i][1]);
						color(11);
						printf("■");
					}
					*len=4;
					tail[2]=0;//把尾巴丢掉 
					apple_current[0] = rand()%N + 1;//产生新的苹果来取代被吃的苹果 ，作为当前苹果 
                    apple_current[1] = rand()%N + 1;
                    gotoxy(apple_current[0]*2,apple_current[1]) ;
                    color(12);
                    printf("●");
                    color(11);
                    apple_new[0]=apple_current[0];//初始化新苹果 
                    apple_new[1]=apple_current[1];
				}
				else
                	tail[2] = 1;
           }
           else 
                tail[2] = 0;
                return snake;
}

char SnakeDirection(char snake[2],char dir_apple_current,char dir_snake_current){
	int dx=abs(snake[0]-apple_current[0]);
	int dy=abs(snake[1]-apple_current[1]);
	char dir_snake_new;
	char dir_snake_another;//蛇的另外一个可选方向 
	//苹果主动靠近的情况 
	if(((apple_current[0]<snake[0])&&(dir_apple_current=='d'||dir_apple_current=='D'))||((apple_current[0]>snake[0])&&(dir_apple_current=='a'||dir_apple_current=='A'))||((apple_current[1]<snake[1])&&(dir_apple_current=='w'||dir_apple_current=='W'))||((apple_current[1]>snake[1])&&(dir_apple_current=='s'||dir_apple_current=='S'))){
		//苹果横行 
		if(dir_apple_current=='a'||dir_apple_current=='A'||dir_apple_current=='d'||dir_apple_current=='D'){
			if(dx>dy+Va){
				//相向更优 
				//设置蛇的另外一个可选方向：垂向 
				if(apple_current[1]<=snake[1])//存在同经同纬的情况，用等号将其归为这一类 
					dir_snake_another='s';
				else
					dir_snake_another='w';
				//设置蛇的最优方向 
				if(dir_apple_current=='a'||dir_apple_current=='A')
					dir_snake_new='d';
				else
					dir_snake_new='a';
			}
			else{
				//垂向更优
				//设置蛇的另外一个可选方向：相向 
				if(apple_current[0]<=snake[0])//存在同经同纬的情况，用等号将其归为这一类 
					dir_snake_another='a';
				else
					dir_snake_another='d';
				//设置蛇的最优方向 
				if(apple_current[1]<snake[1]){
					dir_snake_new='s';
				}
				else{
					dir_snake_new='w';
				}
			}
		}
		//苹果纵行 
		else{
			if(dy>dx+Va){
				//相向更优 
				//设置蛇的另外一个可选方向：垂向 
				if(apple_current[0]<snake[0])
					dir_snake_another='a';
				else
					dir_snake_another='d';
				//设置蛇的最优方向 
				if(dir_apple_current=='w'||dir_apple_current=='W')
					dir_snake_new='s';
				else
					dir_snake_new='w';
			}
			else{
				//垂向更优
				//设置蛇的另外一个可选方向：相向 
				if(apple_current[1]<snake[1])
					dir_snake_another='s';
				else
					dir_snake_another='w';
				//设置蛇的最优方向 
				if(apple_current[0]<snake[0]){
					dir_snake_new='a';
				}
				else{
					dir_snake_new='d';
				}
			}
		}
		
	}
	//苹果远离的情况
	else{
		//苹果横行
		if(dir_apple_current=='a'||dir_apple_current=='A'||dir_apple_current=='d'||dir_apple_current=='D'){
		 	if(dx>dy-Va){
		 		//同向更优
				dir_snake_new=dir_apple_current; 
				//设置蛇的另外一个可选方向：垂向
				if(apple_current[1]<snake[1])
					dir_snake_another='s';
				else
					dir_snake_another='w'; 
			 }
			else{
			 	//垂向更优
			 	//设置蛇的另外一个可选方向：同向
				dir_snake_another=dir_apple_current;
				//设置蛇的最优方向 
				if(apple_current[1]<snake[1]){
					dir_snake_new='s';
				}
				else{
					dir_snake_new='w';
				}  
			}
		}
		 //苹果纵行
		else{
		 	if(dy>dx-Va){
		 		//同向更优
				dir_snake_new=dir_apple_current;
				//设置蛇的另外一个可选方向：垂向
				if(apple_current[0]<snake[0])
					dir_snake_another='a';
				else
					dir_snake_another='d'; 
			}
			else{
				//垂向更优
				//设置蛇的另外一个可选方向：同向 
				dir_snake_another=dir_apple_current;
				//设置蛇的最优方向  
				if(apple_current[0]<snake[0]){
					dir_snake_new='a';
				}
				else{
					dir_snake_new='d';
				}   
			} 
		} 	
	}
	//如果最优方向与蛇的当前方向相反，由于蛇不能首尾颠倒运动，只能选择剩下的另一种方向 
	switch(dir_snake_new){
		case 'w':case 'W':if(dir_snake_current=='s'||dir_snake_current=='S') dir_snake_new=dir_snake_another;break;
		case 's':case 'S':if(dir_snake_current=='w'||dir_snake_current=='W') dir_snake_new=dir_snake_another;break;
		case 'a':case 'A':if(dir_snake_current=='d'||dir_snake_current=='d') dir_snake_new=dir_snake_another;break;
		case 'd':case 'D':if(dir_snake_current=='a'||dir_snake_current=='A') dir_snake_new=dir_snake_another;break;
		default:;
	}
	return dir_snake_new;
}
//char SnakeDirection(char snake[2],char dir_apple_current){  
//		int dix;
//		int diy;
//		char snake_direction;
//        dix=abs(snake[0]-apple_current[0]);
//		diy=abs(snake[1]-apple_current[1]);
//        if((dir_apple_current='d'||dir_apple_current='D')&&(dix<=fabs(snake[0]-apple_current[0])))
//			{	if((dix-(0.8+diy))>=0){(snake_direction='a')||(snake_direction='A');}
//				if(((dix-(0.8+diy))<0)&&(apple_current[1]>snake[1])){(snake_direction='w')||(snake_direction='W');}
//				if(((dix-(0.8+diy))<0)&&(apple_current[1]<snake[1])){(snake_direction='s')||(snake_direction='S');}
//			}
//		if(((dir_apple_current='a')||(dir_apple_current='A'))&&dix<=fabs(snake[0]-apple_current[0]))
//			{	if((dix-(0.8+diy))>=0){(snake_direction='d')||(snake_direction='D');}
//				if(((dix-(0.8+diy))<0)&&(apple_current[1]>snake[1])){(snake_direction='w')||(snake_direction='W');}
//				if(((dix-(0.8+diy))<0)&&(apple_current[1]<snake[1])){(snake_direction='s')||(snake_direction='S');}
//			}
//		if(((dir_apple_current='w')||(dir_apple_current='W'))&&diy<fabs(snake[1]-apple_current[1]))
//			{	if((diy-(0.8+dix))<=0){(snake_direction='s')||(snake_direction='S');}					
//				if(((diy-(0.8+dix))>0)&&(apple_current[0]>snake[0])){(snake_direction='d')||(snake_direction='D');}
//				if(((diy-(0.8+dix))>0)&&(apple_current[0]<snake[0])){(snake_direction='a')||(snake_direction='A');}	
//			}
//		if(((dir_apple_current='s')||(dir_apple_current='S'))&&diy<fabs(snake[1]-apple_current[1]))
//			{	if((diy-(0.8+dix))<=0){(snake_direction='w')||(snake_direction='W');}						
//				if(((diy-(0.8+dix))>0)&&(apple_current[0]>snake[0])){(snake_direction='d')||(snake_direction='D');}
//				if(((diy-(0.8+dix))>0)&&(apple_current[0]<snake[0])){(snake_direction='a')||(snake_direction='A');}	
//			}
//						
//		if(((dir_apple_current='d')||(dir_apple_current='D'))&&dix>=fabs(snake[0]-apple_current[0]))
//			{	if((-(dix+(0.8-diy)))<=0){(snake_direction='d')||(snake_direction='D');}
//				if(((-(dix+(0.8-diy)))>0)&&apple_current[1]>snake[1]){(snake_direction='w')||(snake_direction='W');}
//				if(((-(dix+(0.8-diy)))>0)&&apple_current[1]<snake[1]){(snake_direction='s')||(snake_direction='S');}
//			}
//		if(((dir_apple_current='a')||(dir_apple_current='A'))&&dix>=fabs(snake[0]-apple_current[0]))
//			{	if((-(dix+(0.8-diy)))<=0){(snake_direction='a')||(snake_direction='A');}
//				if(((-(dix+(0.8-diy)))>0)&&apple_current[1]>snake[1]){(snake_direction='w')||(snake_direction='W');}
//			    if(((-(dix+(0.8-diy)))>0)&&apple_current[1]<snake[1]){(snake_direction='s')||(snake_direction='S');}
//			}
//		if(((dir_apple_current='w')||(dir_apple_current='W'))&&diy>fabs(snake[1]-apple_current[1]))
//			{	if((diy+(0.8-dix))>=0){(snake_direction='w')||(snake_direction='W');}
//				if(((diy+(0.8-dix))<0)&&snake[0]<apple_current[0]){(snake_direction='a')||(snake_direction='A');}
//				if(((diy+(0.8-dix))<0)&&snake[0]>apple_current[0]){(snake_direction='d')||(snake_direction='D');}
//			}
//		if(((dir_apple_current='s')||(dir_apple_current='S'))&&diy>fabs(snake[1]-apple_current[1]))
//			{	if((diy+(0.8-dix))>=0){(snake_direction='s')||(snake_direction='S');}
//				if(((diy+(0.8-dix))<0)&&snake[0]<apple_current[0]){(snake_direction='a')||(snake_direction='A');}
//				if(((diy+(0.8-dix))<0)&&snake[0]>apple_current[0]){(snake_direction='d')||(snake_direction='D');}
//			}
//			
//		return snake_direction;
//			
//	}


void init(char plate[N+2][N+2], char ***snake_x, int *len)  //初始化 
{
        int i, j;
        char **snake = NULL;
        *len = 3;
        //score[0] = score[3] =3;
        snake = (char **)realloc(snake, sizeof(char *) * (*len));
        for (i = 0; i < *len; ++i)
                snake[i] = (char *)malloc(sizeof(char) * 2);
                
        for (i = 0; i < 3; ++i) 
                {
                snake[i][0] = N/2 + 1;
                snake[i][1] = N/2 + 1 + i;
             } 
             
        for (i = 1; i <= N; ++i) 
                for (j = 1; j <= N; ++j) 
                        plate[i][j] = 1;
                         
        apple_current[0] = rand()%N + 1; apple_current[1] = rand()%N + 1;
        apple_current[2] = 1;

        for (i = 0; i < N + 2; ++i) 
                {
                gotoxy(0, i);
                for (j = 0; j < N + 2; ++j) 
                        {
                        switch (plate[i][j]) 
                                {
                             case 0: 
                                 color(12);printf("□");color(11); continue;
                        case 1: printf("■"); continue;
                             default: ;
                             }
                    }
                   putchar('\n');
            } 
        for (i = 0; i < (*len); ++i)
       		{
       			color(14);
                gotoxy(snake[i][0] * 2, snake[i][1]);
                printf("★");
            } 
        putchar('\n');
        *snake_x = snake;
}

void Manual()
{
        gotoxy((N+5)*2,2);
        color(10);
        printf("按 W S A D 移动方向");
        gotoxy((N+5)*2,4);
        printf("按 space 键暂停"); 
        gotoxy((N+5)*2,8);
        color(11);
        printf("历史最高分为: ");
        color(12);
        gotoxy((N+12)*2,8);
        printf("%d",score[1]*10);
        color(11);
        gotoxy((N+5)*2,12);
        printf("你现在得分为: 0");         
}

int File_in()     //取记录的分数 
{
   FILE *fp;
   if((fp = fopen("C:\\tcs.txt","a+")) == NULL)
   {
            gotoxy(N+18, N+2);
     printf("文件不能打开\n");
         exit(0);
   }
   if((score[1] = fgetc(fp)) != EOF);
   else
   score[1] = 0;
   return 0;
}

int File_out()    //存数据 
{
        
        FILE *fp;
        if(score[1] > score[0]) 
        {gotoxy(10,10);
        color(12);
        puts("闯关失败 加油耶");
        gotoxy(0,N+2); 
        return 0;
        }
        if((fp = fopen("C:\\tcs.txt","w+")) == NULL)
        {
                printf("文件不能打开\n");
                exit(0);
        }
    if(fputc(--score[0],fp)==EOF)
    {       printf("输出失败\n");
    gotoxy(10,10);
        color(12);
        puts("恭喜您打破记录"); 
        gotoxy(0,N+2);
        return 0;
}
}

void Free(char **snake, int len)    //释放空间 
{
        int i;
        for (i = 0; i < len; ++i) 
                free(snake[i]);
        free(snake);
}

int main(void)
{
        int len,move=0,section2=0,control_snake=0;//move响应键盘输入的空格键，实现游戏暂停或继续 
        char dir_snake_current= 'w',ch_new,dir_snake_new,dir_apple_current='w',dir_apple_new; 
		//蛇与苹果的运动方向均保存两个，一个为current，保存上一步的运动方向，另一个为new，保存 新输入的方向，这样做的目的是在键盘输入错误，即输入内容不在方向
		//控制键范围内时，以上一步的运动方向运动 
        char a[N+2][N+2] = {{0}};
        char **snake;
        srand((unsigned)time(NULL));
        //BootAnimation();
        color(11);
        File_in();
        init(a, &snake, &len);
        Manual();
        while (ch_new != 0x1B)   // 按 ESC 结束 
        { 
                Draw(snake, len,section2);
                if(!section2){
                	if (!apple_current[2]) {
                        apple_current[0] = rand()%N + 1;
                        apple_current[1] = rand()%N + 1;
                        apple_current[2] = 1;
                	}
				}
                
                Sleep(300-score[0]*10);
                setbuf(stdin, NULL);
                if (kbhit())
                   {
                        gotoxy(0, N+2);
                        ch_new = getche();
                        if(ch_new==' ')
						switch(move){
							case 0:move=1;break;
							case 1:move=0;break;
							default:;
						};
						if(section2){
							if(ch_new=='j'|ch_new=='J'|ch_new=='L'|ch_new=='l'|ch_new=='K'|ch_new=='k'|ch_new=='I'|ch_new=='i'){
								control_snake=1;
								switch(ch_new){
									case 'j': case 'J':dir_snake_new='A';break;
									case 'l': case 'L':dir_snake_new='D';break;
									case 'k': case 'K':dir_snake_new='S';break;
									case 'i': case 'I':dir_snake_new='W';break;
									default:;
								}
							}
							else{
								dir_apple_new=ch_new;
							}	
						}
						else{
							dir_snake_new=ch_new;
						}
                    }
                if(move){
                	//游戏第二回合的控制内容 
                	if(section2){
                //根据dir_apple_new产生苹果新的位置 
                //刚开始让苹果处于静止状态，因为刚开始dir_apple_new未被赋值 
						if(dir_apple_new) {
                			switch(dir_apple_new){
                				case 'a':case 'A':apple_new[0]=apple_current[0]-1;dir_apple_current=dir_apple_new;break;
                				case 'd':case 'D':apple_new[0]=apple_current[0]+1;dir_apple_current=dir_apple_new;break;
                				case 's':case 'S':apple_new[1]=apple_current[1]+1;dir_apple_current=dir_apple_new;break;
                				case 'w':case 'W':apple_new[1]=apple_current[1]-1;dir_apple_current=dir_apple_new;break;
                				default:
									switch(dir_apple_current){
										case 'a':case 'A':apple_new[0]=apple_current[0]-1;break;
                						case 'd':case 'D':apple_new[0]=apple_current[0]+1;break;
                						case 's':case 'S':apple_new[1]=apple_current[1]+1;break;
                						case 'w':case 'W':apple_new[1]=apple_current[1]-1;break;
									}
							}
							//判断苹果出界 
							if((apple_new[0] < 1) || (apple_new[0] > N) || (apple_new[1] < 1) || (apple_new[1] > N)){
								gotoxy(N+2, N+2);
                        		puts("你输了");
                        		File_out();
                        		Free(snake, len);
                        		getche();
                        		exit(0);
							}
						}
						//如果是单人游戏时，即control_snake=0时，蛇的运动方向需要自动产生 
						//蛇自动生成的方向，受到蛇头位置、苹果位置以及苹果运动方向的影响 
						if(!control_snake){	
                        	dir_snake_new=SnakeDirection(snake[0],dir_apple_current,dir_snake_current) ;
                    	} 
                    } 
//						int i;
//                        	for(i=0;i<4;i++){
//                            if((apple_current[0]==snake[i][0])&&(apple_current[1]==snake[i][1])||(apple_current[0]<1)||(apple_current[0]>N)||(apple_current[1]<1)||(apple_current[1]>N))
//								{	gotoxy(N+2,N+2);
//									puts("你输了");
//									File_out();
//									Free(snake,len);
//									getche();
//									exit(0);
//		
//								}
//                            }
//						} 
//					}
                 	snake = Move(snake, &dir_snake_current,dir_snake_new,&len,&section2); 
				} 
                 if (Block(snake[0])==1||HitSelf(snake,len)==1) 
                  {
                        gotoxy(N+2, N+2);
                        puts("你输了");
                        File_out();
                        Free(snake, len);
                        getche();
                        exit(0); 
                  }                        
        }
        Free(snake, len);
        exit(0);
}

