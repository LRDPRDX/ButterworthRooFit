#include "../Butterworth.cpp"

#include <RooPlot.h>
#include <RooNumConvolution.h>
#include <RooCFunction1Binding.h>
#include <RooTFnBinding.h>

#include <TStyle.h>
#include <TLegend.h>
#include <TMath.h>
#include <TAxis.h>
#include <TF1.h>
#include <TGraph.h>
#include <TCanvas.h>


using namespace RooFit;


const Double_t T_MAX = 100;

Double_t Sine( Double_t *x, Double_t *p )
{
    Double_t t = x[0];
    Double_t w = p[0];

    return ((t < 0.) || (t > 0.9*T_MAX)) ? 0. : TMath::Sin( w * t );
}


void Frequency()
{
    RooRealVar t( "t", "t", -1, T_MAX );
    RooRealVar omegaC( "omegaC", "omegaC", 2 );
    Butterworth bw = Butterworth( "bw", "h_{bw}(t)", t, omegaC, 3 );

    TF1 *fSin = new TF1( "fSin", Sine, -1, T_MAX, 1 );
    RooRealVar omega( "omega", "#omega", 0.1, 0.1, 10 );
    RooTFnBinding sine( "Sine", "Sine", fSin, t, omega );
    RooNumConvolution out( "out", "out", t, bw, sine );

    UInt_t nPoints = 50;
    TGraph *g = new TGraph( nPoints );
        g->SetTitle( "The magnitude of the frequency response of BW of the order 3" );
        g->SetMarkerStyle( kCircle );
        g->GetXaxis()->SetTitle( "#omega, [rad/s]" );
        g->GetYaxis()->SetTitle( "G(#omega)" );

    for( UInt_t i = 0; i < nPoints; i++ )
    {
        omega.setVal( 0.1 + 0.1 * i );

        TF1 *outTF = out.asTF( t, omega );
        g->SetPoint( i, omega.getVal(), outTF->GetMaximum( 0.2*T_MAX, 0.8*T_MAX ) );
    }

    TF1 *fTheory = new TF1( "fTheory", "1./sqrt(1.+pow(x/[0],2*[1]))", 0, 100 );
        fTheory->SetParNames( "#hat{#omega}_{c}", "#hat{n}" );
        fTheory->SetParameter( 0, 2 );
        fTheory->SetParLimits( 0, 1, 3 );
        fTheory->SetParameter( 1, 3 );
        fTheory->SetParLimits( 1, 1, 10 );

    TCanvas *c = new TCanvas( "c", "c", 800, 600 );
        c->SetGrid();

    gStyle->SetOptFit( 1 );

    g->Fit( fTheory, "R" );
    g->Draw("AP");
    fTheory->Draw("same");

    TLegend *leg = new TLegend( 0.8, 0.7, 0.6, 0.85 );
        leg->AddEntry( g, "Numerical", "p" );
        leg->AddEntry( fTheory, "Fit", "l" );

    leg->Draw();
}
