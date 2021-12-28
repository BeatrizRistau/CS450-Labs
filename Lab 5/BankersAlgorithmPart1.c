/* For this lab, we will setup the initial data structures required to implement Banker’s algorithm. 
Customers request resources to accomplish a task and then release resources. The banker will 
grant a request only if it leaves the system in a safe state. A request that leaves the system in an 
unsafe state will be denied. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

// global variables
int i, j;

// the available amount of each resource
int available[NUMBER_OF_RESOURCES];
// the maximum demand of each customer
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
// the amount currently allocated to each customer
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
// the remaining need of each customer
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int request[NUMBER_OF_RESOURCES];
int release[NUMBER_OF_RESOURCES];

// function to update arrays when allocation changes
void needArray()
{
	// initialize need array
	for(i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
		for(j = 0; j < NUMBER_OF_RESOURCES; j ++) {
			need[i][j] = 0;
		} 
	}
	
	// adjusting array
	for(i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
		for(j = 0; j < NUMBER_OF_RESOURCES; j ++) {
			need[i][j] = maximum[i][j] - allocation[i][j];
		}
	}	
} 

// function to get each number of instances for each resource from user
void availableResources() 
{
    printf("\n");
	for(i = 0; i < NUMBER_OF_RESOURCES; i++) {
		printf("Please enter number of resources for R%d: ", i+1);
		scanf(" %d", &available[i]);
	} 
}

// function to read a file entered by the user
void readMaxRequirement() 
{
	int option = 1; // true
	
	// read file & store value in the allocation array variable
	while(option == 1) {
		FILE *file_ptr;
		char file_name[50];
		printf("\nEnter the relative path location of the file (without .txt): ");
		scanf(" %[^\n]s", &file_name); // read input until enter is pressed or new line is added
		strcat(file_name, ".txt");
		file_ptr = fopen(file_name, "r");
		char *isEmpty = NULL;
		
		// if file does not exist, display error message
		if(file_ptr == NULL) {
			printf("\nFile was not found. Please try again.");
		}
				
		// if file exists then read file
		else {
			printf("\n");
			const unsigned MAX_LENGTH = 256;
			char buffer[MAX_LENGTH];
			for(i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
				isEmpty = fgets(buffer, MAX_LENGTH, file_ptr);
				
				// if file is empty
				if(isEmpty == NULL) {
					printf("File is empty.\n");
				}
				
				// sscanf() to read formatted data. Read data from a String.
				sscanf(buffer, "%d,%d,%d,%d", &maximum[i][0],&maximum[i][1],&maximum[i][2],&maximum[i][3]);
			}
			printf("The file was read successfuly.");
			option = 0; // false
		} 
		printf("\n"); 
	}
}

// Function to get the current allocations of resources for each costumer
void readCurrentAllocation() 
{
	int option = 1; // true
	
	// read file & store value in the allocation array variable
	while(option == 1) {
		FILE *file_ptr;
		char file_name[50];
		printf("\nEnter the relative path location of the file without .txt: ");
		scanf(" %[^\n]s", &file_name); 
		strcat(file_name, ".txt"); 
		file_ptr = fopen(file_name, "r");
		char *isEmpty = NULL;
		
		// if file does not exist display error message
		if(file_ptr == NULL) {
			printf("\nFile was not found. Please try again.");
		}	
		
		// if file exists read file
		else {
			printf("\n");
			const unsigned MAX_LENGTH = 256;
			char buffer[MAX_LENGTH];
			// read line by line
			for (i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
				isEmpty = fgets(buffer, MAX_LENGTH, file_ptr); 
				//printf("%s", buffer); // for debugging
				
				if (isEmpty == NULL) {
					printf("File is empty.\n");
				}

				// sscanf() to read formatted data. Read data from a String.
				sscanf(buffer, "%d,%d,%d,%d", &allocation[i][0],&allocation[i][1],&allocation[i][2],&allocation[i][3]);
			}
			printf("The file was read successfuly.");
			option = 0; // false
		}
		printf("\n");
	}
}

// function to display values of avilable, maximum, allocation, and need arrays
void statusDataStructure()
{
	/// Available Resources
	printf("\nAvailable:\n      R1   R2   R3   R4\n   ");
	for(i = 0; i < NUMBER_OF_RESOURCES; i++) {
		printf("%5d", available[i]);
	}
	printf("\n");
	
	// Current Allocation
	printf("\nAllocation:\n      R1   R2   R3   R4\n");
	for(i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
		printf("P%d:", i+1);
		for(j = 0; j < NUMBER_OF_RESOURCES; j ++) {
			printf("%5d", allocation[i][j]);
		}
		printf("\n");
	}
	
	// Maximum
	printf("\nMaximum:\n      R1   R2   R3   R4\n");
	for(i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
		printf("P%d:", i+1);
		for(j = 0; j < NUMBER_OF_RESOURCES; j ++) {
			printf("%5d", maximum[i][j]);
		}
		printf("\n");
	} 	
	
	// Need
	printf("\nNeed:\n      R1   R2   R3   R4\n");
	for(i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
		printf("P%d:", i+1);
		for(j = 0; j < NUMBER_OF_RESOURCES; j ++) {
			printf("%5d", need[i][j]);
		}
		printf("\n");
	} 
}

// function to display menu, get user input, and call appropriate function
void menuOptions()
{
	int n_customer;
	int n_resource;
	char option;
	printf("Welcome to Banker's Algorithm!\n\n"); 
	printf("Select an option from the menu: \n");
	// loop that will execute until user selects to exit the program
	while(1)
	{
		printf("\n------------------------- Menu ------------------------------\n");
		printf("1. Available Resources\n");
		printf("2. Maximum Requirement (Read a file)\n");
		printf("3. Current Allocation (Read a file)\n");
		printf("4. Request Resources\n");
		printf("5. Release Resources \n");
		printf("6. Status of the data structures\n");
		printf("7. Exit\n");
		printf("-------------------------------------------------------------\n");
		printf("option:> ");
		scanf("%s", &option);
		
		// call appropriate function depending on the user's input
		switch(option)
		{
			case '1':
				// initialize available array
				for (i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
					available[i] = 0;
				}
				availableResources();
				break;
			case '2':
				// initialize maximum array
				for(i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
					for(j = 0; j < NUMBER_OF_RESOURCES; j ++) {
						maximum[i][j] = 0;	
					}
				}
				readMaxRequirement(); 
				needArray(); 
				break;
			case '3':
				// initialize allocation array 
				for(i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
					for(j = 0; j < NUMBER_OF_RESOURCES; j ++) {
						allocation[i][j] = 0;
					}
				} 
				readCurrentAllocation(); 
				needArray();
				break;
			case '4': 
				printf("\nThis option won't be available until next lab.\n");
				break;
			case '5':
                printf("\nThis option won't be available until next lab.\n");
				break;
			case '6':
				statusDataStructure();
				break;
			case '7':
				// display exit message & ends the program
				printf("\nThank you for using the program. Goodbye!\n");
				exit(0);
			default:
				// display error message if user enters invalid input
				printf("\nInvalid input. Input must be a number between 1 and 7.\n");
				break;
		}
	}
}

// main fucntion
int main() {
	// Program starts here
	menuOptions(); // call function menu
	return 0;
}
