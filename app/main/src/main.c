/*
 *
 *   file  :  main.cpp
 *
 */



/***************   includes    *******************/

#include "_project_typedefs.h"
#include "_project_defines.h"
#include "_project_func_declarations.h"
#include "_project_tasks_defines.h"

#include "dev_management_api.h"

#include "os_wrapper.h"

/***************   defines    *******************/


/***************   typedefs    *******************/


/**********   external variables    **************/

/***********   loacal variables    **************/

static char hello_world_str[] = "++++++++hello world x\r\n";


/**
 * test_thread_func()
 *
 * return:
 */
static void test_thread_func(void * aHandle)
{
	uint32_t cnt;
	struct dev_desc_t * dev;

	dev = DEV_OPEN("semihosting_dev");
	if (NULL != dev)
	{
		DEV_IOCTL_0_PARAMS(dev, IOCTL_DEVICE_START);
	}

	cnt = 0;
	while (1)
	{
		hello_world_str[20] = '0' + (uint8_t)cnt++;
		if (9 == cnt)
		{
			cnt = 0;
		}
		if (NULL != dev)
		{
			DEV_WRITE(
				dev, (uint8_t*)hello_world_str, sizeof(hello_world_str) - 1);
		}
		os_delay_ms(1000);

		//os_stack_test(); //requires PRINTF_DBG
	}
}


/*
 * function : main()
 *
 */
int main(void)
{
	struct dev_desc_t * dev;

	dev = DEV_OPEN("soc_clock_control_dev");
	if (NULL == dev) goto error;
	DEV_IOCTL_0_PARAMS(dev , IOCTL_DEVICE_START  );

	dev = DEV_OPEN("systick_dev");
	if (NULL == dev) goto error;
	os_set_tick_timer_dev(dev);
	os_init();

	os_create_task("test_thread" , test_thread_func, NULL,
				TEST_THREAD_STACK_SIZE_BYTES, TEST_THREAD_PRIORITY);

	os_start();

	while(1);
error :
	while(1);
}


