1) 4-Bit ALU (entity + architecture) 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity alu_4bit is
  Port (
    A     : in  STD_LOGIC_VECTOR (3 downto 0);
    B     : in  STD_LOGIC_VECTOR (3 downto 0);
    SEL   : in  STD_LOGIC_VECTOR (2 downto 0);
    Y     : out STD_LOGIC_VECTOR (3 downto 0);
    CARRY : out STD_LOGIC
  );
end alu_4bit;

architecture Behavioral of alu_4bit is
  signal temp : STD_LOGIC_VECTOR(4 downto 0);
begin
  process(A, B, SEL)
  begin
    case SEL is
      when "000" =>
        temp <= ('0' & A) + ('0' & B);
        Y    <= temp(3 downto 0);
        CARRY<= temp(4);
      when "001" =>
        temp <= ('0' & A) - ('0' & B);
        Y    <= temp(3 downto 0);
        CARRY<= temp(4);
      when "010" =>
        Y <= A AND B;
        CARRY <= '0';
      when "011" =>
        Y <= A NAND B;
        CARRY <= '0';
      when "100" =>
        Y <= A XOR B;
        CARRY <= '0';
      when "101" =>
        Y <= A XNOR B;
        CARRY <= '0';
      when "110" =>
        Y <= A OR B;
        CARRY <= '0';
      when others =>
        Y <= "0000";
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
      A     : in  STD_LOGIC_VECTOR(3 downto 0);
      B     : in  STD_LOGIC_VECTOR(3 downto 0);
      SEL   : in  STD_LOGIC_VECTOR(2 downto 0);
      Y     : out STD_LOGIC_VECTOR(3 downto 0);
      CARRY : out STD_LOGIC
    );
  end component;

  signal A, B   : STD_LOGIC_VECTOR(3 downto 0);
  signal SEL    : STD_LOGIC_VECTOR(2 downto 0);
  signal Y      : STD_LOGIC_VECTOR(3 downto 0);
  signal CARRY  : STD_LOGIC;
begin
  uut: alu_4bit port map (
    A => A,
    B => B,
    SEL => SEL,
    Y => Y,
    CARRY => CARRY
  );

  stim_proc: process
  begin
    A <= "0101"; B <= "0011"; SEL <= "000"; wait for 100 ns;
    A <= "0101"; B <= "0011"; SEL <= "001"; wait for 100 ns;
    A <= "0101"; B <= "0011"; SEL <= "010"; wait for 100 ns;
    A <= "0101"; B <= "0011"; SEL <= "011"; wait for 100 ns;
    A <= "0101"; B <= "0011"; SEL <= "100"; wait for 100 ns;
    A <= "0101"; B <= "0011"; SEL <= "101"; wait for 100 ns;
    A <= "0101"; B <= "0011"; SEL <= "110"; wait for 100 ns;
    wait;
  end process;
end Behavioral;


2. 4-Bit Shift Register (entity + architecture)

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity shift_reg_4bit is
  Port (
    CLK  : in  STD_LOGIC;
    RST  : in  STD_LOGIC;
    IL   : in  STD_LOGIC;
    IR   : in  STD_LOGIC;
    MODE : in  STD_LOGIC_VECTOR(1 downto 0);
    I    : in  STD_LOGIC_VECTOR(3 downto 0);
    Q    : out STD_LOGIC_VECTOR(3 downto 0)
  );
end shift_reg_4bit;

architecture Behavioral of shift_reg_4bit is
  signal temp : STD_LOGIC_VECTOR(3 downto 0) := (others => '0');
begin
  process(CLK, RST)
  begin
    if RST = '1' then
      temp <= (others => '0');
    elsif rising_edge(CLK) then
      case MODE is
        when "00" =>
          temp <= I;                               -- parallel load
        when "01" =>
          temp <= IR & temp(3 downto 1);           -- shift right (msb <- IR)
        when "10" =>
          temp <= temp(2 downto 0) & IL;           -- shift left  (lsb <- IL)
        when others =>
          temp <= temp;                            -- hold
      end case;
    end if;
  end process;

  Q <= temp;
end Behavioral;


//TestBench 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity shift_reg_4bit_tb is
end shift_reg_4bit_tb;

architecture Behavioral of shift_reg_4bit_tb is
  component shift_reg_4bit
    Port (
      CLK  : in  STD_LOGIC;
      RST  : in  STD_LOGIC;
      IL   : in  STD_LOGIC;
      IR   : in  STD_LOGIC;
      MODE : in  STD_LOGIC_VECTOR(1 downto 0);
      I    : in  STD_LOGIC_VECTOR(3 downto 0);
      Q    : out STD_LOGIC_VECTOR(3 downto 0)
    );
  end component;

  signal CLK, RST, IL, IR : STD_LOGIC;
  signal MODE             : STD_LOGIC_VECTOR(1 downto 0);
  signal I                : STD_LOGIC_VECTOR(3 downto 0);
  signal Q                : STD_LOGIC_VECTOR(3 downto 0);
