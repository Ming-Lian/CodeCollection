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
//����ɫ11
//ǰ��ɫ���ߡ��� 14��ƻ������12
 
int apple_current[3];
int apple_new[2];
char score[3];
char tail[3]; 
int nextsectionwarning=1;
int Va=1;//���õڶ�����ƻ�����ٶ� 
int lastone=2;

//��������
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




 
void gotoxy(int x, int y)    //������� 
{
        COORD pos; 
        pos.X = x; 
        pos.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void color(int b)         //��ɫ���� 
{
    HANDLE hConsole = GetStdHandle((STD_OUTPUT_HANDLE)) ; 
    SetConsoleTextAttribute(hConsole,b) ;
} 

void BootAnimation(){
	int i,j;
	PlaySound("Initial.wav", NULL, SND_FILENAME | SND_ASYNC);
	color(14);
	for(i=0;i<N;i++){
		//���������� 
		gotoxy(i*2,i);
		for(j=0;j<N-i*2;j++)
			printf("��");
		gotoxy(i*2,N-1-i);
		for(j=0;j<N-i*2;j++)
			printf("��");
		Sleep(100);
	}
	Sleep(12000);
} 

int Block(char head[2])   //�жϳ��� 
{
        if ((head[0] < 1) || (head[0] > N) || (head[1] < 1) || (head[1] > N))
                return 1;
        return 0;
}

int HitSelf(char **snake,int len)	//�ж�ײ���Լ� 
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
	
                      
int Eat(char snake[2],int section2)   //����ƻ�� 
{		if(section2){
			if((snake[0] == apple_new[0]) && (snake[1] == apple_new[1])){
				PlaySound("eat3.wav",NULL,SND_FILENAME | SND_ASYNC |SND_NOSTOP);
				gotoxy(N+2, N+2);
                puts("������");
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

void Draw(char **snake, int len,int section2)    //���ƶ� 
{
        if(section2){
        	gotoxy(apple_current[0]*2,apple_current[1]);//��֮ǰ��λ���ϵ�ƻ������
        	color(11); 
			printf("��"); 
			gotoxy(apple_new[0]*2,apple_new[1]);//���µ�λ�û���ƻ��
			color(12); 
			printf("��");
			color(11);
			apple_current[0]=apple_new[0];//����ƻ���ĵ�ǰλ�� 
			apple_current[1]=apple_new[1];
		}
		else{
			if (apple_current[2]) {
                gotoxy(apple_current[0] * 2, apple_current[1]);
                color(12);
                printf("��");
                color(11);
        	}
    	}
        gotoxy(tail[0] * 2, tail[1]);
        if (tail[2]) //����β�� 
         {  
		 	color(14);
        	printf("��");
            color(11);
         }
    	else 
    	if(tail[0]&&tail[1])
        printf("��");//����β�� 
        gotoxy(snake[0][0] * 2, snake[0][1]);
        color(14);
        printf("��");//����ͷ 
        color(11);
        putchar('\n');
}

char** Move(char **snake, char *dir_current,char dir_new, int *len,int *section2)   //���Ʒ��� 
{		int Section2=*section2; 
        int i, eat = Eat(snake[0],Section2);
        char CurrentDir=*dir_current;
        memcpy(tail, snake[(*len)-1], 2);
        for (i=(*len)-1;i>0;--i) 
                memcpy(snake[i], snake[i-1], 2);
        switch (dir_new) 	//������������˶�����ע�⣺�µ��˶����򲻿��Ե�ǰ�˶������෴��������ǰΪ�ϣ����µķ��򲻿���Ϊ�£����򱣳�ԭ�˶����� 
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
           default: switch(*dir_current){	//�������� ��W A S D "��� ��w a s d�����򱣳�ԭ�˶����� 
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
                //���߳��ȴﵽ18������Ҫ����ڶ��غ�ʱ�����ž������� 
                if(*len==2+lastone){
                	if(nextsectionwarning){
                		PlaySound("NextSectionWarning.wav",NULL,SND_FILENAME | SND_ASYNC | SND_NOSTOP);
                		nextsectionwarning=0;
					}
				}
				//���ߵĳ��ȴﵽ19��������16��ƻ��ʱ�������ڶ��غ� 
                if(*len==3+lastone){
                	*section2=1;
                	//�ڵڶ��غ��У�Ϊ�˱��������Զ��˶�������ײ���Լ������ߵĳ������̵�4
					//��Ҫ��������߽ڲ���
					for(i=(*len)-1;i>3;i--) {
						gotoxy(snake[i][0]*2,snake[i][1]);
						color(11);
						printf("��");
					}
					*len=4;
					tail[2]=0;//��β�Ͷ��� 
					apple_current[0] = rand()%N + 1;//�����µ�ƻ����ȡ�����Ե�ƻ�� ����Ϊ��ǰƻ�� 
                    apple_current[1] = rand()%N + 1;
                    gotoxy(apple_current[0]*2,apple_current[1]) ;
                    color(12);
                    printf("��");
                    color(11);
                    apple_new[0]=apple_current[0];//��ʼ����ƻ�� 
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
	char dir_snake_another;//�ߵ�����һ����ѡ���� 
	//ƻ��������������� 
	if(((apple_current[0]<snake[0])&&(dir_apple_current=='d'||dir_apple_current=='D'))||((apple_current[0]>snake[0])&&(dir_apple_current=='a'||dir_apple_current=='A'))||((apple_current[1]<snake[1])&&(dir_apple_current=='w'||dir_apple_current=='W'))||((apple_current[1]>snake[1])&&(dir_apple_current=='s'||dir_apple_current=='S'))){
		//ƻ������ 
		if(dir_apple_current=='a'||dir_apple_current=='A'||dir_apple_current=='d'||dir_apple_current=='D'){
			if(dx>dy+Va){
				//������� 
				//�����ߵ�����һ����ѡ���򣺴��� 
				if(apple_current[1]<=snake[1])//����ͬ��ͬγ��������õȺŽ����Ϊ��һ�� 
					dir_snake_another='s';
				else
					dir_snake_another='w';
				//�����ߵ����ŷ��� 
				if(dir_apple_current=='a'||dir_apple_current=='A')
					dir_snake_new='d';
				else
					dir_snake_new='a';
			}
			else{
				//�������
				//�����ߵ�����һ����ѡ�������� 
				if(apple_current[0]<=snake[0])//����ͬ��ͬγ��������õȺŽ����Ϊ��һ�� 
					dir_snake_another='a';
				else
					dir_snake_another='d';
				//�����ߵ����ŷ��� 
				if(apple_current[1]<snake[1]){
					dir_snake_new='s';
				}
				else{
					dir_snake_new='w';
				}
			}
		}
		//ƻ������ 
		else{
			if(dy>dx+Va){
				//������� 
				//�����ߵ�����һ����ѡ���򣺴��� 
				if(apple_current[0]<snake[0])
					dir_snake_another='a';
				else
					dir_snake_another='d';
				//�����ߵ����ŷ��� 
				if(dir_apple_current=='w'||dir_apple_current=='W')
					dir_snake_new='s';
				else
					dir_snake_new='w';
			}
			else{
				//�������
				//�����ߵ�����һ����ѡ�������� 
				if(apple_current[1]<snake[1])
					dir_snake_another='s';
				else
					dir_snake_another='w';
				//�����ߵ����ŷ��� 
				if(apple_current[0]<snake[0]){
					dir_snake_new='a';
				}
				else{
					dir_snake_new='d';
				}
			}
		}
		
	}
	//ƻ��Զ������
	else{
		//ƻ������
		if(dir_apple_current=='a'||dir_apple_current=='A'||dir_apple_current=='d'||dir_apple_current=='D'){
		 	if(dx>dy-Va){
		 		//ͬ�����
				dir_snake_new=dir_apple_current; 
				//�����ߵ�����һ����ѡ���򣺴���
				if(apple_current[1]<snake[1])
					dir_snake_another='s';
				else
					dir_snake_another='w'; 
			 }
			else{
			 	//�������
			 	//�����ߵ�����һ����ѡ����ͬ��
				dir_snake_another=dir_apple_current;
				//�����ߵ����ŷ��� 
				if(apple_current[1]<snake[1]){
					dir_snake_new='s';
				}
				else{
					dir_snake_new='w';
				}  
			}
		}
		 //ƻ������
		else{
		 	if(dy>dx-Va){
		 		//ͬ�����
				dir_snake_new=dir_apple_current;
				//�����ߵ�����һ����ѡ���򣺴���
				if(apple_current[0]<snake[0])
					dir_snake_another='a';
				else
					dir_snake_another='d'; 
			}
			else{
				//�������
				//�����ߵ�����һ����ѡ����ͬ�� 
				dir_snake_another=dir_apple_current;
				//�����ߵ����ŷ���  
				if(apple_current[0]<snake[0]){
					dir_snake_new='a';
				}
				else{
					dir_snake_new='d';
				}   
			} 
		} 	
	}
	//������ŷ������ߵĵ�ǰ�����෴�������߲�����β�ߵ��˶���ֻ��ѡ��ʣ�µ���һ�ַ��� 
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


void init(char plate[N+2][N+2], char ***snake_x, int *len)  //��ʼ�� 
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
                                 color(12);printf("��");color(11); continue;
                        case 1: printf("��"); continue;
                             default: ;
                             }
                    }
                   putchar('\n');
            } 
        for (i = 0; i < (*len); ++i)
       		{
       			color(14);
                gotoxy(snake[i][0] * 2, snake[i][1]);
                printf("��");
            } 
        putchar('\n');
        *snake_x = snake;
}

void Manual()
{
        gotoxy((N+5)*2,2);
        color(10);
        printf("�� W S A D �ƶ�����");
        gotoxy((N+5)*2,4);
        printf("�� space ����ͣ"); 
        gotoxy((N+5)*2,8);
        color(11);
        printf("��ʷ��߷�Ϊ: ");
        color(12);
        gotoxy((N+12)*2,8);
        printf("%d",score[1]*10);
        color(11);
        gotoxy((N+5)*2,12);
        printf("�����ڵ÷�Ϊ: 0");         
}

int File_in()     //ȡ��¼�ķ��� 
{
   FILE *fp;
   if((fp = fopen("C:\\tcs.txt","a+")) == NULL)
   {
            gotoxy(N+18, N+2);
     printf("�ļ����ܴ�\n");
         exit(0);
   }
   if((score[1] = fgetc(fp)) != EOF);
   else
   score[1] = 0;
   return 0;
}

int File_out()    //������ 
{
        
        FILE *fp;
        if(score[1] > score[0]) 
        {gotoxy(10,10);
        color(12);
        puts("����ʧ�� ����Ү");
        gotoxy(0,N+2); 
        return 0;
        }
        if((fp = fopen("C:\\tcs.txt","w+")) == NULL)
        {
                printf("�ļ����ܴ�\n");
                exit(0);
        }
    if(fputc(--score[0],fp)==EOF)
    {       printf("���ʧ��\n");
    gotoxy(10,10);
        color(12);
        puts("��ϲ�����Ƽ�¼"); 
        gotoxy(0,N+2);
        return 0;
}
}

void Free(char **snake, int len)    //�ͷſռ� 
{
        int i;
        for (i = 0; i < len; ++i) 
                free(snake[i]);
        free(snake);
}

int main(void)
{
        int len,move=0,section2=0,control_snake=0;//move��Ӧ��������Ŀո����ʵ����Ϸ��ͣ����� 
        char dir_snake_current= 'w',ch_new,dir_snake_new,dir_apple_current='w',dir_apple_new; 
		//����ƻ�����˶����������������һ��Ϊcurrent��������һ�����˶�������һ��Ϊnew������ ������ķ�����������Ŀ�����ڼ���������󣬼��������ݲ��ڷ���
		//���Ƽ���Χ��ʱ������һ�����˶������˶� 
        char a[N+2][N+2] = {{0}};
        char **snake;
        srand((unsigned)time(NULL));
        //BootAnimation();
        color(11);
        File_in();
        init(a, &snake, &len);
        Manual();
        while (ch_new != 0x1B)   // �� ESC ���� 
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
                	//��Ϸ�ڶ��غϵĿ������� 
                	if(section2){
                //����dir_apple_new����ƻ���µ�λ�� 
                //�տ�ʼ��ƻ�����ھ�ֹ״̬����Ϊ�տ�ʼdir_apple_newδ����ֵ 
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
							//�ж�ƻ������ 
							if((apple_new[0] < 1) || (apple_new[0] > N) || (apple_new[1] < 1) || (apple_new[1] > N)){
								gotoxy(N+2, N+2);
                        		puts("������");
                        		File_out();
                        		Free(snake, len);
                        		getche();
                        		exit(0);
							}
						}
						//����ǵ�����Ϸʱ����control_snake=0ʱ���ߵ��˶�������Ҫ�Զ����� 
						//���Զ����ɵķ����ܵ���ͷλ�á�ƻ��λ���Լ�ƻ���˶������Ӱ�� 
						if(!control_snake){	
                        	dir_snake_new=SnakeDirection(snake[0],dir_apple_current,dir_snake_current) ;
                    	} 
                    } 
//						int i;
//                        	for(i=0;i<4;i++){
//                            if((apple_current[0]==snake[i][0])&&(apple_current[1]==snake[i][1])||(apple_current[0]<1)||(apple_current[0]>N)||(apple_current[1]<1)||(apple_current[1]>N))
//								{	gotoxy(N+2,N+2);
//									puts("������");
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
                        puts("������");
                        File_out();
                        Free(snake, len);
                        getche();
                        exit(0); 
                  }                        
        }
        Free(snake, len);
        exit(0);
}

