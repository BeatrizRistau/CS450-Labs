#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* The objective of this lab is to implement one of the basic CPU scheduling algorithms First-Come, First
Served (FCFS) scheduling. For this lab, assume that all processes arrive at the same time instance and in
the same order in which user enters their information or the order in which information is read from
file. */

// global variables
int i;

// function to calculate FCFS scheduling and display results
void FCFS (int n_process, int b_time[])
{
	// declaring variables
    int wait_time[50];
    int turnaround_time[50];
	double avg_wt = 0.0;
	double avg_tat = 0.0;
	double total_wt = 0.0;
	double total_tat = 0.0;

	wait_time[0] = 0; // wait time for first process is 0
	
	// calculate waiting time
	for(i = 1; i < n_process; i++) {
		wait_time[i] = b_time[i-1] + wait_time[i-1];
		total_wt += wait_time[i]; // get total waiting time
	}
	
	// calculate turnaround time
	for(i = 0; i < n_process; i++) {
		turnaround_time[i] = b_time[i] + wait_time[i];
		total_tat += turnaround_time[i];  // get total turnaround time
	} 

	// create columns to display result
    printf("--------------------------------------------------\n");
	printf("\t P#\t BT\t TAT\t WT\n");

	// display output
	for(i = 0; i < n_process; i++) {
		printf("\t %d\t %d\t %d\t %d\n", i+1, b_time[i], turnaround_time[i], wait_time[i]);
	}
	
	// display output for average times
	avg_wt = total_wt / n_process; // calculate avg wait time
	avg_tat = total_tat / n_process; // calculate avg turnaround time
    printf("\nAverage Wait Time is: %1.2f", avg_wt);
	printf("\nAverage Turnaround Time is: %1.2f\n\n", avg_tat);
	return;	// return to menu options
} 

// function to get user input on no. of processes and burst time
void optionB()
{   
    int process_id; 

	// get input from user
	printf("\nEnter the number of processes: ");
	scanf(" %d", &process_id);
	
	int burst_time[50]; // declare variable
	
	// get input from user and store in array
	for (i = 0; i < process_id; i++) {
		printf("Burst time of process %d: ", i+1);
		scanf(" %d", &burst_time[i]);
	}
	
	FCFS(process_id, burst_time); // call function for calculations
} 

// function to read input from a file given by the user
void optionA()
{
    // declaring variables
    char file_name[50];
    int burst_time[50];
    int process_id;
	FILE *file_path;

	printf("\nPlease enter the name of the file (without .txt): ");
	scanf(" %s", file_name);
	strcat(file_name, ".txt"); // append extension .txt to the name of the file
	file_path = fopen(file_name, "r"); // open file given by the user

	// check if file name exists, and show error message if it doesn't
	if (file_path == NULL) {
		printf("\nError! Cannot open the file.\n");	
		return; // return to menu options
	}

    fscanf(file_path, "%d", &process_id); // read first line to get number of processes
    //printf("%d\n", process_id); // for debugging
	
    // go through second line and store values separated by comma into array
	for (i = 0; i < process_id; i++) {
		fscanf(file_path, "%d,", &burst_time[i]);
		//printf("%d\n", burst_time[i]); // for debugging
    } 
    
	fclose(file_path); // closing file
	FCFS(process_id, burst_time); // calling function for calculations
}

// function where user selects an option from the menu options
void optionsMenu() {

	char options;

	// keeps executing until user selects to exit the program
	while(1) {
		printf("--------------------------------------------------\n");
		printf("Please select from the following options: \n");
		printf("a) Read input from file \nb) Enter the input \nc) Exit\n");
		printf("--------------------------------------------------\n");
		printf(":> ");
		scanf(" %c", &options); // get user's input

		if(options == 'a') 
			optionA();
		else if(options == 'b')
			optionB();
		else if(options == 'c') {
			printf("\nThank you for using the program. Goodbye!");
			sleep(1);
			exit(0);
		}
		// if user enters invalid input, show error message
		else	
			printf("\nInvalid input. Input must be a character a-c.\n");
	} 
}

// main function
int main() {
	// programs starts here
	printf("FCFS Scheduling\n");
	optionsMenu(); // call method to display menu and get input from user      
	return 0;  
} 