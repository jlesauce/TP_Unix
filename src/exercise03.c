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
 *  TP Unix - Chapter 1 - Exercise 3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // sleep
#include <signal.h>     // signal
#include <string.h>     // strsignal

typedef void (*sighandler_t)(int);

int g_countOfReceivedSigInt = 0;

void registerToSignal(int signalToHandle, sighandler_t signalHandler);
void handleSignal(int receivedSignal);
short isLimitOfReceivedSigIntReached();

int main(void)
{
    registerToSignal(SIGINT, handleSignal);

    while(!isLimitOfReceivedSigIntReached())
    {
        printf("Count of SIGINT not yet reached...\n");
        pause();
    }

    return EXIT_SUCCESS;
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

    if(receivedSignal == SIGINT)
    {
        g_countOfReceivedSigInt++;
        printf("Count of SIGINT received = %d\n", g_countOfReceivedSigInt);
    }
}

short isLimitOfReceivedSigIntReached()
{
    return g_countOfReceivedSigInt >= 3;
}

