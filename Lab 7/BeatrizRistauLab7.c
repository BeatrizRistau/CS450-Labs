#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

#define SIZE 10

// global variables
int n_cylinders = 0;
int head_position = 0;
int cylinders[10];
int seek_distance = 0;
int i = 0;
int j = 0;
int file_found = 0; // false
int found;
int idx = 0;
FILE *file_ptr;
char file_name[0];
int curr_head;

// function to calculate FCFS disk algorithm
int FCFS()
{
	seek_distance = 0; // reset
	
	seek_distance = abs(cylinders[0] - head_position); 
	
	printf("\nFCFS -> Order of Requests:\n");
	printf("%d ", head_position); // set head position in the beginning
	
	// FCFS Calculation
	for(i = 0; i < SIZE-1; i++)
	{ 
		seek_distance += abs(cylinders[i+1] - cylinders[i]); // calculate the total seek distance
		printf("-> %d ", cylinders[i]); // display order of request for FCFS
	}
	printf("\nTotal Seek Distance: %d\n", seek_distance); // display the total seek distance
	
	return seek_distance;
} // end function

// function to execute SSTF algorithm in order to calculate the total seek distance
int SSTF()
{
	// declare variables
	int curr_head = head_position;
	seek_distance = 0; // reset
	
	int available_cylinders[SIZE];
	
	// create a copy of array and store in a new variable
	for(i = 0; i < SIZE; i++)
		available_cylinders[i] = cylinders[i];
	
	printf("\nSSTF -> Order of Requests Handled:\n");
	printf("%d ", curr_head);
	
	// SSTF calculation
	// find all the distances
	for(i = 0; i < SIZE; i++)
	{ 
		// declare variables
		int curr_shortest = 10000;
		int idx = 0; // the next position for the scheduler
		int check_shortest; 
		
		//  check if request has already been passed
		for(j = 0; j < SIZE; j++)
		{
			// skip if it was already used
			if(cylinders[j] == -1)
				continue;
			
			check_shortest = abs(available_cylinders[j] - curr_head); // store calculated distances
			
			// grab the closest request
			if(check_shortest < curr_shortest)
			{
				curr_shortest = check_shortest;
				idx = j;
			}
			
		} // end for
		seek_distance += abs(available_cylinders[idx] - curr_head); // calculate the total seek distance
		printf("-> %d ", available_cylinders[idx]); // display order of request for FCFS
		curr_head = available_cylinders[idx]; // update head
		available_cylinders[idx] = -1;
	} // end for
	
	printf("\nTotal Seek Distance: %d\n", seek_distance); // display total seek distance
	return seek_distance;
} // end function

// function to execute SCAN algorithm in order to calculate the total seek distance
int SCAN()
{
	// declare variables
	int curr_head = head_position;
	seek_distance = 0; // reset
	int max = 0;
	int available_cylinders[SIZE];
	int right = 1; // 1: right, 0: left
	
	// create a copy of array & find the max
	for(i = 0; i < SIZE; i++)
	{
		available_cylinders[i] = cylinders[i];
		if(available_cylinders[i] > max)
			max = available_cylinders[i];
	}
	printf("\nSCAN -> Order of Requests:\n");
	
	printf("%d ", curr_head);
	
	// SCAN calulation
	// run through each for each number of requests to check which one is next
	for(i = 0; i < SIZE; i++)
	{
		int right_next = max + 1;
		int left_next = 0;
		idx = 0;
		int check;
		for(j = 0; j < SIZE-1; j++)
		{	
			// skip if it was already used
			if(available_cylinders[j] == -1) 
				continue;
				
			// tell whether it is going forward (right) or not
			if(right == 1)
			{ 
				if(available_cylinders[j] >= curr_head && available_cylinders[j] <= right_next)
				{
					right_next = available_cylinders[j];
					idx = j;
				}
			} // end if
			
			// if it is going back (left)
			else if(right == 0)
			{ 
				if(available_cylinders[j] <= curr_head && available_cylinders[j] >= left_next)
				{
					left_next = available_cylinders[j];
					idx = j;
				}
				if(available_cylinders[j] == max)
				{
					available_cylinders[j] = -1;
					continue;
				}
			} // end else if
		} // end for
		
		// If we reach the last, we move to end and switch directions
		if(curr_head == max && right)
		{ 
			seek_distance += abs(curr_head - (n_cylinders-1));
			printf("-> %d ", (n_cylinders-1));
			curr_head = n_cylinders - 1;
			right = 0; // false
			continue;
		}
		
		// skip if it was already used
		if(available_cylinders[idx] == -1) 
			continue;
		seek_distance += abs(available_cylinders[idx] - curr_head);
		printf("-> %d ", available_cylinders[idx]);
		curr_head = available_cylinders[idx];
		available_cylinders[idx] = -1;
	} // end for
	
	printf("\nTotal Seek Distance: %d\n", seek_distance); // display total seek distance
	return seek_distance;
} // end function

