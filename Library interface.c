#define _CRT_SECURE_NO_WARNINGS
#define BOOK_NUM 4
#define NAME_LENGTH 200
#define AVAILABLE 10
#define NOT_AVAILABLE 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The function giving the struct of Book.
typedef struct book {
	char name[NAME_LENGTH];
	char author[NAME_LENGTH];
	int available;
	int times_borrowed;
} Book;

// the function will free all the memory we used in the program.
// godel - is the number of books we have.
// books - the pinters for the memory we want to free.
void free_memo(int godel, Book *books[])
{
	for (int i = 0; i < godel; i++) // scan all books
	{
		free(books[i]);
	}

}

// The function sorting the books array from big to small.
// godel - is the number of books we have.
// books - the pinters for the memory we want to free.
void sorting(int godel, Book *books[])
{
	for (int i = 0; i < godel; i++)
	{
		for (int j = i + 1; j < godel; j++)
		{
			//if the value upstream is larger, switch
			if (books[j]->times_borrowed > books[i]->times_borrowed)
			{
				Book temp = *books[i];
				*books[i] = *books[j];
				*books[j] = temp;
			}
		}
	}
}


// -----------------------------------------------
//                     Main
// -----------------------------------------------
// The main function
// operation - giving by the user, the type of operation the user want to do.
// godel - is the number of books we have.
// book_name[] - is string of chars that is given by the user. represent the name of the book he want to use in the operations.
// books[] - is the sring of pointers who points to the struct Book for all the books in the system.
// is_exist - a flag for checking for us if the book is existiong or not. 0 - not existing, 1- existing.
int main()
{
	int operation, godel = 0, is_exist = 0;
	char book_name[NAME_LENGTH];
	Book *books[BOOK_NUM];

	while (1 == 1)  // entering the while loop
	{
		printf("-------------------------------------\n Welcome to the library\n-------------------------------------\nLibrary menu :\n1. Add a book / Return a book\n2. Take a book\n3. Print books by times borrowed from the library\n4. Quit\nPlease choose the desired option[1 - 4] : ");
		scanf("%d", &operation);

		switch (operation)
		{
		case 1: // the add a book or return a book case
			
			//get book name
			printf("Please enter book name: ");
			scanf("%201s", book_name);

			//restart existing
			is_exist = 0;

			//scan all books
			for (int i = 0; i < godel; i++)
			{
				//if there is such book (the names are equal)
				if (strcmp(books[i]->name, book_name) == 0)
				{
					is_exist = 1; // flag

					//check if book is available
					if ((*books[i]).available == AVAILABLE)
					{
						printf("This book is already in the library :)\n");
					}

					//if not available
					else if ((*books[i]).available == NOT_AVAILABLE)
					{
						printf("Thank you for returning the book!\n");
						(*books[i]).available = AVAILABLE;
					}
					break;
				}
			}
			// if there is no book like this
			if (is_exist == 0)
			{
				// check for free space. if there is none,
				if (godel == BOOK_NUM)
				{
					printf("There is no space in the library\n");
					break;
				}
				// allocate space
				books[godel] = (Book*)malloc(sizeof(Book));

				//verify good alocation
				if (books[godel] == NULL)
				{
					printf("Memory allocation failed!\n");
					free_memo(godel, books);
					return 1;
				}
				//insert book name
				strcpy(books[godel]->name, book_name);

				//insert book author
				printf("Please enter author name: ");
				scanf("%s", books[godel]->author);

				//define parameters of new book
				books[godel]->times_borrowed = 0;
				books[godel]->available = AVAILABLE;

				//print message
				printf("The book %s was successfully added!\n", book_name);
				godel++;
			}
			break;

		case 2: //the taking book case

			//get book name
			printf("Please enter book name: ");
			scanf("%201s", book_name);

			//restart existing
			is_exist = 0;

			//scan all books
			for (int i = 0; i < godel; i++)
			{
				if (strcmp(books[i]->name, book_name) == 0)
				{
					is_exist = 1; // flag

					//check availability
					if ((*books[i]).available == AVAILABLE) // book is available
					{
						printf("Enjoy the book!\n");
						books[i]->available = NOT_AVAILABLE;
						books[i]->times_borrowed++;
					}
					else if ((*books[i]).available == NOT_AVAILABLE) // book it not available
						printf("I am sorry someone was quicker than you :(\n");
				}
			}
			// if the book does not exist
			if (is_exist == 0)
				printf("I am sorry we don't have this book.\n");
			break;

		case 3: // the print books by count case
			sorting(godel, books);
			// now that the array is sorted, print from biggest to smallest
			printf("Books available\n--------------------------------------\n");
			for (int i = 0; i < godel; i++)
				printf("Name: %s\nAuthor: %s\n-------------------------------------\n",books[i]->name,books[i]->author);
			break;

		case 4: // the quiting case
			free_memo(godel, books);
			printf("leaving so soon?");
			return 0;
		}
	}
	return 0;
}