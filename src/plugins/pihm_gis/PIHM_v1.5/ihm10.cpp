/*******************************************************************************
 * File        : ihm10.c                                                       *
 * Function    : This is main entrance of IHM 1.0                              *
 * Programmer  : Yizhong Qu @ Pennsylvania State Univeristy                    *
 * Version     : May, 2004 (1.0)                                               * 
 *-----------------------------------------------------------------------------*
 *                                                                             *
 * IHM1.0 is an implementation of Yizhong Qu's PhD Thesis. It is an integrated *
 * finite volume hydrologic model. It simulates channel routing, overland flow *
 * and groundwater flow in full coupled scheme. It uses semi-discrete approach *
 * to discretize PDE into ODE, and solved it with SUNDIAL (LLNL) package.      *
 *                                                                             *
 * Reference                                                                   *
 *                                                                             *
 * 1.Yizhong Qu, An Integrated Hydrological Model Using Semi-Discrete Finite   *
 *               Volume Formulation, PhD Thesis, the Pennsylvanian State       *
 *               University, 2004                                              *
 * 2.Yizhong Qu, Christopher Duffy, Toward An Integrate Hydrological Model For *
 *               River Basins: A Semi-Discrete, Finite Volume Formulation      *
 *                                                                             *
 * This code is free for users with research purpose only, if appropriate      *
 * citation is refered. However, there is no warranty in any format for this   *
 * product.                                                                    *
 *                                                                             *
 * For questions or comments, please contact the authors of the reference.     *
 * One who want to use it for other consideration may also contact Dr.Duffy    *
 * at cxd11@psu.edu.                                                           *
 *                                                                             *      
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <iostream>

/* SUNDIAL Header Files */
#include "sundials_types.h"   /* realtype, integertype, booleantype defination */
#include "cvode.h"           /* CVODE header file                             */
#include "cvode_spgmr.h"         /* CVSPGMR linear header file                    */
#include "sundials_smalldense.h"      /* use generic DENSE linear solver for "small"   */
#include "nvector_serial.h"  /* contains the definition of type N_Vector      */
#include "sundials_math.h"    /* contains UnitRoundoff, RSqrt, SQR functions   */
#include "cvode_dense.h"         /* CVDENSE header file                           */
#include "sundials_dense.h"           /* generic dense solver header file              */

/* IHM Header Files */
#include "ihm.h"             /* Definations for all date Structure in IHM     */
#include "calib.h"
#include "et_is.h"
#include "ihm10.h"
#include "print_functions.h"

#include "initialize.h"
#include "read_alloc.h"
#include "f.h"
#include <QtGui/QProgressBar>
#include "progress.h"

using namespace std;


/* Function to calculate right hand side of ODE systems */
//void initialize(char *, Model_Data, Control_Data *, N_Vector);
//void calET(realtype, realtype, N_Vector, Model_Data);
//void calIS(realtype, realtype, Model_Data, N_Vector);
//int f(realtype, N_Vector, N_Vector, void *);
//void read_alloc(char *, Model_Data, Control_Data *);

realtype CS_AreaOrPerem1(int rivOrder, realtype rivDepth, realtype rivCoeff, realtype a_pBool);
realtype OverlandFlow1( int loci, int locj, int surfmode, realtype avg_y, realtype grad_y, realtype avg_sf, realtype alfa, realtype beta, realtype crossA, realtype avg_rough, int eletypeBool, realtype avg_perem);
void printRiverFlux(Model_Data, N_Vector, FILE *res_flux_file);



int satEle, ovrEle;
  
