library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity SN74_Combined_tb is
end SN74_Combined_tb;

architecture Behavioral of SN74_Combined_tb is
    -- Component declaration for UUT (SN74)
    component SN74
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
    end component;

    -- Component declaration for Etalon (SN74_etalon)
    component SN74_etalon
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
    end component;

    -- Test signals
    signal G1_tb  : STD_LOGIC := '0';
    signal G2A_tb : STD_LOGIC := '0';
    signal G2B_tb : STD_LOGIC := '0';
    signal C_tb   : STD_LOGIC := '0';
    signal B_tb   : STD_LOGIC := '0';
    signal A_tb   : STD_LOGIC := '0';

    -- Outputs from UUT
    signal Y0_uut  : STD_LOGIC;
    signal Y1_uut  : STD_LOGIC;
    signal Y2_uut  : STD_LOGIC;
    signal Y3_uut  : STD_LOGIC;
    signal Y4_uut  : STD_LOGIC;
    signal Y5_uut  : STD_LOGIC;
    signal Y6_uut  : STD_LOGIC;
    signal Y7_uut  : STD_LOGIC;

    -- Outputs from Etalon
    signal Y0_etalon  : STD_LOGIC;
    signal Y1_etalon  : STD_LOGIC;
    signal Y2_etalon  : STD_LOGIC;
    signal Y3_etalon  : STD_LOGIC;
    signal Y4_etalon  : STD_LOGIC;
    signal Y5_etalon  : STD_LOGIC;
    signal Y6_etalon  : STD_LOGIC;
    signal Y7_etalon  : STD_LOGIC;

begin
    -- Instantiate the Unit Under Test (UUT)
    uut: SN74 port map (
        G1  => G1_tb,
        G2A => G2A_tb,
        G2B => G2B_tb,
        C   => C_tb,
        B   => B_tb,
        A   => A_tb,
        Y0  => Y0_uut,
        Y1  => Y1_uut,
        Y2  => Y2_uut,
        Y3  => Y3_uut,
        Y4  => Y4_uut,
        Y5  => Y5_uut,
        Y6  => Y6_uut,
        Y7  => Y7_uut
    );

    -- Instantiate the Etalon
    etalon: SN74_etalon port map (
        G1  => G1_tb,
        G2A => G2A_tb,
        G2B => G2B_tb,
        C   => C_tb,
        B   => B_tb,
        A   => A_tb,
        Y0  => Y0_etalon,
        Y1  => Y1_etalon,
        Y2  => Y2_etalon,
        Y3  => Y3_etalon,
        Y4  => Y4_etalon,
        Y5  => Y5_etalon,
        Y6  => Y6_etalon,
        Y7  => Y7_etalon
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
                    for c_val in 0 to 1 loop
                        C_tb <= std_logic(to_unsigned(c_val, 1)(0));
                        for b_val in 0 to 1 loop
                            B_tb <= std_logic(to_unsigned(b_val, 1)(0));
                            for a_val in 0 to 1 loop
                                A_tb <= std_logic(to_unsigned(a_val, 1)(0));
                                -- Wait to observe the outputs
                                wait for 10 ns;

                                -- Compare outputs
                                assert (Y0_uut = Y0_etalon) report "Mismatch on Y0 for inputs G1=" & std_logic'image(G1_tb) & ", G2A=" & std_logic'image(G2A_tb) & ", G2B=" & std_logic'image(G2B_tb) & ", C=" & std_logic'image(C_tb) & ", B=" & std_logic'image(B_tb) & ", A=" & std_logic'image(A_tb) severity error;
                                assert (Y1_uut = Y1_etalon) report "Mismatch on Y1 for inputs G1=" & std_logic'image(G1_tb) & ", G2A=" & std_logic'image(G2A_tb) & ", G2B=" & std_logic'image(G2B_tb) & ", C=" & std_logic'image(C_tb) & ", B=" & std_logic'image(B_tb) & ", A=" & std_logic'image(A_tb) severity error;
                                assert (Y2_uut = Y2_etalon) report "Mismatch on Y2 for inputs G1=" & std_logic'image(G1_tb) & ", G2A=" & std_logic'image(G2A_tb) & ", G2B=" & std_logic'image(G2B_tb) & ", C=" & std_logic'image(C_tb) & ", B=" & std_logic'image(B_tb) & ", A=" & std_logic'image(A_tb) severity error;
                                assert (Y3_uut = Y3_etalon) report "Mismatch on Y3 for inputs G1=" & std_logic'image(G1_tb) & ", G2A=" & std_logic'image(G2A_tb) & ", G2B=" & std_logic'image(G2B_tb) & ", C=" & std_logic'image(C_tb) & ", B=" & std_logic'image(B_tb) & ", A=" & std_logic'image(A_tb) severity error;
                                assert (Y4_uut = Y4_etalon) report "Mismatch on Y4 for inputs G1=" & std_logic'image(G1_tb) & ", G2A=" & std_logic'image(G2A_tb) & ", G2B=" & std_logic'image(G2B_tb) & ", C=" & std_logic'image(C_tb) & ", B=" & std_logic'image(B_tb) & ", A=" & std_logic'image(A_tb) severity error;
                                assert (Y5_uut = Y5_etalon) report "Mismatch on Y5 for inputs G1=" & std_logic'image(G1_tb) & ", G2A=" & std_logic'image(G2A_tb) & ", G2B=" & std_logic'image(G2B_tb) & ", C=" & std_logic'image(C_tb) & ", B=" & std_logic'image(B_tb) & ", A=" & std_logic'image(A_tb) severity error;
                                assert (Y6_uut = Y6_etalon) report "Mismatch on Y6 for inputs G1=" & std_logic'image(G1_tb) & ", G2A=" & std_logic'image(G2A_tb) & ", G2B=" & std_logic'image(G2B_tb) & ", C=" & std_logic'image(C_tb) & ", B=" & std_logic'image(B_tb) & ", A=" & std_logic'image(A_tb) severity error;
                                assert (Y7_uut = Y7_etalon) report "Mismatch on Y7 for inputs G1=" & std_logic'image(G1_tb) & ", G2A=" & std_logic'image(G2A_tb) & ", G2B=" & std_logic'image(G2B_tb) & ", C=" & std_logic'image(C_tb) & ", B=" & std_logic'image(B_tb) & ", A=" & std_logic'image(A_tb) severity error;
                            end loop;
                        end loop;
                    end loop;
                end loop;
            end loop;
        end loop;

        -- Report completion
        report "Test completed." severity note;

        -- End simulation
        wait;
    end process;

end Behavioral;