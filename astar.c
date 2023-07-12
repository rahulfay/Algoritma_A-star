#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

#define GRID_SIZE_X 8
#define GRID_SIZE_Y 12
#define OBSTACLE -1
#define START 0
#define GOAL 1
#define EMPTY 2
int path[GRID_SIZE_X * GRID_SIZE_Y][2];
int tem_path[GRID_SIZE_X * GRID_SIZE_Y][2];
int pathIndex = 0;
int i=0;
typedef struct Node {
    int x, y;
    int g, h, f;
    struct Node* parent;
} Node;

Node* createNode(int x, int y, Node* parent) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->x = x;
    newNode->y = y;
    newNode->g = 0;
    newNode->h = 0;
    newNode->f = 0;
    newNode->parent = parent;
    return newNode;
}

int heuristic(Node* a, Node* b) {
    // return abs(a->x - b->x) + abs(a->y - b->y);
    return sqrt(pow(a->x,2) - pow(b->x,2)) + sqrt(pow(a->y,2) - pow(b->y,2));
}

bool isValid(int x, int y) {
    return x >= 0 && x < GRID_SIZE_X && y >= 0 && y < GRID_SIZE_Y;
}

bool isObstacle(int grid[GRID_SIZE_X][GRID_SIZE_Y], int x, int y) {
    return grid[x][y] == OBSTACLE;
}

bool isGoal(int grid[GRID_SIZE_X][GRID_SIZE_Y], int x, int y) {
    return grid[x][y] == GOAL;
}

void aStar(int grid[GRID_SIZE_X][GRID_SIZE_Y], Node* start, Node* goal) {
    // Inisialisasi open list dan closed list
    Node* openList[GRID_SIZE_X * GRID_SIZE_Y];
    bool closedList[GRID_SIZE_X][GRID_SIZE_Y];
    for (int i = 0; i < GRID_SIZE_X * GRID_SIZE_Y; i++) {
        openList[i] = NULL;
    }
    for (int i = 0; i < GRID_SIZE_X; i++) {
        for (int j = 0; j < GRID_SIZE_Y; j++) {
            closedList[i][j] = false;
        }
    }

    // Tambahkan node awal ke open list
    openList[0] = start;

    while (true) {
        // Cari node dengan nilai f terendah di open list
        int minIndex = -1;
        int minValue = INT_MAX;
        for (int i = 0; i < GRID_SIZE_X * GRID_SIZE_Y; i++) {
            if (openList[i] != NULL && openList[i]->f < minValue) {
                minValue = openList[i]->f;
                minIndex = i;
            }
        }

        // Jika tidak ada node di open list, berarti tidak ada solusi
        if (minIndex == -1) {
            printf("Tidak ada solusi.\n");
            return;
        }

        // Ambil node dengan nilai f terendah dan hapus dari open list
        Node* currentNode = openList[minIndex];
        openList[minIndex] = NULL;
        closedList[currentNode->x][currentNode->y] = true;

        // Jika node saat ini adalah node tujuan, cetak jalur
        if (isGoal(grid, currentNode->x, currentNode->y)) {
            printf("Jalur ditemukan:\n");
            Node* pathNode = currentNode;
            while (pathNode != NULL) {
                printf("(%d, %d) -> ", pathNode->x, pathNode->y);
                path[pathIndex][0]=pathNode->x;
                path[pathIndex][1]=pathNode->y;
                pathIndex++;
                pathNode = pathNode->parent;
            }
            printf("Selesai\n");
            return;
        }

        // Cek tetangga node saat ini
        int dx[] = {-1, 0, 1, 0, 1, 1, -1, -1};
        int dy[] = {0, 1, 0, -1, 1, -1, 1, 1};
        double cost [] = {1, 1, 1, 1, 1.414, 1.414, 1.414, 1.414};
        for (int i = 0; i < 8; i++) {
            int newX = currentNode->x + dx[i];
            int newY = currentNode->y + dy[i];

            if (isValid(newX, newY) && !isObstacle(grid, newX, newY) && !closedList[newX][newY]) {
                Node* neighbor = createNode(newX, newY, currentNode);
                neighbor->g = currentNode->g + cost[i];
                neighbor->h = heuristic(neighbor, goal);
                neighbor->f = neighbor->g + neighbor->h;

                // Cek apakah tetangga sudah ada di open list
                bool inOpenList = false;
                for (int j = 0; j < GRID_SIZE_X * GRID_SIZE_Y; j++) {
                    if (openList[j] != NULL && openList[j]->x == newX && openList[j]->y == newY) {
                        inOpenList = true;
                        if (openList[j]->g > neighbor->g) {
                            openList[j] = neighbor;
                        }
                        break;
                    }
                }

                // Jika tetangga belum ada di open list, tambahkan
                if (!inOpenList) {
                    for (int j = 0; j < GRID_SIZE_X * GRID_SIZE_Y; j++) {
                        if (openList[j] == NULL) {
                            openList[j] = neighbor;
                            break;
                        }
                    }
                }
            }
        }
    }
}

int a,b;
void updateGrid_addObstacle(int grid[GRID_SIZE_X][GRID_SIZE_Y])
{
	for(int i = 0; i < GRID_SIZE_X; i++)
	{
		for(int j = 0; j < GRID_SIZE_Y; j++)
		{
			grid[i][j] = 2;
		}
	}
}
void clear(int grid[GRID_SIZE_X][GRID_SIZE_Y])
{
	for(int i = 0; i < GRID_SIZE_X; i++)
	{
		for(int j = 0; j < GRID_SIZE_Y; j++)
		{
			grid[i][j] = 0;
		}
	}
}
int main() {
    int grid[GRID_SIZE_X*GRID_SIZE_Y][GRID_SIZE_Y] = {
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, OBSTACLE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {START, OBSTACLE, EMPTY, GOAL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, OBSTACLE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        
    };

    for(int i = 0;i<2;i++)
    {
        if(i==1)
        {
            pathIndex=0;
            updateGrid_addObstacle(grid);
            // clear(path);
            grid[2][0]=0;
            grid[3][3]=1;
            grid[3][1]=-1;

        }
        else
        {

        }
        Node* start = createNode(2, 0, NULL);
        Node* goal = createNode(3, 3, NULL);

        aStar(grid, start, goal);

        printf("Jalur dari belakang ke depan:\n");
        for (int i = pathIndex - 1; i >= 0; i--) {
            printf("(%d, %d)", path[i][0], path[i][1]);
            grid[path[i][0]][path[i][1]]=4;
            if (i > 0) {
                printf(" -> ");
            }
        }
        printf("\n");
        for(int i=0; i < GRID_SIZE_X; i++) {
            for(int j=0; j < GRID_SIZE_Y; j++){
                printf("%d|",grid[i][j]);
            }
            printf("\n");
        }
        printf("\n");


    }
        return 0;
    }


