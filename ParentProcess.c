/*
 * Author: Suha Memon
 * This code has been completed entirely by me.
 * Description of purpose of program: ParentProcess.c
    * This has a main method, helper methods for creating the answer and then finding the answer
    * choose_operator, find_answer, and choose_num
    * 
    * The main method handles the quiz game itself
    * This would handle all of the timing of my program
    * Have the parent program (ParentProcess.c) pick numbers between in some range, 
    * and an operator (+,-,*) 
    * and see if the person can answer correctly within 5 seconds.
    * If a correct answer is provided within 5 seconds, show the next question 
    and set the timer back to 0.
    An incorrect answer redisplays the question.
    If no answer with 10 seconds, the Timer process should notify the parent class,
    indicating that time is up.
    When time is up, add 1 to the count the number of missed questions 
    and go to the next question. 
    You may need to notify the timer to start.

    The parent program should prompt the user at the beginning of 
    the program for the number of questions in the quiz. 
    At the end of the questions, the program should print correct answers, 
    incorrect answer (wrong answers within time frame), 
    and missed questions (no correct answer within timeframe).
*/

// defined: so that i dont get an implicit kill warning on CIMS
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

void handle_sigint(int sig);
void handle_sigalrm(int sig);
int choose_num();
char choose_operator();
int find_answer(int val1, char operator, int val2);


// The parent program should prompt the user at the beginning of
// the program for the number of questions in the quiz.
pid_t pid;
int correct = 0, incorrect = 0, missed = 0;
int num_of_questions = 0;
volatile sig_atomic_t time_up = 0;
volatile sig_atomic_t cant_be_correct = 0;


/*
 * handle_sigalrm:
 * 
 * Purpose: Takes care of the 5 second requirement
 * 
 * Parameters: int sig
 * 
 * Return value: void
 */
void handle_sigalrm(int sig) {
    printf("\n5 seconds passed. Your answer will no longer be counted as correct.\n");
    fflush(stdout);
    cant_be_correct = 1;
}

/*
 * handle_sigint:
 * 
 * Purpose: Takes care of the 10 second requirement
 * 
 * Parameters: int sig
 * 
 * Return value: void
 */
void handle_sigint(int sig) {
    missed++;
    printf("\nTime's up. Type anything to continue to the next question\n");
    fflush(stdout);
    time_up = 1;
}


/*
 * main:
 * 
 * Purpose: Entire quiz program
 * 
 * Parameters: none
 * 
 * Return value: int: 0
 */
int main() {
    // Prompting the user for the number of questions in the quiz.
    printf("Enter the number of questions you want in your quiz: \n");
    scanf("%d", &num_of_questions);
    getchar();

    // need to fork for each of the child processes,
    // following a similar format to the original given in this code
    signal(SIGUSR1, handle_sigalrm);
    signal(SIGINT, handle_sigint);

    // for the random number generating
    srand(time(NULL));
    int user_input, answer;
    char operator;

    int q;
    for (q = 1; q <= num_of_questions; q++) {
        // forking each time: brand new execve call for each child process
        // starts a new timer for each child process
        pid = fork();
        if (pid == 0) {
            // Child process
            char *args[] = { "ChildProcess", NULL };
            // for some reason, execve was causing issues, so im using execv instead
            execv("ChildProcess", args);
            perror("execv"); // in case execv fails for some random reason
            exit(EXIT_FAILURE); 
        }

        // generate a random question and find its answer
        int val1 = choose_num();
        int val2 = choose_num();
        operator= choose_operator();
        answer = find_answer(val1, operator, val2);

        printf("\nQuestion #%d: %d %c %d = \n", q, val1, operator, val2);
        fflush(stdout);
        
        // now that I have asked the question, I will start the timer
        // start the timer
        time_up = 0;
        cant_be_correct = 0;
        // sends one signal at 5 seconds
        // send diff sigal at 10 seconds
        while (!time_up) {
            // get the user's input
            if (scanf("%d", &user_input) > 0) {
                if (user_input == answer) {
                    if (!cant_be_correct){
                        correct++;
                        printf("\nCorrect!\n");
                    } else {
                        printf("\nCorrect (but not counted as correct)\n");
                    }
                    break;
                } else {
                    printf("Incorrect. Try again: %d %c %d = \n", val1, operator, val2);
                    incorrect++;
                }
            // added this block to address entering issues on CIMS
            // works correctly now
            } else if (errno == EINTR) {
                clearerr(stdin); // Clear the error state
                continue;        // Retry input
            }

            // need to make sure the input is correctly coming in
            // had to add this to fix CIMS problems
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
        }


        // When the time's up, i need to alert the user that the time is up and what the correct answer is
        if (time_up) {
            printf("You missed that one. The correct answer was %d.\n", answer);
        }

        // kill the timer process: happens if correct or time's up
        kill(pid, SIGKILL); 
        int status;
        // resolving CIMS issues: need to wait for the child process to ensure cleanup
        waitpid(pid, &status, 0); 
    }
    // End of program: printing out quiz summary
    // correct answers, incorrect answer (wrong answers within time frame),
    // and missed questions (no correct answer within timeframe)
    printf("\nQuiz Summary:\n");
    printf("Correct Answers: %d\n", correct);
    printf("Incorrect Answers: %d\n", incorrect);
    printf("Missed Questions: %d\n", missed);

    return 0;
}


// HELPER METHODS -------------------------------------------
// Creating the question:
// Have the parent program pick numbers between in some range,
// and an operator (+,-,*)

/*
 * choose_num
 * 
 * Purpose: generate random number between 0 and 12
 * Parameters: none
 * 
 * Return value: int: value for main method to use
 */
// 
int choose_num()
{
    return rand() % 13;
}


/*
 * choose_operator
 * 
 * Purpose: finds out which operator to pick
 * 
 * Parameters: none
 * 
 * Return value: char: oeprator selected
 */
char choose_operator()
{
    int gen_selected = rand() % 3; // random between 0 and 2: for the 3 options:
    switch (gen_selected)
    {
    case 0:
        return '+';
    case 1:
        return '-';
    case 2:
        return '*';
    default:
        return '+'; // Just in case
    }
}

/*
 * find_answer
 * 
 * Purpose: calculates the answer and returns it so that the main method has the right answer
 * 
 * Parameters: int val1 (first calculating value), char operator, int val2
 * 
 * Return value: int: answer
 */
int find_answer(int val1, char operator, int val2)
{
    if (operator== '+')
    {
        return val1 + val2;
    }
    else if (operator== '-')
    {
        return val1 - val2;
    }
    else if (operator== '*')
    {
        return val1 * val2;
    }
    return 0;
}
