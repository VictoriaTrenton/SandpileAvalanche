//==============================================================================
//	Phys382 - Computational Physics Project
//	Author: Victoria Trenton
//
// This program models the dynamics of a sandpile using techniques of cellular
// automata.
//==============================================================================


#include <stdio.h>
#include <math.h>
#include <stdlib.h>			// Access to exit & system functions
#include <time.h>				// Access to time function

// Declares functions
int initialize_pile ();
int add_grain ();
int avalanche ();
int relax_pile ();
int avalanche_size ();
int refresh_avaPile ();
void print_pile ();

void output_avalanche_size ();
void output_avalanche_pile ();
void output_pile ();


// Declares global variables
int size, critSlope;
int irand, jrand, a, b;
int checkForavalanches, newAvalanche;
int numGrains, numHoles, numAva;
int sizeAva;

int numSubAva[10001];			// Note: Size should be [numGrains+1]
int numSizeAva[10001];			// Note: Size should be [size*size + 1]

int pile[100][100];			// Note: Size should be [size][size]
int avaPile[100][100];			// Note: Size should be [size][size]

main()
{
	printf ("\n-------------------------------------------------");
	printf ("\n	   Sandpile Model");
	printf ("\n-------------------------------------------------\n");
	
	//------------------------------------------------------------------------
	// Initializes system parameters
	//------------------------------------------------------------------------
	
	srand(time(NULL));			// Seeds different random numbers
	critSlope = 3;				// Any higher slope causes avalanche
	checkForavalanches = 0;			// Initially no avalanches
	newAvalanche = 0;				// Initially no new avalanches
	numAva = 0;					// Keeps track of current avalanche #
	
	// Variables that can change
	size = 100;					// Size of lattice
	numGrains = 10000;			// Number of grains to add to pile
	numHoles = 2000;				// Number of holes to add to pile
	
	// Sets initial number of subsequent avalanches to 0
	// Each row represents the ith avalanche
	// 0th avalanche will always have 0 subsequent avalanches
	for (int i=0; i <= numGrains; i++)
	{
		numSubAva[0] = 0;
	}
	
	// Sets the number of avalanches for each avalanche size to 0
	// Each row represents the size of the avalanches
	// Size 0 will always have 0 avalanches
	for (int i=0; i <= size*size; i++)
	{
		numSizeAva[i] = 0;
	}
	
      
      // Calculates and ouputs
	initialize_pile ();
//	print_pile ();
	
	for (int i=1; i <= numGrains; i++)
	{
//		printf ("\n---------Add Grain---------\n");
		add_grain ();
//		print_pile ();
		
		// Checks for an avalanche
		// If slope is > 3 and not a hole, initial avalanche occurs
		if (pile[irand][jrand] > critSlope && pile[irand][jrand] < 8)
		{
//			printf ("The added grain will cause an avalanche to occur!\n");
			checkForavalanches = 1;
			a = irand;
			b = jrand;
			avalanche ();		// Initial avalanche
//			print_pile ();
			numAva++;			// Increments current avalanche #
//			printf ("\nCurrent avalanche: %d\n", numAva);
					
			relax_pile ();
			avalanche_size ();
			
			// If so many grains have been added, output the avalanche
			// The final written file will be for the last avalanche
			if (i >= 9000)
			{
				output_avalanche_pile ();
			}
			refresh_avaPile ();
		}		
	}
	
	// Ouputs the total number of avalanches of given sizes
	output_avalanche_size ();
	// Ouputs the relaxed pile after all grains have been added
	output_pile ();
	
      printf ("\n\nProgram finished.\n");
	int c = system ("pause");	/* Executes M$-DOS' pause command */
}

