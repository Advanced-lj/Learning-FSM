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
#include <stdlib.h>

#include "efsm.h"
#include "efsm_event.h"

EFSM_DECLEAR(startup);
EFSM_DECLEAR(offline);
EFSM_DECLEAR(online);

/* 1. define state-machine pointer */
EFSM_PTR_CREATE(main);

extern void startup_init();
extern void offline_init();
extern void online_init();

static void event_invoke(EFSM_EVENT_TYPE event)
{
    EFSM_EVENT_HANDLER handler = NULL;

    handler = EFSM_HANDLER(main, event);
    printf("handler: %p\n", handler);
    if(handler)
        handler(event, NULL);
}

int main(void)
{
    startup_init();
    offline_init();
    online_init();

    /* 2. init first state */
    EFSM_PTR_BIND(main, startup);
    printf("******************************************\n");
    /* 3. get a event handler from EFSM */
    event_invoke(EVENT_STOP);

    /* 4.on condition, change state */
    EFSM_TRANSFER_ENABLE(main);
    EFSM_TRANSFER(main, offline);
    EFSM_TRANSFER_DISABLE(main);

    event_invoke(EVENT_PLAY);

    EFSM_TRANSFER_ENABLE(main);
    EFSM_TRANSFER(main, online);
    EFSM_TRANSFER_DISABLE(main);

    event_invoke(EVENT_NEXT);

    printf("******************************************\n");	
    return 0;
}

