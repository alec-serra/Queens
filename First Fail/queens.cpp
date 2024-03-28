#include <stdio.h>
#include <stdlib.h>
#include <time.h>
struct Q
{
	int* board;
	int* valid_next; //Valid next spots for a given row on the board
	int* hc; //Heuristic score of a given move.
	int size;
};

struct States
{
	int size;
	Q* state;
	int total_search; //Total times Dive() has been hit by algorithm.
};

void print_next(Q* queen)
{
	for(int x = 0; x < queen->size; x++)
	{
		printf("%d \n", queen->valid_next[x]);
	}
}
void print_hc(Q* queen)
{
	printf("[");
	for(int x = 0; x < queen->size; x++)
	{
		printf(" %d ", queen->hc[x]);
	}
	printf("] \n");
}

void pretty_print_board(Q* queen)
{
	for(int x = 0; x < queen->size; x++)
	{
		for(int y = 0; y < queen->size; y ++)
		{
			if(y == queen->board[x])
				printf(" ♕ ");
			else
				printf(" . ");
		}
		printf("\n");
		
	}
}
void print_board(Q* queen)
{
	printf("[");
	for(int x = 0; x < queen->size; x++)
	{
		printf(" %d ", queen->board[x]);
	}
	printf("] \n");
}
void get_valid_next(Q* queen, int index=0)
{
	//Check all queens up to the index we are looking at
	int dx, spot1, spot2;
	//Init the valid next state to true
	for(int n = 0; n < queen->size; n ++)
	{
		queen->valid_next[n] = true;
	}
	for(int n = 0; n < queen->size; n ++)
	{
		if (queen->board[n] == -1)
			continue;
		//Set horizontal Squares to false where a queen is present
		queen->valid_next[queen->board[n]] = false;
		
		//Set diag squares to false where a queen is present
		dx = index - n; // row difference remains the same
		// Calculate potential spots for diagonal conflicts
		spot1 = queen->board[n] - dx; // Left diagonal
		spot2 = queen->board[n] + dx; // Right diagonal
		if(spot1 >= 0 && spot1 < queen->size)
		{
			queen->valid_next[spot1] = false;
		}
		if(spot2 >= 0 && spot2 < queen->size)
		{
			queen->valid_next[spot2] = false;
		}
		
	}
} 

void First_Fail(States* queens, int index=0)
{
	Q* queen = &queens->state[index];
	get_valid_next(queen, index);
	int SIZE = queen->size;
	// Initialize heuristic scores to maximum
	for (int i = 0; i < SIZE; i++) {
		queen->hc[i] = SIZE; // Start with max possible value as heuristic score
	}
	for (int v = 0; v < SIZE; v++) {
		// Check if position is valid	
		if (queen->valid_next[v])
		{
			// Simulate placing a queen and calculating valid next positions
			queen->board[index] = v;
			get_valid_next(queen, index + 1);
			
			int validMoves = 0;
			for (int n = 0; n < SIZE; n++)
			{
				if (queen->valid_next[n])
				{
					validMoves++;
				}
			}
			
			queen->hc[v] = validMoves; // Store heuristic score based on valid moves
			queen->board[index] = -1; // Reset board position
			get_valid_next(queen, index); //Resest valid next array
			
		}
		else
		{
			queen->hc[v] = SIZE + 1; // Invalidate this position
		}
	}
}

bool Dive(States* queens, int index=0)
{
	//printf("Diving %d \n", index);
	int SIZE = queens->size;
	if(index == queens->size)
	{
		//We found the bottom.
		return true;
	}
	//Inc total search count.
	queens->total_search += 1;
	//Current Q state we're looking at
	Q* queen = &queens->state[index];
	
	//Forward propogate the changes
	// If not the first queen, copy the board state from the previous queen
	if(index > 0)
	{
		for(int i = 0; i < SIZE; i++)
		{
			queen->board[i] = queens->state[index - 1].board[i];
		}
	}
	pretty_print_board(queen);
	printf("\n");
	
	First_Fail(queens, index);
	for(int choice = 0; choice < SIZE; choice++)
	{
		int minScore = SIZE + 1;
		int bestPosition = -1;
		for (int v = 0; v < SIZE; v++) {
			if (queen->valid_next[v] && queen->hc[v] < minScore) {
				minScore = queen->hc[v];
				bestPosition = v;
			}
		}
		
		if(!queen->valid_next[choice])
			continue;
	
		queen->board[index] = bestPosition;
		if(Dive(queens, index+1))
		{
			return true;
		}
		else
		{
			queen->board[index] = -1;
			queen->hc[bestPosition] = SIZE*2;
		}		
	}

	return false;
}

int main(int argc, char* argv[])
{
	if(argc < 1)
	{
		printf("Enter number of queens \n");
		return (-1);
	}
	int SIZE = atoi(argv[1]);
	//Init the Game State
	States game = {};
	game.size = SIZE;
	game.state = (Q*)malloc(sizeof(Q) * SIZE);
       	for(int n = 0; n < SIZE; n++)
	{
		game.state[n].board = (int*)malloc(sizeof(int)*SIZE);
		game.state[n].valid_next = (int*)malloc(sizeof(int)*SIZE);
		game.state[n].hc = (int*)malloc(sizeof(int)*SIZE);
		for(int nn = 0; nn < SIZE; nn++)
		{
			game.state[n].board[nn] = -1;
			game.state[n].valid_next[nn] = -1;
			game.state[n].hc[nn] = SIZE*2;
			game.state[n].size = SIZE;
		}
	}
	
	clock_t start, end;
	double cpu_time_used;
	
	start = clock();
	if(Dive(&game, 0))
	{
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		
		pretty_print_board(&game.state[SIZE-1]);
		printf("\nTotal Search Range: %d \n", game.total_search);
		printf("Search Time: %f seconds \n", cpu_time_used);
		return(0);
	}
	else
	{
		printf("No solution found \n");
		return -1;
	}
	
	return 0;
}