/* Main Function */
int ihm10(int argc, char *argv[], QProgressBar* bar)
{ 
  realtype Tsteps; 
  //char *filename = "sc";        /* Input file name prefix    */
  char *outputInitFile;
  char *calibFileName;
  char *printFunctionsFileName;
  char *filename;
  char *StateFile;                /* Output file name string   */
  char *FluxFile;
  char *ETISFile;  
  char *QFile;  
  char c_char;  
  Model_Data mData;               /* Model Data                */
  Control_Data cData;             /* Solver Control Data       */
  
  N_Vector CV_Y;                  /* State Variables Vector    */
  
/*  realtype ropt[OPT_SIZE]; */        /* Optional real type and integer type  */
/*  long int iopt[OPT_SIZE];     */   /* vecter for Message Passing to solver */
  
  void *cvode_mem;                /* Model Data Pointer        */
  int flag;                       /* flag to test return value */
  
  FILE *res_state_file;           /* Output file for States    */
  FILE *res_flux_file;            /* Output file for Flux      */
  FILE *res_etis_file;            /* Output file for ET and IS */
  FILE *res_q_file;
  FILE *fpInit;

  //FILE *headFile;
  //FILE *overFile;
  //FILE *baseFile; 
  //FILE *timeFile;

//FILE *hFile;
realtype *head;

//FILE *prepFile;
realtype tempPrep=0.0;
realtype tempNetPrep=0.0;
realtype tempET0 = 0.0;
realtype tempET1 = 0.0;
realtype tempET2 = 0.0;
realtype tempTF = 0.0;
int tempI;

  int N;                          /* Problem size              */
  int i,j,k;                          /* loop index                */
  realtype t;                     /* simulation time           */
  realtype NextPtr, StepSize;     /* stress period & step size */
  
  clock_t start, end_r, end_s;    /* system clock at points    */
  realtype cputime_r, cputime_s;  /* for duration in realtype  */

  realtype totbase1=0,totbase2=0,totbase3,totbase4;
 
  int ctr3,ctr4; 
  realtype massSurf, massRiv, massSat, massUnSat,massP=0, massET1=0, massET2=0, massET3=0,NetMass,massQ=0,massIS,massSnow,massRecharge,massSurfC, massRivC, massSatC, tmpArea=0;

  FILE *fpMassBal;
  char *fpMassBalFile;

  char tmpFileName[200];
  setFileName(tmpFileName);
  filename = (char *)malloc(sizeof(char)*strlen(tmpFileName));
  strcpy(filename, tmpFileName);

  /* allocate memory for model data structure */
  mData = (Model_Data)malloc(sizeof *mData);
  start = clock();
	
  
  /* get user specified file name in command line */
  if(argc >= 2)
  {
    filename = (char *)malloc(strlen(argv[1])*sizeof(char));
    strcpy(filename, argv[1]);
  }  

  //hFile = fopen("sc.h", "w");
  //prepFile = fopen("sc.prep", "w");

  fpMassBalFile = (char *)malloc((strlen(tmpFileName)+5)*sizeof(char));
  strcpy(fpMassBalFile, tmpFileName);
  strcat(fpMassBalFile,".MB");
  //fpMassBal=fopen(fpMassBalFile,"w");
  printf("\nBelt up!  PIHM 2.0 is starting ... \n");


	/* NOTE : DONE BEFORE READ_ALLOC AS THE VARIABLES ARE USED IN READ_ALLOC */
	calibFileName=(char *)malloc((strlen(tmpFileName)+10)*sizeof(char));
	strcpy(calibFileName, filename);
	strcat(calibFileName, ".calib");
	initCalibFile(calibFileName);


  /* read in 7 input files with "filename" as prefix */
  read_alloc(filename, mData, &cData); 


	printFunctionsFileName=(char *)malloc((strlen(tmpFileName)+10)*sizeof(char));
	strcpy(printFunctionsFileName, filename);
	strcat(printFunctionsFileName, ".prn");
	initPrintFunctionsFile(printFunctionsFileName);



  //headFile=fopen("sc.head","w");
  //overFile=fopen("sc.over","w");
  //baseFile=fopen("sc.base","w");
  //timeFile=fopen("sc.time","w");

if(mData->UnsatMode ==1)
	{    
  /* problem size */
  N = 2*mData->NumEle + mData->NumRiv;
  	}
if(mData->UnsatMode ==2)
	{    
  /* problem size */
  N = 3*mData->NumEle + mData->NumRiv;
  printf("\ndefinately here\n");
  	}  	
  
  /* initial state variable depending on machine*/
  CV_Y = N_VNew_Serial(N);
  
  /* initialize mode data structure */
  initialize(filename, mData, &cData, CV_Y);

  FPrintInit(mData);

  /*for (i=0; i<mData->NumEle; i++)
	printf("%lf\t%lf\t%lf\t%lf\n",mData->Ele[i].zmin, mData->Ele[i].zmax, NV_Ith_S(CV_Y, i+mData->NumEle), NV_Ith_S(CV_Y, i+2*mData->NumEle));
  getchar();*/
 
	head=(realtype *)malloc(mData->NumEle*sizeof(realtype));

  if(cData.Debug == 1) {PrintModelData(mData);}  
  
  end_r = clock();
  cputime_r = (end_r - start)/(realtype)CLOCKS_PER_SEC;
  
  printf("\nSolving ODE system ... \n");
  
  /* initial control parameter for CVODE solver. Otherwise the default value by C could cause problems. */
 /* for(i=0; i<OPT_SIZE; i++)
  {
    ropt[i] = 0.0;
    iopt[i] = 0;
  }
  */
 
  /* allocate memory for solver */
  cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
  if(cvode_mem == NULL) {printf("CVodeMalloc failed. \n"); return(1);}
  
  flag = CVodeSetFdata(cvode_mem, mData);
  flag = CVodeSetInitStep(cvode_mem,cData.InitStep);
  flag = CVodeSetStabLimDet(cvode_mem,TRUE);
  flag = CVodeSetMaxStep(cvode_mem,cData.MaxStep);
  flag = CVodeMalloc(cvode_mem, f, cData.StartTime, CV_Y, CV_SS, cData.reltol, &cData.abstol);


  flag = CVSpgmr(cvode_mem, PREC_NONE, 0);
  flag = CVSpilsSetGSType(cvode_mem, MODIFIED_GS);
//CVSpilsSetGSType

/* if(cData.Solver == 1)
  {
    /* using dense direct solver 
    flag = CVDense(cvode_mem, NULL, NULL);
    if(flag != SUCCESS) {printf("CVDense failed. \n"); return(1);} 
  } 
  else if(cData.Solver == 2)
  {
    /* using iterative solver 
    flag = CVSpgmr(cvode_mem, NONE, cData.GSType, cData.MaxK, cData.delt, NULL, NULL,
                       mData, NULL, NULL);
    if (flag != SUCCESS) {printf("CVSpgmr failed."); return(1);}
  } */
  
  /*allocate and copy to get output file name */
  StateFile = (char *)malloc((strlen(filename)+4)*sizeof(char));
  strcpy(StateFile, filename);
  FluxFile  = (char *)malloc((strlen(filename)+5)*sizeof(char));
  strcpy(FluxFile, filename);
  ETISFile  = (char *)malloc((strlen(filename)+5)*sizeof(char));
  strcpy(ETISFile, filename);
  QFile = (char *)malloc((strlen(filename)+2)*sizeof(char));
  strcpy(QFile, filename);
  
  /* open output file */
  if (cData.res_out == 1) {res_state_file = fopen(strcat(StateFile, ".res"), "w");}
  if (cData.flux_out == 1) {res_flux_file = fopen(strcat(FluxFile, ".flux"), "w");}
  if (cData.etis_out == 1) {res_etis_file = fopen(strcat(ETISFile, ".etis"), "w");}
  if (cData.q_out == 1) {res_q_file = fopen(strcat(QFile, ".q"), "w");}
  
  /* print header of output file */
  if (cData.res_out == 1) {FPrintYheader(res_state_file, mData);}
  if (cData.etis_out == 1) {FPrintETISheader(res_etis_file, mData);}
  if (cData.q_out == 1) {FPrintETISheader(res_q_file, mData);}
  printf("\n");
  
  /* set start time */
  t = cData.StartTime;
  
  /* start solver in loops */
  for(i=0; i<cData.NumSteps; i++)
  {
/*    if (cData.Verbose != 1)
    {
      printf("  Running: %-4.1f%% ... ", (100*(i+1)/((realtype) cData.NumSteps))); 
      fflush(stdout);
    } */

    /* inner loops to next output points with ET step size control */
    while(t < cData.Tout[i+1])
    {
      if (t + cData.ETStep >= cData.Tout[i+1])
      {
        NextPtr = cData.Tout[i+1];
      }
      else
      {
        NextPtr = t + cData.ETStep;
      }
      StepSize = NextPtr - t; 
      
      /* calculate Interception Storage */
      calIS(t, StepSize, mData,CV_Y);

/******************************************************************************************/
	//tempNetPrep = 0.0;
	//tempPrep    = 0.0;
	//tempET0     = 0.0;
	//tempTF      = 0.0;
	/*for(tempI=0; tempI<mData->NumEle; tempI++){
		tempPrep += mData->ElePrep[tempI];
		tempNetPrep += mData->EleNetPrep[tempI];
		tempET0 += mData->EleET[tempI][0]*mData->Ele[tempI].VegFrac;
		tempET1 += mData->EleET[tempI][1];
		tempET2 += mData->EleET[tempI][2];
		tempTF  += mData->EleTF[tempI]*mData->Ele[tempI].VegFrac;
	}
	fprintf(prepFile, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", t, tempPrep/mData->NumEle, tempNetPrep/mData->NumEle, tempET0/mData->NumEle, tempET1/mData->NumEle, tempET2/mData->NumEle,tempTF/mData->NumEle);//fflush(prepFile);
*/
/******************************************************************************************/

	Tsteps=t;
printf("\n Tsteps = %f ",t);
      flag = CVode(cvode_mem, NextPtr, CV_Y, &t, CV_NORMAL);  
	setProgressBar(bar, 100*(i+1)/((realtype) cData.NumSteps));


   for(k=0; k<mData->NumPrep; k++){
        while(mData->TSD_Prep[k].iCounter < mData->TSD_Prep[k].length && t/(24.0*60.0) > mData->TSD_Prep[k].TS[mData->TSD_Prep[k].iCounter+1][0]){
                mData->TSD_Prep[k].iCounter++;
        }
   }
   for(k=0; k<mData->NumTemp; k++){
        while(mData->TSD_Temp[k].iCounter < mData->TSD_Temp[k].length && t/(24.0*60.0) > mData->TSD_Temp[k].TS[mData->TSD_Temp[k].iCounter+1][0]){
                mData->TSD_Temp[k].iCounter++;
        }
   }
   for(k=0; k<mData->NumHumidity; k++){
        while(mData->TSD_Humidity[k].iCounter < mData->TSD_Humidity[k].length && t/(24.0*60.0) > mData->TSD_Humidity[k].TS[mData->TSD_Humidity[k].iCounter+1][0]){
                mData->TSD_Humidity[k].iCounter++;
        }
   }
   for(k=0; k<mData->NumWindVel; k++){
        while(mData->TSD_WindVel[k].iCounter < mData->TSD_WindVel[k].length && t/(24.0*60.0) > mData->TSD_WindVel[k].TS[mData->TSD_WindVel[k].iCounter+1][0]){
                mData->TSD_WindVel[k].iCounter++;
        }
   }
   for(k=0; k<mData->NumRn; k++){
        while(mData->TSD_Rn[k].iCounter < mData->TSD_Rn[k].length && t/(24.0*60.0) > mData->TSD_Rn[k].TS[mData->TSD_Rn[k].iCounter+1][0]){
                mData->TSD_Rn[k].iCounter++;
        }
   }
   for(k=0; k<mData->NumG; k++){
        while(mData->TSD_G[k].iCounter < mData->TSD_G[k].length && t/(24.0*60.0) > mData->TSD_G[k].TS[mData->TSD_G[k].iCounter+1][0]){
                mData->TSD_G[k].iCounter++;
        }
   }
   for(k=0; k<mData->NumP; k++){
        while(mData->TSD_Pressure[k].iCounter < mData->TSD_Pressure[k].length && t/(24.0*60.0) > mData->TSD_Pressure[k].TS[mData->TSD_Pressure[k].iCounter+1][0]){
                mData->TSD_Pressure[k].iCounter++;
        }
   }
   for(k=0; k<mData->NumLC; k++){
        while(mData->TSD_LAI[k].iCounter < mData->TSD_LAI[k].length && t/(24.0*60.0) > mData->TSD_LAI[k].TS[mData->TSD_LAI[k].iCounter+1][0]){
                mData->TSD_LAI[k].iCounter++;
        }
   }
   for(k=0; k<mData->NumLC; k++){
        while(mData->TSD_DH[k].iCounter < mData->TSD_DH[k].length && t/(24.0*60.0) > mData->TSD_DH[k].TS[mData->TSD_DH[k].iCounter+1][0]){
                mData->TSD_DH[k].iCounter++;
        }
   }
   for(k=0; k<mData->NumMeltF; k++){
        while(mData->TSD_MeltF[k].iCounter < mData->TSD_MeltF[k].length && t/(24.0*60.0) > mData->TSD_MeltF[k].TS[mData->TSD_MeltF[k].iCounter+1][0]){
                mData->TSD_MeltF[k].iCounter++;
        }
   }
   for(k=0; k<mData->NumSource; k++){
        while(mData->TSD_Source[k].iCounter < mData->TSD_Source[k].length && t/(24.0*60.0) > mData->TSD_Source[k].TS[mData->TSD_Source[k].iCounter+1][0]){
                mData->TSD_Source[k].iCounter++;
        }
   }






/**********************************************MASS BALANCE*****************************************************/

        massSurf=0;
        massRiv=0;
        massSat=0;
        massUnSat=0;
        massIS=0;
        massRecharge=0;
        massSnow=0;
        tmpArea=0;

/*      massNetP=0;
        massET=0; */
      for(j=0;j<N;j++)
      {
        if(j<mData->NumEle)
        {
		head[j]=head[j]+(NV_Ith_S(CV_Y,j)>0.0?NV_Ith_S(CV_Y,j):0.0);
                
                massSurf=massSurf+((NV_Ith_S(CV_Y, j)<0?0:NV_Ith_S(CV_Y, j))-mData->Ele_IC[j].surf)*mData->Ele[j].area;
/*              if(NV_Ith_S(CV_Y, j)>0)
                {                printf("\n%d %e %e %e %e %e %e",j,mData->EleVic[j],mData->EleNetPrep[j],NV_Ith_S(CV_Y, j),NV_Ith_S(CV_Y, j+mData->NumEle),NV_Ith_S(CV_Y, j+2*mData->NumEle),mData->Ele[j].zmax-mData->Ele[j].zmin);
                }
*/              massP=massP+mData->EleNetPrep[j]*mData->Ele[j].area;
/*              massET=massET+(mData->EleET[j][1]+mData->EleET[j][0])*mData->Ele[j].area; */
                massET1=massET1+(mData->EleET[j][0])*mData->Ele[j].area;
                massET2=massET2+(mData->EleET[j][1])*mData->Ele[j].area;
                massET3=massET3+(NV_Ith_S(CV_Y, j+2*mData->NumEle)<0?0:NV_Ith_S(CV_Y, j+2*mData->NumEle))*(mData->EleET[j][2])*mData->Ele[j].area/(mData->Ele[j].zmax-mData->Ele[j].zmin);               
                massIS=massIS+mData->EleIS[j]*mData->Ele[j].area;
                massSnow=massSnow+(mData->EleSnow[j]-mData->Ele_IC[j].snow)*mData->Ele[j].area;
                massRecharge =massRecharge + mData->Recharge[j];
        
        }
        if(j>=3*mData->NumEle)
        {       
                massRiv=massRiv+((NV_Ith_S(CV_Y, j)<0?0:NV_Ith_S(CV_Y, j))-mData->Riv_IC[j].value)*mData->Riv_Shape[mData->Riv[j-3*mData->NumEle].shape - 1].width*mData->Riv[j-3*mData->NumEle].Length;
        }
        if((j>=2*mData->NumEle)&&(j<3*mData->NumEle))
        {

                massSat=massSat+((NV_Ith_S(CV_Y, j)<0?0:NV_Ith_S(CV_Y, j)))*mData->Ele[j-2*mData->NumEle].area*mData->Ele[j-2*mData->NumEle].Porosity;

        }
        if((j>=mData->NumEle)&&(j<2*mData->NumEle))
        {
                massUnSat=massUnSat+((NV_Ith_S(CV_Y, j)<0?0:NV_Ith_S(CV_Y, j)))*mData->Ele[j-mData->NumEle].area*mData->Ele[j-mData->NumEle].Porosity;

        }
      }
      massQ=massQ+mData->Q;
      NetMass=-(massSurf+massSat+massUnSat+massRiv+massIS+massSnow)+massP-massET1-massET3-massQ;
//      fprintf(fpMassBal,"\n %f %f %f %f %f %f %e %f %f %f %f %f %f",t,massSurf,massSat,massUnSat,massRiv,massIS,massRecharge,massP,massET1,massET2,massET3,massQ,NetMass);
/*      fprintf(res_flux_file,"\n%f %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",t,mData->FluxRiv[1199][1],mData->FluxRiv[1199][0],mData->FluxRiv[1141][1],mData->FluxRiv[1141][0],mData->FluxRiv[1126][1],mData->FluxRiv[1126][0],mData->FluxRiv[1129][1],mData->FluxRiv[1129][0],mData->FluxRiv[1136][1],mData->FluxRiv[1136][0],mData->FluxRiv[1137][1],mData->FluxRiv[1137][0],mData->FluxRiv[1138][1],mData->FluxRiv[1138][0]);
  */
 //   fprintf(res_flux_file,"\n%f %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",t,mData->FluxRiv[1136][1],mData->FluxRiv[1136][0],mData->FluxRiv[1136][2],mData->FluxRiv[1136][3],NV_Ith_S(CV_Y,mData->Riv[1136].LeftEle-1),NV_Ith_S(CV_Y,mData->Riv[1136].RightEle-1),NV_Ith_S(CV_Y, 1136+3*mData->NumEle),NV_Ith_S(CV_Y,2*mData->NumEle+mData->Riv[1136].LeftEle-1),NV_Ith_S(CV_Y,2*mData->NumEle+mData->Riv[1136].RightEle-1),NV_Ith_S(CV_Y,1*mData->NumEle+mData->Riv[1136].LeftEle-1),NV_Ith_S(CV_Y,1*mData->NumEle+mData->Riv[1136].RightEle-1),mData->FluxRiv[1137][1],mData->FluxRiv[1137][0],mData->FluxRiv[1137][2],mData->FluxRiv[1137][3],NV_Ith_S(CV_Y,mData->Riv[1137].LeftEle-1),NV_Ith_S(CV_Y,mData->Riv[1137].RightEle-1),NV_Ith_S(CV_Y, 1137+3*mData->NumEle),NV_Ith_S(CV_Y,2*mData->NumEle+mData->Riv[1137].LeftEle-1),NV_Ith_S(CV_Y,2*mData->NumEle+mData->Riv[1137].RightEle-1),NV_Ith_S(CV_Y,1*mData->NumEle+mData->Riv[1137].LeftEle-1),NV_Ith_S(CV_Y,1*mData->NumEle+mData->Riv[1137].RightEle-1),mData->FluxRiv[1138][1],mData->FluxRiv[1138][0],mData->FluxRiv[1138][2],mData->FluxRiv[1138][3],NV_Ith_S(CV_Y,mData->Riv[1138].LeftEle-1),NV_Ith_S(CV_Y,mData->Riv[1138].RightEle-1),NV_Ith_S(CV_Y, 1138+3*mData->NumEle),NV_Ith_S(CV_Y,2*mData->NumEle+mData->Riv[1138].LeftEle-1),NV_Ith_S(CV_Y,2*mData->NumEle+mData->Riv[1138].RightEle-1),NV_Ith_S(CV_Y,1*mData->NumEle+mData->Riv[1138].LeftEle-1),NV_Ith_S(CV_Y,1*mData->NumEle+mData->Riv[1138].RightEle-1));
// fprintf(res_flux_file,"\n%f",t);
/*totbase2=0;
totbase3=totbase4=0.0;
ctr3=ctr4=0;
for(k=0;k<mData->NumRiv;k++)
{
  totbase2=totbase2+mData->FluxRiv[k][4]+mData->FluxRiv[k][5];
  totbase3=totbase3+NV_Ith_S(CV_Y,mData->Riv[k].LeftEle-1+2*mData->NumEle)/(mData->Ele[mData->Riv[k].LeftEle-1].zmax-mData->Ele[mData->Riv[k].LeftEle-1].zmin);
  totbase4=totbase4+NV_Ith_S(CV_Y,mData->Riv[k].RightEle-1+2*mData->NumEle)/(mData->Ele[mData->Riv[k].RightEle-1].zmax-mData->Ele[mData->Riv[k].RightEle-1].zmin);
  if(NV_Ith_S(CV_Y,mData->Riv[k].LeftEle-1)>0)
	{
	ctr3=ctr3+1;
	}
  if(NV_Ith_S(CV_Y,mData->Riv[k].RightEle-1)>0)
	{
	ctr4=ctr4+1;
	}
// printf("\n%d\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf",k,NV_Ith_S(CV_Y,mData->Riv[k].LeftEle-1+2*mData->NumEle),mData->Ele[mData->Riv[k].LeftEle-1].zmax,mData->Ele[mData->Riv[k].LeftEle-1].zmin,NV_Ith_S(CV_Y,mData->Riv[k].RightEle-1+2*mData->NumEle),mData->Ele[mData->Riv[k].RightEle-1].zmax,mData->Ele[mData->Riv[k].RightEle-1].zmin);
//fprintf(res_flux_file,"%f\t",mData->FluxRiv[k][1]);
}*/
//fprintf(res_flux_file,"\n");
//fprintf(res_flux_file,"%lf %lf %lf %lf %lf %lf %lf %lf\n", mData->Riv[343].zmin,  mData->Riv[349].zmin, mData->Riv[350].zmin, mData->Riv[352].zmin, NV_Ith_S(CV_Y, 343+3*mData->NumEle), NV_Ith_S(CV_Y, 349+3*mData->NumEle), NV_Ith_S(CV_Y, 350+3*mData->NumEle), NV_Ith_S(CV_Y, 352+3*mData->NumEle));
//fprintf(res_flux_file,"%lf %lf %lf %lf %lf %lf %lf %lf\n", mData->Riv[343].zmin,  mData->Riv[349].zmin, mData->Riv[350].zmin, mData->Riv[352].zmin, mData->FluxRiv[343][1], mData->FluxRiv[349][1], mData->FluxRiv[350][1], mData->FluxRiv[352][1]);
//fprintf(res_flux_file,"%lf %lf %lf %lf\n", mData->FluxRiv[290][1], mData->FluxRiv[260][1], mData->FluxRiv[273][1], mData->FluxRiv[129][1]);
//printRiverFlux(mData, CV_Y, res_flux_file);
/*for(k=0; k<mData->NumRiv;k++){
	//fprintf(res_flux_file,"%lf\t",mData->FluxRiv[k][1]);
	fprintf(headFile,"%lf\t",NV_Ith_S(CV_Y, k+3*mData->NumEle));
	fprintf(overFile,"%lf\t",mData->FluxRiv[k][2]+mData->FluxRiv[k][3]);
	fprintf(baseFile,"%lf\t",mData->FluxRiv[k][4]+mData->FluxRiv[k][5]);
}*/
satEle=0;
ovrEle=0;
for(k=0; k<mData->NumEle;k++)
{
	if(NV_Ith_S(CV_Y, k+2*mData->NumEle)/(mData->Ele[k].zmax-mData->Ele[k].zmin)>0.99){
		satEle++;
	}
	if(NV_Ith_S(CV_Y, k)>1E-4){
		ovrEle++;
	}
}


//fprintf(res_flux_file,"\n"); //fflush(res_flux_file);
//fprintf(headFile,"\n"); //fflush(headFile);
//fprintf(overFile,"\n");
//fprintf(baseFile,"\n");
//fprintf(timeFile,"%f\t%lf\n",t,(clock()-start)/(realtype)CLOCKS_PER_SEC/60.0);
//fprintf(timeFile,"%f\t%lf\t%d\t%d\n",t,(clock()-start)/(realtype)CLOCKS_PER_SEC/60.0,satEle,ovrEle);
//fflush(timeFile);
//getchar(); 
//totbase1=mData->FluxRiv[145][4]+mData->FluxRiv[145][5]+mData->FluxRiv[163][4]+mData->FluxRiv[163][5]+mData->FluxRiv[164][4]+mData->FluxRiv[164][5]+mData->FluxRiv[166][4]+mData->FluxRiv[166][5]+mData->FluxRiv[167][4]+mData->FluxRiv[167][5]+mData->FluxRiv[168][4]+mData->FluxRiv[168][5]+mData->FluxRiv[178][4]+mData->FluxRiv[178][5]+mData->FluxRiv[182][4]+mData->FluxRiv[182][5]+mData->FluxRiv[183][4]+mData->FluxRiv[183][5]+mData->FluxRiv[201][4]+mData->FluxRiv[201][5]+mData->FluxRiv[202][4]+mData->FluxRiv[202][5]+mData->FluxRiv[203][4]+mData->FluxRiv[203][5]+mData->FluxRiv[204][4]+mData->FluxRiv[204][5]+mData->FluxRiv[205][4]+mData->FluxRiv[205][5]+mData->FluxRiv[207][4]+mData->FluxRiv[207][5]+mData->FluxRiv[208][4]+mData->FluxRiv[208][5]+mData->FluxRiv[209][4]+mData->FluxRiv[209][5]+mData->FluxRiv[210][4]+mData->FluxRiv[210][5]+mData->FluxRiv[211][4]+mData->FluxRiv[211][5];
//fprintf(res_flux_file,"\n%f\t%lf\t%lf",t,mData->FluxRiv[276][1],mData->FluxRiv[132][1]);

//fprintf(res_flux_file,"\n%f\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%f",t,mData->FluxRiv[145][1],mData->FluxRiv[48][1],totbase1,totbase2,totbase3/mData->NumRiv,totbase4/mData->NumRiv,(float)(ctr3+ctr4)/mData->NumRiv);
//fprintf(res_flux_file,"\n%f\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf",t,mData->FluxRiv[145][0],mData->FluxRiv[145][1],NV_Ith_S(CV_Y,2*mData->NumEle+mData->Riv[145].LeftEle-1),NV_Ith_S(CV_Y,2*mData->NumEle+mData->Riv[145].RightEle-1),NV_Ith_S(CV_Y,2*mData->NumEle+869),NV_Ith_S(CV_Y,2*mData->NumEle+772),mData->FluxRiv[48][0],mData->FluxRiv[48][1]);
//fprintf(res_flux_file,"\n%f %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",t,mData->FluxRiv[208][0],mData->FluxRiv[208][1],mData->FluxRiv[208][2],mData->FluxRiv[208][3],mData->FluxRiv[208][4],mData->FluxRiv[208][5],mData->FluxRiv[145][0],mData->FluxRiv[145][1],mData->FluxRiv[145][2],mData->FluxRiv[145][3],mData->FluxRiv[145][4],mData->FluxRiv[145][5],NV_Ith_S(CV_Y,869+2*mData->NumEle));
//      printf("\n%lf %lf %lf %lf ",mData->FluxRiv[1199][1],mData->FluxRiv[1109][1],mData->FluxRiv[1154][1],mData->FluxRiv[2036][1]);

/**********************************************MASS BALANCE*****************************************************/    
    
    }  

/*     if(cData.Verbose == 1) {PrintVerbose(i, t, iopt, ropt);} */

    /* uncomment it if user need it verbose mode */  
    /* if(cData.Verbose == 1) {PrintY(mData, CV_Y, t);} */
    
    /* print out results to files at every output time */
    if (cData.res_out == 1) {FPrintY(mData, CV_Y, t, res_state_file);}
    if (cData.flux_out == 1) {FPrintFlux(mData, t, res_flux_file);}
    if (cData.etis_out == 1) {FPrintETIS(mData, t, res_etis_file);}
  /*  if (cData.q_out == 1) {FPrintQ(mData, t, res_q_file);} */
    
/*    if (cData.Verbose != 1) {printf("\r");}  
    
    if(flag != SUCCESS) {printf("CVode failed, flag = %d. \n", flag); return(flag);} 
  */  
    /* clear buffer */
   fflush(stdout);  
  }

	FPrintInitFile(mData, cData, CV_Y, i);                    /* Routine for .init File : print.c     */
	FPrintCloseAll();


/**********************************/
/*    Routine for .init File      */
/**********************************/
//fpInit = fopen("rhode.init","w");
/*outputInitFile = (char *)malloc((strlen(filename)+20)*sizeof(char));
strcpy(outputInitFile, filename);
strcat(outputInitFile, ".init.out");
*/
//cout<<"\nOutput .init File= "<<outputInitFile<<"\n";
//getchar(); getchar();
/*fpInit = fopen(outputInitFile, "w");
fprintf(fpInit,"%lf\n",cData.Tout[i]);
for(j=0; j<mData->NumEle; j++)
{
	fprintf(fpInit,"%lf\t%lf\t%lf\t%lf\t%lf\n",mData->EleIS[j],mData->EleSnow[j],NV_Ith_S(CV_Y,j), NV_Ith_S(CV_Y,j+mData->NumEle), NV_Ith_S(CV_Y,j+2*mData->NumEle));
}
for(j=0; j<mData->NumRiv; j++)
{
	fprintf(fpInit,"%lf\n",NV_Ith_S(CV_Y, j+3*mData->NumEle));
}
*/
/**********************************/


/*
for(i=0; i<mData->NumEle; i++){
	fprintf(hFile, "%d %lf\n", i+1, head[i]);
} */
//fflush(hFile);

/*fflush(fpInit);
fclose(fpInit);
*/
//fflush(headFile);
//fflush(timeFile);
//fflush(headFile);
//fflush(baseFile);
//fflush(overFile);

  /* free memory */
  /*
  N_VFree(CV_Y);
  CVodeFree(cvode_mem);
  M_EnvFree_Serial(machEnv); 
  */
  
  /* capture time */
  end_s = clock();
  cputime_s = (end_s - end_r)/(realtype)CLOCKS_PER_SEC;
  
  /* print out simulation statistics */
/*  PrintFarewell(cData, iopt, ropt, cputime_r, cputime_s);
  if (cData.res_out == 1) {FPrintFarewell(cData, res_state_file, iopt, ropt, cputime_r, cputime_s);}
  */
  /* close output files */
  if (cData.res_out == 1)  {fclose(res_state_file);}
  if (cData.flux_out == 1) {fclose(res_flux_file);}
  if (cData.etis_out == 1) {fclose(res_etis_file);}
  if (cData.q_out == 1)    {fclose(res_q_file);}
  
  free(mData);
  
  return 0;
}


