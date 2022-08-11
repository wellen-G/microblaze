`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/08/10 18:17:53
// Design Name: 
// Module Name: soc_top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module soc_top(
    input   sys_clk_p   ,
    input   sys_clk_n   ,
    input   sys_clk     ,
    input   sys_rstn    ,
    input   rx          ,
    output  tx    

    );

    soc soc_i(
        .clk_in1_0      (sys_clk         ),
        .reset_rtl_0    (~sys_rstn       ),
        .reset_rtl_0_0  (sys_rstn        ),
        .uart_rtl_0_rxd (rx              ),
        .uart_rtl_0_txd (tx              )
    );
endmodule
