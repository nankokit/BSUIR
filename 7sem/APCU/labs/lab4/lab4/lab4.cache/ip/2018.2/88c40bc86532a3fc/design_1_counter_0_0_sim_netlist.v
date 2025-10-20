// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2018.2 (win64) Build 2258646 Thu Jun 14 20:03:12 MDT 2018
// Date        : Sun Oct  5 21:52:09 2025
// Host        : nankokit running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ design_1_counter_0_0_sim_netlist.v
// Design      : design_1_counter_0_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7vx485tffg1157-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_counter
   (clk,
    du,
    load,
    cten,
    data_in,
    Q);
  input clk;
  input du;
  input load;
  input cten;
  input [3:0]data_in;
  output [3:0]Q;

  wire [3:0]Q;
  wire clk;
  wire \count[0]_C_i_1_n_0 ;
  wire \count[0]_P_i_1_n_0 ;
  wire \count[1]_C_i_1_n_0 ;
  wire \count[1]_P_i_1_n_0 ;
  wire \count[2]_C_i_1_n_0 ;
  wire \count[2]_C_i_2_n_0 ;
  wire \count[2]_P_i_1_n_0 ;
  wire \count[3]_C_i_1_n_0 ;
  wire \count[3]_P_i_2_n_0 ;
  wire \count_reg[0]_C_n_0 ;
  wire \count_reg[0]_LDC_i_1_n_0 ;
  wire \count_reg[0]_LDC_i_2_n_0 ;
  wire \count_reg[0]_LDC_n_0 ;
  wire \count_reg[0]_P_n_0 ;
  wire \count_reg[1]_C_n_0 ;
  wire \count_reg[1]_LDC_i_1_n_0 ;
  wire \count_reg[1]_LDC_i_2_n_0 ;
  wire \count_reg[1]_LDC_n_0 ;
  wire \count_reg[1]_P_n_0 ;
  wire \count_reg[2]_C_n_0 ;
  wire \count_reg[2]_LDC_i_1_n_0 ;
  wire \count_reg[2]_LDC_i_2_n_0 ;
  wire \count_reg[2]_LDC_n_0 ;
  wire \count_reg[2]_P_n_0 ;
  wire \count_reg[3]_C_n_0 ;
  wire \count_reg[3]_LDC_i_1_n_0 ;
  wire \count_reg[3]_LDC_i_2_n_0 ;
  wire \count_reg[3]_LDC_n_0 ;
  wire \count_reg[3]_P_n_0 ;
  wire cten;
  wire [3:0]data_in;
  wire du;
  wire load;
  wire p_2_in;

  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \Q[0]_INST_0 
       (.I0(\count_reg[0]_P_n_0 ),
        .I1(\count_reg[0]_LDC_n_0 ),
        .I2(\count_reg[0]_C_n_0 ),
        .O(Q[0]));
  LUT3 #(
    .INIT(8'hB8)) 
    \Q[1]_INST_0 
       (.I0(\count_reg[1]_P_n_0 ),
        .I1(\count_reg[1]_LDC_n_0 ),
        .I2(\count_reg[1]_C_n_0 ),
        .O(Q[1]));
  LUT3 #(
    .INIT(8'hB8)) 
    \Q[2]_INST_0 
       (.I0(\count_reg[2]_P_n_0 ),
        .I1(\count_reg[2]_LDC_n_0 ),
        .I2(\count_reg[2]_C_n_0 ),
        .O(Q[2]));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \Q[3]_INST_0 
       (.I0(\count_reg[3]_P_n_0 ),
        .I1(\count_reg[3]_LDC_n_0 ),
        .I2(\count_reg[3]_C_n_0 ),
        .O(Q[3]));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT4 #(
    .INIT(16'hF207)) 
    \count[0]_C_i_1 
       (.I0(\count_reg[0]_LDC_n_0 ),
        .I1(\count_reg[0]_P_n_0 ),
        .I2(cten),
        .I3(\count_reg[0]_C_n_0 ),
        .O(\count[0]_C_i_1_n_0 ));
  LUT3 #(
    .INIT(8'h1D)) 
    \count[0]_P_i_1 
       (.I0(\count_reg[0]_C_n_0 ),
        .I1(\count_reg[0]_LDC_n_0 ),
        .I2(\count_reg[0]_P_n_0 ),
        .O(\count[0]_P_i_1_n_0 ));
  LUT6 #(
    .INIT(64'hFFFF969900009666)) 
    \count[1]_C_i_1 
       (.I0(Q[0]),
        .I1(du),
        .I2(\count_reg[1]_P_n_0 ),
        .I3(\count_reg[1]_LDC_n_0 ),
        .I4(cten),
        .I5(\count_reg[1]_C_n_0 ),
        .O(\count[1]_C_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'hE21D1DE2)) 
    \count[1]_P_i_1 
       (.I0(\count_reg[0]_C_n_0 ),
        .I1(\count_reg[0]_LDC_n_0 ),
        .I2(\count_reg[0]_P_n_0 ),
        .I3(du),
        .I4(Q[1]),
        .O(\count[1]_P_i_1_n_0 ));
  LUT6 #(
    .INIT(64'hFFFF659A00006A95)) 
    \count[2]_C_i_1 
       (.I0(\count[2]_C_i_2_n_0 ),
        .I1(\count_reg[2]_P_n_0 ),
        .I2(\count_reg[2]_LDC_n_0 ),
        .I3(Q[1]),
        .I4(cten),
        .I5(\count_reg[2]_C_n_0 ),
        .O(\count[2]_C_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h77711171)) 
    \count[2]_C_i_2 
       (.I0(du),
        .I1(Q[1]),
        .I2(\count_reg[0]_C_n_0 ),
        .I3(\count_reg[0]_LDC_n_0 ),
        .I4(\count_reg[0]_P_n_0 ),
        .O(\count[2]_C_i_2_n_0 ));
  LUT6 #(
    .INIT(64'hFF4700B8B8FF4700)) 
    \count[2]_P_i_1 
       (.I0(\count_reg[0]_P_n_0 ),
        .I1(\count_reg[0]_LDC_n_0 ),
        .I2(\count_reg[0]_C_n_0 ),
        .I3(du),
        .I4(Q[2]),
        .I5(Q[1]),
        .O(\count[2]_P_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT3 #(
    .INIT(8'hE2)) 
    \count[3]_C_i_1 
       (.I0(\count[3]_P_i_2_n_0 ),
        .I1(cten),
        .I2(\count_reg[3]_C_n_0 ),
        .O(\count[3]_C_i_1_n_0 ));
  LUT1 #(
    .INIT(2'h1)) 
    \count[3]_P_i_1 
       (.I0(cten),
        .O(p_2_in));
  LUT5 #(
    .INIT(32'hBF40FD02)) 
    \count[3]_P_i_2 
       (.I0(du),
        .I1(Q[0]),
        .I2(Q[1]),
        .I3(Q[3]),
        .I4(Q[2]),
        .O(\count[3]_P_i_2_n_0 ));
  FDCE \count_reg[0]_C 
       (.C(clk),
        .CE(1'b1),
        .CLR(\count_reg[0]_LDC_i_2_n_0 ),
        .D(\count[0]_C_i_1_n_0 ),
        .Q(\count_reg[0]_C_n_0 ));
  (* XILINX_LEGACY_PRIM = "LDC" *) 
  LDCE #(
    .INIT(1'b0)) 
    \count_reg[0]_LDC 
       (.CLR(\count_reg[0]_LDC_i_2_n_0 ),
        .D(1'b1),
        .G(\count_reg[0]_LDC_i_1_n_0 ),
        .GE(1'b1),
        .Q(\count_reg[0]_LDC_n_0 ));
  LUT2 #(
    .INIT(4'h2)) 
    \count_reg[0]_LDC_i_1 
       (.I0(data_in[0]),
        .I1(load),
        .O(\count_reg[0]_LDC_i_1_n_0 ));
  LUT2 #(
    .INIT(4'h1)) 
    \count_reg[0]_LDC_i_2 
       (.I0(load),
        .I1(data_in[0]),
        .O(\count_reg[0]_LDC_i_2_n_0 ));
  FDPE \count_reg[0]_P 
       (.C(clk),
        .CE(p_2_in),
        .D(\count[0]_P_i_1_n_0 ),
        .PRE(\count_reg[0]_LDC_i_1_n_0 ),
        .Q(\count_reg[0]_P_n_0 ));
  FDCE \count_reg[1]_C 
       (.C(clk),
        .CE(1'b1),
        .CLR(\count_reg[1]_LDC_i_2_n_0 ),
        .D(\count[1]_C_i_1_n_0 ),
        .Q(\count_reg[1]_C_n_0 ));
  (* XILINX_LEGACY_PRIM = "LDC" *) 
  LDCE #(
    .INIT(1'b0)) 
    \count_reg[1]_LDC 
       (.CLR(\count_reg[1]_LDC_i_2_n_0 ),
        .D(1'b1),
        .G(\count_reg[1]_LDC_i_1_n_0 ),
        .GE(1'b1),
        .Q(\count_reg[1]_LDC_n_0 ));
  LUT2 #(
    .INIT(4'h2)) 
    \count_reg[1]_LDC_i_1 
       (.I0(data_in[1]),
        .I1(load),
        .O(\count_reg[1]_LDC_i_1_n_0 ));
  LUT2 #(
    .INIT(4'h1)) 
    \count_reg[1]_LDC_i_2 
       (.I0(load),
        .I1(data_in[1]),
        .O(\count_reg[1]_LDC_i_2_n_0 ));
  FDPE \count_reg[1]_P 
       (.C(clk),
        .CE(p_2_in),
        .D(\count[1]_P_i_1_n_0 ),
        .PRE(\count_reg[1]_LDC_i_1_n_0 ),
        .Q(\count_reg[1]_P_n_0 ));
  FDCE \count_reg[2]_C 
       (.C(clk),
        .CE(1'b1),
        .CLR(\count_reg[2]_LDC_i_2_n_0 ),
        .D(\count[2]_C_i_1_n_0 ),
        .Q(\count_reg[2]_C_n_0 ));
  (* XILINX_LEGACY_PRIM = "LDC" *) 
  LDCE #(
    .INIT(1'b0)) 
    \count_reg[2]_LDC 
       (.CLR(\count_reg[2]_LDC_i_2_n_0 ),
        .D(1'b1),
        .G(\count_reg[2]_LDC_i_1_n_0 ),
        .GE(1'b1),
        .Q(\count_reg[2]_LDC_n_0 ));
  LUT2 #(
    .INIT(4'h2)) 
    \count_reg[2]_LDC_i_1 
       (.I0(data_in[2]),
        .I1(load),
        .O(\count_reg[2]_LDC_i_1_n_0 ));
  LUT2 #(
    .INIT(4'h1)) 
    \count_reg[2]_LDC_i_2 
       (.I0(load),
        .I1(data_in[2]),
        .O(\count_reg[2]_LDC_i_2_n_0 ));
  FDPE \count_reg[2]_P 
       (.C(clk),
        .CE(p_2_in),
        .D(\count[2]_P_i_1_n_0 ),
        .PRE(\count_reg[2]_LDC_i_1_n_0 ),
        .Q(\count_reg[2]_P_n_0 ));
  FDCE \count_reg[3]_C 
       (.C(clk),
        .CE(1'b1),
        .CLR(\count_reg[3]_LDC_i_2_n_0 ),
        .D(\count[3]_C_i_1_n_0 ),
        .Q(\count_reg[3]_C_n_0 ));
  (* XILINX_LEGACY_PRIM = "LDC" *) 
  LDCE #(
    .INIT(1'b0)) 
    \count_reg[3]_LDC 
       (.CLR(\count_reg[3]_LDC_i_2_n_0 ),
        .D(1'b1),
        .G(\count_reg[3]_LDC_i_1_n_0 ),
        .GE(1'b1),
        .Q(\count_reg[3]_LDC_n_0 ));
  LUT2 #(
    .INIT(4'h2)) 
    \count_reg[3]_LDC_i_1 
       (.I0(data_in[3]),
        .I1(load),
        .O(\count_reg[3]_LDC_i_1_n_0 ));
  LUT2 #(
    .INIT(4'h1)) 
    \count_reg[3]_LDC_i_2 
       (.I0(load),
        .I1(data_in[3]),
        .O(\count_reg[3]_LDC_i_2_n_0 ));
  FDPE \count_reg[3]_P 
       (.C(clk),
        .CE(p_2_in),
        .D(\count[3]_P_i_2_n_0 ),
        .PRE(\count_reg[3]_LDC_i_1_n_0 ),
        .Q(\count_reg[3]_P_n_0 ));
endmodule

(* CHECK_LICENSE_TYPE = "design_1_counter_0_0,counter,{}" *) (* downgradeipidentifiedwarnings = "yes" *) (* ip_definition_source = "package_project" *) 
(* x_core_info = "counter,Vivado 2018.2" *) 
(* NotValidForBitStream *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix
   (clk,
    du,
    load,
    cten,
    data_in,
    Q);
  (* x_interface_info = "xilinx.com:signal:clock:1.0 clk CLK" *) (* x_interface_parameter = "XIL_INTERFACENAME clk, FREQ_HZ 100000000, PHASE 0.000" *) input clk;
  input du;
  input load;
  input cten;
  input [3:0]data_in;
  output [3:0]Q;

  wire [3:0]Q;
  wire clk;
  wire cten;
  wire [3:0]data_in;
  wire du;
  wire load;

  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_counter U0
       (.Q(Q),
        .clk(clk),
        .cten(cten),
        .data_in(data_in),
        .du(du),
        .load(load));
endmodule
`ifndef GLBL
`define GLBL
`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;
    wire FCSBO_GLBL;
    wire [3:0] DO_GLBL;
    wire [3:0] DI_GLBL;
   
    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (strong1, weak0) GSR = GSR_int;
    assign (strong1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule
`endif
