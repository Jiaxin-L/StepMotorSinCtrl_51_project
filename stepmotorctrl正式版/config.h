#ifndef _CONFIG_H_
#define _CONFIG_H_
//��ʼ�����£���Ϊ�Ƕ�λ��0���λ�ã�������������ƶ���λ�ڹ���м�
//�����ڼ䲻Ҫ��ת����
#define ROLLBACK_ENABLE  0//�Ƿ�֧�ַ�ת 1 ����  0 ������

//y=A*sin(n*x/N)
//y �������
//A ��ֵ
//n ����ı�Ƶֵ
//N �ڲ���Ƶϵ��
#define DIVIDE_FREQ  10//N ��Ƶϵ��  
#define STEP_AMP 2000 //A ���������ֵ����λ mm�� �������ÿתһ�ȣ��������䶯Լ1mm
#define MAX_INPUT 10 //n ����������ֵ����,��ֵ����Խ�󣬶�Ӧ������ת��Ҫ��Խ��

//����������ã��ο���������������ĵ���ÿһ����0��1��ѡ
#define DCY1 0
#define DCY2 1//50%˥��ģʽ
#define TQ1 1
#define TQ2 0//75%����
	
#endif
