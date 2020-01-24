-- fsm.vhd: Finite State Machine
-- Author(s): 
-- Petr Křehlík (xkrehl04)
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (PRINT_MESSAGE_ERR,PRINT_MESSAGE_OK, FINISH,ERR,KOD_1,KOD1_2,KOD1_3,KOD1_4,KOD1_5,KOD1_6,KOD1_7,KOD1_8,KOD1_9,KOD1_FINISH,KOD2_1,KOD2_2,KOD2_3);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= KOD_1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -xkrehl04 : kod1 = 875556272   kod2 = 8755564156
   when KOD_1 =>
      next_state <= KOD_1;
      if (KEY(8) = '1') then
         next_state <= KOD1_2;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KOD1_2 =>
      next_state <= KOD1_2;
      if (KEY(7) = '1') then
         next_state <= KOD1_3;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KOD1_3 =>
      next_state <= KOD1_3;
      if (KEY(5) = '1') then
         next_state <= KOD1_4;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KOD1_4 =>
      next_state <= KOD1_4;
      if (KEY(5) = '1') then
         next_state <= KOD1_5;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KOD1_5 =>
      next_state <= KOD1_5;
      if (KEY(5) = '1') then
         next_state <= KOD1_6;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KOD1_6 =>
      next_state <= KOD1_6;
      if (KEY(6) = '1') then
         next_state <= KOD1_7;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KOD1_7 =>
      next_state <= KOD1_7;
      if (KEY(2) = '1') then
         next_state <= KOD1_8;
      elsif (KEY(4) = '1') then
         next_state <= KOD2_1;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KOD1_8 =>
      next_state <= KOD1_8;
      if (KEY(7) = '1') then
         next_state <= KOD1_9;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KOD1_9 =>
      next_state <= KOD1_9;
      if (KEY(2) = '1') then
         next_state <= KOD1_FINISH;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KOD2_1 =>
      next_state <= KOD2_1;
      if (KEY(1) = '1') then
         next_state <= KOD2_2;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KOD2_2 =>
      next_state <= KOD2_2;
      if (KEY(5) = '1') then
         next_state <= KOD2_3;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KOD2_3 =>
      next_state <= KOD2_3;
      if (KEY(6) = '1') then
         next_state <= KOD1_FINISH;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when KOD1_FINISH =>
      next_state <= KOD1_FINISH;
      if (KEY(14 downto 0) /= "000000000000000") then
         next_state <= ERR;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_OK;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when ERR =>
      next_state <= ERR;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_ERR; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_OK =>
      next_state <= PRINT_MESSAGE_OK;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_ERR =>
      next_state <= PRINT_MESSAGE_ERR;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= KOD_1; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= KOD_1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_OK =>
      FSM_MX_MEM     <= '1';
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_ERR =>
      FSM_MX_MEM     <= '0';
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   end case;
end process output_logic;

end architecture behavioral;

