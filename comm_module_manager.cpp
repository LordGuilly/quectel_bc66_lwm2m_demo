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
#include "ATCmdParser.h"
#include "UARTSerial.h"

#include "system.h"
#include "mailbox.h"
#include "comm_module.h"
#include "io_module.h"



#define REGISTRATION_URC    "+QLWURC:reg,"
#define DEREGISTRATION_URC  "+QLWURC:dereg,"
#define SERVER_OBSERVE_ACTION_URC   "+QLWURC: 0,"
#define SERVER_READ_ACTION_URC      "+QLWURC: 1,"
#define SERVER_WRITE_ACTION_URC     "+QLWURC: 2,"


#define COMM_MANAGER_MAILBOX_BLOCK_IN_MS (1000)

Lwm2mObject object_table[MSG_LAST_SIGNAL] = {
    /* DIGITAL_INPUT */
    {   .message_id = 0,
        .flag = 0,
        .object_id = 3200,
        .instance_id = 1,
        .resource_id = 5500,
        .value = 0
        },
    /* DIGITAL_OUTPUT */
    {   .message_id = 0,
        .flag = 0,
        .object_id = 3201,
        .instance_id = 1,
        .resource_id = 5550,
        .value = 0
        },
};

char *atcmd_init_seq[] = { 
            "AT",
			"ATE0",
            "AT+QCGDEFCONT=\"IP\"," xstr(NBIOT_SERVICE_PROVIDER_APN),
            "AT+CPSMS=0",
            "AT+QSCLK=0",
            "AT+QNBIOTEVENT=1,1",
            "AT+QBAND=1," xstr(NBIOT_BAND),
            NULL 
            };


char *atcmd_lwm2m_setup_seq[] = { 
            "at+qlwconfig=0," xstr(LWM2M_SERVER_URL) "," \
                              xstr(LWM2M_SERVER_PORT) "," \
                              xstr(LWM2M_DEVICE_IDENTIFIER) "," \
                              xstr(LWM2M_DEVICE_LIFETIME) ",3",
			"at+qlwreg",
            NULL 
            };


message_signal_name_t map_object_to_signal(Lwm2mObject *obj)
{
    int index = 0;
    
    for (index = 0; index < MSG_LAST_SIGNAL; index++)
    {
        if ((object_table[index].object_id == obj->object_id) &&
            (object_table[index].instance_id == obj->instance_id) &&
            (object_table[index].resource_id == obj->resource_id))
        {
            break;        
        }
    }           
    return (message_signal_name_t)index;
}
           
void registration_callback(void)
{
    /* 0 is the success status code, only report if was successful */
    if (comm_module_driver_read_value() == '0')
    {
        mail_t *mail = system_mailbox.alloc();
        mail->action = MSG_UPDATE_REGISTRATION;
        mail->signal = (message_signal_name_t)0;
        mail->value = 1;
        mail->data = NULL;
        system_mailbox.put(mail);
    }
    else
    {
        printf("Ignoring registration report\r\n");    
    }
}
     
                
void deregistration_callback(void)
{
    /* 0 is the success status code, only report if was successful */
    if (comm_module_driver_read_value() == '0')
    {
        mail_t *mail = system_mailbox.alloc();
        mail->action = MSG_UPDATE_REGISTRATION;
        mail->signal = (message_signal_name_t)0;
        mail->value = 1;
        mail->data = NULL;
        system_mailbox.put(mail);
    }
    else
    {
        printf("Ignoring deregistration report\r\n");    
    }
}


void write_callback(void)
{
    Lwm2mObject *obj = new(Lwm2mObject);
    
    if (comm_module_driver_parse_urc("%d,%d,%d,%d,5,1,%d,0\r\n", &obj->message_id, 
                                     &obj->object_id, &obj->instance_id,
                                     &obj->resource_id, &obj->value) == true)
    {
        printf("WRITE REQUEST:\r\n");
        printf("message_id[%d]\r\n",obj->message_id);
        printf("value[%d]\r\n",obj->value);
        printf("object_id[%d]\r\n",obj->object_id);
        printf("instance_id[%d]\r\n",obj->instance_id);
        printf("resource_id%d]\r\n",obj->resource_id);
        
        mail_t *mail = system_mailbox.alloc();
        mail->action = MSG_WRITE_REQUEST;
        mail->value = obj->value;
        mail->signal = map_object_to_signal(obj);
        mail->data = &object_table[mail->signal];
        system_mailbox.put(mail);
    }  
    delete(obj);
}