realtype CS_AreaOrPerem1(int rivOrder, realtype rivDepth, realtype rivCoeff, realtype a_pBool)
{
	realtype rivArea, rivPerem, eq_Wid, EPSILON=0.05;
	switch(rivOrder)
	{
		case 1:
			rivArea = rivDepth*rivCoeff;
			rivPerem= 2.0*rivDepth+rivCoeff;
			eq_Wid=rivCoeff;
			return (a_pBool==1?rivArea:(a_pBool==2?rivPerem:eq_Wid)); //returnVal1(rivArea, rivPerem, eq_Wid, a_pBool);
		case 2:
			rivArea = pow(rivDepth,2)/rivCoeff;
			rivPerem = 2.0*rivDepth*pow(1+pow(rivCoeff,2),0.5)/rivCoeff;
			eq_Wid=2.0*pow(rivDepth+EPSILON,1/(rivOrder-1))/pow(rivCoeff,1/(rivOrder-1));
			return (a_pBool==1?rivArea:(a_pBool==2?rivPerem:eq_Wid)); //returnVal1(rivArea, rivPerem, eq_Wid, a_pBool);
		case 3:
			rivArea = 4*pow(rivDepth,1.5)/(3*pow(rivCoeff,0.5));
			rivPerem =(pow(rivDepth*(1+4*rivCoeff*rivDepth)/rivCoeff,0.5))+(log(2*pow(rivCoeff*rivDepth,0.5)+pow(1+4*rivCoeff*rivDepth,0.5))/(2*rivCoeff));
			eq_Wid=2.0*pow(rivDepth+EPSILON,1/(rivOrder-1))/pow(rivCoeff,1/(rivOrder-1));
			return (a_pBool==1?rivArea:(a_pBool==2?rivPerem:eq_Wid)); //returnVal1(rivArea, rivPerem, eq_Wid, a_pBool);
		case 4:
			rivArea = 3*pow(rivDepth,4.0/3.0)/(2*pow(rivCoeff,1.0/3.0));
			rivPerem = 2*((pow(rivDepth*(1+9*pow(rivCoeff,2.0/3.0)*rivDepth),0.5)/3)+(log(3*pow(rivCoeff,1.0/3.0)*pow(rivDepth,0.5)+pow(1+9*pow(rivCoeff,2.0/3.0)*rivDepth,0.5))/(9*pow(rivCoeff,1.0/3.0))));
			eq_Wid=2.0*pow(rivDepth+EPSILON,1/(rivOrder-1))/pow(rivCoeff,1/(rivOrder-1));
			return (a_pBool==1?rivArea:(a_pBool==2?rivPerem:eq_Wid)); //returnVal1(rivArea, rivPerem, eq_Wid, a_pBool);
		default:
			printf("\n Relevant Values entered are wrong");
			printf("\n Depth: %lf\tCoeff: %lf\tOrder: %d\t");
			return 0;
	}
}

