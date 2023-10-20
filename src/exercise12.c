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
 *              |   Created on      : Nov 15, 2018
 *              |   Author          : Julien LE SAUCE
 * ----------------------------------------------------------------------------
 * DESCRIPTION
 *
 *  TP Unix - Chapter 2 - Exercise 12
 *
 *  Command : ./bin/exercise12 ls / grep in wc -l
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // fork, exit, pipe, close, write, read
#include <string.h>     // strlen
#include <sys/wait.h>   // wait

int g_pipe0[2] = { 0 }; // read / write pipe
int g_pipe1[2] = { 0 }; // read / write pipe

char* g_cmd0[3] = { NULL };
char* g_cmd1[3] = { NULL };
char* g_cmd2[3] = { NULL };

void executeChildProcess(pid_t parentPid, int childID);
void executeParentProcess(pid_t childPid);

int main(int argc, char* argv[])
{
    (void) argc;
    g_cmd0[0] = argv[1];
    g_cmd0[1] = argv[2];
    g_cmd0[2] = NULL;
    g_cmd1[0] = argv[3];
    g_cmd1[1] = argv[4];
    g_cmd1[2] = NULL;
    g_cmd2[0] = argv[5];
    g_cmd2[1] = argv[6];
    g_cmd2[2] = NULL;

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
            /*
             * Re-re-fork
             */
            childPid = fork();
            if(childPid == 0)
            {
                executeChildProcess(parentPid, 2);
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
    close(g_pipe0[0]);
    close(g_pipe0[1]);
    close(g_pipe1[0]);
    close(g_pipe1[1]);
    wait(NULL);
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

        dup2(g_pipe0[1], STDOUT_FILENO); // stdout >> writes in pipe0
        close(g_pipe1[0]);
        close(g_pipe1[1]);

        execvp(g_cmd0[0], g_cmd0); // output ls / >> output writes in pipe0

        printf("Child0: Goodbye from child, pid=%d\n", getpid());
    }
    else if(childID == 1)
    {
        dup2(g_pipe0[0], STDIN_FILENO); // reads pipe0 and injects into stdin of cmd1
        close(g_pipe0[1]);
        close(g_pipe1[0]);

        dup2(g_pipe1[1], STDOUT_FILENO); // stdout >> writes in pipe1
        execvp(g_cmd1[0], g_cmd1); // output grep in >> output writes in pipe1

        printf("Child1: Goodbye from child, pid=%d\n", getpid());
    }
    else
    {
        close(g_pipe0[0]);
        close(g_pipe0[1]);

        dup2(g_pipe1[0], STDIN_FILENO); // reads pipe1 and injects into stdin of cmd2

        close(g_pipe1[1]);

        execvp(g_cmd2[0], g_cmd2); // executes wc -l on pipe1 input

        printf("Child2: Goodbye from child, pid=%d\n", getpid());
    }
}
