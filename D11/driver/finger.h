#ifndef __FINGER_H
#define __FINGER_H

/*includes*/
#include "stm8s.h"
#include "uart.h"

/*defines*/
#define FINGER_GPIO_PORT      (GPIOC)
#define FINGER_GPIO_PIN   (GPIO_PIN_3)
//应答
#define ERR_SUCCESS		0X0000				 //成功
#define ERR_TIMEOUT	        0X8201				 //超时
#define ERR_FP_FULL	        0X8304				 //指纹满
#define ERR_ENROLL_CONTINUE	0X8313				 //继续注册
#define	ERR_NO_MOVE		0X8314				 //移动手指继续注册
#define	ERR_ENROLL_FAIL		0X8315				 //注册失败
#define ERR_FP_COMPARE		0X8402				 //比对失败



//指令
enum{
	CMD_TERMINATE     = 0X00,//终止
	CMD_SYS_CONFIG    = 0X01,//查询指纹数量
	CMD_MATCH_FP 	  = 0X04,//指纹比对
	CMD_DELETE_FP     = 0X05,//删除指纹
        CMD_ENROLL        = 0X13,//指纹录入
};



/*command*/


/*functions*/
void FINGER_init(void);
void FINGER_on(void);
void FINGER_off(void);
void FINGER_abort(void);
void FINGRT_getnum(void);
void FINGRT_verify(void);
void FINGRT_enroll_again(void);
void FINGRT_enroll_first(void);
#endif