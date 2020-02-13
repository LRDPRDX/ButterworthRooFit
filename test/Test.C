#include "../Butterworth.C" //use this to run this example as a named macro
//#include "../Butterworth.h"//use this when to compile

#include <RooPlot.h>
#include <RooCFunction1Binding.h>

#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TMath.h>

using namespace RooFit;


Double_t BW3( Double_t t )
{
    // h(t) = exp( -t ) - 2/sqrt(3) * exp( -t/2 ) * cos( sqrt(3)/2 * t + pi/6 )
    if( t > 0. )
    {
        return TMath::Exp( -t ) - TMath::Exp( -t / 2 ) * TMath::Cos( TMath::Sqrt(3) * t / 2 + TMath::Pi() / 6 ) * 2 / TMath::Sqrt(3);
    }
    return 0.;
}


void Test()
{
    RooRealVar t( "t", "t", -1, 40 );
    RooRealVar omegaC( "omegaC", "omegaC", 1 );

    RooPlot *tFrame = t.frame();
    tFrame->SetTitle( "Butterworth transfer functions of different order in t-domain" );
    tFrame->GetYaxis()->SetTitle( "h(t)" );

    // Draw several BW functions of different order
    for( UInt_t i = 2; i <= 10; i++ )
    {
        Butterworth *bw = new Butterworth( "bw", "h_{bw}(t)", t, omegaC, i );
        bw->plotOn( tFrame, LineColor( 40+i ), Range( -1, 40 ), Name( Form( "n=%d", i ) ) );
    }

    TCanvas *c = new TCanvas( "c", "c", 800, 600 );
    c->Divide( 1, 2 );
    c->cd( 1 );
    gPad->SetGrid();

    tFrame->Draw();

    TLegend *leg = new TLegend( 0.80, 0.87, 0.85, 0.32 );
    for( UInt_t i = 2; i <= 10; i++ )
    {
        leg->AddEntry( Form( "n=%d", i ), Form( "n=%d", i ), "l" );
    }
    leg->Draw( "same" );

    // Compare theory and numerical result 
    Butterworth bw3( "bw3", "h(t)", t, omegaC, 3 );
    RooAbsReal *bw3Theory = bindFunction( "bw3theory", BW3, t );

    RooPlot *tFrame2 = t.frame();
    tFrame2->SetTitle( "Theory vs. Numerical" );
    tFrame2->GetYaxis()->SetTitle( "h(t)" );

    bw3.plotOn( tFrame2, LineColor( 46 ), Range( -1, 15 ), Name("numerical") );
    bw3Theory->plotOn( tFrame2, LineColor( kBlack ), LineStyle( kDashed ), Range( -1, 15 ), Name("theory") );

    c->cd( 2 );
    gPad->SetGrid();

    tFrame2->Draw();

    TLegend *leg2 = new TLegend( 0.7, 0.87, 0.85, 0.5 );
    leg2->AddEntry( "numerical", "numerical", "l" );
    leg2->AddEntry( "theory", "theory", "l" );
    leg2->Draw( "same" );
}
