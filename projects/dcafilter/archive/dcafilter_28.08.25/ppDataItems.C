float pmasses[nmasses] = {0.000510998902, 0.1056583745, 0.13957039, 0.493677, 0.93827208816};
int c2u[20] = {0, 1, 920, 632, 416, 600, 400, 616, 432, 800, 820, 840, 860, 880, 900};

// assign branches
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