//------------------------------------------------------------------------------
// Sets initial values for the sandpile
//------------------------------------------------------------------------------      
int initialize_pile ()     
{
//	pile[0][0]=3;	pile[0][1]=3;	pile[0][2]=3;
//	pile[1][0]=3;	pile[1][1]=3;	pile[1][2]=3;	
//	pile[2][0]=3;	pile[2][1]=3;	pile[2][2]=3;
	
	
	// Randomly assigns a number from 0-3 to each cell in the pile
	// Goes through rows
	for (int i=0; i < size; i++)
	{
		// Goes through columns
		for (int j=0; j < size; j++)
		{
			// Stores a slope of 0, 1, 2, or 3
			pile[i][j] = rand() % 4;
		}
	}

	
	// Randomly adds the selected number of holes to the pile
	if (numHoles > 0)
	{
		// Creates holes in lattice
		for (int i=1; i <= numHoles; i++)
		{
			// Generates two numbers from 0 to size-1
			// Stores them as coordinates of a random cell
			irand = rand() % size;
			jrand = rand() % size;
			
			// If random cell is already a hole, pick new cell
			while (pile[irand][jrand] == 8)
			{
				irand = rand() % size;
				jrand = rand() % size;
			}
			// Holes are marked by a slope of 8
			pile[irand][jrand] = 8;
		}
	}
}

//------------------------------------------------------------------------------
// Adds one grain to the sandpile
//------------------------------------------------------------------------------      
int add_grain ()     
{	
	// Generates two numbers from 0 to size-1
	// Stores them as coordinates of a random cell
	irand = rand() % size;
	jrand = rand() % size;
	//printf ("\nirand = %d	jrand = %d\n", irand, jrand);
	
	// Increases slope by 1 of a random cell unless the cell is a hole
	// A grain added to a hole will fall through
	if (pile[irand][jrand] < 8)
	{
		pile[irand][jrand]++;	
	}
}

//------------------------------------------------------------------------------
// Causes an avalanche in the sandpile
//------------------------------------------------------------------------------      
int avalanche ()
{	
	// Decreases slope by 4 at the critical cell
	pile[a][b] = pile[a][b] - 4;
	// Marks this cell as being part of the avalanche with a slope of 9
	avaPile[a][b] = 9;
	
	// Increases slope by 1 of the 4 surrounding cells
	// Only considers cells that are on the lattice and are not holes
	// If cell is considered, the cell is marked as an avalanche cell
	if (a-1 >= 0)
	{
		if (pile[a-1][b] < 8)
		{
			pile[a-1][b]++;
			avaPile[a-1][b] = 9;
		}
	}
	if (b-1 >= 0)
	{
		if (pile[a][b-1] < 8)
		{
			pile[a][b-1]++;
			avaPile[a][b-1] = 9;
		}
	}
	if (a+1 < size)
	{
		if (pile[a+1][b] < 8)
		{
			pile[a+1][b]++;
			avaPile[a+1][b] = 9;
		}
	}
	if (b+1 < size)
	{
		if (pile[a][b+1] < 8)
		{
			pile[a][b+1]++;
			avaPile[a][b+1] = 9;
		}
	}
}

//------------------------------------------------------------------------------
// Calculates all avalanches until sandpile is relaxed
//------------------------------------------------------------------------------      
int relax_pile ()
{
	while (checkForavalanches == 1)
	{
		newAvalanche = 0;
		// Searches for new avalanches caused by initial avalanche
		// Goes through rows
		for (int i=0; i < size; i++)
		{
			// Goes through columns
			for (int j=0; j < size; j++)
			{
				// If cell will cause an avalanche
				if (pile[i][j] > critSlope && pile[i][j] < 8)
				{
					newAvalanche = 1;
					a = i;
					b = j;
					avalanche ();
//					print_pile ();
					
					// Increments # subsequent avalanches caused by
					// the initial avalanche
					numSubAva[numAva]++;
//					printf ("\nThis was SubAva#%d\n", numSubAva[numAva]);
				}
			}
		}
		// If there were no new avalanches caused by the initial avalanche
		if (newAvalanche == 0)
		{
			checkForavalanches = 0;
		}
	} // End of while loop (Pile has relaxed)
}