void read_callback(void)
{
    Lwm2mObject *obj = new(Lwm2mObject);
    
    if (comm_module_driver_parse_urc("%d,%d,%d,%d\r\n", &obj->message_id, 
                                     &obj->object_id, &obj->instance_id,
                                     &obj->resource_id) == true)
    {
        printf("READ REQUEST:\r\n");
        printf("message_id[%d]\r\n",obj->message_id);
        printf("object_id[%d]\r\n",obj->object_id);
        printf("instance_id[%d]\r\n",obj->instance_id);
        printf("resource_id%d]\r\n",obj->resource_id);
        
        mail_t *mail = system_mailbox.alloc();
        mail->action = MSG_READ_REQUEST;
        mail->signal = map_object_to_signal(obj);
        mail->data = &object_table[mail->signal];
        system_mailbox.put(mail);
    }  
    delete(obj);
}
        
void observe_callback(void)
{
    Lwm2mObject *obj = new(Lwm2mObject);
    
    if (comm_module_driver_parse_urc("%d,%d,%d,%d,%d\r\n", &obj->message_id, 
                                     &obj->flag, &obj->object_id, &obj->instance_id,
                                     &obj->resource_id) == true)
    {
        printf("OBSERVE REQUEST:\r\n");
        printf("message_id[%d]\r\n",obj->message_id);
        printf("flag[%d]\r\n",obj->flag);
        printf("object_id[%d]\r\n",obj->object_id);
        printf("instance_id[%d]\r\n",obj->instance_id);
        printf("resource_id%d]\r\n",obj->resource_id);
        
        mail_t *mail = system_mailbox.alloc();
        mail->action = MSG_OBSERVE_REQUEST;
        
        /* search in object_table looking for obj/ins/res match */
        mail->signal = map_object_to_signal(obj);
        mail->data = (void*) &object_table[mail->signal];
        
        system_mailbox.put(mail);
    }
    delete(obj);
}
       

void parse_callback(void)
{
    int objid = 0;
    int resid = 0;
    
    if (comm_module_driver_parse_urc("%d,%d\r\n", &objid,&resid) == true)
    {
        printf("objid[%d]\r\nresid[%d]\r\n",objid,resid);
        /*
        mail_t *mail = system_mailbox.alloc();
        mail->action = MSG_UPDATE_REGISTRATION;
        mail->signal = (message_signal_type_t)0;
        mail->value = comm_module_driver_read_value() - '0';
        system_mailbox.put(mail);
        */
    }
}
        
void comm_manager_init_modem(void)
{
    
    char modem_version[MAX_BUF];
    /* not working yet :-(
    io_module_turn_on_modem();
    wait(5);
    */
    if (comm_module_driver_send_atcmd_seq(atcmd_init_seq) == false)
    {
        printf("\r\n atcmd_init_seq FAILED\r\n");     
        exit(1);
    }
 
    if ((comm_module_driver_get_module_version(modem_version) == true) &&
        (strncmp(modem_version, MODULE_FW_VERSION, MAX_BUF) != 0))
    {
        printf("\r\n WRONG modemversion, got[%s], expected[%s]\r\n", modem_version, MODULE_FW_VERSION);        
        exit(1);
    }

    if (comm_module_driver_send_atcmd_seq(atcmd_lwm2m_setup_seq) == false)
    {
        printf("\r\n atcmd_lwm2m_setup_seq FAILED\r\n");     
        exit(1);
    }

}

