#include <stdio.h>
#include <stdlib.h>


void get_maz(char *file_name);
void define1();
void print_maz();
void add_path1();
int maz_solver(int row, int col);
void alloc_maz();
void alloc_defined1();

char **maz;
int **defined1;
int rows;
int cols;
int start_row;
int start_col;

//ماز از چهار عنصر فضای خالی،دیوار ، هدف و مسیر رفته شده تشکیل شده است
//که به هر کدام از آن ها یک عدد اختصاص میدهیم
enum terrain1
{
	empty1,
	wall1,
	goal1,
	path1
};

int solve()
{

    //ابتدا فایل ماز را به صورت تکست دریافت میکنیم
	get_maz("maze1.txt");
	//در این بخش تعریف میکنیم که چه کاراکتریفضای خالی، دیوار ، هدف و مسیر حرکت است
	define1();
	//سپس ماز حل نشده را نمایش میدهیم
	print_maz();

    //حال با تابع زیر که به صورت بازگشتی کار میکند به جستجوی
    //راهی برای رسیدن به هدف میگردیم
	if (!maz_solver(start_row, start_col)) {
		printf("No path to the goal1 could be found.\n");
	} else {
		add_path1();
		print_maz();
	}
	return 0;
}

void get_maz(char *file_name)
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
	alloc_maz();

	int i,j;

	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {

			if ((c = getc(maze_file)) == '\n') {
				c = getc(maze_file);
			}

			maz[i][j] = c;

			if (c =='S') {
				start_row = i;
				start_col = j;
			}
		}
	}

	fclose(maze_file);
}


void alloc_maz()
{
    //ابتدا به سطر ها اختصاص داده
	maz = malloc(rows * sizeof(char*));

	int i;
	for (i = 0; i < rows; ++i){
        //سپس به ستون ها اختصاص میدهیم
		maz[i] = malloc(cols * sizeof(char*));
	}
}

void define1()
{
    //آرایه ای دو در دو برای تعریف ماز دریافت شده در تابع قبل تشکیل میدهیم
	alloc_defined1();

	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			if (maz[i][j] == '#') {
				defined1[i][j] = wall1;
			} else if (maz[i][j] == 'G') {
				defined1[i][j] = goal1;
			} else {
				defined1[i][j] = empty1;
			}
		}
	}
}

void alloc_defined1()
{
	defined1 = malloc(rows * sizeof(int*));
	int i;
	for (i = 0; i < rows; ++i){
		defined1[i] = malloc(cols * sizeof(int*));
	}
}



void print_maz()
{
	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {

			printf("%c", maz[i][j]);

		}
		printf("\n");
	}
	printf("\n");
}

void add_path1()
{
	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			if (maz[i][j] != 'S' && defined1[i][j] == path1) {
				maz[i][j] = '.';
			}
		}
	}
}

int maz_solver(int row, int col)
{
    //متغیر زیر وضعیت در لحظه ی ما در ماز را نشان میدهد که از انتها شروع میکنیم
	int *current = &defined1[row][col];

	if (*current == goal1) {
		return 1;
	}

	if (*current == empty1) {
        /*در این بخش با قرار دادن
        path
        به جای
        wall1
        میتوانید تمامی مسیر های جستجو شده را ببینید
        */
		*current = wall1;

		if (maz_solver(row, col - 1)){
			*current = path1;
			return 1;
		}
		if (maz_solver(row + 1, col)){
			*current = path1;
			return 1;
		}
		if (maz_solver(row, col + 1)){
			*current = path1;
			return 1;
		}
		if (maz_solver(row - 1, col)){
			*current = path1;
			return 1;
		}
	}

	return 0;
}


