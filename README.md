
<!-- README.md is generated from README.Rmd. Please edit that file -->

# remotePARTS

<!-- badges: start -->

[![License: GPL
v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

<!-- [![Lifecycle: maturing](https://img.shields.io/badge/lifecycle-maturing-blue.svg)](https://www.tidyverse.org/lifecycle/#maturing) -->

[![Lifecycle:
stable](https://img.shields.io/badge/lifecycle-stable-green.svg)](https://lifecycle.r-lib.org/articles/stages.html#stable)

<!-- [![Travis build status](https://travis-ci.com/morrowcj/remotePARTS.svg?branch=master)](https://travis-ci.com/morrowcj/remotePARTS) -->
<!-- [![Travis build status](https://travis-ci.com/morrowcj/remotePARTS.svg?branch=master)](https://travis-ci.org/github/morrowcj/remotePARTS) -->

[![R-CMD-check](https://github.com/morrowcj/remotePARTS/workflows/R-CMD-check/badge.svg)](https://github.com/morrowcj/remotePARTS/actions)
<!-- badges: end -->

`remotePARTS` is an `R` package that contains tools for analyzing
spatiotemporal data, typically obtained via remote sensing.

## Description

These tools were created to test map-scale hypotheses about trends in
large remotely sensed data sets but any data with spatial and temporal
variation can be analyzed. Tests are conducted using the PARTS method
for analyzing spatially autocorrelated time series (Ives et al., 2021).
The method’s unique approach can handle extremely large data sets that
other spatiotemporal models cannot, while still appropriately accounting
for spatial and temporal autocorrelation. This is done by partitioning
the data into smaller chunks, analyzing chunks separately and then
combining the separate analyses into a single, correlated test of the
map-scale hypotheses.

## Instalation

To install the package and it’s dependencies, use the following R code:

``` r
install.packages("remotePARTS")
```

To install the latest development version of this package from github,
use

``` r
install.packages("devtools") # ensure you have the latest devtools
devtools::install_github("morrowcj/remotePARTS")
```

Then, upon successful installation, load the package with
`library(remotePARTS)`.

The latest version of
[Rtools](https://cran.r-project.org/bin/windows/Rtools/) is required for
Windows and C++11 is required for other systems.

## Example usage

For examples on how to use `remotePARTS`, see the `Alaska` vignette:

``` r
vignette("Alaska")
```

Note that the vignette needs to be built when installing with and may
require the `build_vignettes = TRUE` argument when installing with
`install_github()`.

If you’re having trouble installing or building the package, you may
need to double check that the R build tools are properly installed on
your machine: [official Rstudio development
prerequisites](https://support.rstudio.com/hc/en-us/articles/200486498-Package-Development-Prerequisites)
To do this, use `pkgbuild::has_build_tools(debug = TRUE)` and
`pkgbuild::check_build_tools(debug = TRUE)` to unsure that your build
tools are up to date.

The vignette is also available online:
<https://morrowcj.github.io/remotePARTS/Alaska.html>.

# References

Ives, Anthony R., et al. “Statistical inference for trends in
spatiotemporal data.” Remote Sensing of Environment 266 (2021): 112678.
<https://doi.org/10.1016/j.rse.2021.112678>
