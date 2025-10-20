//Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2018.2 (win64) Build 2258646 Thu Jun 14 20:03:12 MDT 2018
//Date        : Sun Oct  5 22:06:50 2025
//Host        : nankokit running 64-bit major release  (build 9200)
//Command     : generate_target design_1.bd
//Design      : design_1
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "design_1,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=design_1,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=4,numReposBlks=4,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=0,numPkgbdBlks=0,bdsource=USER,da_board_cnt=2,da_clkrst_cnt=2,synth_mode=Global}" *) (* HW_HANDOFF = "design_1.hwdef" *) 
module design_1
   (Q_0,
    cten_0,
    data_in_0,
    du_0,
    load_0,
    sys_diff_clock_clk_n,
    sys_diff_clock_clk_p);
  output [3:0]Q_0;
  input cten_0;
  input [3:0]data_in_0;
  input du_0;
  input load_0;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 sys_diff_clock CLK_N" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME sys_diff_clock, CAN_DEBUG false, FREQ_HZ 200000000" *) input sys_diff_clock_clk_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 sys_diff_clock CLK_P" *) input sys_diff_clock_clk_p;

  wire [25:0]c_counter_binary_0_Q;
  wire clk_wiz_clk_out1;
  wire [3:0]counter_0_Q;
  wire cten_0_1;
  wire [3:0]data_in_0_1;
  wire du_0_1;
  wire load_0_1;
  wire sys_diff_clock_1_CLK_N;
  wire sys_diff_clock_1_CLK_P;
  wire [0:0]xlslice_0_Dout;

  assign Q_0[3:0] = counter_0_Q;
  assign cten_0_1 = cten_0;
  assign data_in_0_1 = data_in_0[3:0];
  assign du_0_1 = du_0;
  assign load_0_1 = load_0;
  assign sys_diff_clock_1_CLK_N = sys_diff_clock_clk_n;
  assign sys_diff_clock_1_CLK_P = sys_diff_clock_clk_p;
  design_1_c_counter_binary_0_0 c_counter_binary_0
       (.CLK(clk_wiz_clk_out1),
        .Q(c_counter_binary_0_Q));
  design_1_clk_wiz_1 clk_wiz
       (.clk_in1_n(sys_diff_clock_1_CLK_N),
        .clk_in1_p(sys_diff_clock_1_CLK_P),
        .clk_out1(clk_wiz_clk_out1));
  design_1_counter_0_0 counter_0
       (.Q(counter_0_Q),
        .clk(xlslice_0_Dout),
        .cten(cten_0_1),
        .data_in(data_in_0_1),
        .du(du_0_1),
        .load(load_0_1));
  design_1_xlslice_0_0 xlslice_0
       (.Din(c_counter_binary_0_Q),
        .Dout(xlslice_0_Dout));
endmodule
