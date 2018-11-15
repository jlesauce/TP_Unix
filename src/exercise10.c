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
 *              |   Filename        : exercise10.c
 *              |   Created on      : Nov 15, 2018
 *              |   Author          : Julien LE SAUCE
 * ----------------------------------------------------------------------------
 * DESCRIPTION
 *
 *  TP Unix - Chapter 2 - Exercise 10
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // fork, exit, pipe, close, write, read
#include <string.h>     // strlen
#include <sys/wait.h>   // wait

char g_inputString0[100] = "J'ai encore rêvé d'elle";
char g_inputString1[100] = "J'ai mal dormi";
int g_pipe0[2] = { 0 }; // read / write pipe
int g_pipe1[2] = { 0 }; // read / write pipe

void executeChildProcess(pid_t parentPid, int childID);
void executeParentProcess(pid_t childPid);

int main(void)
{
    /*
     * Create pipes
     */
    int rc = pipe(g_pipe0);
    if(rc)
    {
        printf("ERROR: Failed to create pipe 0");
        return EXIT_FAILURE;
    }
    rc = pipe(g_pipe1);
    if(rc)
    {
        printf("ERROR: Failed to create pipe 1");
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
        executeChildProcess(parentPid, 0);
    }
    else if(childPid > 0)
    {
        /*
         * Re-fork
         */
        childPid = fork();
        if(childPid == 0)
        {
            executeChildProcess(parentPid, 1);
        }
        else if(childPid > 0)
        {
            executeParentProcess(childPid);
        }
        else
        {
            perror("Child process creation failed:");
        }
    }
    else
    {
        perror("Child process creation failed:");
    }

    return EXIT_SUCCESS;
}

void executeParentProcess(pid_t childPid)
{
    (void)childPid;
    printf("Hello from parent, pid=%d\n", getpid());
    wait(NULL);
    wait(NULL);
    printf("Goodbye from parent\n");
}

void executeChildProcess(pid_t parentPid, int childID)
{
    printf("Hello from child(%d), pid=%d, parent_pid=%d\n", childID, getpid(), parentPid);

    if(childID == 0)
    {
        close(g_pipe0[0]); // Close reading end of pipe 0
        close(g_pipe1[1]); // Close writing end of pipe 1

        write(g_pipe0[1], g_inputString0, strlen(g_inputString0));
        printf("Child0:  written: %s\n", g_inputString0);

        char buffer[100];
        int readCount = read(g_pipe1[0], &buffer, sizeof(buffer));
        buffer[readCount] = '\0';
        printf("Child0: read: <%s>\n", buffer);

        close(g_pipe0[1]); // Close writing end of pipe 0
        close(g_pipe1[0]); // Close reading end of pipe 1
        printf("Child0: Goodbye from child, pid=%d\n", getpid());
    }
    else
    {
        close(g_pipe0[1]); // Close writing end of pipe 0
        close(g_pipe1[0]); // Close reading end of pipe 1

        write(g_pipe1[1], g_inputString1, strlen(g_inputString1));
        printf("Child1:  written: %s\n", g_inputString1);

        char buffer[100];
        int readCount = read(g_pipe0[0], &buffer, sizeof(buffer));
        buffer[readCount] = '\0';
        printf("Child1: read: <%s>\n", buffer);

        close(g_pipe0[0]); // Close reading end of pipe 0
        close(g_pipe1[1]); // Close writing end of pipe 1
        printf("Child1: Goodbye from child, pid=%d\n", getpid());
    }
}
