/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "stats_report.h"
#include "system.h"

#include "io_module.h"
#include "comm_module.h"


Thread io_queue_thread;
Thread comm_thread;

#define SLEEP_TIME                  500 // (msec)
#define PRINT_AFTER_N_LOOPS         20

#if !MBED_TEST_MODE
// main() runs in its own thread in the OS
int main()
{
    SystemReport sys_state( SLEEP_TIME * PRINT_AFTER_N_LOOPS /* Loop delay time in ms */);

    io_module_init(&io_queue_thread);
          
    
    comm_thread.start(comm_manager_task);

    while (true) {
#if PRINT_SYSTEM_REPORT
        wait_ms(SLEEP_TIME);

        if ((0 == count) || (PRINT_AFTER_N_LOOPS == count)) {
            // Following the main thread wait, report on the current system status
            sys_state.report_state();
            count = 0;
        }
        ++count;
#else
        wait(osWaitForever);
#endif        
    }
}
#endif
