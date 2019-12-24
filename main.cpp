/*
*   Copyright [2019] [Sebastian Viviani]
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*        http://www.apache.org/licenses/LICENSE-2.0

*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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
