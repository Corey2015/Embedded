#ifndef __FINGER_H
#define __FINGER_H

/*includes*/
#include "stm8s.h"
#include "uart.h"

/*defines*/
#define FINGER_GPIO_PORT      (GPIOC)
#define FINGER_GPIO_PIN   (GPIO_PIN_3)
//Ӧ��
#define ERR_SUCCESS		0X0000				 //�ɹ�
#define ERR_TIMEOUT	        0X8201				 //��ʱ
#define ERR_FP_FULL	        0X8304				 //ָ����
#define ERR_ENROLL_CONTINUE	0X8313				 //����ע��
#define	ERR_NO_MOVE		0X8314				 //�ƶ���ָ����ע��
#define	ERR_ENROLL_FAIL		0X8315				 //ע��ʧ��
#define ERR_FP_COMPARE		0X8402				 //�ȶ�ʧ��



//ָ��
enum{
	CMD_ABORT         = 0X00,//��ֹ
	CMD_SYS_CONFIG    = 0X01,//��ѯָ������
	CMD_MATCH_FP 	  = 0X04,//ָ�Ʊȶ�
	CMD_DELETE_FP     = 0X05,//ɾ��ָ��
        CMD_ENROLL        = 0X13,//ָ��¼��
};



/*command*/


/*functions*/
void FINGER_init(void);
void FINGER_open(void);
void FINGER_close(void);
void FINGER_abort(void);
void FINGER_getnum(void);
void FINGER_getadmin(void);
void FINGER_verify(void);
void FINGER_verify_admin(void);
void FINGER_enroll_again(void);
void FINGER_enroll_first(void);
void FINGER_enroll_admin_again(void);
void FINGER_enroll_admin_first(void);
void FINGER_delete(void);
void FINGER_timeout(void);
void FINGER_set_finger_on_time(void);
#endif