//------------------------------------------------------------------------------
// Finds the size of the avalanche after it relaxed
//------------------------------------------------------------------------------      
int avalanche_size ()     
{
	sizeAva = 0;			// Holds the size of the avalanche
		
	// Avalanche size is the # cells affected by the avalanche
	// Goes through rows
	for (int i=0; i < size; i++)
	{
		// Goes through columns
		for (int j=0; j < size; j++)
		{
			// Copies cells of the relaxed pile to the avalanche pile
			if (avaPile[i][j] < 9)
			{
				avaPile[i][j] = pile[i][j];
			}
			
			// Updates the number of affected cells
			if (avaPile[i][j] == 9)
			{
				sizeAva++;
			}
		}
	}
	
	// Increments the number of avalanches of the given size
	numSizeAva[sizeAva]++;
}

//------------------------------------------------------------------------------
// Clears the values in the avalanche pile
//------------------------------------------------------------------------------      
int refresh_avaPile ()     
{
	// Goes through rows
	for (int i=0; i < size; i++)
	{
		// Goes through columns
		for (int j=0; j < size; j++)
		{
			avaPile[i][j] = 0;
		}
	}
}

//------------------------------------------------------------------------------
// Prints slopes of the pile (in grid form)
//------------------------------------------------------------------------------      
void print_pile ()     
{	
	printf ("\n\n");
	// Goes through rows
	for (int i=0; i < size; i++)
	{
		// Goes through columns
		for (int j=0; j < size; j++)
		{
			printf ("%d\t", pile[i][j]);
		}
		printf ("\n");
	}
	printf ("\n");
}

//------------------------------------------------------------------------------
// Writes number of avalanches of given sizes to disk
//------------------------------------------------------------------------------      
void output_avalanche_size ()  
{	
	FILE *fp;
	
	// Opens file to write to
      if ( (fp = fopen("sizeAva.dat", "w")) == NULL)
      {
          printf ("\nCannot open file: sizeAva.dat\n");
          exit(8);
      }
      
	// Writes each avalanche size and number of avalanches with that size
	for (int i=1; i <= size*size; i++)
	{
		fprintf (fp, "%d\t%d", i, numSizeAva[i]);
		fprintf (fp, "\n");
	}
	fclose(fp);
}

//------------------------------------------------------------------------------
// Writes slopes of the avalanche pile (in grid form) to disk
//------------------------------------------------------------------------------      
void output_avalanche_pile ()
{
	FILE *fp;
	
	// Opens a file to write to
      if ( (fp = fopen("avaPile.dat", "w")) == NULL)
      {
          printf ("\nCannot open file: avaPile.dat\n");
          exit(8);
      }
      
	// Writes the slopes of the avalanche pile to a file (in grid form)
	// Goes through rows
	for (int i=0; i < size; i++)
	{
		// Goes through columns
		for (int j=0; j < size; j++)
		{
			fprintf (fp, "%d\t", avaPile[i][j]);
		}
		fprintf (fp, "\n");
	}
	fclose(fp);
}
	
//------------------------------------------------------------------------------
// Writes slopes of the pile (in grid form) to disk
//------------------------------------------------------------------------------
void output_pile ()
{
      FILE *fp1, *fp2;
      	
      // Opens first file to write to
      if ( (fp1 = fopen("pile.dat", "w")) == NULL)
      {
          printf ("\nCannot open file: pile.dat\n");
          exit(8);
      }
      
	// Writes the slopes of the pile to a file (in grid form)
	// Goes through rows
	for (int i=0; i < size; i++)
	{
		// Goes through columns
		for (int j=0; j < size; j++)
		{
			fprintf (fp1, "%d\t", pile[i][j]);
		}
		fprintf (fp1, "\n");
	}
	fclose(fp1);
	
	//------------------------------------------------------------------------
	
//	// Opens a second file to write to
//      if ( (fp2 = fopen("numSubAva.dat", "w")) == NULL)
//      {
//          printf ("\nCannot open file: numSubAva.dat\n");
//          exit(8);
//      }
//      
//	// Writes the avalanche # and the # of subsequent avalanches it caused
//	// Goes through the avalanches
//	for (int i=0; i <= numAva; i++)
//	{
//		// Records the # of subsequent avalanches
//		fprintf (fp2, "%d\t%d\n", i, numSubAva[i]);
//	}
//	fclose(fp2);
}


