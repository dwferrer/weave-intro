//fast c based histogram code for generating the 1-d histogram of an array destructively. Requires Intel's tbb to be included and linked seperately, along with std::algorithm

//we assume that data, data_x_weights are already sorted


//{FLOAT} * c_data =  data;//({FLOAT} *)  PyArray_DATA(PyArray_GETCONTIGUOUS(data));

unsigned long long int count = 1;

for(int d = 0; d < Ddata; d++) count*=Ndata[d];



double * counts = bincounts;

int nbins = Nbins [0] -1;

unsigned long long int  b_low [omp_get_num_threads()];
unsigned long long int  b_high[omp_get_num_threads()];

for(int i = 0; i < omp_get_num_threads();i++){{
    b_low[i] = 0;
    b_high[i] = count;
}}

#pragma omp parallel for schedule( dynamic,1)
for( int b = 0; b < nbins; b++){{
    int t = omp_get_thread_num();

    {FLOAT} * p_low = std::lower_bound(data,data+count,bins[b]);
    {FLOAT} * p_high= std::upper_bound(data,data+count,bins[b+1]);

    b_low[t] =  p_low - data;
    b_high[t] = p_high -data;
    
    //double bincount = ParallelSum(weights + b_low[t],b_high[t]-b_low[t]);
    double bincount = 0;
    #pragma simd
    for (size_t i = b_low[t]; i < b_high[t]; i++) bincount += weights[i];
    //printf("l: %f ->%llu, %f->h: %llu. count: %f \n",bins[b],b_low[t],bins[b+1],b_high[t],bincount);
    counts[b] += bincount;
}}