bool comm_manager_add_custom_objects(void)
{
	bool retval = true;

	/* delete both objects, don't care about errors, as they may not exist */
	comm_module_driver_send_atcmd_and_waitfor_urc("at+qlwdelobj=3200", "+QLWDELOBJ :");
	comm_module_driver_send_atcmd_and_waitfor_urc("at+qlwdelobj=3201", "+QLWDELOBJ :");

	if (comm_module_driver_send_atcmd_and_waitfor_urc("at+qlwaddobj=3200,1,1,\"5500\"", "+QLWADDOBJ :0") == false)
	{
		retval = false;
	}

	if (comm_module_driver_send_atcmd_and_waitfor_urc("at+qlwaddobj=3201,1,1,\"5550\"", "+QLWADDOBJ :0") == false)
	{
		retval = false;
	}
	return retval;
}
void comm_manager_notify_server(message_signal_name_t signal, int value)
{
    char atcmd_buf[MAX_BUF] = { 0 };
    
    printf("should notify type(%d), value (%d)\r\n", signal, value);
    
    switch(signal)
    {   
        case MSG_DIGITAL_INPUT:
            snprintf(atcmd_buf, MAX_BUF, "at+qlwnotify=3200,0,5500,5,1,%d", value);
            break;
        case MSG_DIGITAL_OUTPUT:
            snprintf(atcmd_buf, MAX_BUF, "at+qlwnotify=3201,0,5550,5,1,%d", value);
            break;
        default:
            break;
    }
    
    if (atcmd_buf != NULL)
    {
     //   comm_module_driver_send_atcmd_atomic(atcmd_buf);
        printf("%s\r\n", atcmd_buf);
    }
} 
void comm_module_manager_reply_request(message_notification_type_t request, Lwm2mObject *obj)
{
    char buf[MAX_BUF] = { 0 };
    printf("request [%d]\r\n", request);
    printf("message_id[%d]\r\n", obj->message_id);
    printf("flag[%d]\r\n", obj->flag);
    printf("object_id[%d]\r\n", obj->object_id);
    printf("instance_id[%d]\r\n", obj->instance_id);
    printf("resource_id%d]\r\n", obj->resource_id);
    /* send response at command */
    
    switch(request)
    {
        case MSG_OBSERVE_REQUEST:
            /* accept all the observes*/
            snprintf(buf, MAX_BUF, "AT+QLWOBSRSP=%d,1,%d,%d,%d,5,%d,0", obj->message_id,
                        obj->object_id, obj->instance_id, obj->resource_id, obj->value);
            break;
        case MSG_READ_REQUEST:
            /* return the cached value */
            snprintf(buf, MAX_BUF, "AT+QLWRDRSP=%d,1,%d,%d,%d,5,%d,0", obj->message_id,
                        obj->object_id, obj->instance_id, obj->resource_id, obj->value);
            break;
        case MSG_WRITE_REQUEST:
            /* reject any write request */
            snprintf(buf, MAX_BUF, "AT+AT+QLWWRRSP=%d,12", obj->message_id);
            break;
        default:
            printf("REQUEST [%d] not supported\r\n", request);
            exit(1);
    }
    if (comm_module_driver_send_atcmd_atomic(buf) == false)
    {
        printf("\r\n sending [%s] FAILED\r\n", buf);     
        exit(1);
    }
}

void comm_manager_task(void)
{   
    int pattern_id;
    module_state_t module_state = MODULE_RESET;

    pattern_id = io_module_set_fast_state_pattern();
    comm_module_driver_init();
    
    /* register callbacks for URCs */
    comm_module_driver_register_oob(REGISTRATION_URC, callback(registration_callback));
    comm_module_driver_register_oob(DEREGISTRATION_URC, callback(deregistration_callback));
    comm_module_driver_register_oob(SERVER_OBSERVE_ACTION_URC, callback(observe_callback));
    comm_module_driver_register_oob(SERVER_READ_ACTION_URC, callback(read_callback));
    comm_module_driver_register_oob(SERVER_WRITE_ACTION_URC, callback(write_callback));
    
    
    comm_manager_init_modem();
    
    io_module_cancel_state_pattern(pattern_id);
    while(true)
    {
        osEvent evt = system_mailbox.get(COMM_MANAGER_MAILBOX_BLOCK_IN_MS);
        if (evt.status == osEventMail)
        {
            mail_t *mail = (mail_t*)evt.value.p;
            
            switch (mail->action) 
            {
                case MSG_UPDATE_VALUE:
                    /* store the value in the cache, for future use */
                    object_table[mail->signal].value = mail->value;
                    /* only report the values if the module is registered */
                    if (module_state == MODULE_CONFIGURED)
                    {
                        comm_manager_notify_server(mail->signal, object_table[mail->signal].value);
                    }
                    break;
                case MSG_UPDATE_REGISTRATION:
                    printf("registration value[%d]\r\n", mail->value);
                    if (mail->value == 1)
                    {
                    	module_state = MODULE_REGISTERED;
                    }
                    if (comm_manager_add_custom_objects() == true)
                    {
                    	module_state = MODULE_CONFIGURED;
                    }
                    printf("module_state[%d]\r\n", module_state);
                    break;
                case MSG_OBSERVE_REQUEST:                    
                case MSG_READ_REQUEST:      
                case MSG_WRITE_REQUEST:      
                        /* use the cached value */
                        comm_module_manager_reply_request(mail->action, (Lwm2mObject*) mail->data);
                    break;
                default:
                    printf("action [%d] not supported yet\r\n", mail->action);
                    break;
            }
            system_mailbox.free(mail);
        }
        comm_module_driver_check_for_urcs();
    }
}
