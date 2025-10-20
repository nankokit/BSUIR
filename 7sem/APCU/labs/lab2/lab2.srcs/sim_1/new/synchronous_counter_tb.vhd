library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use IEEE.STD_LOGIC_TEXTIO.all;
use STD.TEXTIO.all;

entity synchronous_counter_tb is
end entity synchronous_counter_tb;

architecture behavior of synchronous_counter_tb is
  component counter
    port (
      clk : in std_logic;
      load : in std_logic;
      cten : in std_logic;
      du : in std_logic;
      data_in : in std_logic_vector(3 downto 0);
      q : out std_logic_vector(3 downto 0)
    );
  end component;

  signal clk_t : std_logic := '0';
  signal load_t : std_logic := '1';
  signal cten_t : std_logic := '1';
  signal du_t : std_logic := '0';
  signal data_in_t : std_logic_vector(3 downto 0) := (others => '0');
  signal q_t : std_logic_vector(3 downto 0);

  constant CLK_PERIOD : time := 10 ns;

begin
  -- Instantiate the DUT
  uut : counter
  port map(
    clk => clk_t,
    load => load_t,
    cten => cten_t,
    du => du_t,
    data_in => data_in_t,
    q => q_t
  );

  clk_process : process
  begin
    clk_t <= '0';
    wait for CLK_PERIOD / 2;
    clk_t <= '1';
    wait for CLK_PERIOD / 2;
  end process;

  -- Stimulus process
  test_process : process
    file test_file : text;
    variable line_buffer : line;

    variable v_inp : std_logic_vector(2 downto 0);
    variable v_data_in : std_logic_vector(3 downto 0);
    variable v_y : std_logic_vector(3 downto 0);

    variable test_count : integer := 0;
  begin
    file_open(test_file, "test_file.txt", read_mode);

    wait for CLK_PERIOD / 2;

    while not endfile(test_file) loop
      readline(test_file, line_buffer);

      read(line_buffer, v_inp(0));
      read(line_buffer, v_inp(1));
      read(line_buffer, v_inp(2));

      read(line_buffer, v_data_in);

      read(line_buffer, v_y);

      load_t <= v_inp(0);
      cten_t <= v_inp(1);
      du_t <= v_inp(2);
      data_in_t <= v_data_in;

      wait for CLK_PERIOD;

      test_count := test_count + 1;
      assert q_t = v_y
      report "Test " & integer'image(test_count) & " failed! Inputs: " &
        "load=" & std_logic'image(load_t) & " " &
        "cten=" & std_logic'image(cten_t) & " " &
        "du_t=" & std_logic'image(du_t) & " " &
        "data_in=" & std_logic'image(v_data_in(3)) & std_logic'image(v_data_in(2)) & std_logic'image(v_data_in(1)) & std_logic'image(v_data_in(0)) & " " &
        "q= " & std_logic'image(q_t(3)) & std_logic'image(q_t(2)) & std_logic'image(q_t(1)) & std_logic'image(q_t(0)) & " " &
        "out= " & std_logic'image(v_y(3)) & std_logic'image(v_y(2)) & std_logic'image(v_y(1)) & std_logic'image(v_y(0))
        severity failure;
    end loop;

    file_close(test_file);
    wait;
  end process;

end architecture behavior;