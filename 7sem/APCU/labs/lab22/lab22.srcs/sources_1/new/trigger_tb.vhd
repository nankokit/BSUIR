----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 20.09.2025 20:22:11
-- Design Name: 
-- Module Name: trigger_tb - trigger_tb
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
use IEEE.STD_LOGIC_1164.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity trigger_tb is
end trigger_tb;

architecture trigger_tb of trigger_tb is
    component d_trigger is
        port (
            d, c, not_s, not_r : in std_logic;
            q, not_q : out std_logic
        );
    end component;
    signal d, c : std_logic;
    signal s, r : std_logic := '1';
    signal q, nq : std_logic;
begin
    uut : d_trigger
    port map(
        d => d,
        c => c,
        not_s => s,
        not_r => r,
        q => q,
        not_q => nq
    );
    clk_gen : process
    begin
        c <= '0';
        wait for 10 ns;
        c <= '1';
        wait for 10 ns;
    end process; -- clk_gen
    test_trigger : process
    begin
        d <= '1';
        wait for 20 ns;
        d <= '0';
        wait for 25 ns;
        s <= '0';
        wait for 20 ns;
        s <= '1';
        r <= '0';
        wait;
    end process; -- test_trigger
end trigger_tb;