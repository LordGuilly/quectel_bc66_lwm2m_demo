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

#include "io_module.h"
#include "system.h"
#include "mailbox.h"

/* input output info */
InterruptIn user_button(USER_BUTTON);
DigitalOut led_key(LED3);
DigitalOut led_periodic(LED2);
DigitalOut led_state(LED1);
DigitalOut pwrkey_n(D12);
DigitalOut reset_n(D13);
DigitalOut power_en(A0);

/* OS resources */
EventQueue io_queue(32 * EVENTS_EVENT_SIZE);


void toggle_pin(DigitalOut pin)
{
    pin = !pin;
}

void set_output_to_low(DigitalOut pin)
{
	pin = 0;
}

void notify_update(message_notification_type_t action, message_signal_name_t signal, int value)
{
    if (system_mailbox.full() == false) 
    {
        mail_t *mail = system_mailbox.alloc();
        mail->action = action;
        mail->signal = signal;
        mail->value = value;
        system_mailbox.put(mail);
    }
}

void button_confirm_debounce(void)
{
    if (led_key != user_button)
    {
        led_key = user_button;    
        io_queue.call(notify_update, MSG_UPDATE_VALUE, MSG_DIGITAL_INPUT, user_button.read());
    }
}


void button_begin_debounce(void)
{
    if (led_key != user_button)
    {
        io_queue.call_in(DEBOUNCE_TIME_IN_MS,button_confirm_debounce);
    }    
}

void toggle_led_periodic(void) 
{
    led_periodic = !led_periodic;
    io_queue.call(notify_update, MSG_UPDATE_VALUE, MSG_DIGITAL_OUTPUT, led_periodic.read());
}  



void io_module_init(Thread *io_queue_thread)
{
    pwrkey_n = 0;
    reset_n = 0;
    
    io_queue.call_every(LED_PERIOD_IN_SECONDS*1000, toggle_led_periodic);
    user_button.rise(io_queue.event(button_begin_debounce));
    user_button.fall(io_queue.event(button_begin_debounce));
    
    led_state = 0;
    
    io_queue_thread->start(callback(&io_queue, &EventQueue::dispatch_forever));
}

void io_module_turn_on_modem(void) 
{
    pwrkey_n = 1;
    io_queue.call_in(PWRKEY_TIME_IN_MS,set_output_to_low,pwrkey_n);
}


void io_module_reset_modem(void)
{
    reset_n = 1;
    io_queue.call_in(RESET_TIME_IN_MS,set_output_to_low,reset_n);
}

int io_module_set_fast_state_pattern(void) 
{
    return io_queue.call_every(FAST_PATTERN_TIME_IN_MS, toggle_pin, led_state);
}

void io_module_cancel_state_pattern(int id) 
{
    io_queue.cancel(id);
    led_state = 0;
}


