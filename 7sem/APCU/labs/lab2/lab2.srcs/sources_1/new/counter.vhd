library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;

entity counter is
    port (
        clk : in std_logic;
        du : in std_logic;
        load : in std_logic;
        cten : in std_logic;
        data_in : in std_logic_vector(3 downto 0);
        Q : out std_logic_vector(3 downto 0)

    );
end counter;

architecture counter of counter is
        component divider is
            port (
                CLK_IN : in std_logic;
                CLK_OUT : out std_logic
            );
        end component;

    signal count : unsigned(3 downto 0);
begin    
    process (clk, load)
    begin
        if load = '0' then
            count <= unsigned(data_in);
        elsif rising_edge(clk) then
            if cten = '0' then
                if du = '0' then
                    count <= count + 1;
                else
                    count <= count - 1;
                end if;
            end if;
        end if;
    end process;
    q <= std_logic_vector(count);
    
    top: 
end counter;