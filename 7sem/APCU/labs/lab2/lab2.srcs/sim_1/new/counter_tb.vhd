
library IEEE;
use IEEE.STD_LOGIC_1164.all;


entity counter_tb is
end counter_tb;

architecture counter_tb of counter_tb is
    component counter is
        port (
            clk : in std_logic;
            du : in std_logic;
            load : in std_logic;
            cten : in std_logic;
            dcba : in std_logic_vector(3 downto 0);
            RCO : out std_logic;
            max_min : out std_logic;
            Q : out std_logic_vector(3 downto 0);
            C_c : out std_logic_vector (3 downto 0);
            NQ_nq : out std_logic_vector (3 downto 0)
        );
    end component;
    signal CTEN : std_logic := '1';
    signal DU : std_logic := '0';
    signal clk : std_logic := '0';
    signal load : std_logic := '1';
    signal dcba : std_logic_vector(3 downto 0);
    signal max_min : std_logic;
    signal RCO : std_logic;
    signal Q : std_logic_vector(3 downto 0);
    signal C_c : std_logic_vector (3 downto 0);
    signal NQ_nq :  std_logic_vector (3 downto 0);
begin
    uut : counter
    port map(
        cten => CTEN,
        du => DU,
        clk => clk,
        load => load,
        dcba => dcba,
        max_min => max_min,
        RCO => RCO,
        Q => Q,
        C_c => C_c,
        NQ_nq => NQ_nq
    );

    clk_proc : process
    begin
        while true loop
            clk <= '0';
            wait for 10 ns;
            clk <= '1';
            wait for 10 ns;
        end loop;
    end process;

    stim_proc : process
    begin
        DU <= '0';
        CTEN <= '0';
        wait for 100 ns;
        CTEN <= '1';
        wait for 20 ns;
        CTEN <= '0';
        wait for 220 ns;
        CTEN <= '1';
        dcba <= "0100";
        DU <= '1';
        load <= '0';
        wait for 10 ns;
        load <= '1';
        CTEN <= '0';
        wait for 100 ns;
        CTEN <= '1';
        wait;
    end process;
end counter_tb;