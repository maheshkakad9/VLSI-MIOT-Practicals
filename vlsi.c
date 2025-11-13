1. 4 - Bit Alu

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity alu_4bit is
    Port (
        A, B  : in  STD_LOGIC_VECTOR (3 downto 0);
        SEL   : in  STD_LOGIC_VECTOR (2 downto 0);
        Y     : out STD_LOGIC_VECTOR (3 downto 0);
        CARRY : out STD_LOGIC
    );
end alu_4bit;

architecture Behavioral of alu_4bit is
    signal temp : unsigned(4 downto 0);
begin
    process (A, B, SEL)
    begin
        case SEL is
            when "000" =>  -- Addition
                temp <= ('0' & unsigned(A)) + ('0' & unsigned(B));
                Y <= std_logic_vector(temp(3 downto 0));
                CARRY <= temp(4);

            when "001" =>  -- Subtraction
                temp <= ('0' & unsigned(A)) - ('0' & unsigned(B));
                Y <= std_logic_vector(temp(3 downto 0));
                CARRY <= temp(4);

            when "010" =>  -- AND
                Y <= A AND B;
                CARRY <= '0';

            when "011" =>  -- NAND
                Y <= NOT (A AND B);
                CARRY <= '0';

            when "100" =>  -- XOR
                Y <= A XOR B;
                CARRY <= '0';

            when "101" =>  -- XNOR
                Y <= NOT (A XOR B);
                CARRY <= '0';

            when "110" =>  -- OR
                Y <= A OR B;
                CARRY <= '0';

            when others =>  -- Default
                Y <= (others => '0');
                CARRY <= '0';
        end case;
    end process;
end Behavioral;


TestBench : 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity alu_4bit_tb is
end alu_4bit_tb;

architecture Behavioral of alu_4bit_tb is
    component alu_4bit
        Port (
            A, B  : in  STD_LOGIC_VECTOR (3 downto 0);
            SEL   : in  STD_LOGIC_VECTOR (2 downto 0);
            Y     : out STD_LOGIC_VECTOR (3 downto 0);
            CARRY : out STD_LOGIC
        );
    end component;

    signal A, B  : STD_LOGIC_VECTOR (3 downto 0) := (others => '0');
    signal SEL   : STD_LOGIC_VECTOR (2 downto 0) := (others => '0');
    signal Y     : STD_LOGIC_VECTOR (3 downto 0);
    signal CARRY : STD_LOGIC;
begin
    uut: alu_4bit port map (
        A => A, B => B, SEL => SEL,
        Y => Y, CARRY => CARRY
    );

    stim_proc: process
    begin
        A <= "0101"; B <= "0011"; SEL <= "000"; wait for 50 ns; -- Add
        A <= "0101"; B <= "0011"; SEL <= "001"; wait for 50 ns; -- Sub
        A <= "0101"; B <= "0011"; SEL <= "010"; wait for 50 ns; -- AND
        A <= "0101"; B <= "0011"; SEL <= "011"; wait for 50 ns; -- NAND
        A <= "0101"; B <= "0011"; SEL <= "100"; wait for 50 ns; -- XOR
        A <= "0101"; B <= "0011"; SEL <= "101"; wait for 50 ns; -- XNOR
        A <= "0101"; B <= "0011"; SEL <= "110"; wait for 50 ns; -- OR
        A <= "1111"; B <= "0001"; SEL <= "000"; wait for 50 ns; -- Overflow test
        wait;
    end process;
end Behavioral;



2. Shift Register 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Shift_Reg_4bit is
    Port (
        CLK  : in  STD_LOGIC;
        RST  : in  STD_LOGIC;
        IL   : in  STD_LOGIC;                   -- Input for left shift
        IR   : in  STD_LOGIC;                   -- Input for right shift
        MODE : in  STD_LOGIC_VECTOR(1 downto 0);-- 00: Load, 01: Right, 10: Left, 11: Hold
        I    : in  STD_LOGIC_VECTOR(3 downto 0);
        Q    : out STD_LOGIC_VECTOR(3 downto 0)
    );
end Shift_Reg_4bit;

architecture Behavioral of Shift_Reg_4bit is
    signal temp : STD_LOGIC_VECTOR(3 downto 0) := (others => '0');
begin
    process(CLK, RST)
    begin
        if RST = '1' then
            temp <= (others => '0');
        elsif rising_edge(CLK) then
            case MODE is
                when "00" =>  -- Parallel Load
                    temp <= I;
                when "01" =>  -- Shift Right
                    temp <= IR & temp(3 downto 1);
                when "10" =>  -- Shift Left
                    temp <= temp(2 downto 0) & IL;
                when others =>  -- Hold
                    temp <= temp;
            end case;
        end if;
    end process;
    Q <= temp;
end Behavioral;


Testbench : 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Shift_Reg_4bit_tb is
end Shift_Reg_4bit_tb;

architecture Behavioral of Shift_Reg_4bit_tb is
    component Shift_Reg_4bit
        Port (
            CLK, RST, IL, IR : in  STD_LOGIC;
            MODE : in  STD_LOGIC_VECTOR(1 downto 0);
            I : in  STD_LOGIC_VECTOR(3 downto 0);
            Q : out STD_LOGIC_VECTOR(3 downto 0)
        );
    end component;

    signal CLK, RST, IL, IR : STD_LOGIC := '0';
    signal MODE : STD_LOGIC_VECTOR(1 downto 0) := "00";
    signal I : STD_LOGIC_VECTOR(3 downto 0) := (others => '0');
    signal Q : STD_LOGIC_VECTOR(3 downto 0);
