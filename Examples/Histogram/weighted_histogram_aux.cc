#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"

struct Sum {{
    {FLOAT} value;
    Sum() : value(0) {{}}
    Sum( Sum& s, tbb::split ) {{value = 0;}}
    void operator()( const tbb::blocked_range<{FLOAT}*>& r ) {{
        {FLOAT} temp = value;
        for( {FLOAT}* a=r.begin(); a!=r.end(); ++a ) {{
            temp += *a;
        }}
        value = temp;
    }}
    void join( Sum& rhs ) {{value += rhs.value;}}
}};

{FLOAT} ParallelSum( {FLOAT}  * array, size_t n ) {{
    Sum total;
    tbb::parallel_reduce( tbb::blocked_range<{FLOAT}*>( array, array+n ), 
                     total );
    return total.value;
}}

{FLOAT} * sarray;

inline bool comp(size_t a, size_t b){{
        return sarray[a]<sarray[b];
}}




