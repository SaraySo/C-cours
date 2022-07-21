#define _CRT_SECURE_NO_WARNINGS
#define NAME_LENGTH 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Define the CPU struct
typedef struct cpu
{
	char name[NAME_LENGTH];
	int copies;
	struct cpu *next;
}CPU;

//decomposeString
//Input: string to split: char *s, word array char **words
//Return: the number of words int n
//splits the instruction or cpu row into words delimited by $$$
int decomposeString(char *s, char **words)
{
	int n = 0;
	words[0] = s; // the first word begins at the head of the string

	while (*s != '\0') //scan the row
	{
		if (*s == '$' && *(s + 1) == '$'&& *(s + 2) == '$') // if 3 '$' in a row
		{
			*(s - 1) = '\0'; // the string stops before the $ sign
			s = (s + 4); // advance the pointer + skip space
			n++; // advance word count
			words[n] = s; // the next word starts at the pointer
		}
		else // if not dollar, advance pointer
			s++;
	}
	//remove the last char if it is ' ' or '\n'
	if(*(s-1)==' ' || *(s-1)=='\n')
		*(s - 1) = '\0'; 
	return n;
}

//openFile
//Input: string file name: char fname[], char mode (r\w)
//Return:pointer to files 
//Opens the file given in fname and returns a pointer. returns NULL if failes.
FILE *openFile(char fname[], char mode[])
{
	//open the files to read and write 
	FILE *filepointer = fopen(fname, mode);

	//check that file opening was OK
	if (filepointer == NULL)
	{
		printf("Error: opening %s failed\n", fname);
	}

	return filepointer;
}

//freeMem
//Input: pointer to cpu list head
//Return: void
//frees all the memory used by the cpu list
void freeMem(CPU *cpu_list_head)
{
	CPU *temp = NULL;

	while (cpu_list_head != NULL)
	{
		temp = cpu_list_head;
		cpu_list_head = cpu_list_head->next;
		free(temp);
	}
}

//removeCpu
//Input:  name char[] to_remove, CPU *cpu_list_head
//Output: CPU *cpu_list_head
//removes to_remove from cpu list
CPU *removeCpu(char to_remove[], CPU *cpu_list_head)
{
	CPU *current, *prev = NULL; //pointer
	
	//Check if list is empty
	if (cpu_list_head == NULL)
		return cpu_list_head;

	//Check if the first cpu needs to be removed
	current = cpu_list_head;
	if (strcmp(current->name, to_remove) == 0)
	{
		cpu_list_head = cpu_list_head->next;
		free(current);
		return cpu_list_head;
	}

	//Cpu to remove is in the middle. Scan until the end or until match name
	while (current != NULL && strcmp(to_remove, current->name) != 0)
	{
		prev = current;
		current = current->next;
	}
	
	if (current != NULL) // if to_remove was found
	{
		prev->next = current->next;
		free(current);
	}
	return cpu_list_head;
}


//insertCpu
//Input: pointer to head of cpu list, string cpu_name, int copies
//Return: pointer to head of cpu list
//Inserts a new cpu to the list in the correct location.
//If the list is empty, creates a new list
CPU *insertCpu(CPU *cpu_list_head, char cpu_name[], int copies)
{
	CPU *curr_cpu, *prev_cpu = NULL;

	//Allocate memory space
	CPU *new_cpu = (CPU*)malloc(sizeof(CPU));
	
	//Test allocation
	if (new_cpu != NULL) { // memory allocation sucess
		//Configure the cpu
		strcpy(new_cpu->name, cpu_name);
		new_cpu->copies = copies;
		new_cpu->next = NULL;
	}
	else //memory allocation failed
	{
		//free previous memory
		printf("Error: memory allocation failed\n");
		freeMem(cpu_list_head);
		return NULL;
	}

	//check if list is empty
	if (cpu_list_head == NULL)
		cpu_list_head = new_cpu; // the head is the new cpu

	//If the list is not empty, insert the new cpu in the correct place in the chain
	else
	{
		//Handle beginning
		if (strcmp(new_cpu->name,cpu_list_head->name)<=0) //if the name of new cpu is smaller than name of head
		{
			new_cpu->next = cpu_list_head; //inset new cpu in the head of list
			cpu_list_head = new_cpu;
		}
		else //if not we need to scan the list
		{
			// scan the list untill end of list, or find a smaller cpu name
			curr_cpu = cpu_list_head; 
			while (curr_cpu != NULL && strcmp(new_cpu->name, curr_cpu->name)>0)
			{
				prev_cpu = curr_cpu;
				curr_cpu = curr_cpu->next;
			}

			//Insert new cpu to list
			prev_cpu->next = new_cpu;
			new_cpu->next = curr_cpu;
		}
	}

	//return head of list
	return cpu_list_head;
}