begin
  uut: shift_reg_4bit port map (
    CLK  => CLK,
    RST  => RST,
    IL   => IL,
    IR   => IR,
    MODE => MODE,
    I    => I,
    Q    => Q
  );

  stim_proc: process
  begin
    -- reset
    RST <= '1'; wait for 10 ns;
    RST <= '0'; wait for 10 ns;

    -- parallel load
    I    <= "1010";
    MODE <= "00";
    CLK  <= '0'; wait for 5 ns;
    CLK  <= '1'; wait for 5 ns;
    wait for 10 ns;

    -- shift right (IR = '1')
    MODE <= "01";
    IR   <= '1';
    IL   <= '0';
    CLK  <= '0'; wait for 5 ns;
    CLK  <= '1'; wait for 5 ns;
    wait for 10 ns;

    -- shift left (IL = '0')
    MODE <= "10";
    IL   <= '0';
    IR   <= '0';
    CLK  <= '0'; wait for 5 ns;
    CLK  <= '1'; wait for 5 ns;
    wait for 10 ns;

    -- hold (MODE = "11")
    MODE <= "11";
    CLK  <= '0'; wait for 5 ns;
    CLK  <= '1'; wait for 5 ns;
    wait for 10 ns;

    wait;
  end process;
end Behavioral;


4-Bit Counter (entity + architecture) 

  library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity counter is
  Port (
    CLK : in  STD_LOGIC;
    CLR : in  STD_LOGIC;
    Q   : out STD_LOGIC_VECTOR(3 downto 0)
  );
end counter;

architecture Behavioral of counter is
  signal temp : STD_LOGIC_VECTOR(3 downto 0) := "0000";
begin
  process(CLK, CLR)
  begin
    if CLR = '1' then
      temp <= "0000";
    elsif rising_edge(CLK) then
      temp <= temp + 1;
    end if;
  end process;

  Q <= temp;
end Behavioral;


 //TestBench 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity counter_tb is
end counter_tb;

architecture Behavioral of counter_tb is
  component counter
    Port (
      CLK : in  STD_LOGIC;
      CLR : in  STD_LOGIC;
      Q   : out STD_LOGIC_VECTOR(3 downto 0)
    );
  end component;

  signal CLK : STD_LOGIC := '0';
  signal CLR : STD_LOGIC := '0';
  signal Q   : STD_LOGIC_VECTOR(3 downto 0);
begin
  uut: counter port map (
    CLK => CLK,
    CLR => CLR,
    Q   => Q
  );

  -- clock generation
  CLK <= not CLK after 10 ns;

  process
  begin
    CLR <= '1'; wait for 20 ns;    -- keep clear asserted
    CLR <= '0'; wait for 320 ns;   -- release clear and let counter run
    CLR <= '1'; wait for 20 ns;    -- clear again
    wait;
  end process;
end Behavioral;


4-Word FIFO (entity + architecture) 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity fifo is
  Port (
    CLK     : in  STD_LOGIC;
    ENR     : in  STD_LOGIC;  -- enable read
    ENW     : in  STD_LOGIC;  -- enable write
    DATAIN  : in  STD_LOGIC_VECTOR(3 downto 0);
    DATAOUT : out STD_LOGIC_VECTOR(3 downto 0);
    EMPTY   : out STD_LOGIC;
    FULL    : out STD_LOGIC
  );
end fifo;

architecture Behavioral of fifo is
  type memory_type is array (0 to 3) of STD_LOGIC_VECTOR(3 downto 0);
  signal mem  : memory_type := (others => (others => '0'));
  signal rptr : STD_LOGIC_VECTOR(1 downto 0) := "00";
  signal wptr : STD_LOGIC_VECTOR(1 downto 0) := "00";
  signal dout : STD_LOGIC_VECTOR(3 downto 0) := (others => '0');
begin
  process(CLK)
  begin
    if rising_edge(CLK) then
      if ENW = '1' then
        mem(conv_integer(wptr)) <= DATAIN;
        wptr <= wptr + 1;
      end if;

      if ENR = '1' then
        dout <= mem(conv_integer(rptr));
        rptr <= rptr + 1;
      end if;
    end if;
  end process;

  DATAOUT <= dout;
  FULL  <= '1' when (wptr = rptr) and (wptr /= "00") else '0';
  EMPTY <= '1' when (wptr = rptr) else '0';
end Behavioral;


//TestBench 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity fifo_tb is
end fifo_tb;

architecture Behavioral of fifo_tb is
  component fifo
    Port (
      CLK     : in  STD_LOGIC;
      ENR     : in  STD_LOGIC;
      ENW     : in  STD_LOGIC;
      DATAIN  : in  STD_LOGIC_VECTOR(3 downto 0);
      DATAOUT : out STD_LOGIC_VECTOR(3 downto 0);
      EMPTY   : out STD_LOGIC;
      FULL    : out STD_LOGIC
    );
  end component;

  signal CLK     : STD_LOGIC := '0';
  signal ENR     : STD_LOGIC := '0';
  signal ENW     : STD_LOGIC := '0';
  signal DATAIN  : STD_LOGIC_VECTOR(3 downto 0);
  signal DATAOUT : STD_LOGIC_VECTOR(3 downto 0);
  signal EMPTY   : STD_LOGIC;
  signal FULL    : STD_LOGIC;
begin
  uut: fifo port map (
    CLK     => CLK,
    ENR     => ENR,
    ENW     => ENW,
    DATAIN  => DATAIN,
    DATAOUT => DATAOUT,
    EMPTY   => EMPTY,
    FULL    => FULL
  );

  -- clock
  CLK <= not CLK after 5 ns;

  process
  begin
    ENW <= '0';
    ENR <= '0';
    DATAIN <= "0000";
    wait for 10 ns;

    ENW <= '1';
    ENR <= '0';
    DATAIN <= "1111"; wait for 10 ns;
    DATAIN <= "1101"; wait for 10 ns;
    DATAIN <= "1100"; wait for 10 ns;
    DATAIN <= "0001"; wait for 10 ns;

    ENW <= '0';
    ENR <= '1';
    wait for 50 ns;

    wait;
  end process;
end Behavioral;
