// [[Rcpp::depends(RcppEigen)]]
// [[Rcpp::plugins(openmp)]]
#include <iostream>
#include <RcppEigen.h>
#include <math.h>
// #include <omp.h>

using namespace std;
using namespace Rcpp;

// using Rcpp::as;
using Eigen::LLT;
using Eigen::Lower;
using Eigen::Map;
using Eigen::MatrixXd;
using Eigen::MatrixXi;
using Eigen::Upper;
using Eigen::VectorXd;
using Eigen::VectorXi;

typedef Map<MatrixXd> MapMatd;
typedef Map<MatrixXi> MapMati;
typedef Map<VectorXd> MapVecd;

//==============================================================================
/* AtA(A) ----
 * inline code that computes t(A) %*% A
 */

// [[Rcpp::export]]
inline MatrixXd AtA(const MatrixXd& A) {
  int n(A.cols());
  return MatrixXd(n,n).setZero().selfadjointView<Lower>()
                      .rankUpdate(A.adjoint());
}

//==============================================================================
/* solve_cpp(A, B) ----
 * solves Ax = B for x.
 */

// [[Rcpp:export]]
inline MatrixXd solve_cpp(const MatrixXd& A, const MatrixXd& B){
  return A.colPivHouseholderQr().solve(B);
}

/***R
## Test the inline function AtA()
M <- matrix(as.double(1:6), ncol = 2)
crossprod(M) # native R
AtA(M) # cpp way
*/

//==============================================================================
/* tinvchol_cpp(V, nugget = 0)
 * computes the choleskey decomposition upper triangle (U) of V
 * and then returns the transpose: t(U)
 *
 * if a nugget is included... (nothing yet)
 */

// [[Rcpp::export]]
MatrixXd tinvchol_cpp(const MapMatd& V, double nugget = 0.){

  double n = V.rows(); //dim of V

  if (abs(nugget) > 0) {
    // with nugget
    const MatrixXd M = (1 - nugget) * V.array() + nugget * MatrixXd::Identity(n,n).array();
    const MatrixXd Mn = M.matrix();
    const LLT<MatrixXd> llt(Mn);
    const MatrixXd tUinv = llt.matrixU().solve(MatrixXd::Identity(n, n)).adjoint();
    return tUinv;
  } else {
    // no nugget case
    const LLT<MatrixXd> llt(V); //chol decomp of V: (U)
    const MatrixXd tUinv = llt.matrixU().solve(MatrixXd::Identity(n, n)).adjoint();
    return tUinv;
  }
}

/***R
## Test tinvchol_cpp()
V <- crossprod(M)

# no nugget (i.e. nugget = 0)
t(backsolve(chol(V), diag(nrow(V)))) # native R
tinvchol_cpp(V) # cpp

# with nugget
nug = .1
Vn = (1 - nug) * V + nug * diag(nrow(V))
t(backsolve(chol(Vn), diag(nrow(Vn)))) # R
tinvchol_cpp(Vn, nug) # cpp
*/

//==============================================================================
/* fitGLS_cpp(X, V, y, X0, nugget = 0, threads = 1) ----
 *
 */