begin
    uut: Shift_Reg_4bit port map (
        CLK => CLK, RST => RST, IL => IL, IR => IR,
        MODE => MODE, I => I, Q => Q
    );

    -- Clock generation
    CLK <= not CLK after 10 ns;

    -- Stimulus process
    stim_proc: process
    begin
        -- Reset
        RST <= '1'; wait for 20 ns;
        RST <= '0'; wait for 20 ns;

        -- Parallel Load
        I <= "1010"; MODE <= "00"; wait for 40 ns;

        -- Shift Right
        IR <= '1'; MODE <= "01"; wait for 80 ns;

        -- Shift Left
        IL <= '0'; MODE <= "10"; wait for 80 ns;

        -- Hold
        MODE <= "11"; wait for 80 ns;

        wait;
    end process;
end Behavioral;



3. Counter 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity counter is
    Port (
        clk : in STD_LOGIC;
        clr : in STD_LOGIC;
        q   : out STD_LOGIC_VECTOR(3 downto 0)
    );
end counter;

architecture rtl of counter is
    signal temp : unsigned(3 downto 0) := (others => '0');
begin
    process(clk, clr)
    begin
        if clr = '1' then
            temp <= (others => '0');
        elsif rising_edge(clk) then
            temp <= temp + 1;
        end if;
    end process;
    q <= std_logic_vector(temp);
end rtl;


testbench: 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity counter_tb is
end counter_tb;

architecture test of counter_tb is
    component counter
        Port (
            clk : in STD_LOGIC;
            clr : in STD_LOGIC;
            q   : out STD_LOGIC_VECTOR(3 downto 0)
        );
    end component;

    signal clk : STD_LOGIC := '0';
    signal clr : STD_LOGIC := '0';
    signal q   : STD_LOGIC_VECTOR(3 downto 0);
begin
    uut: counter port map(clk => clk, clr => clr, q => q);

    -- Clock generation (20 ns period)
    clk <= not clk after 10 ns;

    stim_proc: process
    begin
        clr <= '1'; wait for 20 ns;  -- Reset
        clr <= '0'; wait for 160 ns; -- Count up
        clr <= '1'; wait for 20 ns;  -- Reset again
        clr <= '0'; wait for 100 ns; -- Continue counting
        wait;
    end process;
end test;


4. FIFO 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity fifo is
    Port (
        clk      : in  STD_LOGIC;
        enr      : in  STD_LOGIC;                       -- Enable Read
        enw      : in  STD_LOGIC;                       -- Enable Write
        datain   : in  STD_LOGIC_VECTOR(3 downto 0);
        dataout  : out STD_LOGIC_VECTOR(3 downto 0);
        empty    : out STD_LOGIC;
        full     : out STD_LOGIC
    );
end fifo;

architecture rtl of fifo is
    type memory_type is array (0 to 3) of STD_LOGIC_VECTOR(3 downto 0);
    signal mem : memory_type := (others => (others => '0'));
    signal readptr  : unsigned(1 downto 0) := "00";
    signal writeptr : unsigned(1 downto 0) := "00";
    signal count    : integer range 0 to 4 := 0;  -- To track FIFO occupancy
begin
    process(clk)
    begin
        if rising_edge(clk) then
            -- Write operation
            if (enw = '1' and full = '0') then
                mem(to_integer(writeptr)) <= datain;
                writeptr <= writeptr + 1;
                count <= count + 1;
            end if;

            -- Read operation
            if (enr = '1' and empty = '0') then
                dataout <= mem(to_integer(readptr));
                readptr <= readptr + 1;
                count <= count - 1;
            end if;
        end if;
    end process;

    -- Empty / Full Flags
    empty <= '1' when count = 0 else '0';
    full  <= '1' when count = 4 else '0';
end rtl;


TestBench: 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity fifo_tb is
end fifo_tb;

architecture test of fifo_tb is
    component fifo
        Port (
            clk      : in  STD_LOGIC;
            enr      : in  STD_LOGIC;
            enw      : in  STD_LOGIC;
            datain   : in  STD_LOGIC_VECTOR(3 downto 0);
            dataout  : out STD_LOGIC_VECTOR(3 downto 0);
            empty    : out STD_LOGIC;
            full     : out STD_LOGIC
        );
    end component;

    signal clk     : STD_LOGIC := '0';
    signal enr     : STD_LOGIC := '0';
    signal enw     : STD_LOGIC := '0';
    signal datain  : STD_LOGIC_VECTOR(3 downto 0) := (others => '0');
    signal dataout : STD_LOGIC_VECTOR(3 downto 0);
    signal empty   : STD_LOGIC;
    signal full    : STD_LOGIC;
begin
    uut: fifo port map (
        clk => clk,
        enr => enr,
        enw => enw,
        datain => datain,
        dataout => dataout,
        empty => empty,
        full => full
    );

    clk <= not clk after 10 ns;

    stim_proc: process
    begin
        -- Write 4 data elements
        enw <= '1'; enr <= '0';
        datain <= "0001"; wait for 20 ns;
        datain <= "0010"; wait for 20 ns;
        datain <= "0011"; wait for 20 ns;
        datain <= "0100"; wait for 20 ns;

        -- Stop writing
        enw <= '0'; wait for 20 ns;

        -- Read them out
        enr <= '1'; wait for 100 ns;

        -- Try write and read simultaneously
        enw <= '1'; datain <= "1111"; wait for 40 ns;

        wait;
    end process;
end test;

