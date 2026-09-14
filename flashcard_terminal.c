#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 150

// struct for questions
typedef struct {
    char question[MAX_SIZE];
    char optionA[MAX_SIZE];
    char optionB[MAX_SIZE];
    char optionC[MAX_SIZE];
    char optionD[MAX_SIZE];
    char correctOption[MAX_SIZE];
} Question;

// Function prototypes
int get_set_size();
void load_questions(Question* questions, int num_questions);
void generate_questions(Question* questions, int num_questions);
char display_questions(Question* questions, int question_num);
int check_answer(Question* questions, int question_num, char ans);
void track_score(Question* questions, int num_questions, int correct);

int main() {
    // time seed
    srand(time(NULL));

    // calls function to get number of question sin flashcard set
    int num_questions = get_set_size();
    if (num_questions <= 0) {
        printf("No questions loaded or file error.\n");
        return 0;
    }

    // allocated memory for total number of questions
    Question* questions = malloc(sizeof(Question) * num_questions);
    if (questions == NULL) {
        printf("Memory allocation failed.\n");
        return 0;
    }

    // calls function to load the questions in the file to the struct
    load_questions(questions, num_questions);
    // calls function to generate a random questions
    generate_questions(questions, num_questions);

    // frees the memory allocated for questions
    free(questions);
    return 0;
}


// function gets the size of the flash card set by reading the first number in the file and returns tha tot the calling function
int get_set_size() {
    FILE* fp = fopen("flashcard_questions.txt", "r");

    // checks if file was opened properly
    if (fp == NULL) {
        printf("Error opening question file.\n");
        return 0;
    }

    int num_questions = 0;

    // read the number of questions from the file
    if (fscanf(fp, "%d", &num_questions) == 1) {
        fgetc(fp);
    }

    fclose(fp);
    return num_questions;
}

// function loads the questions in the file to the questions struct
void load_questions(Question* questions, int num_questions) {
    FILE* fp = fopen("flashcard_questions.txt", "r");

    // checks if file was opened properly
    if (fp == NULL) {
        printf("Error opening question file.\n");
        return 0;
    }

    // creates buffer for reading from file
    char line[MAX_SIZE];
    // keeps track of the question that is being read
    int question_counter = 0;

    // skips the first line of the file which contains only the number of questions in the set
    fgets(line, sizeof(line), fp);

    // reads each line of the file till the newline
    while (fgets(line, sizeof(line), fp) != NULL) {
        // changes the newline to the end of the line
        line[strcspn(line, "\n")] = '\0';

        char type = line[0];

        // determines which line of the question is being read and saves it to its corresponding place in the questoins struct
        switch (type) {
        case 'Q': strcpy(questions[question_counter].question, line + 3); break;
        case 'a': strcpy(questions[question_counter].optionA, line + 3); break;
        case 'b': strcpy(questions[question_counter].optionB, line + 3); break;
        case 'c': strcpy(questions[question_counter].optionC, line + 3); break;
        case 'd': strcpy(questions[question_counter].optionD, line + 3); break;
        case 'A': strcpy(questions[question_counter].correctOption, line + 3); question_counter++; break;
        default: break;
        }
    }

    fclose(fp);
}


// function generate random question numbers and calls the functions to display the question, check if the user enter answer is correct, and track the score
void generate_questions(Question* questions, int num_questions) {
    // allocates memory for intger array that will indicate if the corresponding question # had already been displayed
    int* done_questions = malloc(sizeof(int) * num_questions);
    int question_num, more_questions, correct = 0;
    char ans;

    // fills the done_questions arrays with 0
    for (int i = 0; i < num_questions; i++) {
        done_questions[i] = 0;
    }

    do {
        more_questions = 0;

        // randomly picks a number in the rang of [0, number of questions]
        do {
            question_num = rand() % num_questions;
        } while (done_questions[question_num] == 1);  // checks if that queston number has already been done

        // updates done_questions array ot show that question has now been done
        done_questions[question_num] = 1;
        // calls function to display the question and get the users answer
        ans = display_questions(questions, question_num);
        // calls function to check if the user's answer is correct, if it is it adds 1 to the number of correct answers
        correct += check_answer(questions, question_num, ans);

        // checks if there are questions that still need to be done
        for (int i = 0; i < num_questions; i++) {
            if (done_questions[i] == 0) {
                more_questions = 1;
            }
        }

    } while (more_questions == 1);   // confirms if there are questions that still need ot be done

    //  calls function to display ending message and how many questions the user got correct
    track_score(questions, num_questions, correct);
    // frees the memory allocated to done_questions
    free(done_questions);
}

// function display the question and their options to the user and gets the users inpupt and return that to the calling function
char display_questions(Question* questions, int question_num) {
    char ans;

    // prints out each part of the question
    printf("Question: %s\n", questions[question_num].question);
    printf("a) %s\n", questions[question_num].optionA);
    printf("b) %s\n", questions[question_num].optionB);
    printf("c) %s\n", questions[question_num].optionC);
    printf("d) %s\n", questions[question_num].optionD);
    printf("Your answer (a/b/c/d): ");

    do {
        // read the users input
        ans = fgetc(stdin);
        while (fgetc(stdin) != '\n');

        // input validation
        if (ans != 'a' && ans != 'b' && ans != 'c' && ans != 'd') {
            printf("Invalid Input, Please Try Again: ");
        }

    } while (ans != 'a' && ans != 'b' && ans != 'c' && ans != 'd');  // confirms input validation

    return ans;
}

// function check if the answer enter by the user is correct then returns a 1 if correct or a 0 if incorrect
int check_answer(Question* questions, int question_num, char ans) {
    int cmp = -1;

    // compares the users answer to the correct answer
    switch (ans) {
    case 'a': cmp = strcmp(questions[question_num].optionA, questions[question_num].correctOption); break;
    case 'b': cmp = strcmp(questions[question_num].optionB, questions[question_num].correctOption); break;
    case 'c': cmp = strcmp(questions[question_num].optionC, questions[question_num].correctOption); break;
    case 'd': cmp = strcmp(questions[question_num].optionD, questions[question_num].correctOption); break;
    default: break;
    }

    // displays correct answer message if answer was correct
    if (cmp == 0) {
        printf("Answer Correct!\n\n");
        return 1;
    }
    // display incorrect answer message and the ocrrect answer if the answer was incorrect
    else {
        printf("Answer Incorrect\nCorrect Answer is....");
        printf("%s\n\n", questions[question_num].correctOption);
        return 0;
    }
}

// function display the number of questions the user got correct and a final message
void track_score(Question* questions, int num_questions, int correct) {
    printf("You got %d/%d questions correct.\n", correct, num_questions);

    if ((double)correct / num_questions > 0.5) {
        printf("Great Job!\n\n");
    }
    else {
        printf("Better luck next time.");
    }
}
