----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 06.09.2025 07:46 AM CEST
-- Design Name: 
-- Module Name: three_input_and - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: Simple 3-input AND gate
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

entity AND3 is
    port (
        A, B, C : in  std_logic;  -- Three input signals
        Y       : out std_logic   -- Output signal
    );
end AND3;

architecture Behavioral of AND3 is
begin
    -- Concurrent signal assignment for 3-input AND
    Y <= A and B and C;
end Behavioral;