/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Julien LE SAUCE
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ----------------------------------------------------------------------------
 *  FILE        |   Project         : TP_Unix
 *              |   Created on      : Oct 5, 2018
 *              |   Author          : Julien LE SAUCE
 * ----------------------------------------------------------------------------
 * DESCRIPTION
 *
 *  TP Unix - Chapter 1 - Exercise 5
 *
 */

#include <stdio.h>
#include <stdlib.h>     // srand
#include <unistd.h>     // getpid
#include <signal.h>     // signal
#include <string.h>     // strsignal
#include <time.h>       // time

int g_guessDurationInSeconds = -1;
int g_multiplicand = 0;
int g_multiplier = 0;

typedef void (*sighandler_t)(int);

void gameLoop();
void registerToSignal(int signalToHandle, sighandler_t signalHandler);
void parseArguments(int argc, char **argv);
void handleSignal(int receivedSignal);
void startGameTimer(int timerLimitInSeconds);
void generateMultiplicandAndMultiplier();
int askUserToEnterTheSolution();

int main(int argc, char **argv)
{
    srand(time(NULL));
    registerToSignal(SIGINT, handleSignal);
    registerToSignal(SIGALRM, handleSignal);

    parseArguments(argc, argv);

    if(g_guessDurationInSeconds <= 0)
    {
        printf("ERROR: You must specify a positive integer\n");
        return EXIT_FAILURE;
    }
    printf("Goal: You must guess the solution of the multiplication in less than %d seconds\n",
           g_guessDurationInSeconds);
    startGameTimer(g_guessDurationInSeconds);

    generateMultiplicandAndMultiplier();
    printf("%d x %d = ?\n", g_multiplicand, g_multiplier);
    gameLoop();

    return EXIT_SUCCESS;
}

void gameLoop()
{
    while(((g_multiplicand * g_multiplier)) != askUserToEnterTheSolution())
    {
        printf("No... Try again looser!\n");
    }
    printf("Congratulation you reached primary school level!\n");
}

void parseArguments(int argc, char **argv)
{
    if(argc > 2)
    {
        printf("ERROR: Only one argument expected\n");
        return;
    }
    else if(argc < 2)
    {
        printf("ERROR: You must specify the timer value in seconds\n");
        return;
    }

    g_guessDurationInSeconds = strtol(argv[1], (char **)NULL, 10);
}

void registerToSignal(int signalToHandle, sighandler_t signalHandler)
{
    sighandler_t returnedSignal = signal(signalToHandle, signalHandler);
    if(returnedSignal == SIG_ERR)
    {
        printf("Signal function failed for signal: %d\n", signalToHandle);
        return;
    }
}

void handleSignal(int receivedSignal)
{
    if(receivedSignal == SIGINT)
    {
        printf("\nYou failed, solution is : %d\n", g_multiplicand * g_multiplier);
        exit(EXIT_SUCCESS);
    }
    else if(receivedSignal == SIGALRM)
    {
        printf("\nTimer expired !\n");
        printf("You failed, solution is : %d\n", g_multiplicand * g_multiplier);
        exit(EXIT_SUCCESS);
    }
}

void startGameTimer(int timerLimitInSeconds)
{
    alarm(timerLimitInSeconds);
}

void generateMultiplicandAndMultiplier()
{
    g_multiplicand = rand() % (10 + 1);
    g_multiplier = rand() % (10 + 1);
}

int askUserToEnterTheSolution()
{
    int guessedSolution = -1;

    printf("Please enter the solution:\n");
    scanf("%d", &guessedSolution);

    while(guessedSolution < 0)
    {
        printf("ERROR: Please enter a number higher or equal to zero\n");
        guessedSolution = askUserToEnterTheSolution();
    }

    return guessedSolution;
}

