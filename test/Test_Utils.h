#pragma once


/*	CHeck whether test loop index and VNLM iterator (being tested) are in sync.
 * 	Both should reach loop end at the same time. 
 * */
template< typename IT_type>
inline
bool
loop_end_condition(
		const IT_type& p,
		const IT_type& e,
		const unsigned int i,
		const unsigned int j,
		const unsigned int i_max,
		const unsigned int j_max )
{
	const bool it_end  = !(p!=e);
	const bool tru_end =   ((i+1)== i_max)
						&& ( j   ==(j_max-1));
	return it_end == tru_end;
}

