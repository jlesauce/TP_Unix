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
 *  TP Unix - Chapter 2 - Exercise 13 (Reader)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // open
#include <unistd.h>     // read

#define PIPE_NAME "MY_NAMED_PIPE"

int main(void)
{
    // Open pipe
    int named_pipe = open(PIPE_NAME, O_RDONLY);
    if(named_pipe < 0)
    {
        perror("Failed to open pipe:");
        return EXIT_FAILURE;
    }

    // Read pipe
    char buffer;
    while(read(named_pipe, &buffer, 1) != 0)
    {
        printf("Reader: read: %s\n", &buffer);
    }

    close(named_pipe);
    unlink(PIPE_NAME);

    return EXIT_SUCCESS;
}
