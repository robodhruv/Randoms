entity SerialAdder is
   port(reset,clk,a,b: in bit;
        s,c: out bit);
end entity;

architecture Formulas of SerialAdder is
	signal last_c, c_sig: bit;
begin

	c_sig <= (last_c and ( a or b)) or (a and b);

	s <= a xor b xor last_c;
	c <= c_sig;

	process(clk,reset)
	begin
		if(clk'event and clk = '1') then
			if(reset = '1') then
				last_c <= '0';
			else
				last_c <= c_sig;
			end if;
		end if;
	end process;
end Formulas;
