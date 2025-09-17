library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity decoder_tb is
end decoder_tb;

architecture behavior of decoder_tb is
    signal a, b, c, g1, not_g2a, not_g2b : std_logic;
    signal y : std_logic_vector(7 downto 0);
    
    component decoder
        Port (
            a, b, c : in std_logic;
            g1, not_g2a, not_g2b : in std_logic;
            y : out std_logic_vector(7 downto 0)
        );
    end component;

begin
    uut: decoder
        port map (
            a => a,
            b => b,
            c => c,
            g1 => g1,
            not_g2a => not_g2a,
            not_g2b => not_g2b,
            y => y
        );
    
    stim_proc: process
    begin
        -- y = 11111111
        a <= '0'; b <= '0'; c <= '0';
        g1 <= '0'; not_g2a <= '0'; not_g2b <= '0';
        wait for 10 ns;
    
        -- y = 11111111
        g1 <= '0'; not_g2a <= '0'; not_g2b <= '0';
        a <= '1'; b <= '1'; c <= '1';
        wait for 10 ns;
    
        -- y = 11111111
        g1 <= '1'; not_g2a <= '1'; not_g2b <= '0';
        a <= '1'; b <= '1'; c <= '1';
        wait for 10 ns;
    
        -- y = 11111111
        g1 <= '1'; not_g2a <= '0'; not_g2b <= '1';
        a <= '1'; b <= '1'; c <= '1';
        wait for 10 ns;
    
        g1 <= '1'; not_g2a <= '0'; not_g2b <= '0';
        
        c <= '0'; b <= '0'; a <= '0'; wait for 10 ns; -- y(0) = '0'
        c <= '0'; b <= '0'; a <= '1'; wait for 10 ns; -- y(1) = '0'
        c <= '0'; b <= '1'; a <= '0'; wait for 10 ns; -- y(2) = '0'
        c <= '0'; b <= '1'; a <= '1'; wait for 10 ns; -- y(3) = '0'
        c <= '1'; b <= '0'; a <= '0'; wait for 10 ns; -- y(4) = '0'
        c <= '1'; b <= '0'; a <= '1'; wait for 10 ns; -- y(5) = '0'
        c <= '1'; b <= '1'; a <= '0'; wait for 10 ns; -- y(6) = '0'
        c <= '1'; b <= '1'; a <= '1'; wait for 10 ns; -- y(7) = '0'
    
        wait;
    end process;

end behavior;