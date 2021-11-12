#include <stdio.h>
#include <stdlib.h>
#include "windows.h"
#include <time.h>
#include "welcome.h"
#include "maze_generator.h"
#include "maze_solver.h"



void get_maze(char* file_name);
void define();
void print_maze();
void add_path();
int maze_solver(int row, int col);
void alloc_maze();
void alloc_defined();
void gotoXY(int x, int y);
void delay(int milli_seconds);
void deleteWrongPath(int row, int col);
void addUnknownPath(int row, int col);
int cantMove(int row, int col);


int speed;
char **maze;
int **defined;
int rows;
int cols;
int start_row;
int start_col;
int win = 0;
int miliseconds = 100;
char PATH = '.', WALL = '#', EMPTY = ' ', WRONG = 'R';

//ماز از چهار عنصر فضای خالی،دیوار ، هدف و مسیر رفته شده تشکیل شده است
//که به هر کدام از آن ها یک عدد اختصاص میدهیم
enum terrain
{
	empty,
	wall,
	goal,
	path,
	unknown,
	wrong
};

int main()
{
    Welcome();
    choose();
    printf("Enter 1 to exit\n");
    scanf("%d",1);
    return 0;
}

void choose()
{
    int choose,choose1,choose2;
    printf("1- To see demo press 1 and enter\n2- To see Maze_Generator and Maze_Solver press 2 and enter\n");
    scanf("%d",&choose);
    if(choose == 1)
    {
        run1();
        speed = 150;
        get_maze("maze.txt");
        define();
        system("cls");
        printWalls();
        if (!maze_solver(start_row, start_col)) {
            printf("No path to the goal could be found.\n");
        }

    }
    else if (choose == 2)
    {
        Intro();
        scanf("%d",&choose1);
        if(choose1 == 1)
            run1();
        else if(choose1 == 2)
            run2();
        else if(choose2 == 3){}
        else
            printf("Invalid input for your choice");


        printf("\n\n\n\nHow do you want to see the solution?\n1-Final path\n2-Show animation\n");
        scanf("%d",&choose2);
        if(choose2 == 1)
            solve();
        else if(choose2 == 2)
        {
            int speed1;
            printf("\nAnimation speed:\n1-Slow\n2-Fast\n3-Very Fast\n");
            scanf("%d",&speed1);
            if(speed1 == 1)
                speed = 500;
            else if(speed1 = 2)
                speed = 200;
            else if(speed1 = 3)
                speed = 1;
            else
                printf("Invalid input for your choice");

            get_maze("maze1.txt");
            define();
            system("cls");
            printWalls();
            if (!maze_solver(start_row, start_col)) {
                printf("No path to the goal could be found.\n");
            }

        }
        else
            printf("Invalid input for your choice");
    }
    else
        printf("Invalid input for your choice\n\n\n");
}



void print_maze()
{
    gotoXY(0, 0);
	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {

            if(maze[i][j]==PATH) {
                    gotoXY(i, j);
                printf("\x1B[32m%c\x1B[0m", maze[i][j]);
            }

            else if(defined[i][j]==wrong) {
                gotoXY(i, j);
                printf("\x1B[31m%c\x1B[0m", maze[i][j]);
            }
		}
	}
}

void printWalls() {

    gotoXY(start_row, start_col);
    printf("S");
    int i, j;
    for(i=0; i < rows; i++) {
        for(j=0; j<cols; j++) {
            if(defined[i][j]==wall || defined[i][j]==goal) {
                gotoXY(i, j);
                printf("%c", maze[i][j]);
            }
        }
    }

}

void deleteWrongPath(int row, int col) {

    int *current = &defined[row][col];
    if(*current=='S')
        return;
    int w=0, e=0, r=0;
    if(defined[row][col+1]==wall) w++;
    if(defined[row][col-1]==wall) w++;
    if(defined[row+1][col]==wall) w++;
    if(defined[row-1][col]==wall) w++;

    if(defined[row][col+1]==empty) e++;
    if(defined[row][col-1]==empty) e++;
    if(defined[row+1][col]==empty) e++;
    if(defined[row-1][col]==empty) e++;

    if(defined[row][col+1]==unknown) r++;
    if(defined[row][col-1]==unknown) r++;
    if(defined[row+1][col]==unknown) r++;
    if(defined[row-1][col]==unknown) r++;

    if((w==1 && e==1 && r==1) || (w==0 && r==1 && e>=1) ||
       defined[row][col+1]==goal ||
       defined[row][col-1]==goal ||
       defined[row+1][col]==goal ||
       defined[row-1][col]==goal) return;
    if(*current==unknown) {
        *current=wrong;

        add_path();
        print_maze();
        if(defined[row][col+1]==unknown) {
            deleteWrongPath(row, col+1);
            add_path();

        }
        else if(defined[row][col-1]==unknown) {
            deleteWrongPath(row, col-1);
            add_path();

        }
        else if(defined[row+1][col]==unknown) {
            deleteWrongPath(row+1, col);
            add_path();

        }
        else if(defined[row-1][col]==unknown) {
            deleteWrongPath(row-1, col);
            add_path();

        }

    }
}

