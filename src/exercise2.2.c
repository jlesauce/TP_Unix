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
 *              |   Filename        : exercise2.2.c
 *              |   Created on      : Oct 6, 2018
 *              |   Author          : Julien LE SAUCE
 * ----------------------------------------------------------------------------
 * DESCRIPTION
 *
 *  TP Unix - Chapter 1 - Exercise 2 (Part 2)
 *
 */

#include <stdio.h>
#include <stdlib.h>     // srand
#include <unistd.h>     // sleep, pause, fork, getpid
#include <signal.h>     // signal, kill
#include <string.h>     // strsignal
#include <sys/types.h>  // getpid
#include <time.h>       // time

#define TRUE  1
#define FALSE 0

typedef void (*sighandler_t)(int);

void executeChildProcess(pid_t parentPid);
void sendRandomSignalToParentProcess(pid_t parentPid);
void sendSignalToParentProcess(pid_t parentPid, int signal);
int getRandomSignal();
void executeParentProcess();
void registerToAllSignals();
void unregisterAllSignals();
void registerToSignal(int signalToHandle, sighandler_t signalHandler);
void handleSignal(int receivedSignal);

int main(void)
{
    srand(time(NULL));

    pid_t parentPid = getpid();
    printf("Parent PID = %d\n", parentPid);

    registerToAllSignals();

    pid_t childPid = fork();
    if(childPid == 0)
    {
        executeChildProcess(parentPid);
    }
    else if(childPid > 0)
    {
        executeParentProcess();
    }
    else
    {
        perror("Child process creation failed:");
    }

    return EXIT_SUCCESS;
}

void executeChildProcess(pid_t parentPid)
{
    printf("Hello from child, pid=%d\n", getpid());
    unregisterAllSignals();

    for(int ellapsedSeconds = 0; ellapsedSeconds < 50; ++ellapsedSeconds)
    {
        sendRandomSignalToParentProcess(parentPid);
        sleep(1);
    }

    sendSignalToParentProcess(parentPid, SIGKILL);
}

void sendRandomSignalToParentProcess(pid_t parentPid)
{
    sendSignalToParentProcess(parentPid, getRandomSignal());
}

void sendSignalToParentProcess(pid_t parentPid, int signal)
{
    int rc = kill(parentPid, signal);
    if(rc < 0)
    {
        perror("kill function failed:");
    }
}

int getRandomSignal()
{
    int signal = SIGHUP + (rand() % SIGUNUSED);

    // Ignore SIGKILL and SIGSTOP signals
    if(signal == SIGKILL || signal == SIGSTOP)
    {
        signal++;
    }

    return signal;
}

void executeParentProcess()
{
    printf("Hello from parent, pid=%d\n", getpid());

    while(TRUE) // Use 'kill -9 <pid>" to kill this program
    {
        pause();
    }
}

void registerToAllSignals()
{
    for(int signal = SIGHUP; signal <= SIGUNUSED; ++signal)
    {
        // Ignore SIGKILL and SIGSTOP signals
        if(signal == SIGKILL || signal == SIGSTOP)
        {
            continue;
        }
        registerToSignal(signal, handleSignal);
    }
}

void unregisterAllSignals()
{
    for(int signalNumber = SIGHUP; signalNumber <= SIGUNUSED; ++signalNumber)
    {
        // Ignore SIGKILL and SIGSTOP signals
        if(signalNumber == SIGKILL || signalNumber == SIGSTOP)
        {
            continue;
        }
        signal(signalNumber, SIG_DFL);
    }
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