// function that will read the random cylinders from the file created
int ReadRandomCylinders(int file_found)
{
	printf("\nEnter the name of the file (without .txt): ");
	scanf(" %s", &file_name); // get user input
	strcat(file_name, ".txt"); // append .txt to the file name entered by the user
	file_ptr = fopen(file_name, "r"); // open & read file 
	if(file_ptr == NULL)
	{
		printf("\nError:> File was not found.");
		fclose(file_ptr); // close file
		file_found = 0; // false
		return file_found;
	}
	// if file was found then read values and store in variable
	else
	{
		// store values from file in the array
		for(i = 0; i < SIZE; i++)
			fscanf(file_ptr, "%d", &cylinders[i]);
	}
	file_found = 1; // true
	return file_found;
} // end function

// function to determine the best disk scheduling algorithm
void DetermineBest()
{
	int fcfs, sstf, scan;
	fcfs = FCFS();
	sstf = SSTF();
	scan = SCAN();
	if(fcfs <= sstf && fcfs <= scan)
		printf("\n The best algorithm is FCFS.\n");
	else if(sstf <= fcfs && sstf <= scan)
		printf("\nThe best algorithm is SSTF.\n");
	else if(scan <= sstf && scan <= fcfs)
		printf("\nThe best algorithm is SCAN.\n");
} // end function

// function to create file and write random numbers in the file
int CreateRandomCylindersFile()
{
	if(n_cylinders <= 0)
		printf("Error:> Not enough cylinders available. \nYou need to select option 1 first to set number of cylinders.\n");
	else
	{
		printf("\nEnter the name of the file (without .txt): ");
		scanf(" %s", &file_name);
		strcat(file_name, ".txt"); // append the name of the file with the extension .txt
		file_ptr = fopen(file_name, "w"); // open file
		// write random values separated by space
		for(i = 0; i < SIZE; i++)
			fprintf(file_ptr, "%d ", rand() % n_cylinders);
		fclose(file_ptr); // close file
	} // end else
} // end function

// function to get user input and call appripriate function
void menuOptions()
{
	printf("Welcome to Disk Algorithm!\n\n"); // display welcome message
	char option[10]; // variable to get user input
	
	// it will keep executing until user selects the option to exit the program
	while(1)
	{
		// display menu for user to select option
		printf("--------------------- Main Menu ----------------\n\n");
		printf("1. Number of Cylinders\n");
		printf("2. Generate Random Requests File\n");
		printf("3. Initial Position of Disk Head \n");
		printf("4. Apply FCFS\n");
		printf("5. Apply SSTF\n");
		printf("6. Apply SCAN\n");
		printf("7. Report the best algorithm\n");
		printf("8. Exit\n");
		printf("\n-----------------------------------------------\n");
		printf("Option:> ");
		
		scanf("%s", &option); // get user input
		
		// call appropriate function according to the user input
		if(strcmp(option, "1") == 0)
		{
			printf("\nPlease enter number of cylinders: ");
			scanf(" %d", &n_cylinders);
		}
		else if(strcmp(option, "2") == 0)
			CreateRandomCylindersFile();
		else if(strcmp(option, "3") == 0)
		{
			printf("\nPlease enter intial position of disk head: ");
			scanf(" %d", &head_position);
		}
		else if(strcmp(option, "4") == 0)
		{
			found = ReadRandomCylinders(file_found);
			if (found == 1)
				FCFS();
		}
		else if(strcmp(option, "5") == 0)
		{
			found = ReadRandomCylinders(file_found);
			if (found == 1)
				SSTF();
		}
		else if(strcmp(option, "6") == 0)
		{
			found = ReadRandomCylinders(file_found);
			if (found == 1)
				SCAN();
		}
		else if(strcmp(option, "7") == 0)
		{
			found = ReadRandomCylinders(file_found);
			if (found == 1)
				DetermineBest();
		}
		else if(strcmp(option, "8") == 0)
		{
			printf("\nThank you for using the program. Goodbye!\n");
			exit(0);
		}
		else
		{
			// if user enters invalid input
			printf("Invalid input. Input must be an integer between 1 and 8.\n"); // display error message
		}
		printf("\n");
	}
}

// program starts here
int main()
{
	menuOptions();
	return 0;
} // end main function