// [[Rcpp::export]]
List fitGLS_cpp(const MapMatd& X,
                const MapMatd& V,
                const MapMatd& y,
                const MapMatd& X0,
                double nugget = 0.,
                const int threads = 1){

  const int nX = X.rows(), pX = X.cols(); // dimensions of X
  const MatrixXd tUinv = tinvchol_cpp(V, nugget); // transpose of chol(V) = t(inv(U))
  const MatrixXd xx = tUinv * X; // t(inv(U)) %*% X
  const MatrixXd yy = tUinv * y; // t(inv(U)) %*% y
  const MatrixXd varX = AtA(xx); // crossprod(xx)
  const MatrixXd XtY(xx.adjoint() * yy); // crossprod(xx, yy)

  // solve for betahat (using one specific solver - there are others)
  const VectorXd betahat(varX.colPivHouseholderQr().solve(XtY));

  // calculate some statistics
  int dft = nX - xx.cols();
  const VectorXd SSE = AtA(yy - xx * betahat); // SSE
  const VectorXd MSE = SSE/dft; // MSE
  const MatrixXd varXinv = varX.colPivHouseholderQr().solve(
    MatrixXd::Identity(varX.rows(), varX.cols()));
  const MatrixXd varcov = varXinv.array() * MSE.array()[0];
  // cout << "\ndiag(vcov):\n" << varcov.matrix().diagonal() << endl; // this works

  // Vectorized operations
  VectorXd se = varcov.matrix().diagonal();
  for (int i = 0; i < se.size(); i++){
    se[i] = std::sqrt(se[i]);
  }

  VectorXd tstat(betahat.size());
  for (int i = 0; i < betahat.size(); i++){
    tstat[i] = betahat[i] / se[i];
  }

  double logDetV = 0;
  for (int i = 0; i < tUinv.rows(); i++){
    logDetV += log(tUinv.diagonal()[i]);
  }
  logDetV *= -2;

  double logLik = -0.5 * (nX * log(2 * M_PI) + nX *
                          log(dft * MSE.array()[0]/nX) + logDetV + nX);

  /* Null Model ----
   * The presence/absence of X0 should be handled entirely by R.
   * fitGLS_cpp() REQUIRES an X0 input.
   */

  const MatrixXd xx0 = tUinv * X0;
  const MatrixXd varX0 = AtA(xx0); // crossprod(xx0)
  const MatrixXd X0tY(xx0.adjoint() * yy); // crossprod(xx0, yy)
  const VectorXd betahat0(solve_cpp(varX0, X0tY));
  int df0 = betahat0.size();
  const VectorXd SSE0 = AtA(yy - xx0 * betahat0); // SSE
  double MSE0 = SSE0.array()[0]/(nX - xx.cols()); // MSE
  double MSR = (SSE0.array()[0] - SSE.array()[0])/(xx.cols() - xx0.cols());
  double logLik0 = -0.5 * (nX * log(2 * M_PI) + nX * log((nX - df0) * MSE0/nX) +
                           logDetV + nX);

  const MatrixXd varX0inv = varX0.colPivHouseholderQr().solve(
    MatrixXd::Identity(varX0.rows(), varX0.cols()));
  const MatrixXd varcov0 = MSE0 * varX0inv.array()

  VectorXd se0 = varcov0.matrix().diagonal();
  for (int i = 0; i < se0.size(); i++){
    se0[i] = std::sqrt(se0[i]);
  }

  /* F test ----
   *
   */
  const double FF = (nX - xx.cols())/(xx.cols() - xx0.cols()) *
    (SSE0.array()[0] - SSE.array()[0]) / SSE.array()[0];
  VectorXi dfF(2);
  dfF(0) = xx.cols() - xx0.cols();
  dfF(1) = nX - xx.cols();

  // return a list of all needed values
  return List::create(Named("betahat") = betahat,
                      Named("VarX") = varX,
                      Named("SSE") = SSE,
                      Named("MSE") = MSE,
                      Named("varcov") = varcov.matrix(),
                      Named("SE") = se,
                      Named("tstat") = tstat,
                      Named("pval.t") = NA_REAL,
                      Named("dft") = dft,
                      Named("logDetV") = logDetV,
                      Named("logLik") = logLik,
                      Named("betahat0") = betahat0,
                      Named("SE0") = se0,
                      Named("SSE0") = SSE0,
                      Named("SSR") = SSE0 - SSE,
                      Named("MSE0") = MSE0,
                      Named("MSR") = MSR,
                      Named("df0") = df0,
                      Named("logLik0") = logLik0,
                      Named("Fstat") = FF,
                      Named("pval.F") = NA_REAL,
                      Named("df.F") = dfF
                      );
}

