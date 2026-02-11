#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5   // number of threads

DWORD ThreadIds[NUM_THREADS];
HANDLE ThreadHandles[NUM_THREADS];
DWORD Sums[NUM_THREADS] = {0};  // each thread has its own sum

DWORD WINAPI Summation(LPVOID Param)
{
    int index = *(int*)Param;   // thread index
    DWORD upper = 100;          // sum 1..100

    printf("Thread %d started - calculating sum from 1 to %lu...\n", index + 1, upper);

    for (DWORD i = 1; i <= upper; i++) {
        Sums[index] += i;
        Sleep(200);  // slower so threads stay visible in Process Explorer
    }

    printf("Thread %d completed calculation. Sum = %lu\n", index + 1, Sums[index]);

    Sleep(2000);  // extra delay to make thread stay alive for screenshot

    return 0;
}

int main()
{
    int params[NUM_THREADS];

    printf("Main: Creating %d threads...\n", NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        params[i] = i;  // thread index
        ThreadHandles[i] = CreateThread(
            NULL,
            0,
            Summation,
            &params[i],
            0,
            &ThreadIds[i]
        );

        if (ThreadHandles[i] == NULL) {
            fprintf(stderr, "Error creating thread %d: %d\n", i+1, GetLastError());
            return 1;
        }

        Sleep(100);  // small delay between thread creation
    }

    // Wait for all threads to finish
    WaitForMultipleObjects(NUM_THREADS, ThreadHandles, TRUE, INFINITE);

    // Close all thread handles
    for (int i = 0; i < NUM_THREADS; i++) {
        CloseHandle(ThreadHandles[i]);
    }

    printf("Main: All threads completed. Press Enter to exit...\n");
    getchar();  // keep console open

    return 0;
}

