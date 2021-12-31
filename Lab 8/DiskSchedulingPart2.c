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
int n_cylinders = 0;
int i,j;
int head_position = 0;
int cylinders[SIZE];
int seek_distance;
int max;
int idx;
int curr_head;
int fcfs, sstf, scan, c_scan, look, c_look;

// function to calculate C-LOOK disk algorithm
int C_LOOK()
{
    // declare & initialize variables
	curr_head = head_position;
	seek_distance = 0; // reset
	max = 0; // reset
	int min = n_cylinders;
	int min_idx = 0;
	int available_cylinders[SIZE];
	int right = 1; // left = 0
	
	// Create a copy of array & find max and min
	for(i = 0; i < SIZE; i++) {
		available_cylinders[i] = cylinders[i];
		if(available_cylinders[i] > max)
			max = available_cylinders[i];
		if(available_cylinders[i] < min) {
			min = available_cylinders[i];
			min_idx = i;
		}
	}
	
	printf("\nC-LOOK: Order of Requests:\n");
    printf("---------------------------------------------------------------------------\n");
	printf("%d ", curr_head);
	
	// run through each for each number of requests to check which is next
	for(i = 0; i < SIZE+1; i++) {
        // declare variables
		int right_next = max + 1;
		int left_next = 0;
		idx = 0; // reset
		for(j = 0; j < SIZE; j++) {
			// skip those already used
			if(available_cylinders[j] == -1) {
				continue;
            }
			// tell whether it is going forward (right) or not
			if(right == 1) {
				if(available_cylinders[j] >= curr_head && available_cylinders[j] <= right_next) {
					right_next = available_cylinders[j];
					idx = j;
				}
			}
			// if it is going back (left)
			else if(right == 0)
			{ 
				// do these checks
				if(available_cylinders[j] <= curr_head && available_cylinders[j] >= left_next) {
					left_next = available_cylinders[j];
					idx = j;
				}
                // mark as unavailable
				if(available_cylinders[j] == max) {
					available_cylinders[j] = -1;
					continue;
				}
			}
		}
		// if we reach the last, we move to main end & switch directions
		if(curr_head == max && right)
		{ 
			// c-look needs to go from max to min
			seek_distance += abs(curr_head - min);
			printf("-> %d ", min);
			available_cylinders[idx] = -1;
			available_cylinders[min_idx] = -1;
			curr_head = min;
			continue;
		}
		// skip those that were already used
		if(available_cylinders[idx] == -1) 
			continue;
		seek_distance += abs(curr_head - available_cylinders[idx]);
		printf("-> %d ", available_cylinders[idx]);
		curr_head = available_cylinders[idx];
		available_cylinders[idx] = -1;
	}
    printf("\n---------------------------------------------------------------------------");
	printf("\nTotal Seek Distance: %d\n", seek_distance);
	return seek_distance;
}

// function to calculate LOOK disk algorithm
int LOOK()
{
	// declare variables
	curr_head = head_position; // initially curr_head = 50
	seek_distance = 0; // reset
	max = 0; // reset
	int available_cylinders[SIZE];
	int right = 1; // right: 1, left: 0
	
	// create a copy of array & find the max 
	for(i = 0; i < SIZE; i++) {
		available_cylinders[i] = cylinders[i];
		if(available_cylinders[i] > max)
			max = available_cylinders[i];
	}
	
	printf("\nLOOK: Order of Requests:\n");
    printf("---------------------------------------------------------------------------\n");
	printf("%d ", curr_head); // set head position in the beginning
	
	for(i = 0; i < SIZE+1; i++) {
		int right_next = max + 1;
		int left_next = 0;
		idx = 0; // reset
		for(j = 0; j < SIZE; j++) {
			// skip if it has been already used
			if(available_cylinders[j]==-1) {
				continue;
            }
			
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
					available_cylinders[j] = -1; // mark as unavailable
					continue;
				}
			}
		}
		
		if(curr_head == max && right) { 
			// for look, it only needs to switch direction at last
			right = 0;
			continue;
		}
		seek_distance += abs(curr_head - available_cylinders[idx]); // calculate total seek distance
		printf("-> %d ", available_cylinders[idx]); // display new order of requests
		curr_head = available_cylinders[idx]; // update current head value
		available_cylinders[idx] = -1; // mark as unavailable
	}
    printf("\n---------------------------------------------------------------------------");
	printf("\nTotal Seek Distance: %d\n", seek_distance); 
	return seek_distance;
}

