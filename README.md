# VLSI-MIOT-Practicals

//VLSI Practicals 
4-Bit ALU

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
entity alu_4bit is
Port ( A,B : in STD_LOGIC_VECTOR (3 downto 0);
SEL : in STD_LOGIC_VECTOR (2 downto 0);
Y : out STD_LOGIC_VECTOR (3 downto 0);
CARRY : out STD_LOGIC);
end alu_4bit;
architecture Behavioral of alu_4bit is
signal temp: STD_LOGIC_VECTOR(4 downto 0);
begin
process(A, B, SEL)
begin
case SEL is
when "000" => temp <= ('0' & A) + ('0' & B); Y <= temp(3 downto 0); CARRY <= temp(4);
when "001" => temp <= ('0' & A) - ('0' & B); Y <= temp(3 downto 0); CARRY <= temp(4);
when "010" => Y <= A AND B; CARRY <= '0';
when "011" => Y <= A NAND B; CARRY <= '0';
when "100" => Y <= A XOR B; CARRY <= '0';
when "101" => Y <= A XNOR B; CARRY <= '0';
when "110" => Y <= A OR B; CARRY <= '0';
when others=> Y <= "0000"; CARRY <= '0';
end case;
end process;
end Behavioral;

//Testbench

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
entity alu_4bit_tb is
end alu_4bit_tb;
architecture Behavioral of alu_4bit_tb is
component alu_4bit
Port (A,B: in STD_LOGIC_VECTOR(3 downto 0);
SEL: in STD_LOGIC_VECTOR(2 downto 0);
Y: out STD_LOGIC_VECTOR(3 downto 0);
CARRY: out STD_LOGIC);
end component;
signal A,B: STD_LOGIC_VECTOR(3 downto 0);
signal SEL: STD_LOGIC_VECTOR(2 downto 0);
signal Y: STD_LOGIC_VECTOR(3 downto 0);
signal CARRY: STD_LOGIC;
begin
uut: alu_4bit port map (A=>A, B=>B, SEL=>SEL, Y=>Y, CARRY=>CARRY);
stim_proc: process
begin
A<="0101"; B<="0011"; SEL<="000"; wait for 100ns;
A<="0101"; B<="0011"; SEL<="001"; wait for 100ns;
A<="0101"; B<="0011"; SEL<="010"; wait for 100ns;
A<="0101"; B<="0011"; SEL<="011"; wait for 100ns;
A<="0101"; B<="0011"; SEL<="100"; wait for 100ns;
A<="0101"; B<="0011"; SEL<="101"; wait for 100ns;
A<="0101"; B<="0011"; SEL<="110"; wait for 100ns;
A<="1111"; B<="0001"; SEL<="000"; wait for 100ns;
A<="1111"; B<="0001"; SEL<="001"; wait for 100ns;
A<="1010"; B<="0101"; SEL<="010"; wait for 100ns;
A<="1010"; B<="0101"; SEL<="100"; wait for 100ns;
A<="1010"; B<="0101"; SEL<="110"; wait for 100ns;
wait;
end process;
end Behavioral;

2. Shift Register

   library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