/*** R
# ## test fitGLS_cpp()
load("../R/vignettes-and-examples/test-gls.rda", verbose = FALSE)
Xnull <- matrix(as.double(1), ncol = 1, nrow = nrow(X.small))
source("../R/fitGLS.R")

tmp.cpp <- fitGLS_cpp(X.small, V.small, y.small, Xnull)

tmp.R <- fitGLS(X.small, V.small, y.small)

## compare results
check.equal <- function(string){
  all.equal(unlist(unname(tmp.R[[string]])), unlist(tmp.cpp[[string]]))
}

sapply(c("betahat","VarX", "SSE", "MSE", "varcov", "SE", "t.stat",
         "df.t", "logDetV", "logLik", "betahat0", "SSE0", "MSE0", "MSR",
         "df0", "logLik0", "FF", "df.F"),
       check.equal)
*/


//==============================================================================
/* LogLikGLS(XX, V, y, nugget)
 *
 */

// [[Rcpp::export]]
inline double LogLikGLS_cpp(double nugget,
                        const MapMatd& X,
                        const MapMatd& V,
                        const MapVecd& y){

  const int nX = X.rows(); // dimensions of X
  const MatrixXd tUinv = tinvchol_cpp(V, nugget); // transpose of chol(V) = t(inv(U))
  const MatrixXd xx = tUinv * X; // t(inv(U)) %*% X
  const MatrixXd yy = tUinv * y; // t(inv(U)) %*% y
  const MatrixXd varX = AtA(xx); // crossprod(xx)
  const MatrixXd XtY(xx.adjoint() * yy); // crossprod(xx, yy)

  // solve for betahat (using one specific solver - there are others)
  const VectorXd betahat(varX.colPivHouseholderQr().solve(XtY));

  // calculate some statistics
  int dft = nX - xx.cols();
  const VectorXd SSE = AtA(yy - xx * betahat); // SSE
  const VectorXd MSE = SSE/dft; // MSE
  double logDetV = 0;
  for (int i = 0; i < tUinv.rows(); i++){
    logDetV += log(tUinv.diagonal()[i]);
  }
  logDetV *= -2;

  double logLik = -0.5 * (nX * log(2 * M_PI) + nX *
                          log(dft * MSE.array()[0]/nX) + logDetV + nX);

  return logLik;
}

/* Optimizer function
 * Currently this function uses the same algorithm that optimize() derived from.
 * This code is a translation of the fortran fmin algorithm:
 * http://www.netlib.org/fmm/fmin.f
 */