// function to calculate C_SCAN disk algorithm
int C_SCAN()
{
	// declare variables
	curr_head = head_position; // initially curr_head = 50
	seek_distance = 0; // reset 
	max = 0; // reset 
	int available_cylinders[SIZE]; // create new array to store new order of requests
	int right = 1; // right = 1, left = 0 
	
	// create a copy of array & find the max 
	for(i = 0; i < SIZE; i++) {
		available_cylinders[i] = cylinders[i];
		if(available_cylinders[i] > max)
			max = available_cylinders[i];
	}

	printf("\nC-SCAN: Order of Requests:\n");
    printf("---------------------------------------------------------------------------\n");
	printf("%d ", curr_head); // set head position in the beginning
	
	// Run through each for each number of requests to check which is next
	for(i = 0; i < SIZE+1; i++)
	{
		int right_next = max + 1;
		int left_next = 0;
		idx = 0; // reset
		for(j = 0; j < SIZE; j++)
		{
			// skip if it has been already used
			if(available_cylinders[j] == -1) {
				continue;
            }
			
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
					available_cylinders[j] = -1; // mark as unavailable
					continue;
				}
			}
		}
		if(curr_head == max && right) { 
			// c-scan moves from max to last of cylinders, then first of cylinders, then stays right
			seek_distance += abs(curr_head - (n_cylinders - 1));
			printf("-> %d ", (n_cylinders - 1));
			curr_head = n_cylinders - 1;
			seek_distance += abs(curr_head - 0);
			printf("-> %d ", 0);
			curr_head = 0;
			continue;
		}
        // calculate seek distance
		seek_distance += abs(curr_head - available_cylinders[idx]);
		printf("-> %d ", available_cylinders[idx]);
		curr_head = available_cylinders[idx];
		available_cylinders[idx] = -1; // mark as unavailable
	} 
    printf("\n---------------------------------------------------------------------------");
	printf("\nTotal Seek Distance: %d\n", seek_distance);
	return seek_distance;
}

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
        // declare variables
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

// function to display the algorithms from best to worst
void reportBestToWorst()
{	
	fcfs = FCFS();
	sstf = SSTF();
	scan = SCAN();
	c_scan = C_SCAN();
	look = LOOK();
	c_look = C_LOOK();
	idx = 0; // reset
	int min = 10000;
	
	// create array with the results from each algorithm
	int best[6] = {fcfs, sstf, scan, c_scan, look, c_look};
	
	printf("\nAlgorithms from best to worst:\n");
	
	// sort algorithms from best to worst
	for(i = 0; i < 6; i++) {
		for(j = 0; j < 6; j++) {
			if(best[j] < min && best[j] != -1) {
				min = best[j];
				idx = j;
			}
		}
		
		switch(idx) {
			case 0:
				printf("\nFCFS: Seek Distance = %d\n", fcfs);
				break;
			case 1:
				printf("\nSSTF: Seek Distance = %d\n", sstf);
				break;
			case 2:
				printf("\nSCAN: Seek Distance = %d\n", scan);
				break;
			case 3:
				printf("\nC-SCAN: Seek Distance = %d\n", c_scan);
				break;
			case 4:
				printf("\nLOOK: Seek Distance = %d\n", look);
				break;
			case 5:
				printf("\nC-LOOK: Seek Distance = %d\n", c_look);
				break;
		}
		
		best[idx] = -1; // mark as unavailable
		min = 10000;
	}
}

