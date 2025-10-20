library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity SN74_etalon_tb is
end SN74_etalon_tb;

architecture Behavioral of SN74_etalon_tb is
    -- Component declaration
    component SN74_etalon
        port (
            G1, G2A, G2B : in std_logic;
            C, B, A : in std_logic;
            Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7 : out std_logic
        );
    end component;

    -- Test signals
    signal G1_tb  : STD_LOGIC := '0';
    signal G2A_tb : STD_LOGIC := '0';
    signal G2B_tb : STD_LOGIC := '0';
    signal C_tb   : STD_LOGIC := '0';
    signal B_tb   : STD_LOGIC := '0';
    signal A_tb   : STD_LOGIC := '0';
    signal Y0_tb  : STD_LOGIC;
    signal Y1_tb  : STD_LOGIC;
    signal Y2_tb  : STD_LOGIC;
    signal Y3_tb  : STD_LOGIC;
    signal Y4_tb  : STD_LOGIC;
    signal Y5_tb  : STD_LOGIC;
    signal Y6_tb  : STD_LOGIC;
    signal Y7_tb  : STD_LOGIC;

begin
    -- Instantiate the Unit Under Test (UUT)
    uut: SN74_etalon port map (
        G1  => G1_tb,
        G2A => G2A_tb,
        G2B => G2B_tb,
        C   => C_tb,
        B   => B_tb,
        A   => A_tb,
        Y0  => Y0_tb,
        Y1  => Y1_tb,
        Y2  => Y2_tb,
        Y3  => Y3_tb,
        Y4  => Y4_tb,
        Y5  => Y5_tb,
        Y6  => Y6_tb,
        Y7  => Y7_tb
    );

    -- Stimulus process
    stim_proc: process
    begin
        -- Initialize all inputs
        G1_tb <= '0';
        G2A_tb <= '0';
        G2B_tb <= '0';
        C_tb <= '0';
        B_tb <= '0';
        A_tb <= '0';
        wait for 10 ns;

        -- Nested loops to test all combinations
        for g1_val in 0 to 1 loop
            G1_tb <= std_logic(to_unsigned(g1_val, 1)(0));
            for g2a_val in 0 to 1 loop
                G2A_tb <= std_logic(to_unsigned(g2a_val, 1)(0));
                for g2b_val in 0 to 1 loop
                    G2B_tb <= std_logic(to_unsigned(g2b_val, 1)(0));
                    -- Test all combinations of C, B, A (2^3 = 8 combinations)
                    for c_val in 0 to 1 loop
                        C_tb <= std_logic(to_unsigned(c_val, 1)(0));
                        for b_val in 0 to 1 loop
                            B_tb <= std_logic(to_unsigned(b_val, 1)(0));
                            for a_val in 0 to 1 loop
                                A_tb <= std_logic(to_unsigned(a_val, 1)(0));
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

end Behavioral;