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


#ifndef _LWM2M_DEMO_HEADER__
#define _LWM2M_DEMO_HEADER__

#define xstr(a) str(a)
#define str(a) #a

/* CONFIGURATION DEFINES
 * Make sure these values match your system under test*/
#define LWM2M_SERVER_URL        "leshan.eclipse.org"

#define LWM2M_SERVER_PORT       5683   
#define LWM2M_DEVICE_IDENTIFIER "LWM2M-Demo-STM32_BC66"
#define NBIOT_SERVICE_PROVIDER_APN    "openlab.vodafone.iot"
#define NBIOT_BAND      (20)
#define MODEM_TX_PIN    (D1)
#define MODEM_RX_PIN    (D0)
#define MODEM_PWRKEY_N_PIN (D12)
#define MODEM_RESET_N_PIN (D13)
#define MODEM_BAUDRATE  (115200)
/* END OF MANDATORY DEFINES


#define LED_PERIOD_IN_SECONDS (10)

#define MODULE_FW_VERSION "BC66NBR01A07"
#define LWM2M_DEVICE_LIFETIME   3600




#endif /* _LWM2M_DEMO_HEADER__ */
