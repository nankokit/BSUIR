library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;

entity synchronous_counter is
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
end entity synchronous_counter;

architecture structural of synchronous_counter is
    signal SYN_T : std_logic_vector(3 downto 0);
    signal CL: std_logic;
    signal PL: std_logic;
    signal NQ : std_logic_vector(3 downto 0) := "1111";
    signal Q : std_logic_vector(3 downto 0) := "0000";

    component t_trigger is
        port (
            t, c, p, pl: in std_logic;
            q, not_q: out std_logic
        );
    end component;

begin
    CL <= not(not CLK and notLOAD);
    PL <= not notLOAD;


    SYN_T(0) <= not (not notCTEN
                    and
                    not(not D_notU and D_notU));
    Q_A : t_trigger
    port map(
        t => SYN_T(0),
        c => CL,
        p => DATA_IN(0),
        pl => PL,
        q => Q(0),
        not_q => NQ(0)
    );
    DATA_OUT(0) <= not NQ(0);

    SYN_T(1) <= not (not notCTEN
    and
    not(not(D_notU
    and
    ((not NQ(3) and not Q(0)) or (not NQ(2) and not Q(0)) or (NQ(1) and not Q(0))))
    and
    not(not D_notU
    and
    ((not NQ(0) and not Q(3)) or (not NQ(1) and not NQ(3))))));
    Q_B : t_trigger
    port map(
        t => SYN_T(1),
        c => CL,
        p => DATA_IN(1),
        pl => PL,
        q => Q(1),
        not_q => NQ(1)
    );
    DATA_OUT(1) <= not NQ(1);


    SYN_T(2) <= not notCTEN
    and
    not(not(D_notU
    and
    ((not Q(0) and not Q(1) and not NQ(2)) or (not Q(0) and not Q(1) and not NQ(3))))
    and
    (not D_notU
    and
    ((not NQ(0) and not NQ(1)) or (not NQ(2) and not NQ(3))))
    );
    Q_C : t_trigger
    port map(
        t => SYN_T(2),
        c => CL,
        p => DATA_IN(2),
        pl => PL,
        q => Q(2),
        not_q => NQ(2)
    );
    DATA_OUT(2) <= not NQ(2);


    SYN_T(3) <= not notCTEN
    and
    not(not(D_notU
    and
    ((not NQ(2) and not NQ(3)) or (not Q(0) and not Q(1) and not Q(2)))
    )
    and
    not(not D_notU
    and
    ((not NQ(1) and not NQ(3)) or (not NQ(0) and not Q(1) and not NQ(2)))
    )
    );
    Q_D : t_trigger
    port map(
        t => SYN_T(3),
        c => CL,
        p => DATA_IN(3),
        pl => PL,
        q => Q(3),
        not_q => NQ(3)
    );
    DATA_OUT(3) <= not NQ(3);

    MIN_MAX <= ((not(Q(0) or Q(1)) and not(Q(2) or Q(3)) and D_notU)
        and
        not(not D_notU and not NQ(3) and not NQ(0)));

    notRCO <= not ((not CLK and notLOAD)
        and
        ((not(Q(0) or Q(1)) and not(Q(2) or Q(3)) and D_notU)
        and
        not(not D_notU and not NQ(3) and not NQ(0)))
        and
        not notCTEN);


end architecture structural;
