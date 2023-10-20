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
 *              |   Created on      : Oct 6, 2018
 *              |   Author          : Julien LE SAUCE
 * ----------------------------------------------------------------------------
 * DESCRIPTION
 *
 *  TP Unix - Chapter 1 - Exercise 4
 *
 *  Use "kill -s SIGUSR1 <child_pid>" to end this program
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // pause, fork, getpid
#include <signal.h>     // signal, kill
#include <string.h>     // strsignal
#include <sys/types.h>  // getpid
#include <sys/wait.h>   // wait

typedef void (*sighandler_t)(int);

void executeChildProcess(pid_t parentPid);
void executeParentProcess(pid_t childPid);
void registerToSignal(int signalToHandle, sighandler_t signalHandler);
void handleSignal(int receivedSignal);

int main(void)
{
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
    printf("Hello from child, pid=%d, parent_pid=%d\n", getpid(), parentPid);
    registerToSignal(SIGUSR1, handleSignal);
    pause(); // Use "kill -s SIGUSR1 <child_pid>" to end this process
}

void executeParentProcess(pid_t childPid)
{
    int wstatus = 0;

    printf("Hello from parent, pid=%d\n", getpid());

    while((childPid = waitpid(childPid, &wstatus, WUNTRACED | WCONTINUED)) > 0)
    {
        if(WIFEXITED(wstatus))
        {
            printf("Child process state changed with status WIFEXITED\n");
        }
        else if(WEXITSTATUS(wstatus))
        {
            printf("Child process state changed with status WEXITSTATUS\n");
        }
        else if(WIFSIGNALED(wstatus))
        {
            printf("Child process state changed with status WIFSIGNALED\n");
        }
        else if(WTERMSIG(wstatus))
        {
            printf("Child process state changed with status WTERMSIG\n");
        }
        else if(WCOREDUMP(wstatus))
        {
            printf("Child process state changed with status WCOREDUMP\n");
        }
        else if(WIFSTOPPED(wstatus))
        {
            printf("Child process state changed with status WIFSTOPPED\n");
        }
        else if(WSTOPSIG(wstatus))
        {
            printf("Child process state changed with status WSTOPSIG\n");
        }
        else if(WIFCONTINUED(wstatus))
        {
            printf("Child process state changed with status WIFCONTINUED\n");
        }
        else
        {
            printf("Child process state changed with unknown status=%d\n", wstatus);
        }
    }

    printf("Goodbye from parent, child terminated with status=%d\n", wstatus);
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
    printf("Received signal %s(%d) (pid=%d)\n", strsignal(receivedSignal), receivedSignal, getpid());
}