realtype OverlandFlow1(int loci, int locj, int surfmode, realtype avg_y, realtype grad_y, realtype avg_sf, realtype alfa, realtype beta, realtype crossA, realtype avg_rough, int eletypeBool, realtype avg_perem) /** delete perimeter should be passed **/
{
	realtype flux;
	int locBool;
	float hydRadius;
	/* if surface gradient is not enough to overcome the friction */
    if(fabs(grad_y) <= avg_sf)
    {
         flux = 0;
    }
    else
    {
         if(grad_y > 0)
         {
              locBool=1;
         }
         else if(grad_y < 0)
         {
	      locBool=-1;
         }
         switch(surfmode)
         {
	            case 1:
	              if(eletypeBool==1)
	              {
                       /* Kinematic Wave Approximation constitutive relationship: Manning Equation */
	                   alfa = sqrt(locBool*grad_y)/avg_rough;
	                   beta = pow(avg_y, 2.0/3.0);
	                   flux = locBool*alfa*beta*crossA;
	                   break;
	              }
	              else
	              {
                       /*alfa = sqrt(locBool*grad_y)/(avg_rough*pow((avg_perem>0?avg_perem:0), 2.0/3.0));
	                   beta = 5.0/3.0;
	                   *flux = locBool*alfa*pow(crossA, beta);
                       */
                       hydRadius = (avg_perem>0?crossA/avg_perem:0);
		               flux = locBool*sqrt(locBool*grad_y)*crossA*pow(hydRadius,2.0/3.0)/avg_rough;
	                   break;
	              }
	            case 2:
	              if(eletypeBool==1)
	              {
	                   /* Diffusion Wave Approximation constitutive relationship: Gottardi & Venutelli, 1993 */
	                   alfa = pow(pow(avg_y, 1.0/3.0),2)/(1.0*avg_rough);
	                   beta = alfa;
	                   flux = locBool*crossA*beta*sqrt(locBool*grad_y);
	                   break;
	              }
	              else
	              {
                       /*alfa = pow(pow(avg_y, 1.0/3.0),2)/(1.0*avg_rough);
	                   beta = alfa;
	                   flux = locBool*crossA*beta*sqrt(locBool*grad_y);
                       */
		               hydRadius = (avg_perem>0?crossA/avg_perem:0);
		               flux = locBool*sqrt(locBool*grad_y)*crossA*pow(hydRadius,2.0/3.0)/(1.0*avg_rough);
		               break;
	              }
	            default:
	              if(eletypeBool==1)
	              {
       	               printf("Fatal Error: Surface Overland Mode Type Is Wrong!");
	              }
	              else
	              {
	                   printf("Fatal Error: River Routing Mode Type Is Wrong!");
	              }
	              exit(1);
	    }
	}
	return flux;
}


