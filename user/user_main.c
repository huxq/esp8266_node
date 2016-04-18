#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "mem.h"
#include "driver/uart.h"
#include "user_interface.h"

#define SIG_RX 0
#define TEST_QUEUE_LEN 4
os_event_t *testQueue;
static const int pin = 1;
static volatile os_timer_t some_timer;


void test_task (os_event_t *e) {
	os_printf("+----------------+\n");
	os_printf("Test starts\n");
	os_printf("wifi op mode :%d \n", wifi_get_opmode());
	os_printf("wifi op default mode :%d \n", wifi_get_opmode_default());
}

void task_init(void) {
    testQueue=(os_event_t *)os_malloc(sizeof(os_event_t)*TEST_QUEUE_LEN);
    system_os_task(test_task, 0, testQueue,TEST_QUEUE_LEN);
}

void some_timerfunc(void *arg)
{
	os_printf("enter timer\n");
}

void ICACHE_FLASH_ATTR set_wifi()
{
	wifi_set_opmode(0x01);
	char myssid[32] = "";
	char mypasswd[64] = "";
	struct station_config my_wifi;
	os_memcpy(&my_wifi.ssid, myssid, 32);
	os_memcpy(&my_wifi.password, mypasswd, 64);
	wifi_station_set_config(&my_wifi);
	wifi_station_set_config_current(&my_wifi);
}


void ICACHE_FLASH_ATTR user_init()
{
	uart_init(115200,0);
	set_wifi();

    os_timer_setfn(&some_timer, (os_timer_func_t *)some_timerfunc, NULL);
	os_timer_arm(&some_timer, 500, 1);

	system_init_done_cb(task_init);
}

