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


#ifndef _IO_MODULE_HEADER__
#define _IO_MODULE_HEADER__

#define FAST_PATTERN_TIME_IN_MS (50)

#define DEBOUNCE_TIME_IN_MS (100)  
#define PWRKEY_TIME_IN_MS (500)  

void io_module_init(Thread *io_queue_thread);

void io_module_turn_on_modem(void);

int io_module_set_fast_state_pattern(void);
void io_module_cancel_state_pattern(int id);



#endif /* _IO_MODULE_HEADER__ */