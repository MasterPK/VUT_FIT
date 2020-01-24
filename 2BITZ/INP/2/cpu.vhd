-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2018 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Petr Krehlik (xkrehl04)
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='1') / zapis do pameti (DATA_RDWR='0')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

signal PC: std_logic_vector(11 downto 0);
signal PC_inc: std_logic;
signal PC_dec: std_logic;

signal MX: std_logic_vector(1 downto 0);

signal PTR: std_logic_vector(9 downto 0);
signal PTR_inc: std_logic;
signal PTR_dec: std_logic;

signal tmp: std_logic_vector(7 downto 0);

type state_fsm is (
idle, load_instruction, inc_ptr, dec_ptr, inc_cell, inc_cell2, dec_cell, dec_cell2, while1, while2, while3, while4, while5, while_end1, while_end2, while_end3,while_end4, print, print2, load_save,load_save2, comment, comment2, comment3, hex_load_save, hex_load_save2, stop ,error

);

signal prev_fsm:state_fsm;
signal next_fsm:state_fsm;


begin



multiplexor: process(IN_DATA, DATA_RDATA, MX) begin
   case (MX) is
      when "00" => DATA_WDATA <= IN_DATA;
      when "01" => DATA_WDATA <= tmp;
      when "10" => DATA_WDATA <= DATA_RDATA - 1;
      when "11" => DATA_WDATA <= DATA_RDATA + 1;
      when others =>
   end case;  
end process multiplexor;

ptr_counter: process(CLK,RESET, PTR, PTR_inc, PTR_dec) begin
   if RESET='1' then 
      PTR <= "0000000000";
   elsif rising_edge(CLK) then
      if PTR_inc = '1' then
         PTR <= PTR + 1;
      elsif PTR_dec = '1' then
         PTR <= PTR - 1;
      end if;
   end if;
   DATA_ADDR <= PTR;
end process ptr_counter;

next_state:process(CLK, RESET) begin
   if RESET='1' then
      prev_fsm <= idle;
   elsif rising_edge(CLK) then
      if EN = '1' then
         prev_fsm <= next_fsm;
      end if;
   end if;

end process next_state;

program_counter: process(CLK,RESET, PC, PC_inc, PC_dec) begin
   if RESET='1' then 
      PC <= "000000000000";
   elsif rising_edge(CLK) then
      if PC_inc = '1' then
         PC <= PC + 1;
      elsif PC_dec = '1' then
         PC <= PC - 1;
      end if;
   end if;
   CODE_ADDR <= PC;
end process program_counter;


