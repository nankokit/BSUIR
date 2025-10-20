// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2018.2 (win64) Build 2258646 Thu Jun 14 20:03:12 MDT 2018
// Date        : Sun Oct  5 21:52:13 2025
// Host        : nankokit running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ design_1_c_counter_binary_0_0_sim_netlist.v
// Design      : design_1_c_counter_binary_0_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7vx485tffg1157-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "design_1_c_counter_binary_0_0,c_counter_binary_v12_0_12,{}" *) (* downgradeipidentifiedwarnings = "yes" *) (* x_core_info = "c_counter_binary_v12_0_12,Vivado 2018.2" *) 
(* NotValidForBitStream *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix
   (CLK,
    Q);
  (* x_interface_info = "xilinx.com:signal:clock:1.0 clk_intf CLK" *) (* x_interface_parameter = "XIL_INTERFACENAME clk_intf, ASSOCIATED_BUSIF q_intf:thresh0_intf:l_intf:load_intf:up_intf:sinit_intf:sset_intf, ASSOCIATED_RESET SCLR, ASSOCIATED_CLKEN CE, FREQ_HZ 100000000, PHASE 0.0, CLK_DOMAIN design_1_clk_wiz_1_clk_out1" *) input CLK;
  (* x_interface_info = "xilinx.com:signal:data:1.0 q_intf DATA" *) (* x_interface_parameter = "XIL_INTERFACENAME q_intf, LAYERED_METADATA xilinx.com:interface:datatypes:1.0 {DATA {datatype {name {attribs {resolve_type immediate dependency {} format string minimum {} maximum {}} value data} bitwidth {attribs {resolve_type generated dependency bitwidth format long minimum {} maximum {}} value 26} bitoffset {attribs {resolve_type immediate dependency {} format long minimum {} maximum {}} value 0} integer {signed {attribs {resolve_type immediate dependency {} format bool minimum {} maximum {}} value false}}}} DATA_WIDTH 26}" *) output [25:0]Q;

  wire CLK;
  wire [25:0]Q;
  wire NLW_U0_THRESH0_UNCONNECTED;

  (* C_AINIT_VAL = "0" *) 
  (* C_CE_OVERRIDES_SYNC = "0" *) 
  (* C_FB_LATENCY = "0" *) 
  (* C_HAS_CE = "0" *) 
  (* C_HAS_SCLR = "0" *) 
  (* C_HAS_SINIT = "0" *) 
  (* C_HAS_SSET = "0" *) 
  (* C_IMPLEMENTATION = "0" *) 
  (* C_SCLR_OVERRIDES_SSET = "1" *) 
  (* C_SINIT_VAL = "0" *) 
  (* C_VERBOSITY = "0" *) 
  (* C_WIDTH = "26" *) 
  (* C_XDEVICEFAMILY = "virtex7" *) 
  (* c_count_by = "1" *) 
  (* c_count_mode = "0" *) 
  (* c_count_to = "1" *) 
  (* c_has_load = "0" *) 
  (* c_has_thresh0 = "0" *) 
  (* c_latency = "1" *) 
  (* c_load_low = "0" *) 
  (* c_restrict_count = "0" *) 
  (* c_thresh0_value = "1" *) 
  (* downgradeipidentifiedwarnings = "yes" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_c_counter_binary_v12_0_12 U0
       (.CE(1'b1),
        .CLK(CLK),
        .L({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .LOAD(1'b0),
        .Q(Q),
        .SCLR(1'b0),
        .SINIT(1'b0),
        .SSET(1'b0),
        .THRESH0(NLW_U0_THRESH0_UNCONNECTED),
        .UP(1'b1));
endmodule

(* C_AINIT_VAL = "0" *) (* C_CE_OVERRIDES_SYNC = "0" *) (* C_COUNT_BY = "1" *) 
(* C_COUNT_MODE = "0" *) (* C_COUNT_TO = "1" *) (* C_FB_LATENCY = "0" *) 
(* C_HAS_CE = "0" *) (* C_HAS_LOAD = "0" *) (* C_HAS_SCLR = "0" *) 
(* C_HAS_SINIT = "0" *) (* C_HAS_SSET = "0" *) (* C_HAS_THRESH0 = "0" *) 
(* C_IMPLEMENTATION = "0" *) (* C_LATENCY = "1" *) (* C_LOAD_LOW = "0" *) 
(* C_RESTRICT_COUNT = "0" *) (* C_SCLR_OVERRIDES_SSET = "1" *) (* C_SINIT_VAL = "0" *) 
(* C_THRESH0_VALUE = "1" *) (* C_VERBOSITY = "0" *) (* C_WIDTH = "26" *) 
(* C_XDEVICEFAMILY = "virtex7" *) (* downgradeipidentifiedwarnings = "yes" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_c_counter_binary_v12_0_12
   (CLK,
    CE,
    SCLR,
    SSET,
    SINIT,
    UP,
    LOAD,
    L,
    THRESH0,
    Q);
  input CLK;
  input CE;
  input SCLR;
  input SSET;
  input SINIT;
  input UP;
  input LOAD;
  input [25:0]L;
  output THRESH0;
  output [25:0]Q;

  wire \<const1> ;
  wire CLK;
  wire [25:0]Q;
  wire NLW_i_synth_THRESH0_UNCONNECTED;

  assign THRESH0 = \<const1> ;
  VCC VCC
       (.P(\<const1> ));
  (* C_AINIT_VAL = "0" *) 
  (* C_CE_OVERRIDES_SYNC = "0" *) 
  (* C_FB_LATENCY = "0" *) 
  (* C_HAS_CE = "0" *) 
  (* C_HAS_SCLR = "0" *) 
  (* C_HAS_SINIT = "0" *) 
  (* C_HAS_SSET = "0" *) 
  (* C_IMPLEMENTATION = "0" *) 
  (* C_SCLR_OVERRIDES_SSET = "1" *) 
  (* C_SINIT_VAL = "0" *) 
  (* C_VERBOSITY = "0" *) 
  (* C_WIDTH = "26" *) 
  (* C_XDEVICEFAMILY = "virtex7" *) 
  (* c_count_by = "1" *) 
  (* c_count_mode = "0" *) 
  (* c_count_to = "1" *) 
  (* c_has_load = "0" *) 
  (* c_has_thresh0 = "0" *) 
  (* c_latency = "1" *) 
  (* c_load_low = "0" *) 
  (* c_restrict_count = "0" *) 
  (* c_thresh0_value = "1" *) 
  (* downgradeipidentifiedwarnings = "yes" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_c_counter_binary_v12_0_12_viv i_synth
       (.CE(1'b0),
        .CLK(CLK),
        .L({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .LOAD(1'b0),
        .Q(Q),
        .SCLR(1'b0),
        .SINIT(1'b0),
        .SSET(1'b0),
        .THRESH0(NLW_i_synth_THRESH0_UNCONNECTED),
        .UP(1'b0));
endmodule
`pragma protect begin_protected
`pragma protect version = 1
`pragma protect encrypt_agent = "XILINX"
`pragma protect encrypt_agent_info = "Xilinx Encryption Tool 2015"
`pragma protect key_keyowner="Cadence Design Systems.", key_keyname="cds_rsa_key", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=64)
`pragma protect key_block
RhedCTftnR/lFLJouqVu00X8CC4TkDlMiW/WeWJSYDyQHVO1xW1z9+hmgAziXI4s3uGElBs9cXRS
4yVMV7QH0w==

`pragma protect key_keyowner="Synopsys", key_keyname="SNPS-VCS-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
qX90FYlZfOA3sZpcv0rrvWRKCSlr3skWpeAe5OSxHcZPsVQFyY0hhWVDtP/vB+dV9hIUwAN29Fn9
+L9OEXYMlIw5gH6s9NmquAs3lmPRLTrrpKJWdvf6+b+LeG9CPwLz87bXAk4qQW80zUHBaDKDLV3q
pd/gEf8Y3st+mLOGjNU=

`pragma protect key_keyowner="Aldec", key_keyname="ALDEC15_001", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
chpH3Rj5RAirYZHkpJvTu4EJpydPPSy15v646y2lN/1w3bwHI+M8EpLMBjimx8uIWRJZ6dDWPR8E
zkwK1TMroEKFaL8IkFMSHPyzqbrH9l1jxYFs0ee8Itp8Rg8qenv5RXM+h4JRTtRmzHk1A8s8zeKY
MgXzIBCAzBa/vSgzDuDaUnO8r8f/5KtRjmE28JLNXXAxyijBrMTCiIHMRJZL5/+LgUyVq7/Zr5yx
7kuNGWv7Q0wESEqhsQbK6UNel5ak1cor7647dYJgIxnNZ6jGVJPkqi8ydAIZ0z0Dy4txBvVaMyP6
2kYYnbVN6S6q6yr/QcJHEOgefF59B/8JuWzdoQ==

`pragma protect key_keyowner="ATRENTA", key_keyname="ATR-SG-2015-RSA-3", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
Z2XauOF3/9FUIv1kEFfEtdy93+zHY5q9dH1pJCNLytoWWhhJBfCI5Uc2w/fQLrvVw2Ivy0/rs9qH
9fomTNECWeCphNDVpWGNcFDGE51jt6SDWt7FmgfZq4iXN7XWrfmkQa4DB7QbtSBHCMcBT53TKbDH
suKNhAWMV0htWeNEgN4Y0biiz8U4RLT1stdsMMtEzfYVhtrTmFWLihJ/9gJ01pm/kv4OB2cJEslg
sjbxCE2B4Y1uSj93tnBAw4/f2RYGfPcSrkaXkNgOYK2dc5NQgd82XvP8siAK/ETcZQ1lBK367Rxv
nlr6QUMwKALmjLVe/IThpCRGbOSy3QykkwnpHw==

`pragma protect key_keyowner="Xilinx", key_keyname="xilinxt_2017_05", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
F/mF2RV90mf5PFUZZUjMej6jQS+Qx16uCeiDQxUX+H+O2yjP6UECegDbtLmGk9algbDuGBCE0KgZ
HzSxX5pMwDq1J7nFyBsu1dGyu8NeJxfu0fFA/qS/SYJSTtwnZ/eIj09mNLJ3w8wGR87ke1ETTRpx
4Ni9DzGJ/aaWFaUenL/x4UWS9yqlaNi5Utcpa4kcUHC6fW0asZsThZJBqpArO54TF40nxZAD+V82
6mBGFOKUzgmHqXNsbVif4JqUd63LG8YWxjrOeesq61xzyjw9caQMuA9v/5sQslCFaeSt2atgqyaA
y4mcm0kU60s6mYqJr4KZt8DWG7LPGoIjhjpnMg==

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VELOCE-RSA", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
YNrNxIZl8p2OCPz+4Y4awQ09ZZ41X8EdKu2SX1gXfB3qoV3EOXf4eFHtYJ+bFIccfEIqPfZ+dnr7
udcSDAJMcxqZe+YNk5hTq+uX4PlnQH/IVlkgyYiDhQ7aRIS5pwtIRC3biXn80933ov4zlWLI+ZBq
bG8lodZjxKh6HZPWi7s=

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VERIF-SIM-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
eWToRZf4bzAQhHTj44yEkOqolJ3BOvlBPKnKoDCpSvCHSrnRcJKgOd47PnboABTqLVstQenz8AFe
rWHBbaad7KOIh3LsEXBwDHKDdby7iDAB5nd3j2Wp7qiHOv7WpIJ5RG1GMQa8+QXHMVaV4jSvsdmt
d0D9H0WEVaqrSFW2ucpsMYNE0LnKcSJ1SejKOcgISzaGJgXcmEAOVAOnCDPc4slwc3dt7Jne1KvY
i66An36Mfhhd1b0RikMf4yqpM+uVrL5XWIP+TBtw3iQRE0ZkUSn3K0My73/2vKKBfwyV0c+M/Il+
aMxNaD44Gq+/f6zGjkelgMEI4BjB5rs1KRvSCg==

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-PREC-RSA", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
Vp6fI25JhCmajDHYYhlBbQPZ007ZfGAizUdOG1ZL+2c//McQVH9TDmw4cvHeqNYAOKeiA6ElBTdQ
mJUBLo5OGGprvzZAUFZ8bPDqCFQ3/iRNWfkkyCBWwwkoKqjXE+/AmLx9P6AGGO9lcBAHpRmtxN/y
NGnmuuS/DhycHptBWRjYA34ngJi9qJnILflyzeE+poUaKofFmtndIWZ6nCm0OeQzj9Xk7dnaqvda
OLpF8dw6Lm+3rK9Xu/sZndRPUWdfwPtvpYhTF58b0PtR4x9QC8qVr+mVqb+SDLbuiCJ/7UdEuWKx
esKQslyJI/f7pIY8FLTcSu0DkfkcAIeXNQsK4A==

`pragma protect key_keyowner="Synplicity", key_keyname="SYNP15_1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
aQd0/bpa/SWeimAB+zuDHY/qw4zWhybw14EwPFVuEfYAutX0cVdrQr/OkJoJmyTXY/WJNtF8klBd
vC2b4RvCsrJJJIOMbpN8n73yof4TJGytsl54RM5p8Ve5jgecq8GOjBMtUUcRNtdk6KOwfanpnqGZ
ErxNgmwa0S1RiLBmxMR18eldz4jR80sADb6v4+gYojBabUa5hzpIG5WWKQKEXDKyLBSNqERqYrZV
X9adF6nsfTWQU8sggyjJxguxPf76EKVUjbt9uG4xerxvCDZdc+hAQXoWIYLMlf0CP7YH2nwJ2FRd
zMpYxaZfV9sg7WfD8GRAdh6M7oSeXcu43hVl9w==

`pragma protect data_method = "AES128-CBC"
`pragma protect encoding = (enctype = "BASE64", line_length = 76, bytes = 14976)
`pragma protect data_block
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
`pragma protect end_protected
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
