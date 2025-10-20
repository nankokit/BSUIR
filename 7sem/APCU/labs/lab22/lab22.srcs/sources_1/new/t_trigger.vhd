library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity t_trigger is
    port (
        t, c, p, pl : in std_logic;
        q, not_q : out std_logic
    );
end entity t_trigger;

architecture t_trigger of t_trigger is
    signal D_clk, D_d, notS, notR : std_logic;
    signal QQ : std_logic := '0';
    signal NQQ : std_logic := '1';
    component d_trigger is
        port (
            d, c, not_s, not_r : in std_logic;
            q, not_q : out std_logic
        );
    end component;
begin
    notS <= not (p and pl);
    notR <= not ((not p) and pl);
    D_clk <= c and t;
    D_d <= NQQ;

    D : d_trigger
    port map(
        d => D_d,
        c => D_clk,
        not_s => notS,
        not_r => notR,
        q => QQ,
        not_q => NQQ
    );

    q <= QQ;
    not_q <= NQQ;

end architecture t_trigger;