int cantMove(int row, int col) {
    int w=0, u=0;
    if(defined[row][col+1]==wall) w++;
    if(defined[row][col-1]==wall) w++;
    if(defined[row+1][col]==wall) w++;
    if(defined[row-1][col]==wall) w++;

    if(defined[row][col+1]==unknown) u++;
    if(defined[row][col-1]==unknown) u++;
    if(defined[row+1][col]==unknown) u++;
    if(defined[row-1][col]==unknown) u++;

    if(w==3 && u==1) {
        return 1;
    }
    return 0;
}

int maze_solver(int row, int col)
{
    //متغیر زیر وضعیت در لحظه ی ما در ماز را نشان میدهد که از انتها شروع میکنیم
	int *current = &defined[row][col];

	if (*current == goal) {
        win = 1;
        gotoXY(0, cols + 3);
        exit(0);
		return 1;
	}

	if (*current == empty) {
        *current = unknown;
        addUnknownPath(row, col);
        /*در این بخش با قرار دادن
        path
        به جای
        wall
        میتوانید تمامی مسیر های جستجو شده را ببینید
        */

		if (maze_solver(row, col - 1)){
			return 1;
		}
		if (maze_solver(row + 1, col)){
			return 1;
		}
		if (maze_solver(row, col + 1)){
			return 1;
		}
		if (maze_solver(row - 1, col)){
			return 1;
		}
	}
	if(*current==wall || *current==wrong)
        return 0;
    else if(cantMove(row, col) && win!=1 && maze[row][col]!='S')
        deleteWrongPath(row, col);
	return 0;
}

void addUnknownPath(int row, int col) {
    if(maze[row][col]!='S')
        maze[row][col] = PATH;
	print_maze();
    delay(speed);
}

void add_path()
{
	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
            if(maze[i][j] != 'S') {
            if(defined[i][j]==unknown) {
                maze[i][j]=PATH;
            }
			else if (defined[i][j] == path) {
				maze[i][j] = PATH;
			}
			else if(defined[i][j]==empty) {
                maze[i][j]=EMPTY;
			}
			else if(defined[i][j]==wall) {
                maze[i][j]=WALL;
			}
			else if(defined[i][j]==wrong)
                maze[i][j]=WRONG;
            }
		}
	}
	delay(miliseconds);
}

void get_maze(char *file_name)
{
	char c;
	char rows_s[5] = { '\0' };
	char cols_s[5] = { '\0' };
	int rows_i = 0;
	int cols_i = 0;
	int swap = 0;

	FILE *maze_file = fopen(file_name, "r");

    //در این بخش طول و عرض ماز مشخص میشود
	if (maze_file)
        fscanf(maze_file, "%d,%d", &rows,&cols);
    else {
		printf("File not found!");
		return;
	}

    //در این بخش یک آرایه دو در دو با حافظه تخصیص یافته تشکیل میدهیم
	alloc_maze();

	int i,j;

	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {

			if ((c = getc(maze_file)) == '\n') {
				c = getc(maze_file);
			}

            if(c=='1') maze[i][j] = WALL;
            else if(c=='0') maze[i][j] = EMPTY;
            else maze[i][j] = c;

			if (c =='S') {
				start_row = i;
				start_col = j;
			}
		}
	}

	fclose(maze_file);
}


void alloc_maze()
{
    //ابتدا به سطر ها اختصاص داده
	maze = malloc(rows * sizeof(char*));

	int i;
	for (i = 0; i < rows; ++i){
        //سپس به ستون ها اختصاص میدهیم
		maze[i] = malloc(cols * sizeof(char*));
	}
}

void define()
{
    //آرایه ای دو در دو برای تعریف ماز دریافت شده در تابع قبل تشکیل میدهیم
	alloc_defined();

	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			if (maze[i][j] == WALL) {
				defined[i][j] = wall;
			} else if (maze[i][j] == 'G') {
				defined[i][j] = goal;
			} else if (maze[i][j] == EMPTY){
				defined[i][j] = empty;
			}
		}
	}
}

void alloc_defined()
{
	defined = malloc(rows * sizeof(int*));
	int i;
	for (i = 0; i < rows; ++i){
		defined[i] = malloc(cols * sizeof(int*));
	}
}

void gotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void delay(int milli_seconds)
{
    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds)
        ;
}
