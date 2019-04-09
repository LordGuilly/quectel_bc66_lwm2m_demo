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


#ifndef _COMM_MANAGER_MODULE_HEADER__
#define _COMM_MANAGER_MODULE_HEADER__
#include "mbed.h"


#define MAX_BUF (256)

class Lwm2mObject {
    public:
        int message_id;
        int flag;
        int object_id;
        int instance_id;
        int resource_id;
        int value;
        bool observed;
}; 

typedef enum {
	MODULE_RESET,
	MODULE_REGISTERED,
	MODULE_CONFIGURED,
} module_state_t;

void comm_manager_task(void);

void comm_module_driver_init(void);
bool comm_module_driver_send_atcmd_seq(const char *atcmd_seq[]);
bool comm_module_driver_send_atcmd_atomic(const char* atcmd);
bool comm_module_driver_send_atcmd_and_waitfor_urc(const char *atcmd, const char *urc);
bool comm_module_driver_get_module_version(char* version);
void comm_module_driver_check_for_urcs(void);
void comm_module_driver_register_oob(const char *prefix, Callback<void()> cb);
char comm_module_driver_read_value(void);
bool comm_module_driver_parse_urc(const char *response, ...);
#endif /* _COMM_MANAGER_MODULE_HEADER__ */