entity Shift_Reg_4bit is
Port (
CLK : in STD_LOGIC;
RST : in STD_LOGIC;
IL : in STD_LOGIC;
IR : in STD_LOGIC;
MODE : in STD_LOGIC_VECTOR(1 downto 0);
I : in STD_LOGIC_VECTOR(3 downto 0);
Q : out STD_LOGIC_VECTOR(3 downto 0)
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
when "00" => temp <= I;
when "01" => temp <= IR & temp(3 downto 1);
when "10" => temp <= temp(2 downto 0) & IL;
when others => temp <= temp;
end case;
end if;
end process;
Q <= temp;
end Behavioral;

// Testbench

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
entity Shift_Reg_4bit_tb is
end Shift_Reg_4bit_tb;
architecture Behavioral of Shift_Reg_4bit_tb is
component Shift_Reg_4bit
Port (
CLK, RST, IL, IR : in STD_LOGIC;
MODE : in STD_LOGIC_VECTOR(1 downto 0);
I : in STD_LOGIC_VECTOR(3 downto 0);
Q : out STD_LOGIC_VECTOR(3 downto 0)
);
end component;
signal CLK, RST, IL, IR : STD_LOGIC;
signal MODE : STD_LOGIC_VECTOR(1 downto 0);
signal I : in STD_LOGIC_VECTOR(3 downto 0);
signal Q : out STD_LOGIC_VECTOR(3 downto 0);
begin
uut: Shift_Reg_4bit port map (
CLK => CLK, RST => RST, MODE => MODE,
I => I, IL => IL, IR => IR, Q => Q
);
stim_proc: process
begin
RST <= '1'; wait for 10 ns;
RST <= '0'; wait for 10 ns;
I <= "1010"; MODE <= "00"; CLK <= '0'; wait for 5 ns;
CLK <= '1'; wait for 5 ns; wait for 10 ns;
MODE <= "01"; IR <= '1'; IL <= '0'; CLK <= '0'; wait
for 5 ns;
CLK <= '1'; wait for 5 ns; wait for 10 ns;
MODE <= "10"; IL <= '0'; IR <= '0'; CLK <= '0'; wait for 5 ns;
CLK <= '1'; wait for 5 ns; wait for 10 ns;
MODE <= "11"; CLK <= '0'; wait for 5 ns;
CLK <= '1'; wait for 5 ns; wait for 10 ns;
wait;
end process;
end Behavioral;

// 3 . Counter 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
entity counter is
Port(clk, clr : in STD_LOGIC;
q : out STD_LOGIC_VECTOR(3 downto 0));
end counter;
architecture rtl of counter is
signal temp : unsigned(3 downto 0);
begin
process(clk, clr)
begin
if clr='1' then temp <= (others=>'0');
elsif rising_edge(clk) then temp <= temp+1;
end if;
end process;
q <= std_logic_vector(temp);
end rtl;

Testbench : 
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
entity counter_tb is
end counter_tb;
architecture test of counter_tb is
component counter port(clk, clr: in STD_LOGIC;
q: out STD_LOGIC_VECTOR(3 downto 0));
end component;
signal clk, clr : STD_LOGIC := '0';
signal q : STD_LOGIC_VECTOR(3 downto 0);
begin
uut: counter port map(clk=>clk, clr=>clr, q=>q);
clk <= not clk after 10 ns;
process
begin
clr <= '1'; wait for 20 ns;
clr <= '0'; wait for 320 ns;
clr <= '1'; wait for 20 ns;
wait;
end process;
end test;

4 . FIFO

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
entity fifo is
port(
clk : in std_logic;
enr : in std_logic;
enw : in std_logic;
datain : in std_logic_vector(3 downto 0);
dataout : out std_logic_vector(3 downto 0);
empty : out std_logic;
full : out std_logic
);
end fifo;
architecture rtl of fifo is
type memory_type is array (0 to 3) of std_logic_vector(3 downto 0);
signal temp : memory_type := (others => (others => '0'));
signal readptr : unsigned(1 downto 0) := "00";
signal writeptr : unsigned(1 downto 0) := "00";
begin
process(clk)
begin
if rising_edge(clk) then
if enr = '1' then
dataout <= temp(to_integer(readptr));
readptr <= readptr + 1;
end if;
if enw = '1' then
temp(to_integer(writeptr)) <= datain;
writeptr <= writeptr + 1;
end if;
if writeptr = "11" then
full <= '1';
writeptr <= "00";
else
full <= '0';
end if;
if writeptr = "00" then
empty <= '1';
else
empty <= '0';
end if;
end if;
end process;
end rtl; 

// testbench 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
entity fifo_tb is
end fifo_tb;
architecture test of fifo_tb is
component fifo
port(
clk : in std_logic;
enr : in std_logic;
enw : in std_logic;
datain : in std_logic_vector(3 downto 0);
dataout : out std_logic_vector(3 downto 0);
empty : out std_logic;
full : out std_logic
);
end component;
signal clk : std_logic := '0';
signal enr : std_logic := '0';
signal enw : std_logic := '0';
signal datain : std_logic_vector(3 downto 0);
signal dataout : std_logic_vector(3 downto 0);
signal empty : std_logic;
signal full : std_logic;
begin
uut: fifo port map(
clk => clk,
enr => enr,
enw => enw,
datain => datain,
dataout => dataout,
empty => empty,
full => full
);
clk <= not clk after 5 ns;
process
begin
enw <= '0';
enr <= '0';
datain <= "0000";
wait for 10 ns;
enw <= '1';
enr <= '0';
datain <= "1111"; wait for 20 ns;
datain <= "1101"; wait for 20 ns;
datain <= "1100"; wait for 20 ns;
datain <= "0001"; wait for 20 ns;
enw <= '0';
enr <= '1';
wait for 100 ns;
wait;
end process;
end test;
