// =============================================================================
// === qrdemo_gpu.cpp ==========================================================
// =============================================================================

// A simple C++ demo of SuiteSparseQR.  The comments give the MATLAB equivalent
// statements.  See also qrdemo.m

#include "SuiteSparseQR.hpp"
#include "SuiteSparseGPU_Runtime.hpp"

int main (int argc, char **argv)
{
    cholmod_sparse *A ;
    cholmod_dense *X, *B ;
    double t ;

    int mtype ;
    long m, n ;
    size_t total_mem, available_mem ;

    printf ("\nqrdemo_gpu: Testing SPQR on the GPU:\n") ;

    // start CHOLMOD
    cholmod_common *cc, Common ;
    cc = &Common ;
    cholmod_l_start (cc) ;

    // warmup the GPU.  This can take some time, but only needs
    // to be done once
    cc->useGPU = 0 ;
//    cc->useGPU = true ;

   // cholmod_l_gpu_memorysize (&total_mem, &available_mem, cc) ;
  //  cc->gpuMemorySize = available_mem ;

  //  printf ("available GPU memory: %g MB\n",
   //     (double) (cc->gpuMemorySize) / (1024 * 1024)) ;

    // A = mread (stdin) ; read in the sparse matrix A
    FILE *file = fopen("A.mtx", "r");
    A = (cholmod_sparse *) cholmod_l_read_matrix (file, 1, &mtype, cc) ;
    fclose(file);
    if (mtype != CHOLMOD_SPARSE)
    {
        printf ("input matrix must be sparse\n") ;
        exit (1) ;
    }

    // [m n] = size (A) ;
    m = A->nrow ;
    n = A->ncol ;

    long ordering = 2;

#if 1
    printf ("Matrix %6ld-by-%-6ld nnz: %6ld\n",
        m, n, cholmod_l_nnz (A, cc)) ;
#endif

    // B = mread (stdin) ; read in the matrix B
    file = fopen("b.mtx", "r");
    B = (cholmod_dense *) cholmod_l_read_matrix (file, 1, &mtype, cc) ;
    fclose(file);

    t = SuiteSparse_time ( ) ;

    // X = A\B ; with default ordering and default column 2-norm tolerance
    // A, X, and B are all real
     X = SuiteSparseQR <double> (A, B, cc) ;
    // X = SuiteSparseQR <double>(SPQR_ORDERING_DEFAULT, SPQR_DEFAULT_TOL, A, B, c  c) ;

    t = SuiteSparse_time ( ) - t ;

    printf ("solving time: %g\n", t) ;

    // write out X to a file
    FILE *f = fopen ("X.mtx", "w") ;
    cholmod_l_write_dense (f, X, NULL, cc) ;
    fclose (f) ;

    // free everything

    cholmod_l_free_sparse (&A, cc) ;
    cholmod_l_free_dense (&X, cc) ;
    cholmod_l_free_dense (&B, cc) ;
    cholmod_l_finish (cc) ;

    return (0) ;
}
