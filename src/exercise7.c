/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Julien LE SAUCE
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
 *              |   Filename        : exercise7.c
 *              |   Created on      : Nov 15, 2018
 *              |   Author          : Julien LE SAUCE
 * ----------------------------------------------------------------------------
 * DESCRIPTION
 *
 *  TP Unix - Chapter 2 - Exercise 7
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // fork, exit, pipe, close, write, read
#include <string.h>     // strlen
#include <sys/wait.h>   // wait

char g_inputString[100] = "Hello!";
int g_pipe0[2] = { 0 }; // read / write pipe

void executeChildProcess(pid_t parentPid);
void executeParentProcess(pid_t childPid);

int main(void)
{
    /*
     * Create pipe
     */
    int rc = pipe(g_pipe0);
    if(rc)
    {
        printf("ERROR: Failed to create pipe");
        return EXIT_FAILURE;
    }

    /*
     * Fork
     */
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

void executeParentProcess(pid_t childPid)
{
    (void) childPid;
    printf("Hello from parent, pid=%d\n", getpid());

    close(g_pipe0[0]); // Close reading end of pipe

    // Write input string and close writing end of pipe
    write(g_pipe0[1], g_inputString, strlen(g_inputString) + 1);
    close(g_pipe0[1]);

    wait(NULL);
}

void executeChildProcess(pid_t parentPid)
{
    printf("Hello from child, pid=%d, parent_pid=%d\n", getpid(), parentPid);

    close(g_pipe0[1]); // Close writing end of pipe

    // Read a string using pipe
    char readString[100];
    read(g_pipe0[0], readString, 100);

    close(g_pipe0[0]); // Close reading end of pipe

    printf("Child: Received string : <%s>\n", readString);
}

