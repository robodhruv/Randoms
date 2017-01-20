library ieee ;
use ieee . std_logic_1164 .all ;


entity PriorityEncoder is
	port ( x7 , x6 , x5 , x4 , x3 , x2 , x1 , x0 :in bit ;
	s2 , s1 , s0 , N :out bit ) ;
end PriorityEncoder ;

architecture comb of PriorityEncoder is
begin
	N <= not( x7 or x6 or x5 or x4 or x3 or x2 or x1 or x0 ) ;
	
	s0 <= ( x1 and not x0 ) or
		(x3 and not x2 and not x1 and not x0) or
		( x5 and not x4 and not x3 and not x2 and
		not x1 and not x0 ) or
		( x7 and not x6 and not x5 and not x4
		and not x3 and not x2 and not x1
		and not x0 ) ;
	s1 <= ( x2 and not x1 and not x0 ) or
		( x3 and not x2 and not x1 and not x0 ) or
		( x6 and not x5 and not x4 and not x3 and
		not x2 and not x1 and not x0 ) or
		( x7 and not x6 and not x5 and not x4 and
		not x3 and not x2 and not x1 and not x0 ) ;
	s2 <= ( x4 and not x3 and not x2 and
		not x1 and not x0 ) or
		( x5 and not x4 and not x3 and not x2 and
		not x1 and not x0 ) or
		( x6 and not x5 and not x4 and not x3
		and not x2 and not x1 and not x0 ) or
		( x7 and not x6 and not x5 and not x4 and not x3
		and not x2 and not x1 and not x0 ) ;
end comb ;