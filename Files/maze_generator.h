#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

int sx,sy;
int gx,gy;

void InitiateMaze(int n, int m, char maze[][m]){
    for ( int i = 0; i < n ; i++ )
        for( int j = 0 ; j < m ; j++ )
            maze[i][j] = '#';
    maze[sy][sx]=' ';
}

int InBounds( int x, int y, int n, int m) {

    // Returns "1" if x and y are both in-bounds.
    if ( x < 1 || x >= (m-1) )
        return 0;
    if ( y < 1 || y >= (n-1) )
        return 0;

    return 1;
}


void Visit(int n, int m, char maze[][m], int x, int y) {
    //PrintMaze(n, m, maze);

    int side_Block_Number = 0;
    if ( maze[y+1][x] == ' ' ) side_Block_Number++;
    if ( maze[y-1][x] == ' ' ) side_Block_Number++;
    if ( maze[y][x+1] == ' ' ) side_Block_Number++;
    if ( maze[y][x-1] == ' ' ) side_Block_Number++;
    if (side_Block_Number < 2 && InBounds(x, y, n, m) ){
        maze[y][x] = ' ';

        char dirs[4];

        dirs[0] = UP;
        dirs[1] = RIGHT;
        dirs[2] = DOWN;
        dirs[3] = LEFT;

        for (int i = 0; i < 4; i++) {

            int j = rand() % 4;
            int key = dirs[j];
            dirs[j] = dirs[i];
            dirs[i] = key;
        }

        for (int i = 0; i < 4; i++){

            int dx = 0, dy = 0;

            switch (dirs[i]) {
                case UP:    dy = -1;    break;
                case DOWN:  dy = 1;     break;
                case RIGHT: dx = 1;     break;
                case LEFT:  dx = -1;    break;
            }

            int x2 = x + dx;
            int y2 = y + dy;

            if (maze[y2][x2] == '#') {
                Visit(n, m, maze, x2, y2);
            }

        }

    }

}



void PrintMaze(int n, int m, char maze[][m]) {

    maze[sy][sx] = 'S';
    maze[gy][gx] = 'G';

    /*for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++){
            if (maze[i][j] == '#')
                printf("\u2588");
            else
                printf("%c", maze[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");*/

}


int filecount(){

    int num = -4;

    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            num++;

        }
        closedir(d);
    }

    return num;
}

void makefile(int n, int m, char maze[][m]) {

    FILE *f;

    int x = 1;
    char filename[15];
    sprintf(filename, "maze%d.txt", x);
    f = fopen(filename, "w");
    if (f != NULL) {
        fprintf(f, "%d, %d\n", n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++){
                if (maze[i][j] == '$')
                    fprintf(f, "\u2588");
                else
                    fprintf(f, "%c", maze[i][j]);
            }
            fprintf(f, "\n");
        }
        fprintf(f, "\n");
        fprintf(f, "\n");
        printf("File %s was successfully created\n", filename);
    }
    else
        printf("File could not be created\n");

    fclose(f);

}


int run1() {

    int m = 20, n = 20;

    char maze[n][m];

    sy = 3; sx = 3; gx = 11; gy = 11;

    srand(time(0));


    do {
        InitiateMaze(n, m, maze);
        Visit(n, m, maze, sx, sy);
    } while (maze[gy][gx] != ' ');

    PrintMaze(n, m, maze);
    makefile(n, m, maze);


    return 0;
}


int run2() {

    int m, n;

    printf("\n\nHow many rows does the maze have? ");
    scanf("%d", &n);
    printf("\nHow many columns does the maze have? ");
    scanf("%d",&m);

    char maze[n][m];


    printf("\nEnter your start cell:\nrow:(between 1 and %d) ",n-2);
    scanf("%d" , &sy );
    printf("column:(between 1 and %d) ",m-2);
    scanf("%d" ,&sx );

    printf("\nEnter your goal cell:\nrow:(between 1 and %d) ",n-2 );
    scanf("%d" , &gy );
    printf("column:(between 1 and %d) ",m-2 );
    scanf("%d" , &gx );

    srand(time(0));


    do {
        InitiateMaze(n, m, maze);
        Visit(n, m, maze, sx, sy);
    } while (maze[gy][gx] != ' ');

    PrintMaze(n, m, maze);
    makefile(n, m, maze);


    return 0;
}
