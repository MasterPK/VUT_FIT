library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port (
	ROW		:out std_logic_vector (0 to 7);
	LED		:out std_logic_vector (0 to 7);
	RESET	:in std_logic;
	SMCLK	:in std_logic

);
end entity ledc8x8;

architecture main of ledc8x8 is
	signal row_select:std_logic_vector(7 downto 0):="10000000";
	signal led_select		:std_logic_vector(7 downto 0);
	signal cnt_sec			:std_logic_vector(20 downto 0);
	signal cnt_row			:std_logic_vector(11 downto 0);
	signal enable			:std_logic:='0';
	signal symbol_select	:std_logic:='0';
	signal letter_select	:std_logic:='0';

begin

    counter_sec:process(RESET,SMCLK,cnt_sec) begin
    	if RESET='1' then 
    		cnt_sec <= (others => '0');
    	elsif rising_edge(SMCLK) then
    		cnt_sec <= cnt_sec+1;
    		if cnt_sec(20 downto 0)="111000010000000000000" then
    			cnt_sec <= (others => '0');
    			if symbol_select = '0' and letter_select='0' then symbol_select<='1';
    			elsif symbol_select = '1' and letter_select='0' then symbol_select<='0'; letter_select<='1';
    			elsif symbol_select = '0' and letter_select='1' then symbol_select<='1'; letter_select<='1';
    			elsif symbol_select = '1' and letter_select='1' then symbol_select<='0'; letter_select<='0'; 
    			end if;
    		end if;
    	end if;
    end process counter_sec;

    counter_row:process(RESET,SMCLK,cnt_row) begin
    	if RESET='1' then 
    		cnt_row <= (others => '0');
    	elsif rising_edge(SMCLK) then
    		cnt_row <= cnt_row+1;
    		if cnt_row(11 downto 0)= "111000010000" then
    			enable <= '1';
    			cnt_row <= (others => '0');
    		else
    			enable <= '0';
    		end if;
    	end if;
    end process counter_row;

    rotate:process(RESET,SMCLK,enable,row_select) begin
    	if RESET='1' then
    		row_select <= "10000000";
    	elsif rising_edge(SMCLK) and enable='1' then
    		row_select <= row_select(0) & row_select(7 downto 1);
    	end if;
    end process rotate;

    columns:process(letter_select,row_select,symbol_select) begin
    	if symbol_select = '0' then
	    	if letter_select = '0' then
	    		case row_select is
	    			when "10000000" => led_select <= "00111111";
	    			when "01000000" => led_select <= "01011111";
	    			when "00100000" => led_select <= "01101111";
	    			when "00010000" => led_select <= "01011111";
	    			when "00001000" => led_select <= "00111111";
	    			when "00000100" => led_select <= "01111111";
	    			when "00000010" => led_select <= "01111111";
	    			when "00000001" => led_select <= "01111111";
	    			when others 	=> led_select <= "01111111";
	    		end case;
	    	else
	    		case row_select is
	    			when "10000000" => led_select <= "01111011";
	    			when "01000000" => led_select <= "01110111";
	    			when "00100000" => led_select <= "01101111";
	    			when "00010000" => led_select <= "01011111";
	    			when "00001000" => led_select <= "00111111";
	    			when "00000100" => led_select <= "01011111";
	    			when "00000010" => led_select <= "01101111";
	    			when "00000001" => led_select <= "01110111";
	    			when others 	=> led_select <= "01111011";
	    		end case;
	    	end if;
	    else
	    	led_select <= "11111111";
	    end if;

    end process columns;

    ROW<=row_select;
    LED<=led_select;

end main;
