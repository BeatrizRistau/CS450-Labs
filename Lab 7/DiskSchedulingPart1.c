/*
   Name: Beatriz Ristau
   Course: CS 450
   Professor: Dr. G
   Institution: WVU Tech 
*/

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

#define SIZE 10

// global variables
int i, j;
int seek_distance = 0;
int cylinders[10];
int n_cylinders = 0;
int head_position = 0;
int max = 0;

// function to calculate SCAN disk algorithm
int SCAN()
{
	// declare variables
	int curr_head = head_position;
	int available_cylinders[SIZE];
	int right = 1; // 1: right, 0: left
    int idx;
    seek_distance = 0; // reset
    max = 0; // reset
	
	// create a copy of array & find the max
	for(i = 0; i < SIZE; i++) {
		available_cylinders[i] = cylinders[i];
		if(available_cylinders[i] > max) 
			max = available_cylinders[i];
	}

	printf("\nSCAN: Order of Requests\n");
    printf("---------------------------------------------------------------------------\n");
	printf("%d ", curr_head);
	
	// SCAN calulation
	// run through each for each number of requests to check which one is next
	for(i = 0; i < SIZE+1; i++) {
		int right_next = max + 1;
		int left_next = 0;
		idx = 0;
		int check;

		for(j = 0; j < SIZE; j++) {	
			// skip if it was already used
			if(available_cylinders[j] == -1) 
				continue;
				
			// tell whether it is going forward (right) or not
			if(right == 1) { 
				if(available_cylinders[j] >= curr_head && available_cylinders[j] <= right_next) {
					right_next = available_cylinders[j];
					idx = j;
				}
			}
			// if it is going back (left)
			else if(right == 0) { 
				if(available_cylinders[j] <= curr_head && available_cylinders[j] >= left_next) {
					left_next = available_cylinders[j];
					idx = j;
				}
				if(available_cylinders[j] == max) {
					available_cylinders[j] = -1;
					continue;
				}
			}
		}
		// If we reach the last, we move to end and switch directions
		if(curr_head == max && right) { 
			seek_distance += abs(curr_head - (n_cylinders-1));
			printf("-> %d ", (n_cylinders-1));
			curr_head = n_cylinders - 1;
			right = 0; // false
			continue;
		}
		seek_distance += abs(available_cylinders[idx] - curr_head);
		printf("-> %d ", available_cylinders[idx]);
		curr_head = available_cylinders[idx];
		available_cylinders[idx] = -1;
	}
    printf("\n---------------------------------------------------------------------------");	
	printf("\nTotal Seek Distance: %d\n", seek_distance); // display total seek distance
	return seek_distance;
}

// function to calculate SSTF disk algorithm
int SSTF()
{
	// declare variables
	int curr_head = head_position;
	seek_distance = 0; // reset
	int available_cylinders[SIZE];
	
	// create a copy of array and store in a new variable
	for(i = 0; i < SIZE; i++) {
		available_cylinders[i] = cylinders[i];
    }
	
	printf("\nSSTF: Order of Requests\n");
    printf("----------------------------------------------------------------------\n");
	printf("%d ", curr_head);
	
	// SSTF calculation
	// find all the distances
	for(i = 0; i < SIZE; i++) { 
		// declare variables
		int curr_shortest = 10000;
		int idx = 0; // the next position for the scheduler
		int check_shortest; 
		
		//  check if request has already been passed
		for(j = 0; j < SIZE; j++) {
			// skip if it was already used
			if(cylinders[j] == -1)
				continue;
			
			check_shortest = abs(available_cylinders[j] - curr_head); // store calculated distances
			
			// grab the closest request
			if(check_shortest < curr_shortest) {
				curr_shortest = check_shortest;
				idx = j;
			}	
		}
		seek_distance += abs(available_cylinders[idx] - curr_head); // calculate the total seek distance
		printf("-> %d ", available_cylinders[idx]); // display order of request for FCFS
		curr_head = available_cylinders[idx]; // update head
		available_cylinders[idx] = -1;
	}
    printf("\n----------------------------------------------------------------------");
	printf("\nTotal Seek Distance: %d\n", seek_distance); // display total seek distance
	return seek_distance;
}