// [[Rcpp::export]]
double optimizeNugget_cpp(const MapMatd& X, const MapMatd& V, const MapVecd &y,
                          double lower = 0, double upper = 1, double tol = .00001,
                          bool debug = false){

  // varible declaration
  double ax = lower;
  double bx = upper;
  double a,b,c,d,e,eps,xm,p,q,r,tol1,tol2,u,v,w;
  double fu,fv,fw,fx,x, f_min;

  // c is the squared inverse of the golden ratio
  c = 0.5*(3. - sqrt(5.));

  // eps is approximately sqrt of machine precision
  // eps = std::numeric_limits<double>::epsilon();
  // eps = sqrt(eps);
  // cout << "eps_std = "<<eps<<endl;

  eps = 1.;
  lab0:
    eps = eps/2.;
    tol1 = 1. + eps;
    if (tol1 > 1.) {goto lab0;}
    eps = sqrt(eps);
    // cout << "eps =" << eps<< endl;

  // initialization
  a = ax;
  b = bx;
  v = a + c*(b - a);
  w = v;
  x = v;
  e = 0.;
  fx = -LogLikGLS_cpp(x, X, V, y); //invert function to minimize
  fv = fx;
  fw = fx;

  int i = 0;
  // main loop start
  lab1:
    // cout << "Loop Start (lab1): iteration " <<i<<endl;
    if (debug) {
    i += 1;
      if (i >= 10) {
        cout << "breaking loop, too many iterations"<<endl;
        goto lab8;
      }
    }
    xm = 0.5*(a + b);
    tol1 = eps*abs(x) + tol/3.;
    tol2 = 2.*tol1;
    // check stoping criteria
    if (abs(x - xm) <= (tol2 - 0.5*(b - a))) {goto lab8;}
    // cout << "stop crit. not met: "<<abs(x-xm)<<" > "<<tol2-.5*(b-a)<<endl;
    // is golden section necessary?
    if (abs(e) <= tol1) {goto lab3;}
    // fit parabola
    r = (x - w)*(fx - fv);
    q = (x - v)*(fx - fw);
    p = (x - v)*q - (x - w)*r;
    q = 2.*(q - r);
    if (q > 0.) {p = -p;}
    q =  abs(q);
    r = e;
    e = d;
  lab2:
    // cout << "check parabola (lab2)" << endl;
    // is parabola acceptable?
    if (abs(p) >= abs(0.5*q*r)) {goto lab3;}
    if (p <= q*(a - x)) goto lab3;
    if (p >= q*(b - x)) goto lab3;
    // parabolic interpolation step
    d = p/q;
    u = x + d;
    // f must not be evaluated too close to ax or bx
    if ((u - a) < tol2) {d = copysign(tol1, xm - x);}
    if ((b - u) < tol2) {d = copysign(tol1, xm - x);}
    goto lab4;
  lab3:
    // cout << "golden section step (lab3)" <<endl;
    // golden section step
    if (x >= xm) {e = a - x;}
    if (x < xm) {e = b - x;}
    d = c*e;
  lab4:
    // cout << "check tolerance and update vars (lab4)" << endl;
    //f must not be evaluated too close to x
    if (abs(d) >= tol1) {u = x + d;}
    if (abs(d) < tol1) {u = x + copysign(tol1, d);}
    fu = -LogLikGLS_cpp(u, X, V, y);
    //update  a, b, v, w, and x
    if (fu > fx) {goto lab5;}
    if (u >= x) {a = x;}
    if (u < x) {b = x;}
    v = w;
    fv = fw;
    w = x;
    fw = fx;
    x = u;
    fx = fu;
    goto lab1;
  lab5:
    // cout << "conditional variable reset (lab5)" << endl;
    if (u < x) {a = u;}
    if (u >= x) {b = u;}
    if (fu <= fw) {goto lab6;}
    if (w == x) {goto lab6;}
    if (fu <= fv) {goto lab7;}
    if (v == x) {goto lab7;}
    if (v == w) {goto lab7;}
    goto lab1;
  lab6:
    // cout << "update function results (lab6)" << endl;
    v = w;
    fv = fw;
    w = u;
    fw = fu;
    goto lab1;
  lab7:
    // cout << "update function results alterante (lab7)" << endl;
    v = u;
    fv = fu;
    goto lab1;
  // end of main loop
  lab8:
    // cout << "return statement (lab8)" << endl;
    f_min = x;
    if (ax + tol >= f_min){
      if (fx <= -LogLikGLS_cpp(f_min, X, V, y)){
        return ax;
      }
    }
    return f_min;
}

/***R
tol <- .00001
system.time(tmp1 <- fitNugget(X.small, V.small, y.small, c(0, 1), tol))
system.time(tmp2 <- optimizeNugget_cpp(X.small, V.small, y.small, 0, 1, tol))
system.time(tmp3 <- fitNugget_Rcpp(X.small, V.small, y.small, c(0,1), tol))

(vals <- c(tmp1, tmp2, tmp3))
# xs <- seq(0, 1, length.out = 20)
# fxs <- sapply(xs, function(x){LogLikGLS_cpp(x, X.small, V.small, y.small)})
# plot(fxs ~ xs);abline(v = vals, col = c("red", "green", "blue"), lty = 1:3)

*/
