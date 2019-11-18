#ifndef DRIVE_H
#define DRIVE_H
#include "sys.h"

#define L01  PFin(1)//ǰ���洫����1���� 
#define L02  PFin(2)//���洫����2���� 
#define L03  PFin(0)//���洫����3���� 
#define L04  PFin(4)//���洫����4���� 
#define L05  PEin(6)//���洫����5���� 
#define L06  PFin(5)//���洫����6���� 
#define L07  PEin(5)//���洫����7���� 
#define L08  PFin(6)//���洫����8���� 

#define L11  PEin(2)//����洫����1���� 
#define L12  PBin(4)//���洫����2����
#define L13  PEin(1)//���洫����3����
#define L14  PBin(5)//���洫����4���� 
#define L15  PBin(9)//���洫����5���� 
#define L16  PBin(6)//���洫����6����
#define L17  PBin(8)//���洫����7����
#define L18  PBin(7)//���洫����8����
 
void Laser_Init(void);//���洫������ʼ������

int Laser_scan(int aspect);
int Laser_Scan(int aspect);//���洫����ʵ��

#endif

