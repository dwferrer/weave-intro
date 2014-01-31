//fast c based histogram code for generating the 1-d histogram of an array destructively. Requires Intel's tbb to be included and linked seperately, along with std::algorithm



{FLOAT} * c_data =  data;//({FLOAT} *)  PyArray_DATA(PyArray_GETCONTIGUOUS(data));

unsigned long long int count = 1;

for(int d = 0; d < Ddata; d++) count*=Ndata[d];

//{FLOAT} * data = ({FLOAT} *)  PyArray_DATA((PyArrayObject *)c_data);

tbb::parallel_sort(data,data+count);

{FLOAT} * cbins = bins;

double * counts = bincounts;

int nbins = Nbins [0] -1;

unsigned long long int  b_low [omp_get_num_threads()];
unsigned long long int  b_high[omp_get_num_threads()];

for(int i = 0; i < omp_get_num_threads();i++){{
    b_low[i] = 0;
    b_high[i] = count;
}}


#pragma omp parallel for schedule(dynamic,1)
for( int b = 0; b < nbins; b++){{
    int t = omp_get_thread_num();

    {FLOAT} * p_low = std::lower_bound(data,data+count,bins[b]);
    {FLOAT} * p_high= std::upper_bound(data,data+count,bins[b+1]);

    b_low[t] =  p_low - data;
    b_high[t] = p_high -data;
    
    //printf("l: %f ->%llu, %f->h: %llu. count: %llu \n",bins[b],b_low[t],bins[b+1],b_high[t],b_high[t] - b_low[t]);
    double bincount = b_high[t] - b_low[t];
    counts[b] += bincount;
}}





