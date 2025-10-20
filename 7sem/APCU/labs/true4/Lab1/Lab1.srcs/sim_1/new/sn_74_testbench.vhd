----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 06.09.2025 09:50 AM CEST
-- Design Name: 
-- Module Name: SN74_TB - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: Testbench for SN74 component
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments: Tests all input combinations
-- 
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity sn74_tb is
end sn74_tb;

architecture behavior of sn74_tb is
    -- Component Declaration for the Unit Under Test (UUT)
    component sn74
        port (
            G1, G2A, G2B : in std_logic;
            C, B, A : in std_logic;
            Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7 : out std_logic
        );
    end component;

    -- Inputs
    signal G1, G2A, G2B : std_logic := '0';
    signal C, B, A : std_logic := '0';

    -- Outputs
    signal Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7 : std_logic;

    -- Clock (optional, if needed later)
    -- signal clk : std_logic := '0';
    -- constant CLK_PERIOD : time := 10 ns;

begin
    -- Instantiate the Unit Under Test (UUT)
    uut: sn74 port map (
        G1 => G1,
        G2A => G2A,
        G2B => G2B,
        C => C,
        B => B,
        A => A,
        Y0 => Y0,
        Y1 => Y1,
        Y2 => Y2,
        Y3 => Y3,
        Y4 => Y4,
        Y5 => Y5,
        Y6 => Y6,
        Y7 => Y7
    );

    -- Stimulus process
    stim_proc: process
    begin
        -- Test all combinations of G1, G2A, G2B (2^3 = 8 combinations)
        for g1_val in 0 to 1 loop
            G1 <= std_logic(to_unsigned(g1_val, 1)(0));
            for g2a_val in 0 to 1 loop
                G2A <= std_logic(to_unsigned(g2a_val, 1)(0));
                for g2b_val in 0 to 1 loop
                    G2B <= std_logic(to_unsigned(g2b_val, 1)(0));
                    -- Test all combinations of C, B, A (2^3 = 8 combinations)
                    for c_val in 0 to 1 loop
                        C <= std_logic(to_unsigned(c_val, 1)(0));
                        for b_val in 0 to 1 loop
                            B <= std_logic(to_unsigned(b_val, 1)(0));
                            for a_val in 0 to 1 loop
                                A <= std_logic(to_unsigned(a_val, 1)(0));
                                -- Wait to observe the outputs
                                wait for 10 ns;
                            end loop;
                        end loop;
                    end loop;
                end loop;
            end loop;
        end loop;

        -- End simulation
        wait;
    end process;

end behavior;