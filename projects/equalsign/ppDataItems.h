// particle masses
extern const int nmasses;
extern float pmasses[nmasses];
extern int c2u[20];

// maximum number of tracks in an event
extern const int nelemsMax;

// assign branches
extern int RunNumber, Bc, TotalTracks, NumContrib, GlobalNonPVtracks;
extern int OccupancyInTime, Trs, Trofs, Hmpr, Tfb, ItsRofb, Sbp, ZvtxFT0vsPv, VtxITSTPC;
extern double HadronicRate;

extern float PosX, PosY, PosZ;
extern float TotalFT0AmplitudeA, TotalFT0AmplitudeC, TotalFV0AmplitudeA;
extern float TimeFT0A, TimeFT0C, TimeFV0A;

extern Int_t TrkSign[nelemsMax];
extern float TrkPx[nelemsMax], TrkPy[nelemsMax], TrkPz[nelemsMax];
extern float TrkDCAxy[nelemsMax], TrkDCAz[nelemsMax];
extern float TrkTimeRes[nelemsMax];
extern UInt_t ITSclusterSizes[nelemsMax];
extern float TrkTPCsignal[nelemsMax], TrkTPCnSigmaEl[nelemsMax], TrkTPCnSigmaMu[nelemsMax], TrkTPCnSigmaPi[nelemsMax], TrkTPCnSigmaKa[nelemsMax], TrkTPCnSigmaPr[nelemsMax], TrkTPCinnerParam[nelemsMax];
extern float TrkTOFsignal[nelemsMax], TrkTOFnSigmaEl[nelemsMax], TrkTOFnSigmaMu[nelemsMax], TrkTOFnSigmaPi[nelemsMax], TrkTOFnSigmaKa[nelemsMax], TrkTOFnSigmaPr[nelemsMax], TrkTOFexpMom[nelemsMax];
