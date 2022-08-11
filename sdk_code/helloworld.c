/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xuartlite.h"
#include "xuartlite_l.h"
#include "xintc.h"
#include "xil_io.h"
#include "sleep.h"

#define GPIO_EXAMPLE_DEVICE_ID  XPAR_GPIO_0_DEVICE_ID
#define UART_DEVICE_ID XPAR_UARTLITE_0_DEVICE_ID     //串口器件ID
#define INTC_DEVICE_ID   XPAR_INTC_0_DEVICE_ID      //中断控制器ID
#define AXI_GPIO_INTR_ID XPAR_INTC_0_GPIO_0_VEC_ID  //AXI GPIO中断ID
#define UART_INTR_ID   XPAR_INTC_0_UARTLITE_0_VEC_ID //串口中断ID
#define EXCEPTION_ID     XIL_EXCEPTION_ID_INT       //中断异常ID


#define RX_NOEMPTY XUL_SR_RX_FIFO_VALID_DATA // 接收 FIFO 非空

static XGpio Gpio; /* The Instance of the GPIO Driver */
static XIntc    Intc;                              //中断控制器实例
static XUartLite Uart;      //串口实例

void GpioHandler(void *CallbackRef);
void uart_handler(void *CallbackRef);

int main()
{
    init_platform();

    print("Hello World\n\r");
//设备初始化
    XGpio_Initialize(&Gpio, GPIO_EXAMPLE_DEVICE_ID);
    XGpio_SetDataDirection(&Gpio, 1, 0xffff);
    XUartLite_Initialize(&Uart , UART_DEVICE_ID);
    //初始化中断控制器
    XIntc_Initialize(&Intc, INTC_DEVICE_ID);
//关联中断ID和中断服务函数
    //中断服务函数是需要我们自己编写的， 用于响应和处理 AXI GPIO 中断的函数
   XIntc_Connect(&Intc,AXI_GPIO_INTR_ID,(Xil_ExceptionHandler)GpioHandler,&Gpio );
   XIntc_Connect(&Intc, UART_INTR_ID,(XInterruptHandler)uart_handler,&Uart);
//外设中断 使能
   //使能GPIO中断
   XGpio_InterruptEnable(&Gpio, 1);
   //使能GPIO全局中断
   XGpio_InterruptGlobalEnable(&Gpio);
   //使能串口中断
   XUartLite_EnableInterrupt(&Uart);
//启用外设对应的中断向量
   XIntc_Enable(&Intc,AXI_GPIO_INTR_ID);
   XIntc_Enable(&Intc,UART_INTR_ID);
//启动中断控制器
   XIntc_Start(&Intc, XIN_REAL_MODE);
//设置并打开中断异常处理
   Xil_ExceptionInit();
   Xil_ExceptionRegisterHandler(EXCEPTION_ID,
           (Xil_ExceptionHandler)XIntc_InterruptHandler,&Intc);
   Xil_ExceptionEnable();


    for(;1;){

    	Xil_Out32(0x40000000 , 0xffff);
    	sleep(1);
    	Xil_Out32(0x40000000 , 0x0000);
    	sleep(1);
    	print("Hello\n\r");
    	xil_printf("word\n\r");
    	sleep(1);

    }

    cleanup_platform();
    return 0;
}

void GpioHandler(void *CallbackRef){
    XGpio *GpioPtr = (XGpio *)CallbackRef;
        print("gpio interrupt\n\r");
        XGpio_InterruptDisable(GpioPtr, 1);  //关闭中断
        XGpio_InterruptClear(GpioPtr, 1);    //清除中断
        XGpio_InterruptEnable(GpioPtr, 1);   //使能中断
}


void uart_handler(void *CallbackRef)//中断处理函数
{
    u8 Read_data;
    u32 isr_status;
    XUartLite *InstancePtr= (XUartLite *)CallbackRef;
    //读取状态寄存器
     isr_status = XUartLite_ReadReg(InstancePtr->RegBaseAddress ,
     XUL_STATUS_REG_OFFSET);
     if(isr_status & RX_NOEMPTY){ //接收 FIFO 中有数据
     //读取数据
     Read_data=XUartLite_ReadReg(InstancePtr->RegBaseAddress ,
     XUL_RX_FIFO_OFFSET);
     //发送数据
     XUartLite_WriteReg(InstancePtr->RegBaseAddress ,
     XUL_TX_FIFO_OFFSET, Read_data);
     xil_printf("%x\n",Read_data);
     print("\n\r");
     }
     print("uart interrupt\n\r");
}

