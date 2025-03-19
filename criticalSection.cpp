#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
CRITICAL_SECTION chopsteak[5];
HANDLE philosopher[5];

DWORD WINAPI meal(LPVOID lparam) {
    BOOL success;
    int temp = *((int*)lparam);
    int chopsteak1 = temp;
    int chopsteak2 = ((temp + 1) % 5);

    EnterCriticalSection(&chopsteak[chopsteak1]);
    success = TryEnterCriticalSection(&chopsteak[chopsteak2]);
    while (!success)
    {
        LeaveCriticalSection(&chopsteak[chopsteak1]);
        EnterCriticalSection(&chopsteak[chopsteak1]);
        success = TryEnterCriticalSection(&chopsteak[chopsteak2]);
    }

    printf("Shhh! Philosopher #%d eats now\n", temp+1);
    LeaveCriticalSection(&chopsteak[chopsteak1]);
    LeaveCriticalSection(&chopsteak[chopsteak2]);
    return 1;
}


int main()
{
    clock_t begin = clock();

    for (int i = 0; i < 5; i++)
    {
        InitializeCriticalSection(&chopsteak[i]);
    }

#pragma region Params
    INT* num_of_philo_1 = (INT*)malloc(sizeof(INT));
    *num_of_philo_1 = 0;
    LPVOID p_num_of_philo_1 = num_of_philo_1;
    INT* num_of_philo_2 = (INT*)malloc(sizeof(INT));
    *num_of_philo_2 = 1;
    LPVOID p_num_of_philo_2 = num_of_philo_2;
    INT* num_of_philo_3 = (INT*)malloc(sizeof(INT));
    *num_of_philo_3 = 2;
    LPVOID p_num_of_philo_3 = num_of_philo_3;
    INT* num_of_philo_4 = (INT*)malloc(sizeof(INT));
    *num_of_philo_4 = 3;
    LPVOID p_num_of_philo_4 = num_of_philo_4;
    INT* num_of_philo_5 = (INT*)malloc(sizeof(INT));
    *num_of_philo_5 = 4;
    LPVOID p_num_of_philo_5 = num_of_philo_5;
#pragma endregion

#pragma region Threads
    //for (int i = 0; i < 10000; i++) {
        philosopher[0] = CreateThread(NULL, 0, meal, num_of_philo_1, 0, NULL);
        philosopher[1] = CreateThread(NULL, 0, meal, num_of_philo_2, 0, NULL);
        philosopher[2] = CreateThread(NULL, 0, meal, num_of_philo_3, 0, NULL);
        philosopher[3] = CreateThread(NULL, 0, meal, num_of_philo_4, 0, NULL);
        philosopher[4] = CreateThread(NULL, 0, meal, num_of_philo_5, 0, NULL);
    //}

#pragma endregion

    WaitForMultipleObjects(5, philosopher, TRUE, INFINITE);
    for (size_t i = 0; i < 5; i++)
    {
        CloseHandle(philosopher[i]);
        DeleteCriticalSection(&chopsteak[i]);
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The meal ended in %f ns", time_spent);
    printf("Test");
    printf("Test2");
}
