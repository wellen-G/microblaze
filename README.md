# microblaze
### FPGA芯片选择 KU060-FFVA1156-2-I
### 介绍

使用microblaze软核搭建处理器，实现中断
![image](https://user-images.githubusercontent.com/109273341/184079504-895b2743-2305-44b2-8aae-f9891a4ea644.png)
其中microblaze_0为microbalze内核，外接128Kb的local memory、利用JTAG调试的mdm内核、中断管理器axi_intc以及uatr外设和GPIO外设。

GPIO中断的实现利用vio IP核实现，16位的VIO接到GPIO作为输入来模拟按键输入。

UART中断外接串口调试助手或者直接利用SDK terminal实现。

concat IP核将多个中断源合成一条总线提供给中断管理器。

### 实验结果

jtag调试段循环输出hello word
![image](https://user-images.githubusercontent.com/109273341/184083396-5228a015-8fe9-44bb-9b43-197054862c6b.png)
ILA抓取的GPIO_o实现0000——FFFF的转换

GPIO_t为0xffff，说明16个gpio全为输入，至于GPIO_o为什么会实现数据转换，这是因为此时的信号还没有经过三态门的转换，在进过三态门转换后16个gpio全为输入
![image](https://user-images.githubusercontent.com/109273341/184083548-76f0a3b2-e396-4c98-960f-21b74a7e2aee.png)

**利用vio改变GPIO的输入，使GPIO控制器产生中断，这时候程序进入GPIO的中断处理函数，在调试端打印“gpio interrupt”.**
![image](https://user-images.githubusercontent.com/109273341/184084460-8cc0dec5-a24d-43ad-a106-b8e6aa112576.png)
![image](https://user-images.githubusercontent.com/109273341/184084439-1940ccbb-bc63-430a-b405-ff5c75011324.png)
**利用串口调试助手，输入一段16进制的数字，会看到相同数字的返回值，这是调用中断返回的值，串口调试助手发送了3个8bit的数值，产生了四个中断，其中前三个中断为接收中断，最后一个为发送中断**
![image](https://user-images.githubusercontent.com/109273341/184084879-166b4448-0a44-4916-9941-71825026f0fe.png)
![image](https://user-images.githubusercontent.com/109273341/184084949-08700c59-0146-4d2a-9998-a60b917f3486.png)
