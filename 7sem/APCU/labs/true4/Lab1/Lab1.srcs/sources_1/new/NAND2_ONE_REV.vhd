----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 06.09.2025 08:53:54
-- Design Name: 
-- Module Name: NAND2_ONE_REV - Behavioral
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

entity NAND2_ONE_REV is
    Port (
        A, B : in std_logic;
        C : out std_logic
     );
end NAND2_ONE_REV;

architecture Behavioral of NAND2_ONE_REV is

begin
    C <= not(A and (not B)); 

end Behavioral;
