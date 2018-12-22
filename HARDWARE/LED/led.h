#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED_RUN PAout(7)
#define ZIGBEE_RES PAout(5)
#define ZIGBEE_SW PAout(4)

#define BLE_RST PCout(6)
#define BLE_WAKEUP PCout(7)
void LED_Init(void);//≥ı ºªØ

		 				    
#endif
