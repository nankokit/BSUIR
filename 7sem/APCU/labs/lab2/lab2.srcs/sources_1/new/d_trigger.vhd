library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity d_trigger is
    port (
        d, c, not_s, not_r : in std_logic;
        q, not_q : out std_logic
    );
end entity d_trigger;

architecture d_trigger of d_trigger is
    signal state : std_logic := '0';
begin
    process (c, not_s, not_r)
    begin
        if not_r = '0' then
            state <= '0';
        elsif not_s = '0' then
            state <= '1';
        elsif rising_edge(c) and c = '1' then
            state <= d;
        end if;
    end process;

    q <= state;
    not_q <= not state;
end architecture d_trigger;