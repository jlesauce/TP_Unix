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
#include <stdlib.h>
#include <unistd.h>     // getpid
#include <signal.h>     // signal
#include <string.h>     // strsignal
#include <sys/wait.h>   // waitpid

#define MAX_NB_OF_ELEMENTS 100

int g_sizeOfList = 0;
int g_listOfElements[MAX_NB_OF_ELEMENTS] = { 0 };
int g_searchedElement = 0;

typedef void (*sighandler_t)(int);

void executeChildProcess(pid_t parentPid);
void executeParentProcess(pid_t childPid);
int searchElementInArray(int startIndex, int stopIndex);
void sendSignalToParentProcess(pid_t parentPid, int signal);
int parseArguments(int argc, char **argv);
void printArguments();

int main(int argc, char **argv)
{
    int rc = parseArguments(argc, argv);
    if(rc)
    {
        printf("ERROR: Argument parsing failed\n");
        return EXIT_FAILURE;
    }

    pid_t parentPid = getpid();
    printf("Parent PID = %d\n", parentPid);

    pid_t childPid = fork();
    if(childPid == 0)
    {
        executeChildProcess(parentPid);
    }
    else if(childPid > 0)
    {
        executeParentProcess(childPid);
    }
    else
    {
        perror("Child process creation failed:");
    }

    return EXIT_SUCCESS;
}

void executeChildProcess(pid_t parentPid)
{
    int startIndex = g_sizeOfList / 2;
    int stopIndex = g_sizeOfList;

    printf("Hello from child, pid=%d, parent_pid=%d\n", getpid(), parentPid);
    printf("Child will search from index %d to %d\n", startIndex, stopIndex);

    int found = searchElementInArray(startIndex, stopIndex);
    if(found)
    {
        printf("** Element found by child **\n");
        sendSignalToParentProcess(parentPid, SIGKILL);
    }

    printf("Goodbye from child\n");
}

void executeParentProcess(pid_t childPid)
{
    int wstatus = 0;
    int startIndex = 0;
    int stopIndex = g_sizeOfList / 2;

    printf("Hello from parent, pid=%d\n", getpid());
    printf("Parent will search from index %d to %d\n", startIndex, stopIndex);

    while((childPid = waitpid(childPid, &wstatus, WUNTRACED | WCONTINUED)) > 0)
    {
        int found = searchElementInArray(startIndex, stopIndex);
        if(found)
        {
            kill(childPid, SIGKILL);
            printf("** Element found by parent **\n");
        }
    }

    printf("Goodbye from parent, child terminated with status=%d\n", wstatus);
}

int searchElementInArray(int startIndex, int stopIndex)
{
    for(int elementIndex = startIndex; elementIndex < stopIndex; ++elementIndex)
    {
        if(g_listOfElements[elementIndex] == g_searchedElement)
        {
            return 1;
        }
    }
    return 0;
}

void sendSignalToParentProcess(pid_t parentPid, int signal)
{
    int rc = kill(parentPid, signal);
    if(rc < 0)
    {
        perror("kill function failed:");
    }
}

int parseArguments(int argc, char **argv)
{
    if(argc < 3)
    {
        printf("ERROR: You must specify an array of integers + the integer to search\n");
        return -1;
    }

    // Read list of integers
    g_sizeOfList = argc - 2;
    for(int elementIndex = 0; elementIndex < g_sizeOfList; ++elementIndex)
    {
        int intValue = strtol(argv[elementIndex + 1], (char **)NULL, 10);
        g_listOfElements[elementIndex] = intValue;
    }

    // Read element to search
    g_searchedElement = strtol(argv[argc - 1], (char **)NULL, 10);

    printArguments();

    return 0;
}

void printArguments()
{
    if(g_sizeOfList > 0)
    {
        printf("Size of list: %d\n", g_sizeOfList);
        printf("List: ");
        for(int elementIndex = 0; elementIndex < g_sizeOfList; ++elementIndex)
        {
            printf("%d, ", g_listOfElements[elementIndex]);
        }
        printf("\nSearched element: %d\n", g_searchedElement);
    }
}
