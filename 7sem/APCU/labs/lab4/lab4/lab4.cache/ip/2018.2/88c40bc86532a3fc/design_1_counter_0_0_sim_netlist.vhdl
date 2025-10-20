-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.2 (win64) Build 2258646 Thu Jun 14 20:03:12 MDT 2018
-- Date        : Sun Oct  5 21:52:09 2025
-- Host        : nankokit running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
--               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ design_1_counter_0_0_sim_netlist.vhdl
-- Design      : design_1_counter_0_0
-- Purpose     : This VHDL netlist is a functional simulation representation of the design and should not be modified or
--               synthesized. This netlist cannot be used for SDF annotated simulation.
-- Device      : xc7vx485tffg1157-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_counter is
  port (
    clk : in STD_LOGIC;
    du : in STD_LOGIC;
    load : in STD_LOGIC;
    cten : in STD_LOGIC;
    data_in : in STD_LOGIC_VECTOR ( 3 downto 0 );
    Q : out STD_LOGIC_VECTOR ( 3 downto 0 )
  );
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_counter;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_counter is
  signal \^q\ : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal \count[0]_C_i_1_n_0\ : STD_LOGIC;
  signal \count[0]_P_i_1_n_0\ : STD_LOGIC;
  signal \count[1]_C_i_1_n_0\ : STD_LOGIC;
  signal \count[1]_P_i_1_n_0\ : STD_LOGIC;
  signal \count[2]_C_i_1_n_0\ : STD_LOGIC;
  signal \count[2]_C_i_2_n_0\ : STD_LOGIC;
  signal \count[2]_P_i_1_n_0\ : STD_LOGIC;
  signal \count[3]_C_i_1_n_0\ : STD_LOGIC;
  signal \count[3]_P_i_2_n_0\ : STD_LOGIC;
  signal \count_reg[0]_C_n_0\ : STD_LOGIC;
  signal \count_reg[0]_LDC_i_1_n_0\ : STD_LOGIC;
  signal \count_reg[0]_LDC_i_2_n_0\ : STD_LOGIC;
  signal \count_reg[0]_LDC_n_0\ : STD_LOGIC;
  signal \count_reg[0]_P_n_0\ : STD_LOGIC;
  signal \count_reg[1]_C_n_0\ : STD_LOGIC;
  signal \count_reg[1]_LDC_i_1_n_0\ : STD_LOGIC;
  signal \count_reg[1]_LDC_i_2_n_0\ : STD_LOGIC;
  signal \count_reg[1]_LDC_n_0\ : STD_LOGIC;
  signal \count_reg[1]_P_n_0\ : STD_LOGIC;
  signal \count_reg[2]_C_n_0\ : STD_LOGIC;
  signal \count_reg[2]_LDC_i_1_n_0\ : STD_LOGIC;
  signal \count_reg[2]_LDC_i_2_n_0\ : STD_LOGIC;
  signal \count_reg[2]_LDC_n_0\ : STD_LOGIC;
  signal \count_reg[2]_P_n_0\ : STD_LOGIC;
  signal \count_reg[3]_C_n_0\ : STD_LOGIC;
  signal \count_reg[3]_LDC_i_1_n_0\ : STD_LOGIC;
  signal \count_reg[3]_LDC_i_2_n_0\ : STD_LOGIC;
  signal \count_reg[3]_LDC_n_0\ : STD_LOGIC;
  signal \count_reg[3]_P_n_0\ : STD_LOGIC;
  signal p_2_in : STD_LOGIC;
  attribute SOFT_HLUTNM : string;
  attribute SOFT_HLUTNM of \Q[0]_INST_0\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \Q[3]_INST_0\ : label is "soft_lutpair2";
  attribute SOFT_HLUTNM of \count[0]_C_i_1\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \count[1]_P_i_1\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \count[2]_C_i_2\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \count[3]_C_i_1\ : label is "soft_lutpair2";
  attribute XILINX_LEGACY_PRIM : string;
  attribute XILINX_LEGACY_PRIM of \count_reg[0]_LDC\ : label is "LDC";
  attribute XILINX_LEGACY_PRIM of \count_reg[1]_LDC\ : label is "LDC";
  attribute XILINX_LEGACY_PRIM of \count_reg[2]_LDC\ : label is "LDC";
  attribute XILINX_LEGACY_PRIM of \count_reg[3]_LDC\ : label is "LDC";
begin
  Q(3 downto 0) <= \^q\(3 downto 0);
\Q[0]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"B8"
    )
        port map (
      I0 => \count_reg[0]_P_n_0\,
      I1 => \count_reg[0]_LDC_n_0\,
      I2 => \count_reg[0]_C_n_0\,
      O => \^q\(0)
    );
\Q[1]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"B8"
    )
        port map (
      I0 => \count_reg[1]_P_n_0\,
      I1 => \count_reg[1]_LDC_n_0\,
      I2 => \count_reg[1]_C_n_0\,
      O => \^q\(1)
    );
