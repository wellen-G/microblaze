# microblaze
# FPGA芯片选择 KU060-FFVA1156-2-I
使用microblaze软核搭建处理器，实现中断
![image](https://user-images.githubusercontent.com/109273341/184079504-895b2743-2305-44b2-8aae-f9891a4ea644.png)
其中microblaze_0为microbalze内核，外接128Kb的local memory、利用JTAG调试的mdm内核、中断管理器axi_intc以及uatr外设和GPIO外设。

GPIO中断的实现利用vio IP核实现，16位的VIO接到GPIO作为输入来模拟按键输入。

UART中断外接串口调试助手或者直接利用SDK terminal实现。

concat IP核将多个中断源合成一条总线提供给中断管理器。
