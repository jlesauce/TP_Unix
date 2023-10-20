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
 *              |   Created on      : Nov 19, 2018
 *              |   Author          : Julien LE SAUCE
 * ----------------------------------------------------------------------------
 * DESCRIPTION
 *
 *  TP Unix - Chapter 3 - Exercise 15 (Server)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // O_* constants
#include <sys/stat.h>   // For mode constants
#include <mqueue.h>     // mqd_t
#include <mqueue.h>     // mq_open
#include <signal.h>     // signal

typedef void (*sighandler_t)(int);

#define SERVER_FILE "/mqRequest"

void createServer(mqd_t* msgQueue, struct mq_attr* serverAttributes);
int disconnectServer();
void registerToSignal(int signalToHandle, sighandler_t signalHandler);
void handleSignal(int receivedSignal);
void printCounty(const int countyCode);

char* g_counties[20] = { "N/A", "Ain", "Aisne", "Allier", "Alpes-de-Haute-Provence", "Hautes-Alpes", "Alpes-Maritimes",
                         "Ardèche", "Ardennes", "Ariège", "Aube", "Aude", "Aveyron", "Bouches-du-Rhône", "Calvados",
                         "Cantal", "Charente", "Charente-Maritime", "Cher", "Corrèze" };

int main()
{
    struct mq_attr serverAttributes;
    mqd_t msgQueue = 0;
    createServer(&msgQueue, &serverAttributes);

    char* buffer = (char*)malloc((size_t)serverAttributes.mq_msgsize);
    printf("[Log] SERVER: Ready\n");

    unsigned int msgPriority = 0;
    while(1)
    {
        int nbBytesReceived = mq_receive(msgQueue, buffer, (size_t)serverAttributes.mq_msgsize, &msgPriority);
        if(nbBytesReceived == -1)
        {
            perror("[Log] SERVER (ERROR): Receive failed");
            exit(EXIT_FAILURE);
        }

        int countyCode = atoi(buffer);
        printCounty(countyCode);
    }

    return EXIT_SUCCESS;
}

void createServer(mqd_t* msgQueue, struct mq_attr * serverAttributes)
{
    printf("[Log] SERVER: Create server file..\n");
    *msgQueue = mq_open(SERVER_FILE, O_RDONLY | O_CREAT, 0644, NULL);
    if(*msgQueue == (mqd_t)-1)
    {
        perror("[Log] SERVER (ERROR): Failed to create server file");
        exit(EXIT_FAILURE);
    }

    registerToSignal(SIGINT, handleSignal);

    printf("[Log] SERVER: Retrieve server attributes..\n");
    if(mq_getattr(*msgQueue, serverAttributes) != 0)
    {
        perror("[Log] SERVER (ERROR): Failed to get server attributes");
        exit(EXIT_FAILURE);
    }
}

int disconnectServer()
{
    int rc = mq_unlink("/mqRequest");
    if(rc != 0)
    {
        perror("[Log] SERVER (ERROR): Failed to disconnect server");
        return EXIT_FAILURE;
    }
    printf("[Log] SERVER: Disconnected %s\n", SERVER_FILE);
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
    if(SIGINT == receivedSignal)
    {
        int rc = disconnectServer();
        exit(rc);
    }
}

void printCounty(const int countyCode)
{
    if(countyCode < 1 || countyCode >= 20)
    {
        printf("ERROR: County code out of bounds\n");
        return;
    }
    printf("County: %s\n", g_counties[countyCode]);
}
