/*===========================================*/
/* This is an example of getting the output  */
/* after Butterworth filter of exponential   */
/* input :                                   */
/* y(t) = h_3(t) * exp( -t/2 ),              */
/* where                                     */ 
/*  h_3(t) - BW of the order 3               */
/*  * - convolution                          */
/*===========================================*/
#include "../Butterworth.cpp"

#include <RooPlot.h>
#include <RooNumConvolution.h>
#include <RooCFunction1Binding.h>
#include <RooTFnBinding.h>

#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TF1.h>


using namespace RooFit;


Double_t Exponent( Double_t t )
{
    return (t < 0.) ? 0. : TMath::Exp( -t / 2 );
}


Double_t Theory( Double_t t )
{
    // This function is analytical expression of output
    if( t < 0. ) return 0.;
    return TMath::Exp(  -t / 2 ) * ( 2. - 2. * TMath::Exp( -t / 2 ) - 4. / 3. * TMath::Sin( TMath::Sqrt( 3 ) / 2 * t + TMath::Pi() / 6 ) + 2./3. ); 
}


void Convolution()
{
    RooRealVar t( "t", "t", -1, 20 );
    RooRealVar omegaC( "omegaC", "omegaC", 1 );

    Butterworth bw3( "bw3", "bw3", t, omegaC, 3 );
    RooAbsReal *expo = bindFunction( "expo", Exponent, t );
    RooAbsReal *theory = bindFunction( "theory", Theory, t );

    RooNumConvolution conv( "conv", "conv", t, bw3, *expo );

    RooPlot *tFrame = t.frame();
    tFrame->SetTitle( "y(t) = h_{3}(t) * e^{-#frac{t}{2}}" );
    tFrame->GetYaxis()->SetTitle( "y(t)" );

    TCanvas *c = new TCanvas( "c", "c", 800, 600 );
    c->SetGrid();

    //bw3.plotOn( tFrame, LineColor( 46 ), Name( "bw3" ) );
    expo->plotOn( tFrame, LineColor( 46 ), Name( "exp" ) );
    conv.plotOn( tFrame, LineColor( 47 ), Name( "numerical" ) );
    theory->plotOn( tFrame, LineColor( kBlack ), LineStyle( kDashed ), Name( "theory" ) );

    tFrame->Draw();

    TLegend *leg = new TLegend( 0.8, 0.7, 0.6, 0.85 );
        leg->AddEntry( "exp", "input", "l" );
        leg->AddEntry( "theory", "output (theory)", "l" );
        leg->AddEntry( "numerical", "output (num)", "l" );

    leg->Draw();
}
