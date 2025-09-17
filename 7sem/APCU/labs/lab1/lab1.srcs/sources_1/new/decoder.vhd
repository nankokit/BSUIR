library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity decoder is
  Port (
    a, b, c: in std_logic;
    g1, not_g2a, not_g2b: in std_logic;
    y: out std_logic_vector(7 downto 0)
   );
end decoder;

architecture decoder of decoder is
    signal enable: std_logic;
begin
    enable <= g1 and (not not_g2a) and (not not_g2b);
    
    y(0) <= not ((not a) and (not b) and (not c) and enable);
    y(1) <= not (     a  and (not b) and (not c) and enable);
    y(2) <= not ((not a) and      b  and (not c) and enable);
    y(3) <= not (     a  and      b  and (not c) and enable);
    y(4) <= not ((not a) and (not b) and      c  and enable);
    y(5) <= not (     a  and (not b) and      c  and enable);
    y(6) <= not ((not a) and      b  and      c  and enable);
    y(7) <= not (     a  and      b  and      c  and enable);

end decoder;
