#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <math.h>

enum state{
    end = 37,
    nothing = 79,
    wall = 35,
    bomb = 42,
    start = 65,
    hole
};

typedef struct cell{
    enum state cell_state;
    struct cell* tunnel_cell;
    uint8_t amount_of_neighbors;
    struct cell** neighbors;
    uint16_t index;
    uint8_t visited;
}cell;

uint16_t maze_tunnels;
uint16_t maze_size;
uint16_t maze_rows;
uint16_t maze_columns;
cell* maze;
uint16_t start_cell_index;
uint8_t end_paths=0;
uint8_t path_amount = 0;

uint8_t cell_exists(int32_t posy, int32_t posx);
void get_neighbors(uint16_t index);
void appent_element(cell* current_cell, cell* append_cell);
void DFS(cell* c_cell);

int main(int argc, char* argv[]){
    //input ./a.out 3 6 1 2 3 2 1
    //###*OOO#OA%O###*OO
    uint8_t current_cell;
    uint8_t next_cell;
    maze_rows = atoi(argv[1]);
    maze_columns = atoi(argv[2]);
    maze_size = maze_rows * maze_columns;
    char* maze_in = malloc(sizeof(char) * maze_size);
    printf("Insert maze: ");
    scanf("%s", maze_in);
    maze = malloc( maze_size * sizeof(cell));
    maze_tunnels = atoi(argv[3]);
    for (size_t i = 0; i < maze_size; i++)
    {
        maze[i].cell_state = maze_in[i];
        maze[i].index = i;
        maze[i].neighbors = NULL;
        maze[i].amount_of_neighbors = 0;
        maze[i].visited = !(maze_in[i]-35);
        if (maze[i].cell_state == (enum state)start) start_cell_index = i;
    }
    for (size_t i = 4; i < argc-1; i++)
    {
        current_cell = (atoi(argv[i]) * (maze_columns)) + atoi(argv[++i]);
        next_cell = (atoi(argv[++i]) * (maze_columns)) + atoi(argv[++i]);

        maze[current_cell].cell_state = (enum state)hole;
        maze[current_cell].tunnel_cell = &maze[next_cell];

        maze[next_cell].cell_state = (enum state)hole;
        maze[next_cell].tunnel_cell = &maze[current_cell];
    }
    for (size_t i = 0; i < maze_size; i++)
    {
        get_neighbors(i);
    }
    DFS(&maze[start_cell_index]);
    printf("end paths %d\n", end_paths);
    printf("path amount %d", path_amount);
    return 0;
}

void DFS(cell* c_cell){
    cell* current_cell = c_cell;
    current_cell->visited = 1;
    if (!(c_cell->cell_state-66))
    {
        current_cell = c_cell->tunnel_cell;
    }else if (!(c_cell->cell_state-37))
    {
        end_paths++;
        return;
    }else if (!(c_cell->cell_state-42))
    {
        return;
    }
    for (size_t i = 0; i < current_cell->amount_of_neighbors; i++)
    {
        if (!(*current_cell->neighbors[i]).visited)
        {
            DFS(current_cell->neighbors[i]);
            path_amount++;
        }
    }
}
//###*OOO#OA%O###*OO
void appent_element(cell* current_cell, cell* append_cell){
    cell**temp = realloc(current_cell->neighbors, (++current_cell->amount_of_neighbors) * sizeof *temp);
    if (!temp)
    {
        printf("Haha, error go brrr");
        exit(EXIT_FAILURE);
    }
    current_cell->neighbors = temp;
    current_cell->neighbors[current_cell->amount_of_neighbors-1] = append_cell;
}

void get_neighbors(uint16_t index){
    int current_c;
    cell* current_cell = &maze[index];
    printf("current cell %d\n", index);
    for (int i = -1; i < 2; i+=2)
    {
        current_c = index;
        current_c += i;
        printf("index %d i %d current c %d\n",index, i, current_c);
        if (cell_exists(floor(current_c/maze_columns),current_c%maze_columns))
        {
            printf("added neighbor %d\n", current_c);
            appent_element(current_cell, &maze[current_c]);
        }

        current_c = index;
        current_c += (i * maze_columns);
        printf("index %d i %d current c %d\n",index, i, current_c);
        if (cell_exists(floor(current_c/maze_columns),current_c%maze_columns))
        {
            printf("added neighbor %d\n", current_c);
            appent_element(current_cell, &maze[current_c]);
        }
    }
    printf("\n");
    
}
//###*OOO#OA%O###*OO
uint8_t cell_exists(int32_t posy, int32_t posx){
	return !(posx < 0 || posy < 0 || posx >= maze_columns || posy >= maze_rows);
}
