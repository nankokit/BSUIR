library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_TEXTIO.ALL;
use STD.TEXTIO.ALL;

entity file_sn74_tb is
end entity file_sn74_tb;

-- функция преобразования std_logic_vector -> string


architecture testbench of file_sn74_tb is
    -- Объявление компонента тестируемого устройства
    component sn74 
        port (
            G1, G2A, G2B : in std_logic;
            C, B, A : in std_logic;
            Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7 : out std_logic
        );
    end component;

    -- Входные сигналы
    signal G1_tb, G2A_tb, G2B_tb : std_logic;
    signal C_tb, B_tb, A_tb : std_logic;
    
    -- Выходные сигналы
    signal Y0_tb, Y1_tb, Y2_tb, Y3_tb : std_logic;
    signal Y4_tb, Y5_tb, Y6_tb, Y7_tb : std_logic;

begin
    -- Instantiate the Unit Under Test (UUT)
    uut: sn74 PORT MAP (
        G1 => G1_tb, 
        G2A => G2A_tb, 
        G2B => G2B_tb,
        C => C_tb, 
        B => B_tb, 
        A => A_tb,
        Y0 => Y0_tb, 
        Y1 => Y1_tb, 
        Y2 => Y2_tb, 
        Y3 => Y3_tb,
        Y4 => Y4_tb, 
        Y5 => Y5_tb, 
        Y6 => Y6_tb, 
        Y7 => Y7_tb
    );

    -- Тестовый процесс
    test_process: process
        file test_file: text;
        variable line_buffer: line;
        
        -- Входные переменные
        variable v_G1, v_G2A, v_G2B : std_logic;
        variable v_C, v_B, v_A : std_logic;
        
        -- Ожидаемые выходные переменные
        variable v_Y0, v_Y1, v_Y2, v_Y3 : std_logic;
        variable v_Y4, v_Y5, v_Y6, v_Y7 : std_logic;
        
        -- Счетчик тестов
        variable test_count : integer := 0;
        variable error_count : integer := 0;
    begin
        -- Открываем файл с тестами
        file_open(test_file, "tests.txt", read_mode);
        
        while not endfile(test_file) loop
            -- Читаем строку из файла
            readline(test_file, line_buffer);
            
            -- Пропускаем комментарии и пустые строки
            next when line_buffer'length = 0 or line_buffer(1) = '#';
            
            -- Считываем входные сигналы
            read(line_buffer, v_G1);
            read(line_buffer, v_G2A);
            read(line_buffer, v_G2B);
            read(line_buffer, v_C);
            read(line_buffer, v_B);
            read(line_buffer, v_A);
            
            -- Считываем ожидаемые выходные сигналы
            read(line_buffer, v_Y0);
            read(line_buffer, v_Y1);
            read(line_buffer, v_Y2);
            read(line_buffer, v_Y3);
            read(line_buffer, v_Y4);
            read(line_buffer, v_Y5);
            read(line_buffer, v_Y6);
            read(line_buffer, v_Y7);
            
            -- Устанавливаем входные сигналы
            G1_tb <= v_G1;
            G2A_tb <= v_G2A;
            G2B_tb <= v_G2B;
            C_tb <= v_C;
            B_tb <= v_B;
            A_tb <= v_A;
            
            -- Ждем установления сигналов
            wait for 10 ns;
            
            -- Проверяем выходные сигналы
            test_count := test_count + 1;
            
            assert (Y0_tb = v_Y0 and Y1_tb = v_Y1 and Y2_tb = v_Y2 and Y3_tb = v_Y3 and
                    Y4_tb = v_Y4 and Y5_tb = v_Y5 and Y6_tb = v_Y6 and Y7_tb = v_Y7)
            report "Test " & integer'image(test_count) & " failed!" & 
                   " Inputs: G1=" & std_logic'image(v_G1) & 
                   " G2A=" & std_logic'image(v_G2A) & 
                   " G2B=" & std_logic'image(v_G2B) & 
                   " C=" & std_logic'image(v_C) & 
                   " B=" & std_logic'image(v_B) & 
                   " A=" & std_logic'image(v_A)
            severity failure;
        end loop;
        
        -- Закрываем файл
        file_close(test_file);
        
        -- Выводим результаты
        report "Total tests: " & integer'image(test_count);
        
        -- Завершаем симуляцию
        wait;
    end process;
end architecture testbench;