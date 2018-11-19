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
 *              |   Filename        : exercise15_client.c
 *              |   Created on      : Nov 19, 2018
 *              |   Author          : Julien LE SAUCE
 * ----------------------------------------------------------------------------
 * DESCRIPTION
 *
 *  TP Unix - Chapter 3 - Exercise 15 (Client)
 *
 *  Run: ./bin/exercise15_client <county_code>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // O_* constants
#include <sys/stat.h>   // For mode constants
#include <mqueue.h>     // mqd_t
#include <mqueue.h>     // mq_open
#include <string.h>     // strlen

#define SERVER_FILE "/mqRequest"

void createClient(mqd_t* msgQueue);
void sendMessage(const mqd_t msgQueue, char* message);
void parseArguments(const int argc, char **argv);

int main(int argc, char* argv[])
{
    parseArguments(argc, argv);

    mqd_t msgQueue = 0;
    createClient(&msgQueue);
    sendMessage(msgQueue, argv[1]);

    return EXIT_SUCCESS;
}

void createClient(mqd_t* msgQueue)
{
    printf("[Log] CLIENT: Create client file\n");
    *msgQueue = mq_open(SERVER_FILE, O_WRONLY);
    if(*msgQueue == (mqd_t)-1)
    {
        perror("[Log] CLIENT (ERROR): Failed to create client file");
        exit(EXIT_FAILURE);
    }
}

void sendMessage(const mqd_t msgQueue, char* message)
{
    printf("[Log] CLIENT: Send message to server: \"%s\"\n", message);
    int rc = mq_send(msgQueue, message, strlen(message) + 1, 1);
    if(rc == -1)
    {
        perror("[Log] CLIENT (ERROR): Failed to write in queue");
        exit(EXIT_FAILURE);
    }
}

void parseArguments(const int argc, char **argv)
{
    if(argc < 2)
    {
        printf("ERROR: You must specify a department\n");
        exit(EXIT_FAILURE);
    }
    printf("Département: %s\n", argv[1]);
}
