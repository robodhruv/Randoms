-- A DUT entity is used to wrap your design.
--  This example shows how you can do this for the
--  Serial adder.
entity DUT is
   port(input_vector: in bit_vector(3 downto 0);
       	output_vector: out bit_vector(1 downto 0));
end entity;

architecture DutWrap of DUT is
   component SerialAdder is
     port(reset, clk, a, b: in bit;
         	s, c: out bit);
   end component;
begin

   -- input/output vector element ordering is critical,
   -- and must match the ordering in the trace file!
   add_instance: SerialAdder 
			port map (
					reset => input_vector(3),
					clk => input_vector(2),
					a => input_vector(1),
					b => input_vector(0),
					s => output_vector(1),
					c => output_vector(0));

end DutWrap;