//Finalize
//Input: pointer to cpu list head *cpu_list_head, pointer to output file *fPtrOut
//Return: int 0 if no errors, 1 if error
//Writes cpu list to file, frees memory and exists
int finalize(CPU *cpu_list_head, FILE *fPtrOut)
{
	CPU *temp = cpu_list_head;
	//Scan cpu list and print to file
	while (temp != NULL)
	{
		fprintf(fPtrOut, "%s $$$ %d", temp->name, temp->copies);
		if (temp->next != NULL)
			fprintf(fPtrOut,"\n");
		temp = temp->next;
	}

	//free memory
	freeMem(cpu_list_head);
	//close file
	fclose(fPtrOut);
	return 0;
}

//findCpu
//Input: pointer to cpu list head *cpu_list_head, cpu_name string
//Return: pointer to cpu
//returns a pointer to the cpu that has the cpu_name name
CPU *findCpu(CPU *cpu_list_head, char cpu_name[])
{
	while (cpu_list_head != NULL)
	{
		if (strcmp(cpu_list_head->name, cpu_name) == 0)
			return cpu_list_head;
		cpu_list_head = cpu_list_head->next;
	}
	return NULL; // if not found
}

//execute_instructions
//Input: pointer to each file *fPtrInstruct,*fPtrList,*fPtrOut
//Return: int 0 if no errors, 1 if error
//Scans the instruction file and executes
int execute_instructions(FILE *fPtrInstruct,FILE *fPtrList, FILE *fPtrOut)
{
	//Define variables
	char instruction[401], *prev_name, *new_name, *instruction_params[3]; // holds the current instruction
	char cpu_row[401], *cpu_name[2];// for reading cpu file

	int n = 0, copies = 0;
	CPU *cpu_list_head = NULL, *temp = NULL;

	//scan instruction file
	while (fgets(instruction, 401, fPtrInstruct) != NULL)
	{		
		//decompose using decomposeString
		n = decomposeString(instruction, instruction_params);

		//check what instruction to execute
		//Initialize instruction
		if (strstr(instruction, "Initialize") != NULL)
		{
			//Read the cpus.txt file until we reach EOF
			while (fgets(cpu_row, 401, fPtrList) != NULL)
			{
				//Split string into name and copies
				decomposeString(cpu_row, cpu_name);
				copies = atoi(cpu_name[1]);
				//printf("%s;%d;\n",cpu_name[0], copies);

				//insert new cpu to cpu list
				cpu_list_head = insertCpu(cpu_list_head, cpu_name[0], copies);

				//Verify memory allocation
				if (cpu_list_head == NULL)
					return 1;
			}
		}

		//Finalize instruction
		else if (strstr(instruction, "Finalize") != NULL)
		{
			//close files
			fclose(fPtrInstruct);
			fclose(fPtrList);

			//call finalize
			return finalize(cpu_list_head, fPtrOut);
		}
		//Rename instruction
		else if (strstr(instruction, "Rename") != NULL)
		{
			//get new and old names
			prev_name = instruction_params[1];
			new_name = instruction_params[2];

			// find #of copies
			temp = findCpu(cpu_list_head, prev_name); //return a pointer to the old CPU

			if (temp != NULL) // if CPU exists in stock
			{
				copies = temp->copies;
				//remove old entry and insert again at correct location
				cpu_list_head = removeCpu(prev_name, cpu_list_head);
				cpu_list_head = insertCpu(cpu_list_head, new_name, copies);
			}
		}
		//Returned_from_customer instruction
		else if (strstr(instruction, "Returned_from_customer") != NULL)
		{
			//get the cpu name and copies to return
			prev_name = instruction_params[1];
			copies = atoi(instruction_params[2]);

			//locate the cpu
			temp = findCpu(cpu_list_head, prev_name);

			//check if such a processor exits
			if (temp == NULL) //doesn't exist in stock, insert new CPU to stock
				cpu_list_head = insertCpu(cpu_list_head, prev_name, copies);
			else //exists in stock, add copies
				temp->copies += copies;
		}
		//Fatal_malfunction instruction
		else if (strstr(instruction, "Fatal_malfunction") != NULL)
		{
			//get the cpu name and copies to destroy
			prev_name = instruction_params[1];
			copies = atoi(instruction_params[2]);

			//locate the cpu
			temp = findCpu(cpu_list_head, prev_name);
			if (temp != NULL) // we have found the CPU
			{
				if (temp->copies < copies) // if there are more cpus to destroy than stock
					temp->copies = 0;
				else
					temp->copies -= copies; //remove from stock
			}
		}
	}
	return 1;
}

//-----------------------------------------------------------------------------------------------
//                                      MAIN
//-----------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	//Check for correct number of arguments
	if (argc != 4)
	{
		printf("Error: invalid number of arguments (<%d> instead of 3)\n", argc);
		return 1;
	}

	//Open files and check validity using *openFile
	FILE *fPtrList = openFile(argv[1], "r");
	FILE *fPtrInstruct = openFile(argv[2], "r");
	FILE *fPtrOut = openFile(argv[3], "w");

	//exit with code 1 if any are NULL
	if (fPtrInstruct == NULL || fPtrList == NULL || fPtrOut == NULL)
		return 1;

	return execute_instructions(fPtrInstruct, fPtrList, fPtrOut);
}