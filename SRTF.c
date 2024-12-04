#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct{
    int iPID;
    int iArrival, iBurst, iBurstRemain;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[])
{
    srand(time(0));
    for (int i = 0; i < n; i++)
    {
//        printf("Process P%d\n", i + 1);
//        P[i].iPID = i + 1;
//        printf("Arrival time: ");
//        scanf("%d", &P[i].iArrival);
//        printf("Burst time: ");
//        scanf("%d", &P[i].iBurst);
        P[i].iPID = i + 1;
        P[i].iArrival = rand() % 21;
        P[i].iBurst = (rand() % 11) + 2;
    }
    // Print all the current processes
    printf("Current processes:\n");
    printf("PID\tArrival\tBurst\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\n", P[i].iPID, P[i].iArrival, P[i].iBurst);
    }
}

void printProcess(int n, PCB P[])
{
    for (int i = 0; i < n; i++)
    {
        printf("Tien trinh %d:\n", P[i].iPID);
        printf("Arrival time: %d, Burst time: %d\n", P[i].iArrival, P[i].iBurst);
        printf("Start time: %d, Finish time: %d\n", P[i].iStart, P[i].iFinish);
        printf("Waiting time: %d, Response time: %d, Turn around time: %d\n", P[i].iWaiting, P[i].iResponse, P[i].iTaT);
    }
}

