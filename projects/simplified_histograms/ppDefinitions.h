const int nmasses = 5;
const int nelemsMax = 8;


float pmasses[nmasses];
int c2u[20];

int RunNumber, Bc, TotalTracks, NumContrib, GlobalNonPVtracks;
int OccupancyInTime, Trs, Trofs, Hmpr, Tfb, ItsRofb, Sbp, ZvtxFT0vsPv, VtxITSTPC;
double HadronicRate;

float PosX, PosY, PosZ;
float TotalFT0AmplitudeA, TotalFT0AmplitudeC, TotalFV0AmplitudeA;
float TimeFT0A, TimeFT0C, TimeFV0A;



Int_t TrkSign[nelemsMax];
float TrkPx[nelemsMax], TrkPy[nelemsMax], TrkPz[nelemsMax];
float TrkDCAxy[nelemsMax], TrkDCAz[nelemsMax];
float TrkTimeRes[nelemsMax];
UInt_t ITSclusterSizes[nelemsMax];
float TrkTPCsignal[nelemsMax], TrkTPCnSigmaEl[nelemsMax], TrkTPCnSigmaMu[nelemsMax], TrkTPCnSigmaPi[nelemsMax], TrkTPCnSigmaKa[nelemsMax], TrkTPCnSigmaPr[nelemsMax], TrkTPCinnerParam[nelemsMax];
float TrkTOFsignal[nelemsMax], TrkTOFnSigmaEl[nelemsMax], TrkTOFnSigmaMu[nelemsMax], TrkTOFnSigmaPi[nelemsMax], TrkTOFnSigmaKa[nelemsMax], TrkTOFnSigmaPr[nelemsMax], TrkTOFexpMom[nelemsMax];
