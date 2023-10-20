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
 *  TP Unix - Chapter 2 - Exercise 13 (Writer)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // close, write
#include <sys/wait.h>   // wait
#include <sys/stat.h>   // mkfifo
#include <fcntl.h>      // open

#define PIPE_NAME "MY_NAMED_PIPE"

int main(void)
{
    int named_pipe = 0;
    int rc = 0;

    // Unlink named pipe if still existing
    rc = unlink(PIPE_NAME);
    if(rc == 0)
    {
        printf("Old pipe reset\n");
    }

    // Create named pipe
    rc = mkfifo(PIPE_NAME, 0640); // -rw-r----- (stat -c %a <file>)
    if(rc < 0)
    {
        perror("Failed to create pipe:");
        return EXIT_FAILURE;
    }

    // Open pipe
    named_pipe = open(PIPE_NAME, O_WRONLY);
    if(named_pipe < 0)
    {
        perror("Failed to open pipe:");
        return EXIT_FAILURE;
    }

    // Write in pipe
    printf("Writer: Write first sentence\n");
    write(named_pipe, "Ils ont des chapeaux ronds, Vive la Bretagne", 44);
    sleep(4);

    printf("Writer: Write second sentence\n");
    write(named_pipe, "Ils ont des tonneaux ronds, Vive les Bretons", 44);

    close(named_pipe);

    return EXIT_SUCCESS;
}
