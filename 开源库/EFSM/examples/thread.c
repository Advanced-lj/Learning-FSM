/* 
 * MIT License
 *
 * Copyright (c) 2019 极简美 @ konishi5202@163.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "efsmt.h"
#include "efsm_event.h"

EFSM_DECLEAR(startup);
EFSM_DECLEAR(offline);
EFSM_DECLEAR(online);

/* 1.define state-machine */
EFSMT_CREATE(main);

extern void startup_init();
extern void offline_init();
extern void online_init();

int main(void)
{
    int i;

    startup_init();
    offline_init();
    online_init();

    /* 2.bind init state */
    EFSMT_BIND(main, startup);
    
    printf("******************************************\n");
    /* 3.invoke a event */
    EFSMT_INVOKE(main, EVENT_STOP, NULL);
    
    /* 4.on condition, change state */
    EFSM_TRANSFER_ENABLE(main);
    EFSM_TRANSFER(main, offline);
    EFSM_TRANSFER_DISABLE(main);

    for(i = 0; i < 40;i++)
        EFSMT_INVOKE(main, EVENT_STOP, NULL);
    
    EFSM_TRANSFER_ENABLE(main);
    EFSM_TRANSFER(main, online);
    EFSM_TRANSFER_DISABLE(main);

    for(i = 0; i < 20;i++)
        EFSMT_INVOKE(main, EVENT_NEXT, NULL);
    for(i = 0; i < 20;i++)
        EFSMT_INVOKE(main, EVENT_PLAY, NULL);

    usleep(5000); // wait a while for calling handler
    printf("******************************************\n");
    EFSMT_DESTROY(main);
    
    return 0;
}