\Q[2]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"B8"
    )
        port map (
      I0 => \count_reg[2]_P_n_0\,
      I1 => \count_reg[2]_LDC_n_0\,
      I2 => \count_reg[2]_C_n_0\,
      O => \^q\(2)
    );
\Q[3]_INST_0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"B8"
    )
        port map (
      I0 => \count_reg[3]_P_n_0\,
      I1 => \count_reg[3]_LDC_n_0\,
      I2 => \count_reg[3]_C_n_0\,
      O => \^q\(3)
    );
\count[0]_C_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"F207"
    )
        port map (
      I0 => \count_reg[0]_LDC_n_0\,
      I1 => \count_reg[0]_P_n_0\,
      I2 => cten,
      I3 => \count_reg[0]_C_n_0\,
      O => \count[0]_C_i_1_n_0\
    );
\count[0]_P_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"1D"
    )
        port map (
      I0 => \count_reg[0]_C_n_0\,
      I1 => \count_reg[0]_LDC_n_0\,
      I2 => \count_reg[0]_P_n_0\,
      O => \count[0]_P_i_1_n_0\
    );
\count[1]_C_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFF969900009666"
    )
        port map (
      I0 => \^q\(0),
      I1 => du,
      I2 => \count_reg[1]_P_n_0\,
      I3 => \count_reg[1]_LDC_n_0\,
      I4 => cten,
      I5 => \count_reg[1]_C_n_0\,
      O => \count[1]_C_i_1_n_0\
    );
\count[1]_P_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"E21D1DE2"
    )
        port map (
      I0 => \count_reg[0]_C_n_0\,
      I1 => \count_reg[0]_LDC_n_0\,
      I2 => \count_reg[0]_P_n_0\,
      I3 => du,
      I4 => \^q\(1),
      O => \count[1]_P_i_1_n_0\
    );
\count[2]_C_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFF659A00006A95"
    )
        port map (
      I0 => \count[2]_C_i_2_n_0\,
      I1 => \count_reg[2]_P_n_0\,
      I2 => \count_reg[2]_LDC_n_0\,
      I3 => \^q\(1),
      I4 => cten,
      I5 => \count_reg[2]_C_n_0\,
      O => \count[2]_C_i_1_n_0\
    );
\count[2]_C_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"77711171"
    )
        port map (
      I0 => du,
      I1 => \^q\(1),
      I2 => \count_reg[0]_C_n_0\,
      I3 => \count_reg[0]_LDC_n_0\,
      I4 => \count_reg[0]_P_n_0\,
      O => \count[2]_C_i_2_n_0\
    );
\count[2]_P_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FF4700B8B8FF4700"
    )
        port map (
      I0 => \count_reg[0]_P_n_0\,
      I1 => \count_reg[0]_LDC_n_0\,
      I2 => \count_reg[0]_C_n_0\,
      I3 => du,
      I4 => \^q\(2),
      I5 => \^q\(1),
      O => \count[2]_P_i_1_n_0\
    );
\count[3]_C_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"E2"
    )
        port map (
      I0 => \count[3]_P_i_2_n_0\,
      I1 => cten,
      I2 => \count_reg[3]_C_n_0\,
      O => \count[3]_C_i_1_n_0\
    );
\count[3]_P_i_1\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => cten,
      O => p_2_in
    );
\count[3]_P_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"BF40FD02"
    )
        port map (
      I0 => du,
      I1 => \^q\(0),
      I2 => \^q\(1),
      I3 => \^q\(3),
      I4 => \^q\(2),
      O => \count[3]_P_i_2_n_0\
    );
\count_reg[0]_C\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \count_reg[0]_LDC_i_2_n_0\,
      D => \count[0]_C_i_1_n_0\,
      Q => \count_reg[0]_C_n_0\
    );
\count_reg[0]_LDC\: unisim.vcomponents.LDCE
    generic map(
      INIT => '0'
    )
        port map (
      CLR => \count_reg[0]_LDC_i_2_n_0\,
      D => '1',
      G => \count_reg[0]_LDC_i_1_n_0\,
      GE => '1',
      Q => \count_reg[0]_LDC_n_0\
    );
\count_reg[0]_LDC_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => data_in(0),
      I1 => load,
      O => \count_reg[0]_LDC_i_1_n_0\
    );
\count_reg[0]_LDC_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => load,
      I1 => data_in(0),
      O => \count_reg[0]_LDC_i_2_n_0\
    );
\count_reg[0]_P\: unisim.vcomponents.FDPE
     port map (
      C => clk,
      CE => p_2_in,
      D => \count[0]_P_i_1_n_0\,
      PRE => \count_reg[0]_LDC_i_1_n_0\,
      Q => \count_reg[0]_P_n_0\
    );
\count_reg[1]_C\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \count_reg[1]_LDC_i_2_n_0\,
      D => \count[1]_C_i_1_n_0\,
      Q => \count_reg[1]_C_n_0\
    );
