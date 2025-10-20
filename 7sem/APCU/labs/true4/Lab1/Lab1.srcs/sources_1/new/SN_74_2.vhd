----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 06.09.2025 12:16:04
-- Design Name: 
-- Module Name: SN_74_2 - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity SN_74_2 is
     port (
           G1, G2A, G2B : in std_logic;
           C, B, A : in std_logic;
           Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7 : out std_logic
       );
end SN_74_2;

architecture Behavioral of SN_74_2 is
    signal enable : std_logic;
    signal wire1 : std_logic;
    signal wire2 : std_logic;
    signal wire3 : std_logic;
    signal wire4 : std_logic;
    signal wire5 : std_logic;
    signal wire6 : std_logic;
    signal wire7 : std_logic;
    
    signal andout1 : std_logic;
    signal andout2 : std_logic;
    signal andout3 : std_logic;
    signal andout4 : std_logic;
    signal andout5 : std_logic;
    signal andout6 : std_logic;
    signal andout7 : std_logic;
    signal andout8 : std_logic;
    
    signal Gout1 : std_logic;
    signal Gout2 : std_logic;
    signal Gout3 : std_logic;
    
    
    component SIMPLE_BUFFER 
        port (
            A : in std_logic;
            Y: out std_logic
        );
    end component;
    
    component N_BUFFER
        Port ( 
            A: in std_logic;
            Y: out std_logic
        );
    end component;
    
    component AND3
        port (
                A, B, C : in  std_logic;  -- Three input signals
                Y       : out std_logic   -- Output signal
        );
    end component;
    
    component NAND2_ONE_REV
        Port (
                A, B : in std_logic;
                C : out std_logic
         );
    end component;
    
    component NAND3
        Port(
                A, B, C: in std_logic;
                Y: out std_logic
        );
    end component;
       
begin

    buffer1: SIMPLE_BUFFER port map (
        A => A, 
        Y => wire1
    );
    buffer2: SIMPLE_BUFFER port map (
        A => B, 
        Y => wire6
    );
    buffer3: N_BUFFER port map (
        A => wire1, 
        Y => wire2
    );
    buffer4: N_BUFFER port map (
        A => wire6, 
        Y => wire4
    );
    buffer5: N_BUFFER port map (
        A => C, 
        Y => wire3
    );
    
    and3_1: AND3 port map(
        A => wire2,
        B => wire4,
        C => wire3, 
        Y => andout1
    );
    and3_2: AND3 port map(
        A => wire1,
        B => wire4,
        C => wire3, 
        Y => andout2
    );
    and3_3: AND3 port map(
        A => wire2,
        B => wire6,
        C => wire3, 
        Y => andout3
    );
    and3_4: AND3 port map(
        A => wire1,
        B => wire6,
        C => wire3, 
        Y => andout4
    );
    and3_5: AND3 port map(
        A => wire2,
        B => wire4,
        C => C, 
        Y => andout5
    );
    and3_6: AND3 port map(
        A => wire1,
        B => wire4,
        C => C, 
        Y => andout6
    );
    and3_7: AND3 port map(
        A => wire2,
        B => wire6,
        C => C, 
        Y => andout7
    );
    and3_8: AND3 port map(
        A => wire1,
        B => wire6,
        C => C, 
        Y => andout8
    );
    
    buffer6: SIMPLE_BUFFER port map (
        A => G1, 
        Y => Gout1
    );
    buffer7: N_BUFFER port map (
        A => G2A, 
        Y => Gout2
    );
    buffer8: N_BUFFER port map (
        A => G2B, 
        Y => Gout3
    );
    
    nand_3_1 : NAND3 port map(
        A => Gout1,
        B => Gout2,
        C => Gout3,
        Y => wire7
    );

    nand_2_1: NAND2_ONE_REV port map(
        A => andout1,
        B => wire7,
        C => Y0
    );
    nand_2_2: NAND2_ONE_REV port map(
        A => andout2, 
        B => wire7, 
        C => Y1
    );
    nand_2_3: NAND2_ONE_REV port map(
        A => andout3,
        B => wire7,
        C => Y2
    );
    nand_2_4: NAND2_ONE_REV port map(
        A => andout4,
        B => wire7,
        C => Y3
    );
    nand_2_5: NAND2_ONE_REV port map(
        A => andout5,
        B => wire7,
        C => Y4
    );
    nand_2_6: NAND2_ONE_REV port map(
        A => andout6,
        B => wire7,
        C => Y5
    );
    nand_2_7: NAND2_ONE_REV port map(
        A => andout7,
        B => wire7,
        C => Y6
    );
    nand_2_8: NAND2_ONE_REV port map(
        A => andout8,
        B => wire7,
        C => Y7
    );    

end Behavioral;