void  printRiverFlux(Model_Data mData, N_Vector CV_Y, FILE *res_flux_file)
{
	int i;
	realtype TotalY_Riv, Perem, TotalY_Riv_down, Perem_down, Avg_Perem, Avg_Y_Riv, Avg_Rough, Distance, Dif_Y_Riv, Avg_Sf, CrossA, Alfa, Beta;
	realtype Flux;
	for(i=0; i<mData->NumRiv; i++)
	{
		TotalY_Riv = NV_Ith_S(CV_Y, i + 3*mData->NumEle) + mData->Riv[i].zmin;
		Perem = CS_AreaOrPerem1(mData->Riv_Shape[mData->Riv[i].shape - 1].interpOrd,NV_Ith_S(CV_Y, i + 3*mData->NumEle),mData->Riv_Shape[mData->Riv[i].shape - 1].coeff,2);
		if(mData->Riv[i].down > 0)
		{
			TotalY_Riv_down = NV_Ith_S(CV_Y, mData->Riv[i].down - 1 + 3*mData->NumEle)  + mData->Riv[mData->Riv[i].down - 1].zmin;
		        Perem_down = CS_AreaOrPerem1(mData->Riv_Shape[mData->Riv[mData->Riv[i].down - 1].shape - 1].interpOrd,NV_Ith_S(CV_Y, mData->Riv[i].down - 1 + 3*mData->NumEle),mData->Riv_Shape[mData->Riv[mData->Riv[i].down - 1].shape - 1].coeff,2);
		        Avg_Perem = (Perem + Perem_down)/2.0;	/** Avg perimeter **/
			if(mData->Riv[mData->Riv[i].down - 1].zmin>mData->Riv[i].zmin)
			{
				if(mData->Riv[mData->Riv[i].down - 1].zmin > TotalY_Riv)
				{
		                        Avg_Y_Riv=NV_Ith_S(CV_Y, mData->Riv[i].down - 1 + 3*mData->NumEle)/2;
			        }
				else
				{
					Avg_Y_Riv=(TotalY_Riv-mData->Riv[mData->Riv[i].down - 1].zmin+NV_Ith_S(CV_Y, mData->Riv[i].down - 1 + 3*mData->NumEle))/2;
			        }
		         }
			 else
			 {
				if(mData->Riv[i].zmin>TotalY_Riv_down)
				{
		                        Avg_Y_Riv=NV_Ith_S(CV_Y, i + 3*mData->NumEle)/2;
			        }
				else
				{
					Avg_Y_Riv=(NV_Ith_S(CV_Y, i + 3*mData->NumEle)+TotalY_Riv_down-mData->Riv[i].zmin)/2;
			        }
			 }
		         Avg_Rough = (mData->Riv_Mat[mData->Riv[i].material - 1].Rough + mData->Riv_Mat[mData->Riv[mData->Riv[i].down - 1].material-1].Rough)/2.0;
		         
		         Distance = (mData->Riv[i].Length+mData->Riv[mData->Riv[i].down - 1].Length)/2;
		
		         Dif_Y_Riv = (TotalY_Riv - TotalY_Riv_down)/Distance;
		         Avg_Sf = (mData->Riv_Mat[mData->Riv[i].material - 1].Sf + mData->Riv_Mat[mData->Riv[mData->Riv[i].down - 1].material-1].Sf)/2.0;
		             /*CrossA = 0.5*(CS_AreaOrPerem1(mData->Riv_Shape[mData->Riv[i].shape - 1].interpOrd,DummyY[i + 3*MD->NumEle],MD->Riv_Shape[MD->Riv[i].shape - 1].coeff,1)+CS_AreaOrPerem(MD->Riv_Shape[MD->Riv[MD->Riv[i].down - 1].shape - 1].interpOrd,DummyY[MD->Riv[i].down - 1 + 3*MD->NumEle],MD->Riv_Shape[MD->Riv[MD->Riv[i].down - 1].shape - 1].coeff,1));
		             */
			 CrossA = CS_AreaOrPerem1(mData->Riv_Shape[mData->Riv[i].shape - 1].interpOrd,Avg_Y_Riv,mData->Riv_Shape[mData->Riv[i].shape - 1].coeff,1);
			 Flux = OverlandFlow1(i,1,mData->RivMode, Avg_Y_Riv,Dif_Y_Riv,Avg_Sf,Alfa,Beta,CrossA,Avg_Rough,0,Avg_Perem);
		         /* Correction is being done in flux terms which can be > 0 even when there is no source water level present */
		         if(NV_Ith_S(CV_Y, i + 3*mData->NumEle) <= 0 && Flux > 0)
		         {
		         	Flux = 0;
		         }
		         else if(NV_Ith_S(CV_Y, mData->Riv[i].down - 1 + 3*mData->NumEle) <= 0 && Flux < 0)
		         {
		                Flux = 0;
		         }

         	}
		else{
			Flux = 0.0;
		}
         	fprintf(res_flux_file, "%lf\t", Flux);
         }
         fprintf(res_flux_file, "\n");
}
         	