\count_reg[1]_LDC\: unisim.vcomponents.LDCE
    generic map(
      INIT => '0'
    )
        port map (
      CLR => \count_reg[1]_LDC_i_2_n_0\,
      D => '1',
      G => \count_reg[1]_LDC_i_1_n_0\,
      GE => '1',
      Q => \count_reg[1]_LDC_n_0\
    );
\count_reg[1]_LDC_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => data_in(1),
      I1 => load,
      O => \count_reg[1]_LDC_i_1_n_0\
    );
\count_reg[1]_LDC_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => load,
      I1 => data_in(1),
      O => \count_reg[1]_LDC_i_2_n_0\
    );
\count_reg[1]_P\: unisim.vcomponents.FDPE
     port map (
      C => clk,
      CE => p_2_in,
      D => \count[1]_P_i_1_n_0\,
      PRE => \count_reg[1]_LDC_i_1_n_0\,
      Q => \count_reg[1]_P_n_0\
    );
\count_reg[2]_C\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \count_reg[2]_LDC_i_2_n_0\,
      D => \count[2]_C_i_1_n_0\,
      Q => \count_reg[2]_C_n_0\
    );
\count_reg[2]_LDC\: unisim.vcomponents.LDCE
    generic map(
      INIT => '0'
    )
        port map (
      CLR => \count_reg[2]_LDC_i_2_n_0\,
      D => '1',
      G => \count_reg[2]_LDC_i_1_n_0\,
      GE => '1',
      Q => \count_reg[2]_LDC_n_0\
    );
\count_reg[2]_LDC_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => data_in(2),
      I1 => load,
      O => \count_reg[2]_LDC_i_1_n_0\
    );
\count_reg[2]_LDC_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => load,
      I1 => data_in(2),
      O => \count_reg[2]_LDC_i_2_n_0\
    );
\count_reg[2]_P\: unisim.vcomponents.FDPE
     port map (
      C => clk,
      CE => p_2_in,
      D => \count[2]_P_i_1_n_0\,
      PRE => \count_reg[2]_LDC_i_1_n_0\,
      Q => \count_reg[2]_P_n_0\
    );
\count_reg[3]_C\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \count_reg[3]_LDC_i_2_n_0\,
      D => \count[3]_C_i_1_n_0\,
      Q => \count_reg[3]_C_n_0\
    );
\count_reg[3]_LDC\: unisim.vcomponents.LDCE
    generic map(
      INIT => '0'
    )
        port map (
      CLR => \count_reg[3]_LDC_i_2_n_0\,
      D => '1',
      G => \count_reg[3]_LDC_i_1_n_0\,
      GE => '1',
      Q => \count_reg[3]_LDC_n_0\
    );
\count_reg[3]_LDC_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => data_in(3),
      I1 => load,
      O => \count_reg[3]_LDC_i_1_n_0\
    );
\count_reg[3]_LDC_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => load,
      I1 => data_in(3),
      O => \count_reg[3]_LDC_i_2_n_0\
    );
\count_reg[3]_P\: unisim.vcomponents.FDPE
     port map (
      C => clk,
      CE => p_2_in,
      D => \count[3]_P_i_2_n_0\,
      PRE => \count_reg[3]_LDC_i_1_n_0\,
      Q => \count_reg[3]_P_n_0\
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  port (
    clk : in STD_LOGIC;
    du : in STD_LOGIC;
    load : in STD_LOGIC;
    cten : in STD_LOGIC;
    data_in : in STD_LOGIC_VECTOR ( 3 downto 0 );
    Q : out STD_LOGIC_VECTOR ( 3 downto 0 )
  );
  attribute NotValidForBitStream : boolean;
  attribute NotValidForBitStream of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is true;
  attribute CHECK_LICENSE_TYPE : string;
  attribute CHECK_LICENSE_TYPE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "design_1_counter_0_0,counter,{}";
  attribute downgradeipidentifiedwarnings : string;
  attribute downgradeipidentifiedwarnings of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "yes";
  attribute ip_definition_source : string;
  attribute ip_definition_source of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "package_project";
  attribute x_core_info : string;
  attribute x_core_info of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix : entity is "counter,Vivado 2018.2";
end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix;

architecture STRUCTURE of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  attribute x_interface_info : string;
  attribute x_interface_info of clk : signal is "xilinx.com:signal:clock:1.0 clk CLK";
  attribute x_interface_parameter : string;
  attribute x_interface_parameter of clk : signal is "XIL_INTERFACENAME clk, FREQ_HZ 100000000, PHASE 0.000";
begin
U0: entity work.decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_counter
     port map (
      Q(3 downto 0) => Q(3 downto 0),
      clk => clk,
      cten => cten,
      data_in(3 downto 0) => data_in(3 downto 0),
      du => du,
      load => load
    );
end STRUCTURE;