main_fsm:process(CODE_DATA, IN_VLD, OUT_BUSY, DATA_RDATA, prev_fsm) begin

   PTR_inc <= '0';
   PTR_dec <= '0';

   PC_inc <= '0';
   PC_dec <= '0';

   CODE_EN <= '1';
   DATA_EN <= '0';
   DATA_RDWR <= '0';

   IN_REQ <= '0';
   OUT_WE <= '0';
   MX<="00";

   case prev_fsm is
      when idle =>
        CODE_EN <= '1';
        next_fsm <= load_instruction;

      when load_instruction =>
         case(CODE_DATA) is 
            when X"3E" => next_fsm <= inc_ptr;
            when X"3C" => next_fsm <= dec_ptr;
            when X"2B" => next_fsm <= inc_cell;
            when X"2D" => next_fsm <= dec_cell;
            when X"5B" => next_fsm <= while1;
            when X"5D" => next_fsm <= while_end1;
            when X"2E" => next_fsm <= print;
            when X"2C" => next_fsm <= load_save;
            when X"23" => next_fsm <= comment;
            when X"30" => next_fsm <= hex_load_save;
            when X"31" => next_fsm <= hex_load_save;
            when X"32" => next_fsm <= hex_load_save;
            when X"33" => next_fsm <= hex_load_save;
            when X"34" => next_fsm <= hex_load_save;
            when X"35" => next_fsm <= hex_load_save;
            when X"36" => next_fsm <= hex_load_save;
            when X"37" => next_fsm <= hex_load_save;
            when X"38" => next_fsm <= hex_load_save;
            when X"39" => next_fsm <= hex_load_save;
            when X"41" => next_fsm <= hex_load_save2;
            when X"42" => next_fsm <= hex_load_save2;
            when X"43" => next_fsm <= hex_load_save2;
            when X"44" => next_fsm <= hex_load_save2;
            when X"45" => next_fsm <= hex_load_save2;
            when X"46" => next_fsm <= hex_load_save2;
            when X"00" => next_fsm <= stop;
            when others => next_fsm <= stop;
         end case;

      when inc_ptr => 
         PC_inc <= '1';
         PTR_inc <= '1';
         next_fsm <= idle;

      when dec_ptr => 
         PC_inc <= '1';
         PTR_dec <= '1';
         next_fsm <= idle;

      when inc_cell => 
         DATA_RDWR <= '1';
         DATA_EN <= '1';
         next_fsm <= inc_cell2;

      when inc_cell2 => 
         MX <= "11";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         PC_inc <= '1';
         next_fsm <= idle;

      when dec_cell => 
         DATA_RDWR <= '1';
         DATA_EN <= '1';
         
         next_fsm <= dec_cell2;

      when dec_cell2 => 
         MX <= "10";
         DATA_EN <= '1';
         DATA_RDWR <= '0';
         PC_inc <= '1';
         next_fsm <= idle;

      when print =>
         if OUT_BUSY='1' then 
            next_fsm <= print;
         elsif OUT_BUSY='0' then
            DATA_EN<='1';
            DATA_RDWR <= '1';
            next_fsm <= print2;
         end if;

      when print2 =>
         OUT_DATA <= DATA_RDATA;
         OUT_WE<='1';
         PC_inc<='1';
         next_fsm <=idle;

      when load_save =>
         IN_REQ <= '1';
         next_fsm <= load_save2;

      when load_save2 =>
         if IN_VLD = '1' then 
            MX <= "00";
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            PC_inc <= '1';
            next_fsm <= idle;
         else
            next_fsm <= prev_fsm;
         end if;

      when comment =>
         PC_inc <= '1';
         next_fsm <= comment2;

      when comment2 =>
         next_fsm <= comment3;

      when comment3 =>
         if CODE_DATA=X"23" then
            PC_inc <= '1';
            next_fsm <= idle;
         else
            next_fsm <= comment;
         end if;

      when hex_load_save =>
         PC_inc <= '1';
         DATA_EN <= '1';
         MX <= "01";
         tmp <= CODE_DATA(3 downto 0) & "0000";
         
         next_fsm <= idle;

      when hex_load_save2 =>
         PC_inc <= '1';
         DATA_EN <= '1';
         MX <= "01";
         DATA_RDWR <= '0';
         tmp <= (CODE_DATA(3 downto 0) + 9) & "0000";
         
         next_fsm <= idle;

      when while1 =>
          PC_inc <= '1';
          DATA_RDWR <= '1';
          DATA_EN <= '1';
          next_fsm <= while2;


      when while2 =>
          if (DATA_RDATA = "00000000") then
              next_fsm <= while5;
          else
              next_fsm <= idle;
          end if;

      when while3 =>
          PC_inc <= '1';
          next_fsm <= while4;

      when while4 =>
          next_fsm <= while5;

      when while5 =>
          if (CODE_DATA = X"5D") then
              PC_inc <= '1';
              next_fsm <= idle;
          else
              next_fsm <= while3;
          end if;

      when while_end1 =>
          PC_dec <= '1';
          next_fsm <= while_end2;

      when while_end2 =>
          next_fsm <= while_end3;

      when while_end3 =>
          if (CODE_DATA = X"5B") then
              next_fsm <= while1;
          else
              next_fsm <= while_end1;
          end if;

      when stop =>
        next_fsm <= stop;
      
      when others => 
        PC_inc <= '1';
   end case;
end process main_fsm;



end behavioral;