void exportGanttChart (int n, PCB P[])
{
    // top bar
    printf(" ");
    for (int i = 0; i < n; i++)
    {
        if (i > 0 && P[i].iStart > P[i - 1].iFinish)
        {
            for (int j = 0; j < (P[i].iStart - P[i - 1].iFinish); j++) printf("--");
            printf(" ");
        }
        for (int j = 0; j < P[i].iBurst; j++) printf("--");
        printf(" ");
    }
    printf("\n|");
    // Process ID and stuffs
    for (int i = 0; i < n; i++)
    {
        if (i > 0 && P[i].iStart > P[i - 1].iFinish)
        {
            for (int j = 0; j < (P[i].iStart - P[i - 1].iFinish); j++) printf("  ");
            printf("|");
        }
        for (int j = 0; j < P[i].iBurst - 1; j++) printf(" ");
        printf("P%d", P[i].iPID);
        for (int j = 0; j < P[i].iBurst - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    // bottom bar
    for (int i = 0; i < n; i++)
    {
        if (i > 0 && P[i].iStart > P[i - 1].iFinish)
        {
            for (int j = 0; j < (P[i].iStart - P[i - 1].iFinish); j++) printf("--");
            printf(" ");
        }
        for (int j = 0; j < P[i].iBurst; j++) printf("--");
        printf(" ");
    }
    printf("\n");
    // Timeline
    printf("%d", P[0].iStart);
    for (int i = 0; i < n; i++)
    {
        if (i > 0 && P[i].iStart > P[i - 1].iFinish)
        {
            for (int j = 0; j < (P[i].iStart - P[i - 1].iFinish); j++) printf("  ");
            if (P[i].iStart > 10) printf("\b%d", P[i].iStart);
            else printf("%d", P[i].iStart);
        }
        for (int j = 0; j < P[i].iBurst; j++) printf("  ");
        if (P[i].iFinish > 10) printf("\b%d", P[i].iFinish);
        else printf("%d", P[i].iFinish);
    }
    printf("\n");
} // Need iStart, iFinish and iBurst

void pushProcess(int *n, PCB P[], PCB Q)
{
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int index, PCB P[])
{
    for (int i = index; i < *n - 1; i++)
    {
        P[i] = P[i + 1];
    }
    (*n)--;
}

int swapProcess(PCB *P, PCB *Q)
{
    PCB temp;
    temp = *P;
    *P = *Q;
    *Q = temp;
    return 0;
}

int partition (PCB P[], int low, int high, int iCriteria)
{
    int pivot;
    if (iCriteria == SORT_BY_ARRIVAL) pivot = P[high].iArrival;
    else if (iCriteria == SORT_BY_PID) pivot = P[high].iPID;
    else pivot = P[high].iBurst;

    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (iCriteria == SORT_BY_ARRIVAL)
        {
            if (P[j].iArrival < pivot)
            {
                i++;
                swapProcess(&P[i], &P[j]);
            }
            else if (P[j].iArrival == pivot)
            {
                // swap by burst time
                if (P[j].iBurst < pivot)
                {
                    i++;
                    swapProcess(&P[i], &P[j]);
                }
            }
        }
        else if (iCriteria == SORT_BY_PID)
        {
            if (P[j].iPID < pivot)
            {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        }
        else if (iCriteria == SORT_BY_BURST)
        {
            if (P[j].iBurst < pivot)
            {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        }
        else if (iCriteria == SORT_BY_START)
        {
            if (P[j].iStart < pivot)
            {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        }
    }
    i++;
    swapProcess(&P[i], &P[high]);
    return i;
}

void quickSort(PCB P[], int low, int high, int iCriteria)
{
    if (low >= high) return; // base case

    int pivot = partition(P, low, high, iCriteria);
    quickSort(P, low, pivot - 1, iCriteria);
    quickSort(P, pivot + 1, high, iCriteria);
}

void calculateAWT(int n, PCB P[])
{
    double AWT = 0;
    for (int i = 1; i <= n; i++)
    {
        AWT += (P[i].iFinish - P[i].iArrival - P[i].iBurst);
    }
    AWT /= n;
    printf("AWT: %.2f\n", AWT);
}

void calculateATaT(int n, PCB P[])
{
    double ATaT = 0;
    for (int i = 1; i <= n; i++)
    {
        ATaT += P[i].iFinish - P[i].iArrival;
    }
    ATaT /= n;
    printf("ATaT: %.2f\n", ATaT);
}

int main()
{
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    PCB OriginalProcess[10];

    // Initialize all the arrays to 0
    memset(&Input, 0, sizeof(PCB) * 10);
    memset(&ReadyQueue, -1, sizeof(PCB) * 10);
    memset(&TerminatedArray, 0, sizeof(PCB) * 10);
    memset(&OriginalProcess, -1, sizeof(PCB) * 10);

    int iNumberOfProcess;
    printf("Please input the number of Process: ");
    scanf("%d", &iNumberOfProcess);

    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    inputProcess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);

    for (int i = 0; i < iNumberOfProcess; i++) OriginalProcess[Input[i].iPID] = Input[i];

    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

    printf("\nReady Queue: ");
    printProcess(1, ReadyQueue);

    int currentTime = ReadyQueue[0].iArrival;
    ReadyQueue[0].iBurstRemain = ReadyQueue[0].iBurst;
    OriginalProcess[ReadyQueue[0].iPID].iStart = ReadyQueue[0].iStart;

    // Shortest Remaining Time First
    while (iRemain > 0 || iReady > 0)
    {
        int flag = 0;
        for (int i = 0; i < iRemain; i++)
        {
            // Check if any process arrives at the current time
            if (Input[i].iArrival <= currentTime)
            {
                if (Input[0].iBurst < ReadyQueue[0].iBurstRemain) flag = 1;
                Input[i].iBurstRemain = Input[i].iBurst;
                pushProcess(&iReady, ReadyQueue, Input[i]);
                removeProcess(&iRemain, i, Input);
                i--;
            }
        }
        if (flag)
        {
            PCB temp = ReadyQueue[0];
            temp.iStart = ReadyQueue[0].iStart;
            temp.iFinish = currentTime;
            temp.iBurst = temp.iFinish - temp.iStart;
            if (temp.iBurst != 0)
            {
                pushProcess(&iTerminated, TerminatedArray, temp);
                quickSort(ReadyQueue, 0, iReady - 1, SORT_BY_BURST);
                ReadyQueue[0].iStart = currentTime;
                OriginalProcess[ReadyQueue[0].iPID].iStart = currentTime;
            }
        }
        currentTime++;
        ReadyQueue[0].iBurstRemain--;
        if (ReadyQueue[0].iBurstRemain == 0)
        {
            PCB temp = ReadyQueue[0];
            temp.iFinish = currentTime;
            temp.iBurst = temp.iFinish - temp.iStart;
            pushProcess(&iTerminated, TerminatedArray, temp);
            removeProcess(&iReady, 0, ReadyQueue);

            // Push process to OriginalProcess
            OriginalProcess[temp.iPID].iFinish = currentTime;

            quickSort(ReadyQueue, 0, iReady - 1, SORT_BY_BURST);
            if (iReady > 0) ReadyQueue[0].iStart = currentTime;
            else if (iRemain > 0)
            {
                quickSort(Input, 0, iRemain - 1, SORT_BY_ARRIVAL);
                currentTime = Input[0].iArrival;
                Input[0].iBurstRemain = Input[0].iBurst;
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                ReadyQueue[0].iStart = currentTime;
            }
        }
    }

    printf("\n===== SRTF Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    calculateAWT(iNumberOfProcess, OriginalProcess);
    calculateATaT(iNumberOfProcess, OriginalProcess);
    return 0;
}
