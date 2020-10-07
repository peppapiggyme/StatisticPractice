#include "Exercise_5.h"
#include "ExUtils.h"
#include "Utils.h"

#include <iostream>

/**
 * @brief RooStats example: workspace, modelCofig
 *
 * Statistic: parameter estimation, confidence interval,
 * calculators: PL, Bayesian, MCMC
 *
 * Adapt from Wouter Verkerke's slides
 * Advanced Statistics Course (NIKHEF)
 *
 * @name Ex5
 * @author Bowen Zhang
 * @date 2020/10/07
 *
 * @todo I need to understand it and maybe write a better confidence
 * interval example instead of this gaussian mean estimator ..
 *
 */

#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/SimpleInterval.h"
#include "RooStats/MCMCInterval.h"

using namespace std;

void Exercise_5::test() const
{
    // Create a new workspace
    // ----------------------
    RooWorkspace* ws = new RooWorkspace();

    // Create pdf Gauss(x | mu, 1) and variable x, mu, sigma using the factory syntax
    // ------------------------------------------------------------------------------
    ws->factory("Gaussian::normal(x[-10,10], mu[-1,1], sigma[1])");

    // Define parameter sets for POI and observables
    // ---------------------------------------------
    ws->defineSet("poi", "mu");
    ws->defineSet("obs", "x");

    // Create a new ModelConfig
    // ------------------------
    RooStats::ModelConfig* modelConfig = new RooStats::ModelConfig("G(x|mu,1)");
    modelConfig->SetWorkspace(*ws);
    modelConfig->SetPdf(*ws->pdf("normal"));
    modelConfig->SetParametersOfInterest(*ws->set("poi"));
    modelConfig->SetObservables(*ws->set("obs"));

    // Create a toy dataset
    // --------------------
    RooDataSet* data = ws->pdf("normal")->generate(*ws->set("obs"), 100);

    // Set confidence level
    // --------------------
    double nCL = 0.95;

    // Example use of profile likelihood calculator
    // --------------------------------------------
    SP::IO::println("\n\nProfileLikelihoodCalculator Demo\n");
    RooStats::ProfileLikelihoodCalculator plc(*data, *modelConfig);
    plc.SetConfidenceLevel(nCL);
    RooStats::LikelihoodInterval* plInt = plc.GetInterval();

    // Example use of Bayesian calculator
    // ----------------------------------
    SP::IO::println("\n\nBayesianCalculator Demo\n");
    ws->factory("Uniform::prior(mu)");
    modelConfig->SetPriorPdf((*ws->pdf("prior"))); // needs to specify a prior
    RooStats::BayesianCalculator bc(*data, *modelConfig);
    bc.SetConfidenceLevel(nCL);
    RooStats::SimpleInterval* bcInt = bc.GetInterval();

    // Example use of MCMC calculator
    // ------------------------------
    SP::IO::println("\n\nMCMCCalculator Demo\n");
    RooStats::MCMCCalculator mc(*data, *modelConfig);
    mc.SetConfidenceLevel(nCL);
    mc.SetNumBins(1000);
    mc.SetNumBurnInSteps(500);
    mc.SetNumIters(100000);
    RooStats::MCMCInterval* mcInt = mc.GetInterval();

    RooRealVar* mu = (RooRealVar*)modelConfig->GetParametersOfInterest()->first();
    SP::IO::println("pl interval is [%, %]", plInt->LowerLimit(*mu), plInt->UpperLimit(*mu));
    SP::IO::println("bc interval is [%, %]", bcInt->LowerLimit(), bcInt->UpperLimit());
    SP::IO::println("mc interval is [%, %]", mcInt->LowerLimit(*mu), mcInt->UpperLimit(*mu));

    delete ws;
    delete modelConfig;
}
