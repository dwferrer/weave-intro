#!/usr/bin/python

import numpy as np
import scipy.weave
from scipy.weave.converters import c_spec
import os

abacuspath = os.getenv("ABACUS","NONE")
if abacuspath == "NONE":
    print("Error: Please define $ABACUS to be the absolute path to your abacus distribution")
    sys.exit(1)


def argsort(data):
    data = data.copy()
    codefile = open(abacuspath+"/Analysis/PowerSpectrum/argsort.cc","r")
    code = codefile.read().format(FLOAT = c_spec.num_to_c_types[data.dtype.char])
    codefile.close()
    auxcodefile = open(abacuspath+"/Analysis/PowerSpectrum/weighted_histogram_aux.cc","r")
    auxcode = auxcodefile.read().format(FLOAT = c_spec.num_to_c_types[data.dtype.char])
    auxcodefile.close()
    idx = np.ascontiguousarray(np.arange(data.size,dtype = np.uint64))
    scipy.weave.inline(code,["data","idx"],compiler="intelem",headers=["<algorithm>","\"tbb/parallel_sort.h\"","<omp.h>"],libraries =["tbb","iomp5"],support_code=auxcode,verbose=2)
    return idx


def Histogram(data,bins):
    bins = np.ascontiguousarray(bins,dtype = data.dtype)
    bincounts = np.ascontiguousarray(np.zeros(bins.shape[0] -1,dtype = np.float64))
    data = np.ascontiguousarray(np.ravel(np.copy(data)))

    codefile = open(abacuspath+"/Analysis/PowerSpectrum/inline_histogram.cc","r")
    code = codefile.read().format(FLOAT = c_spec.num_to_c_types[data.dtype.char])
    codefile.close()
    scipy.weave.inline(code,["bins","data","bincounts"],compiler="intelem",headers=["<algorithm>","\"tbb/parallel_sort.h\"","<omp.h>"],libraries =["tbb","iomp5"],verbose=2)
    return bincounts, bins

def HistogramWeighted(data,bins,weights,sidx=None):
    bins = np.ascontiguousarray(bins,dtype = data.dtype)
    bincounts = np.ascontiguousarray(np.zeros(bins.shape[0] -1,dtype = np.float64))
    data = np.ravel(data.copy())
    weights = np.ravel(weights.copy())

    if sidx is None: sidx = argsort(data)
    print( "Done Sorting")
    data = np.ascontiguousarray(data[sidx])
    weights = np.ascontiguousarray(weights[sidx],dtype=data.dtype)
    codefile = open(abacuspath+"/Analysis/PowerSpectrum/inline_histogram_weighted.cc","r")
    code = codefile.read().format(FLOAT = c_spec.num_to_c_types[data.dtype.char])
    codefile.close()
    auxcodefile = open(abacuspath+"/Analysis/PowerSpectrum/weighted_histogram_aux.cc","r")
    auxcode = auxcodefile.read().format(FLOAT = c_spec.num_to_c_types[data.dtype.char])
    auxcodefile.close()
    scipy.weave.inline(code,["bins","data","weights","bincounts"],compiler="intelem",headers=["<algorithm>","\"tbb/parallel_sort.h\"","<omp.h>"],libraries =["tbb","iomp5"],support_code=auxcode,verbose=2)
    return bincounts, bins,sidx
