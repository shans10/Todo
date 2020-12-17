#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
    // Printing usage if program run without any arguments or with argument "help".
    if(argc == 1 || strcmp(argv[1], "help") == 0)
    {
        printf("Usage :-\n"
                "$ ./todo add \"todo item\"  # Add a new todo\n"
                "$ ./todo ls               # Show remaining todos\n"
                "$ ./todo del NUMBER       # Delete a todo\n"
                "$ ./todo done NUMBER      # Complete a todo\n"
                "$ ./todo help             # Show usage\n"
                "$ ./todo report           # Statistics\n");

        // return the program after printing usage with error code 0.
        return 0;
    }

    // Creating files if not already exists to avoid possible segmentation file down the line.
    FILE *todo = fopen("todo.txt", "a");
    FILE *done = fopen("done.txt", "a");

    // Closing both files.
    fclose(todo);
    fclose(done);

    // For "ls" argument. Showing all the todos in todo.txt with recent first.
    if(strcmp(argv[1], "ls") == 0)
    {
        // Opening todo.txt file in read mode.
        todo = fopen("./todo.txt", "r");

        // Checking if file is empty or not.
        int size = 0;
        char buff[BUFSIZ];
        while ((fgets(buff, sizeof(buff), todo) != NULL))
        {
            size++;
        }

        // Closing the file.
        fclose(todo);

        // If file is empty then printing error message.
        if(size > 1)
        {
            // Reopening todo.txt file in read mode.
            todo = fopen("./todo.txt", "r");

            // Creating a 2D character array to store todos from todo.txt.
            int sentence = 128, lines = 100;
            char **sentences = (char **) malloc(lines * sizeof(char *));
            for(int i = 0; i < lines; i++)
                sentences[i] = (char *) malloc(sentence * sizeof(char));

            int count = 0;     // Counter to count number of todos.

            int i = 0;         // Variable to be used as array index.
            // Storing all the todos in 2D character array.
            while(!feof(todo))
            {
                count++;
                fgets(sentences[i], sentence * sizeof(char), todo);
                i++;
            }

            // Printing all the todos in reverse order(most recent first).
            for(i = count - 2; i >= 0; i--)
                printf("[%i] %s", i + 1, sentences[i]);

            // Freeing memory allotted to 2D character array.
            for(i = 0; i < lines; i++)
                free(sentences[i]);
            free(sentences);

            // Closing the file.
            fclose(todo);
        }

        // Printing error message if no todos in todo.txt
        else
            printf("There are no pending todos!");
    }

    // For "add" argument. Adding todos to todo.txt.
    else if(strcmp(argv[1], "add") == 0)
    {
        // Checking for correct number of arguments.
        if(argc == 3)
        {
            // Opening todo.txt file in append mode.
            todo = fopen("./todo.txt", "a");

            // Inserting the todo within quotes given as argument into the todo.txt file.
            fputs(argv[2], todo);
            // Inserting new line after the newly added todo.
            fputs("\n", todo);

            // Printing the success message.
            printf("Added todo: \"%s\"", argv[2]);

            // Closing the todo.txt file.
            fclose(todo);
        }

        // Printing error message if argc < 3.
        else
            printf("Error: Missing todo string. Nothing added!");
    }

    // For "del" argument. Delete a todo from todo.txt
    else if(strcmp(argv[1], "del") == 0)
    {
        // Checking for correct number of arguments.
        if(argc == 3)
        {
            // Opening todo.txt file in read mode.
            todo = fopen("./todo.txt", "r");

            // Creating a 2D character array to store todos from todo.txt.
            int sentence = 128, lines = 100;
            char **sentences = (char **) malloc(lines * sizeof(char *));
            for(int i = 0; i < lines; i++)
                sentences[i] = (char *) malloc(sentence * sizeof(char));

            int count = 0;     // Counter to count number of todos.
            int i = 0;         // Variable to be used as array index.

            // Storing all the todos in 2D character array.
            while(!feof(todo))
            {
                count++;
                fgets(sentences[i], sentence * sizeof(char), todo);
                i++;
            }

            // Closing the file opened in read mode.
            fclose(todo);

            // If todo number does not exist then print error and return program.
            if(atoi(argv[2]) < count && atoi(argv[2]) > 0)
            {
                // Reopening todo.txt file in write mode.
                todo = fopen("./todo.txt", "w");

                // Writing the todos back into the todo.txt file skipping the deleted one.
                for(i = 0; i < count - 1; i++)
                {
                    // Skipping the deleted todo.
                    if(i + 1 == atoi(argv[2]))
                    {
                        // Print the message for the deleted todo in the terminal.
                        printf("Deleted todo #%i", i + 1);
                        continue;
                    }

                    // Writing all the other todos back into the file.
                    fputs(sentences[i], todo);
                }

                // Freeing memory allotted to 2D character array.
                for(i = 0; i < lines; i++)
                    free(sentences[i]);
                free(sentences);

                // Closing the file.
                fclose(todo);
            }

            // Printing error message if invalid todo number is given.
            else
                printf("Error: todo #%i does not exist. Nothing deleted.", atoi(argv[2]));
        }

        // Printing error message if argc < 3.
        else
            printf("Error: Missing NUMBER for deleting todo.");
    }

    // For "done" argument. Marking the todo as done and saving it in done.txt.
    else if(strcmp(argv[1], "done") == 0)
    {
        // Checking for correct number of arguments.
        if(argc == 3)
        {
            // Getting the current date in yyyy-mm-dd format.
            time_t t = time(0);
            struct tm *lt = localtime(&t);

            // Opening todo.txt file in read mode.
            todo = fopen("./todo.txt", "r");

            // Creating a 2D character array to store todos from todo.txt.
            int sentence = 128, lines = 100;
            char **sentences = (char **) malloc(lines * sizeof(char *));
            for(int i = 0; i < lines; i++)
                sentences[i] = (char *) malloc(sentence * sizeof(char));

            int count = 0;     // Counter to count number of todos.

            int i = 0;         // Variable to be used as array index.
            // Storing all the todos in 2D character array.
            while(!feof(todo))
            {
                count++;
                fgets(sentences[i], sentence * sizeof(char), todo);
                i++;
            }

            // Closing the file opened in read mode.
            fclose(todo);

            // Checking if todo number is valid or not.
            if(atoi(argv[2]) < count && atoi(argv[2]) > 0)
            {

                // Reopening todo.txt file in write mode.
                todo = fopen("./todo.txt", "w");
                // Opening done.txt file in append mode.
                done = fopen("./done.txt", "a");

                // Writing the todos back into the todo.txt file skipping the deleted one.
                for(i = 0; i < count - 1; i++)
                {
                    // Skipping the deleted todo.
                    if(i + 1 == atoi(argv[2]))
                    {
                        // Print the message for the deleted todo in the terminal.
                        printf("Marked todo #%i as done.", i + 1);
                        // Add the todo to done file with current date.
                        fprintf(done, "x %d-%d-%d ",1900+lt->tm_year, lt->tm_mon + 1, lt->tm_mday);
                        fputs(sentences[i], done);
                        continue;
                    }

                    // Writing all the other todos back into the file.
                    fputs(sentences[i], todo);
                }

                // Freeing memory allotted to 2D character array.
                for(i = 0; i < lines; i++)
                    free(sentences[i]);
                free(sentences);

                // Closing the files.
                fclose(todo);
                fclose(done);
            }

            // Printing error message if todo number is invalid.
            else
                printf("Error: todo #%i does not exist.\n", atoi(argv[2]));
        }

        // Printing error message if argc < 3.
        else
            printf("Error: Missing NUMBER for marking todo as done.");
    }

    // For "report" argument. Generate a report of all the todos whether done or pending.
    else if(strcmp(argv[1], "report") == 0)
    {
        // Getting the current date in yyyy-mm-dd format.
        time_t t = time(0);
        struct tm *lt = localtime(&t);

        // Opening both todo.txt and done.txt in read mode.
        todo = fopen("./todo.txt", "r");
        done = fopen("./done.txt", "r");

        // Initializing counters for completed todos and pending todos.
        int pending = 0, completed = 0;
        // To iterate over files character by character.
        char ch;

        // Counting the pending todos.
        while((ch=fgetc(todo)) != EOF)
        {
            if(ch == '\n')
                pending++;
        }

        // Counting the completed todos.
        while((ch=fgetc(done)) != EOF)
        {
            if(ch == '\n')
                completed++;
        }

        // Printing the result with current date.
        printf("%d-%d-%d Pending : %d Completed : %d\n", 1900+lt->tm_year, lt->tm_mon + 1, lt->tm_mday, pending, completed);
    }

    return 0;
}
