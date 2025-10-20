library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;

entity synchronous_counter_tb is
end entity synchronous_counter_tb;

architecture behavioral of synchronous_counter_tb is
    -- Component declaration for the DUT (Device Under Test)
    component synchronous_counter is
        port (
            CLK : in std_logic;
            D_notU : in std_logic;
            notLOAD : in std_logic;
            notCTEN : in std_logic;
            DATA_IN : in std_logic_vector(3 downto 0);
            DATA_OUT : out std_logic_vector(3 downto 0);
            notRCO : out std_logic;
            MIN_MAX : out std_logic
        );
    end component;

    -- Signal declarations
    signal CLK : std_logic := '0';
    signal D_notU : std_logic := '0';
    signal notLOAD : std_logic := '1';
    signal notCTEN : std_logic := '1';
    signal DATA_IN : std_logic_vector(3 downto 0) := "0000";
    signal DATA_OUT : std_logic_vector(3 downto 0);
    signal notRCO : std_logic;
    signal MIN_MAX : std_logic;

    -- Clock period definition
    constant CLK_PERIOD : time := 10 ns;

begin
    -- Instantiate the DUT
    uut: synchronous_counter
    port map (
        CLK => CLK,
        D_notU => D_notU,
        notLOAD => notLOAD,
        notCTEN => notCTEN,
        DATA_IN => DATA_IN,
        DATA_OUT => DATA_OUT,
        notRCO => notRCO,
        MIN_MAX => MIN_MAX
    );

    -- Clock process
    clk_process: process
    begin
        while True loop
            CLK <= '0';
            wait for CLK_PERIOD / 2;
            CLK <= '1';
            wait for CLK_PERIOD / 2;
        end loop;
    end process;

    -- Stimulus process
    stim_proc: process
    begin
        -- Test case 1: Load initial value (e.g., "1010")
        wait for CLK_PERIOD * 2;
        notLOAD <= '0'; -- Enable load
        DATA_IN <= "1010";
        wait for CLK_PERIOD;
        notLOAD <= '1'; -- Disable load
        wait for CLK_PERIOD;

        -- Test case 2: Up counting (D_notU = '1', notCTEN = '0')
        D_notU <= '1';
        notCTEN <= '0'; -- Enable counting
        wait for CLK_PERIOD * 10; -- Count up for 10 cycles

        -- Test case 3: Disable counter (notCTEN = '1')
        notCTEN <= '1'; -- Disable counting
        wait for CLK_PERIOD * 5; -- Hold for 5 cycles

        -- Test case 4: Down counting (D_notU = '0', notCTEN = '0')
        D_notU <= '0';
        notCTEN <= '0'; -- Enable counting
        wait for CLK_PERIOD * 10; -- Count down for 10 cycles

        -- Test case 5: Load another value (e.g., "0000") and check MIN_MAX
        notLOAD <= '0';
        DATA_IN <= "0000";
        wait for CLK_PERIOD;
        notLOAD <= '1';
        D_notU <= '0';
        notCTEN <= '0';
        wait for CLK_PERIOD * 5; -- Check MIN_MAX behavior

        -- Test case 6: Load max value (e.g., "1111") and check notRCO
        notLOAD <= '0';
        DATA_IN <= "1111";
        wait for CLK_PERIOD;
        notLOAD <= '1';
        D_notU <= '1';
        notCTEN <= '0';
        wait for CLK_PERIOD * 5; -- Check notRCO behavior

        -- End simulation
        wait;
    end process;

end architecture behavioral;