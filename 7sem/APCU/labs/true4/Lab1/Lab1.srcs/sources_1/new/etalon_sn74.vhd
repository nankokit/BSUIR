library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity SN74_etalon is
    Port ( G1  : in  STD_LOGIC;
           G2A : in  STD_LOGIC;
           G2B : in  STD_LOGIC;
           C   : in  STD_LOGIC;
           B   : in  STD_LOGIC;
           A   : in  STD_LOGIC;
           Y0  : out STD_LOGIC;
           Y1  : out STD_LOGIC;
           Y2  : out STD_LOGIC;
           Y3  : out STD_LOGIC;
           Y4  : out STD_LOGIC;
           Y5  : out STD_LOGIC;
           Y6  : out STD_LOGIC;
           Y7  : out STD_LOGIC);
end SN74_etalon;

architecture Behavioral of SN74_etalon is
    signal enable : STD_LOGIC;
    signal select_val : STD_LOGIC_VECTOR(2 downto 0);
begin
    -- Enable logic: G1 must be '1', and both G2A and G2B must be '0'
    enable <= G1 and (not G2A) and (not G2B);

    -- Combine select inputs into a vector
    select_val <= C & B & A;

    -- Output logic based on enable and select inputs
    Y0 <= not enable when select_val = "000" else '1';
    Y1 <= not enable when select_val = "001" else '1';
    Y2 <= not enable when select_val = "010" else '1';
    Y3 <= not enable when select_val = "011" else '1';
    Y4 <= not enable when select_val = "100" else '1';
    Y5 <= not enable when select_val = "101" else '1';
    Y6 <= not enable when select_val = "110" else '1';
    Y7 <= not enable when select_val = "111" else '1';

end Behavioral;