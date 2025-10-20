//Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2018.2 (win64) Build 2258646 Thu Jun 14 20:03:12 MDT 2018
//Date        : Sun Oct  5 22:06:50 2025
//Host        : nankokit running 64-bit major release  (build 9200)
//Command     : generate_target design_1_wrapper.bd
//Design      : design_1_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module design_1_wrapper
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
  input sys_diff_clock_clk_n;
  input sys_diff_clock_clk_p;

  wire [3:0]Q_0;
  wire cten_0;
  wire [3:0]data_in_0;
  wire du_0;
  wire load_0;
  wire sys_diff_clock_clk_n;
  wire sys_diff_clock_clk_p;

  design_1 design_1_i
       (.Q_0(Q_0),
        .cten_0(cten_0),
        .data_in_0(data_in_0),
        .du_0(du_0),
        .load_0(load_0),
        .sys_diff_clock_clk_n(sys_diff_clock_clk_n),
        .sys_diff_clock_clk_p(sys_diff_clock_clk_p));
endmodule
