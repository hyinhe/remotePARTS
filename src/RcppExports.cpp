// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include "remoteSTAR_types.h"
#include <RcppEigen.h>
#include <Rcpp.h>

using namespace Rcpp;

// AtA
inline Eigen::MatrixXd AtA(const MatrixXd& A);
RcppExport SEXP _remoteSTAR_AtA(SEXP ASEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const MatrixXd& >::type A(ASEXP);
    rcpp_result_gen = Rcpp::wrap(AtA(A));
    return rcpp_result_gen;
END_RCPP
}
// solve_cpp
Eigen::MatrixXd solve_cpp(const MatrixXd& A, const MatrixXd& B);
RcppExport SEXP _remoteSTAR_solve_cpp(SEXP ASEXP, SEXP BSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const MatrixXd& >::type A(ASEXP);
    Rcpp::traits::input_parameter< const MatrixXd& >::type B(BSEXP);
    rcpp_result_gen = Rcpp::wrap(solve_cpp(A, B));
    return rcpp_result_gen;
END_RCPP
}
// solve_ident_cpp
Eigen::MatrixXd solve_ident_cpp(const MatrixXd& A);
RcppExport SEXP _remoteSTAR_solve_ident_cpp(SEXP ASEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const MatrixXd& >::type A(ASEXP);
    rcpp_result_gen = Rcpp::wrap(solve_ident_cpp(A));
    return rcpp_result_gen;
END_RCPP
}
// tinvchol_cpp
Eigen::MatrixXd tinvchol_cpp(const MapMatd& V, double nugget);
RcppExport SEXP _remoteSTAR_tinvchol_cpp(SEXP VSEXP, SEXP nuggetSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const MapMatd& >::type V(VSEXP);
    Rcpp::traits::input_parameter< double >::type nugget(nuggetSEXP);
    rcpp_result_gen = Rcpp::wrap(tinvchol_cpp(V, nugget));
    return rcpp_result_gen;
END_RCPP
}
// fitGLS_cpp
List fitGLS_cpp(const MapMatd& X, const MapMatd& V, const MapMatd& y, const MapMatd& X0, double nugget, bool save_xx, const int threads);
RcppExport SEXP _remoteSTAR_fitGLS_cpp(SEXP XSEXP, SEXP VSEXP, SEXP ySEXP, SEXP X0SEXP, SEXP nuggetSEXP, SEXP save_xxSEXP, SEXP threadsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const MapMatd& >::type X(XSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type V(VSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type y(ySEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type X0(X0SEXP);
    Rcpp::traits::input_parameter< double >::type nugget(nuggetSEXP);
    Rcpp::traits::input_parameter< bool >::type save_xx(save_xxSEXP);
    Rcpp::traits::input_parameter< const int >::type threads(threadsSEXP);
    rcpp_result_gen = Rcpp::wrap(fitGLS_cpp(X, V, y, X0, nugget, save_xx, threads));
    return rcpp_result_gen;
END_RCPP
}
// LogLikGLS_cpp
inline double LogLikGLS_cpp(double nugget, const MapMatd& X, const MapMatd& V, const MapMatd& y);
RcppExport SEXP _remoteSTAR_LogLikGLS_cpp(SEXP nuggetSEXP, SEXP XSEXP, SEXP VSEXP, SEXP ySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< double >::type nugget(nuggetSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type X(XSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type V(VSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type y(ySEXP);
    rcpp_result_gen = Rcpp::wrap(LogLikGLS_cpp(nugget, X, V, y));
    return rcpp_result_gen;
END_RCPP
}
// optimizeNugget_cpp
double optimizeNugget_cpp(const MapMatd& X, const MapMatd& V, const MapMatd& y, double lower, double upper, double tol, bool debug);
RcppExport SEXP _remoteSTAR_optimizeNugget_cpp(SEXP XSEXP, SEXP VSEXP, SEXP ySEXP, SEXP lowerSEXP, SEXP upperSEXP, SEXP tolSEXP, SEXP debugSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const MapMatd& >::type X(XSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type V(VSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type y(ySEXP);
    Rcpp::traits::input_parameter< double >::type lower(lowerSEXP);
    Rcpp::traits::input_parameter< double >::type upper(upperSEXP);
    Rcpp::traits::input_parameter< double >::type tol(tolSEXP);
    Rcpp::traits::input_parameter< bool >::type debug(debugSEXP);
    rcpp_result_gen = Rcpp::wrap(optimizeNugget_cpp(X, V, y, lower, upper, tol, debug));
    return rcpp_result_gen;
END_RCPP
}
// GLS_worker_cpp
List GLS_worker_cpp(const MapMatd& y, const MapMatd& X, const MapMatd& V, const MapMatd& X0, bool save_xx);
RcppExport SEXP _remoteSTAR_GLS_worker_cpp(SEXP ySEXP, SEXP XSEXP, SEXP VSEXP, SEXP X0SEXP, SEXP save_xxSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const MapMatd& >::type y(ySEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type X(XSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type V(VSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type X0(X0SEXP);
    Rcpp::traits::input_parameter< bool >::type save_xx(save_xxSEXP);
    rcpp_result_gen = Rcpp::wrap(GLS_worker_cpp(y, X, V, X0, save_xx));
    return rcpp_result_gen;
END_RCPP
}
// crosspart_worker_cpp
List crosspart_worker_cpp(const MapMatd& xxi, const MapMatd& xxj, const MapMatd& xxi0, const MapMatd& xxj0, const MapMatd& tUinv_i, const MapMatd& tUinv_j, const MapMatd& Vsub, int df1, int df2);
RcppExport SEXP _remoteSTAR_crosspart_worker_cpp(SEXP xxiSEXP, SEXP xxjSEXP, SEXP xxi0SEXP, SEXP xxj0SEXP, SEXP tUinv_iSEXP, SEXP tUinv_jSEXP, SEXP VsubSEXP, SEXP df1SEXP, SEXP df2SEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const MapMatd& >::type xxi(xxiSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type xxj(xxjSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type xxi0(xxi0SEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type xxj0(xxj0SEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type tUinv_i(tUinv_iSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type tUinv_j(tUinv_jSEXP);
    Rcpp::traits::input_parameter< const MapMatd& >::type Vsub(VsubSEXP);
    Rcpp::traits::input_parameter< int >::type df1(df1SEXP);
    Rcpp::traits::input_parameter< int >::type df2(df2SEXP);
    rcpp_result_gen = Rcpp::wrap(crosspart_worker_cpp(xxi, xxj, xxi0, xxj0, tUinv_i, tUinv_j, Vsub, df1, df2));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_remoteSTAR_AtA", (DL_FUNC) &_remoteSTAR_AtA, 1},
    {"_remoteSTAR_solve_cpp", (DL_FUNC) &_remoteSTAR_solve_cpp, 2},
    {"_remoteSTAR_solve_ident_cpp", (DL_FUNC) &_remoteSTAR_solve_ident_cpp, 1},
    {"_remoteSTAR_tinvchol_cpp", (DL_FUNC) &_remoteSTAR_tinvchol_cpp, 2},
    {"_remoteSTAR_fitGLS_cpp", (DL_FUNC) &_remoteSTAR_fitGLS_cpp, 7},
    {"_remoteSTAR_LogLikGLS_cpp", (DL_FUNC) &_remoteSTAR_LogLikGLS_cpp, 4},
    {"_remoteSTAR_optimizeNugget_cpp", (DL_FUNC) &_remoteSTAR_optimizeNugget_cpp, 7},
    {"_remoteSTAR_GLS_worker_cpp", (DL_FUNC) &_remoteSTAR_GLS_worker_cpp, 5},
    {"_remoteSTAR_crosspart_worker_cpp", (DL_FUNC) &_remoteSTAR_crosspart_worker_cpp, 9},
    {NULL, NULL, 0}
};

RcppExport void R_init_remoteSTAR(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
