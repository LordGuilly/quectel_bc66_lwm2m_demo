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

#define MAILBOX_SIZE    (16)
typedef enum {
	MSG_UPDATE_NETWORK,
    MSG_UPDATE_REGISTRATION,
    MSG_UPDATE_VALUE,
    MSG_GET_VALUE,
    MSG_OBSERVE_REQUEST,
    MSG_READ_REQUEST,
    MSG_WRITE_REQUEST
} message_notification_type_t;

typedef enum {
    MSG_DIGITAL_INPUT,
    MSG_DIGITAL_OUTPUT,
    MSG_LAST_SIGNAL
} message_signal_name_t;

/* Mail */
typedef struct {
  message_notification_type_t   action; 
  int signal;
  int value;
  void *data;
} mail_t;

extern Mail <mail_t, MAILBOX_SIZE> system_mailbox;
