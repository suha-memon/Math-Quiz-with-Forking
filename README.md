# Math Quiz with Forking

In this project, I create a math quiz game by applying forking and processes concepts in C. 

The game works in the following way:
The parent process (ParentProcess.c) prompts the user at the beginning of the program to enter the number of questions that they would like to answer for the quiz. 

Then, the parent process (ParentProcess.c) chooses numbers between 0 and 12, and an operator (+,-,*). It calculates the correct answer, and then it prompts the user to answer correctly within 5 seconds. 
If the user answers correctly within 5 seconds, then it shows the following question and sets the timer back to 0. If the user enters an incorrect answer, then the question is redisplayed. If the user does not answer within 10 seconds, then the timer process notifies the parent class, indicating that the time is up. 

Once the time is up, 1 is added to the count of missed questions, and the program proceeds to ask the user the next question. This happens once the timer is notified to restart the 10 second timer. 

At the end of all of the questins, the program prints the correct answers, incorrect answers (wrong answers within time frame), and missed questions (no correct answer within timeframe).

## Running the program
Enter `make run_project` in your terminal to run the quiz game.

## Details
There are two files for the quiz game:
ParentProcess.c and ChildProcess.c.

### ParentProcess.c
This c file has a main method along with helper methods for creating the answer and then finding the answer to the question: choose_operator, find_answer, and choose_num

The main method handles the quiz game itself. This handles all timing elements of the quiz.


### ChildProcess.c
This file has a main method that handles all execve("ChildProcess", NULL, NULL). In other words, the quiz timing is handled through this file. Main initializes a handler for SIGINT, so that it can essentially validate that the program runs for 10 seconds. At the 5 second mark, it does kill(ppid, SIGUSR1) back to the parent process to alert the user that 5 seconds have passed. Then, it checks to see if 10 seconds pass. Once this happens, kill(ppid, SIGINT) signals to the ParentProcess that the process is complete and that the main one needs to be killed.