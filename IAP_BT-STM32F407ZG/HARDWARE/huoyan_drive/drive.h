#ifndef DRIVE_H
#define DRIVE_H
#include "sys.h"

#define L01  PFin(1)//前火焰传感器1输入 
#define L02  PFin(2)//火焰传感器2输入 
#define L03  PFin(0)//火焰传感器3输入 
#define L04  PFin(4)//火焰传感器4输入 
#define L05  PEin(6)//火焰传感器5输入 
#define L06  PFin(5)//火焰传感器6输入 
#define L07  PEin(5)//火焰传感器7输入 
#define L08  PFin(6)//火焰传感器8输入 

#define L11  PEin(2)//后火焰传感器1输入 
#define L12  PBin(4)//火焰传感器2输入
#define L13  PEin(1)//火焰传感器3输入
#define L14  PBin(5)//火焰传感器4输入 
#define L15  PBin(9)//火焰传感器5输入 
#define L16  PBin(6)//火焰传感器6输入
#define L17  PBin(8)//火焰传感器7输入
#define L18  PBin(7)//火焰传感器8输入
 
void Laser_Init(void);//火焰传感器初始化函数

int Laser_scan(int aspect);
int Laser_Scan(int aspect);//火焰传感器实现

#endif