// function to find the best algorithm
void determineBest()
{
	fcfs = FCFS();
	sstf = SSTF();
	scan = SCAN();
	c_scan = C_SCAN();
	look = LOOK();
	c_look = C_LOOK();
	
	idx = 0; // reset
	
	// create array with the results from each algorithm
	int best[6] = {fcfs, sstf, scan, c_scan, look, c_look};
	
	// find the best algorithm
	for(i = 0; i < 6; i++) {
		if(best[i] < best[idx])
			idx = i;
	}
	
	// display best algorithm
	if(idx == 0)
		printf("\nThe best algorithm is FCFS.\n");
	else if(idx == 1)
		printf("\nThe best algorithm is SSTF.\n");
	else if(idx == 2)
		printf("\nThe best algorithm is SCAN.\n");
	else if(idx == 3)
		printf("\nThe best algorithm is C-SCAN.\n");
	else if(idx == 4)
		printf("\nThe best algorithm is LOOK.\n");
	else if(idx == 5)
		printf("\nThe best algorithm is C-LOOK.\n");
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
    }else {
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

// function to desplay menu for user to select an option
void menuOptions()
{
    // declare & initialize variables
    char option[0];
    int found; // if file is found then it will return true, otherwise returns false
	int file_found = 0; // false 

    printf("\nWelcome to Disk Algorithm!\n\n");
    printf("Select an option from the menu: \n");

    // loop will keep executing until user selects to exit the program
    while(1) {
        // display menu for user to select option
		printf("\n--------------------- Main Menu ----------------\n");
		printf("1. Number of Cylinders\n");
		printf("2. Generate Random Requests File\n");
		printf("3. Initial Position of Disk Head \n");
		printf("4. Apply FCFS\n");
		printf("5. Apply SSTF\n");
		printf("6. Apply SCAN\n");
		printf("7. Apply C-SCAN\n");
		printf("8. Apply LOOK\n");
		printf("9. Apply C-LOOK\n");
		printf("10. Report the best algorithm\n");
		printf("11. Report algorithms from best to worst\n");
		printf("12. Exit\n");
		printf("-----------------------------------------------\n");
		printf("option:> ");
        scanf(" %s", option); // get user input

        if(strcmp(option, "1") == 0) {
            printf("\nPlease enter the number of cylinders: ");
            scanf(" %d", &n_cylinders);
        }else if(strcmp(option, "2") == 0) {
            createRandomCylindersFile();
        }else if(strcmp(option, "3") == 0) {
            printf("\nEnter initial position of disk head: ");
            scanf(" %d", &head_position);
        }else if(strcmp(option, "4") == 0) {
            // if file is found then call function for calculations
            found = readRandomCylinders(file_found);
            if(found == 1)
                fcfs = FCFS();
        }else if(strcmp(option, "5") == 0) {
            found = readRandomCylinders(file_found);
            if(found == 1)
                sstf = SSTF();
        }else if(strcmp(option, "6") == 0) {
            found = readRandomCylinders(file_found);
            if(found == 1)
                scan = SCAN();
        }else if(strcmp(option, "7") == 0) {
            found = readRandomCylinders(file_found);
			if(found == 1)
                c_scan = C_SCAN();
        }else if(strcmp(option, "8") == 0) {
            found = readRandomCylinders(file_found);
            if(found == 1)
                look = LOOK();
        }else if(strcmp(option, "9") == 0) {
            found = readRandomCylinders(file_found);
            if(found == 1)
                c_look = C_LOOK();
        }else if(strcmp(option, "10") == 0) {
            found = readRandomCylinders(file_found);
            if (found == 1)
				determineBest();
        }else if(strcmp(option, "11") == 0) {
            found = readRandomCylinders(file_found);
			if (found == 1)
				reportBestToWorst();
        }else if(strcmp(option, "12") ==0) {
            printf("Thank you for using the program. Goodbye!");
            exit(0);
        }else {
            printf("Error! Input must be a value between 1 and 12.\n");
        }
    }
}

// program starts here
int main() {
    menuOptions();
    return 0;
}