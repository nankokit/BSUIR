library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity decoder is
  Port (
    a, b, c: in std_logic;
    g1, g2a, g2b: in std_logic;
    y: out std_logic_vector(7 downto 0)
   );
end decoder;

architecture decoder of decoder is
begin
    process(a, b, c, g1, g2a, g2b)
        variable enable: std_logic;
        variable input_vector: std_logic_vector(2 downto 0);
    begin
        y <= "11111111";
        enable := g1 and (not g2a) and (not g2b);
        input_vector := (c & b & a);
        if enable='1' then
            case input_vector is
                when "000" => y(0) <= '0';
                when "001" => y(1) <= '0';
                when "010" => y(2) <= '0';
                when "011" => y(3) <= '0';
                when "100" => y(4) <= '0';
                when "101" => y(5) <= '0';
                when "110" => y(6) <= '0';
                when "111" => y(7) <= '0';
                when others => y <= "11111111";
            end case;    
        end if;
    end process;    
end decoder;