// function to calculate FCFS disk algorithm
int FCFS()
{
	seek_distance = 0; // reset
	seek_distance = abs(cylinders[0] - head_position);
	
	printf("\nFCFS: Order of Requests");
    printf("\n----------------------------------------------------------------------\n");
	printf("%d ", head_position); // set head position in the beginning
	
	// FCFS Calculation
	for(i = 0; i < SIZE-1; i++) { 
		seek_distance += abs(cylinders[i+1] - cylinders[i]);
	}

    // display the order of request for FCFS
	for (j = 0; j < SIZE; j++) {
		printf("-> %d ", cylinders[j]); // display order of request for FCFS
    }

    printf("\n----------------------------------------------------------------------");
	printf("\nTotal Seek Distance: %d\n", seek_distance);
	
	return seek_distance;
}

// function to determine the best disk scheduling algorithm
void determineBest()
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
}

// function will read the random values from the file created
int readRandomCylinders(int file_found)
{
    FILE *file_ptr;
    char file_name[0];

	printf("\nEnter the name of the file (without .txt): ");
	scanf(" %s", &file_name); 
	strcat(file_name, ".txt"); // append .txt to the file name entered by the user
	file_ptr = fopen(file_name, "r"); // open & read file 

    // if file does not exist, display error message
	if(file_ptr == NULL) {
		printf("\nError! File does not exist.\n");
		fclose(file_ptr); // close file
		file_found = 0; // false, meaning file was not found
		return file_found;
	}
	// if file was found then read values and store in array
	else {
		for(i = 0; i < SIZE; i++) {
			fscanf(file_ptr, "%d", &cylinders[i]);
        }
	}
	file_found = 1; // true
	return file_found;
}

// function to create and write random numbers in a file
void createRandomCylindersFile()
{
    FILE *file_ptr;
    char file_name[0];

	if(n_cylinders <= 0) {
		printf("\nError! You need to select option 1 first to set number of cylinders.\n");
    } else {
		printf("\nEnter the name of the file (without .txt): ");
		scanf(" %s", file_name);
		strcat(file_name, ".txt"); // append the name of the file with the extension .txt
		file_ptr = fopen(file_name, "w"); // open file
		// write random values separated by space
		for(i = 0; i < SIZE; i++)
			fprintf(file_ptr, "%d ", rand() % n_cylinders);
		fclose(file_ptr); // close file
        printf("\nFile created successfully.\n");
	} 
}

// function to get user input and call appripriate function
void menuOptions() {
	
    char option;
    int found;
    int file_found = 0; // false

	printf("Welcome to Banker's Algorithm!\n\n"); 
	printf("Select an option from the menu: \n");
	// loop that will execute until user selects to exit the program
	while(1)
	{
		printf("\n------------------------- Menu ------------------------------\n");
		printf("1. Number of Cylinders\n");
		printf("2. Generate Random Requests File\n");
		printf("3. Initial Position of Disk Head\n");
		printf("4. Apply FCFS\n");
		printf("5. Apply SSTF\n");
		printf("6. Apply SCAN\n");
		printf("7. Report the best algorithm\n");
        printf("8. Exit\n");
		printf("-------------------------------------------------------------\n");
		printf("option:> ");
		scanf("%s", &option);
		
		// call appropriate function depending on the user's input
		switch(option)
		{
			case '1':
                printf("\nEnter the number of cylinders: ");
                scanf(" %d", &n_cylinders);
				break;
			case '2':
                createRandomCylindersFile();
				break;
			case '3':
                printf("\nEnter initial position of disk head: ");
                scanf(" %d", &head_position);
				break;
			case '4': 
                // if file is found then call function for calculations
                found = readRandomCylinders(file_found);
                if(found == 1)
                    FCFS();
				break;
			case '5':
                found = readRandomCylinders(file_found);
                if(found == 1)
                    SSTF();
				break;
			case '6':
                found = readRandomCylinders(file_found);
                if(found == 1)
                    SCAN();
				break;
			case '7':
                found = readRandomCylinders(file_found);
                if(found == 1)
                    determineBest();
                break;
            case '8':
                printf("\nThank you for using the program. Goodbye!\n");
                exit(0);
                break;
			default:
				// display error message if user enters invalid input
				printf("\nError! Input must be a number between 1 and 8.\n");
                break;
		}
	}
}

// program starts here
int main() {
	menuOptions();
	return 0;
}
