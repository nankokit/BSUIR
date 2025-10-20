library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity board is
    port (
        ledsmain: std_logic_vector(3 downto 0);
        pushbutton: std_logic_vector(3 downto 0);
        dipswitch: std_logic_vector(3 downto 0);
        sysclk_p: std_logic;
        sysclk_n: std_logic
    );
end board;

architecture Behavioral of board is
    component divider is
        port (
            CLK_IN : in std_logic;
            CLK_OUT : out std_logic
        );
    end component;

    component counter is
        port (
            clk : in std_logic;
            du : in std_logic;
            load : in std_logic;
            cten : in std_logic;
            data_in : in std_logic_vector(3 downto 0);
            Q : out std_logic_vector(3 downto 0)
        );
    end component;

    component ibufds is
        port(
            i, ib: in std_logic;
            0: out std_logic
        );
    end component;

    signal clock, not_clock : std_logic;
    data_in : std_logic_vector(3 downto 0);
    Q : std_logic_vector(3 downto 0);
    clk_not_div : std_logic;
begin

    ibufds_inst: ibufds port map(
        i => sysclk_p, 
        ib => sysclk_n,
        o => clk_not_div
    )

    count : counter port map(
        du => pushbuttons(1),
        load => pushbuttons(2),
        cten => pushbuttons(3),
        data_in => dipswitch,
        Q => Q
    );
    
    Q <= ledsmain;

    div : divider port map(
        CLK_IN => clk_not_div,
        CLK_OUT => clock
    );

end Behavioral;