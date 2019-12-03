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
#include "comm_module.h"

#include "mbed.h"
#include "ATCmdParser.h"
#include "UARTSerial.h"

#include "system.h"


UARTSerial *_serial;
ATCmdParser *_parser;

void comm_module_driver_init(void)
{
    _serial = new UARTSerial(MODEM_TX_PIN, MODEM_RX_PIN, MODEM_BAUDRATE);    
    _parser = new ATCmdParser(_serial);
    
    _parser->debug_on(1);
    _parser->set_delimiter("\r\n"); 
}

bool comm_module_driver_get_module_version(char *version)
{
    bool retval = false;
        
    if (_parser->send("ATI") == false)    
    {
        printf("Error sending [ATI]\r\n");
    }
    else if (_parser->recv("Revision : %s\n", version) == false)
    {
        printf("Error parsing version\r\n");
    }
    else
    {
        retval = true;
    }
    return retval;
}

bool comm_module_driver_send_atcmd_atomic(const char* atcmd) 
{
    bool retval = false;
    
    if (_parser->send(atcmd) == false)    
    {
        printf("Error sending [%s]\r\n",atcmd);
    } 
    else if (_parser->recv("OK") == false)
    {
        printf("Error receiving OK\r\n");
    }
    else
    {
        retval = true;
    }
    return (retval);
}

bool comm_module_driver_send_atcmd_and_waitfor_urc(const char *atcmd, const char *urc)
{
    bool retval = false;

    if (_parser->send(atcmd) == false)
    {
        printf("Error sending [%s]\r\n",atcmd);
    }
    else if (_parser->recv("OK") == false)
    {
        printf("Error receiving OK\r\n");
    }
    else if (_parser->recv(urc) == false)
	{
    	printf("Error receiving URC\r\n");
	}
    else
    {
        retval = true;
    }
    return (retval);
}


bool comm_module_driver_waitfor(const char *string)
{
    bool retval = false;

    if (_parser->recv(string) == false)
	{
    	printf("Error receiving string\r\n");
	}
    else
    {
        retval = true;
    }
    return (retval);
}
     
bool comm_module_driver_send_atcmd_seq(const char *atcmd_seq[])
{
    int index = 0;
    
    for (index=0; atcmd_seq[index] != NULL; index++)
    {
        if (comm_module_driver_send_atcmd_atomic(atcmd_seq[index]) == false) 
        {
            printf(" Error at atcmd_seq[%d], '%s'\r\n", index, atcmd_seq[index]);
            return false;
        }    
    }
    return true;
}

void comm_module_driver_check_for_urcs(void)
{
    _parser->process_oob();    
}

void comm_module_driver_register_oob(const char *prefix, Callback<void()> cb) 
{
    _parser->oob(prefix, cb);
}

char comm_module_driver_read_value(void)
{
    return _parser->getc();    
}

bool comm_module_driver_parse_urc(const char *response, ...)
{
    std::va_list args;
    va_start(args, response);
    bool res = _parser->vrecv(response, args);
    va_end(args);
    return res;
}


void comm_module_driver_set_timeout(int timeout)
{
	_parser->set_timeout(timeout);
}
