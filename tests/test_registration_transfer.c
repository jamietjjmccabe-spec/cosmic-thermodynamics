#include "registration_transfer.h"
#include <math.h>
#include <stdio.h>
#define CHECK(c,m) do{if(!(c)){fprintf(stderr,"FAIL: %s\n",m);return 1;}}while(0)
int main(int argc,char **argv){
 const char *path=(argc>1)?argv[1]:"data/sm_thermodynamics_husdal2016.csv"; char error[256]={0}; registration_eos_table table;
 CHECK(registration_eos_load_csv(path,&table,error,sizeof(error))==0,error);
 registration_transfer_parameters p={820.6438184870336,2.0,1e-14};
 double peak_t=0,peak_a=-1;
 for(size_t i=0;i+1<table.size;i++) for(int j=0;j<64;j++){
  double f=(j+.5)/64.0,t=exp((1-f)*table.log_temperature[i]+f*table.log_temperature[i+1]); registration_thermo_state s;
  CHECK(registration_eos_evaluate(&table,t,p.trace_power,p.trace_floor,&s,error,sizeof(error))==0,error);
  if(s.activation>peak_a){peak_a=s.activation;peak_t=t;}
 }
 CHECK(peak_t>0.13&&peak_t<0.18,"n=2 activation is not QCD-centred");
 const double dN=1e-6,shift=2e-5,T=.171,H=2.3e5,rho=7e10,ts=-.83,hs=-1.97,rs=-.31;
 registration_transfer_state c,plus,minus;
 CHECK(registration_transfer_evaluate(&table,&p,T,H,rho,hs*shift,rs*shift,ts*shift,&c,error,sizeof(error))==0,error);
 CHECK(registration_transfer_evaluate(&table,&p,T*exp(ts*dN),H*exp(hs*dN),rho*exp(rs*dN),0,0,0,&plus,error,sizeof(error))==0,error);
 CHECK(registration_transfer_evaluate(&table,&p,T*exp(-ts*dN),H*exp(-hs*dN),rho*exp(-rs*dN),0,0,0,&minus,error,sizeof(error))==0,error);
 double fd=(log(plus.Q)-log(minus.Q))/(2*dN),pred=c.delta_Q_over_Q/shift,external=hs+rs;
 CHECK(fabs(pred-fd)/fabs(fd)<2e-6,"covariant deltaQ failed time-shift identity");
 CHECK(fabs(pred-external)>.5,"external-clock control not distinguishable");
 printf("PASS\nrows=%zu\nactivation_peak_T_GeV=%.9g\nactivation_peak=%.9g\ndlnQ_dN_covariant=%.12g\ndlnQ_dN_finite_difference=%.12g\ndlnQ_dN_external_clock=%.12g\n",table.size,peak_t,peak_a,pred,fd,external);
 registration_eos_free(&table); return 0;
}
