/*
 * Учебная практика
 * Мамонтов А.Ю.
 * Бпи162
 * CLion
*/
#include <iostream>
#include <ctime>
#include <fstream>
#include <chrono>
#include <string>
using namespace std;
using namespace std::chrono;
const int N=9000;
const int numSort=7;
const int numLaunch=100;
const int numArrays=4;
const int numCompileTests=5;

void generateTests(int *,int *,int *,int *);// Прототип функции генерации тестов
void radixSort(int *, int);// Прототип цифровой сортировки
void countingSort(int *,int);// Прототип сортировки подсчетом
void bubbleSort(int *, int);// Прототип пузырька
void bubbleSort1(int *, int);// Прототип пузырька с 1 условием Айверсона
void bubbleSort2(int *, int);// Прототип пузырьька со 2 условием Айверсона
void insertionSort(int *, int);// Прототип сортировки вставками
void binInsertionSort(int *, int);// Прототип сортировки бинарными вставками
void reverseArray(int *,int);// Реверс массива
void bubbleSort2forTest(int *, int);// Это чтобы массивы задать
int* arrayCopy(int *,int);// Копирование массива
/*
 * TODO:
 * функция генерации тестов +
 * засунуть вспомогательные фукнции в сортировки +
 * вывод в файл+
 * подсчет тиков процессора+
 */

int main()
{
	srand(time(0));
    // Задание массивов
    int arr07[N];
    int arr0max[N];
    int arrNearlySorted[N];
    int arrReverseSorted[N];
    generateTests(arr07,arr0max,arrNearlySorted,arrReverseSorted);

    // Открытие файла для записи результатов
    ofstream output("../output.csv");
    output<<"numEl;sortName;arrKind;avgTime"<<endl;
    string sortNames[numSort]={"bubbleSort","bubbleSort1","bubbleSort2","insertionSort","binInsertionSort","countingSort","radixSort"};
    string arrNames[numArrays]={"arr07","arr0max","arrNearlySorted","arrReverseSorted"};
    int *arrays[numArrays]={arr07,arr0max,arrNearlySorted,arrReverseSorted};

    // Массив указателей на функции
    void (*sortFunctions[numSort])(int *, int)={bubbleSort,bubbleSort1,bubbleSort2,insertionSort,binInsertionSort,countingSort,radixSort};

    // Цикл проверки
    for(int i=1000;i<=N;i+=1000)
    {
        for(int z=0;z<numSort;++z)
        {
            long long time[numArrays]={0};
            for(int j=0;j<numArrays;++j)
            {
                // Тестовые прогоны для оптимизирующего компилятора
                for(int g=0;g<numCompileTests;++g)
                {
                    int *testArray=arrayCopy(arrays[j],i);
                    (*sortFunctions[z])(testArray,i);
                    delete [] testArray;
                }
                for(int k=0;k<numLaunch;++k)
                {
                    // Подсчет времени
                    int *curArray=arrayCopy(arrays[j],i);
                    high_resolution_clock::time_point t1 = high_resolution_clock::now();
                    (*sortFunctions[z])(curArray,i);
                    high_resolution_clock::time_point t2 = high_resolution_clock::now();
                    time[j]+=(duration_cast<nanoseconds>( t2 - t1 ).count());
                    delete [] curArray;
                }
                // Запись результатов в файл
                output<<i<<";"<<sortNames[z]<<";"<<arrNames[j]<<";"<<time[j]/numLaunch<<endl;
                
            }
            delete [] time;
        }
    }
    delete [] arrays;
    return 0;
}

// Сортировка подсчетом
void countingSort(int *arr, int n)
{
    int max=0,min=INT8_MAX;
    for(int i=0;i<n;++i)
        if(arr[i]>max)
        {
            if (max < min)
                min = max;
            max = arr[i];
        }
        else if(arr[i]<min)
                min=arr[i];

    int *counter = new int[max - min + 1];
    int *sorted = new int[n];

    for (int i = 0; i < max - min + 1; ++i)
        counter[i] = 0;

    for (int i = 0; i < n; ++i)
        counter[arr[i] - min]++;

    for (int j = 1; j < max - min + 1; ++j)
        counter[j] += counter[j - 1];

    for (int i = n - 1; i >= 0; --i)
    {
        counter[arr[i] - min]--;
        sorted[counter[arr[i] - min]] = arr[i];
    }
    arr = sorted;
    delete [] sorted;
    delete [] counter;
}

// Цифровая сортировка
void radixSort(int *arr, int n)
{
    int base=256;
    union un
    {
        unsigned int a;
        unsigned char ch[4];
    };
    un *temp = new un[n];
    int *cnt = new int[base];
    un sorted[n];
    for (int i = 0; i < n; ++i)
        temp[i].a = (unsigned int) arr[i];

    for (int exp = 0; exp < 4; ++exp)
    {
        for (int i = 0; i < base; ++i)
            cnt[i] = 0;
        for (int i = 0; i < n; ++i)
            cnt[temp[i].ch[exp]]++;
        for (int j = 1; j < base; ++j)
            cnt[j] += cnt[j - 1];
        for (int i = n - 1; i >= 0; --i)
        {
            cnt[temp[i].ch[exp]]--;
            sorted[cnt[temp[i].ch[exp]]] = temp[i];
        }
        for (int i = 0; i < n; ++i)
            temp[i] = sorted[i];
    }
    delete [] cnt;
    delete [] temp;
    for (int i = 0; i < n; ++i)
        arr[i] = sorted[i].a;
}
// Сортировка бинарными вставками
void binInsertionSort(int *arr,int n)
{
    for (int i = 1; i < n; ++i)
        if (arr[i - 1] > arr[i])
        {
            int temp = arr[i];
            // Бинарный поиск
            int l=0,r=i,el=temp;
            int m = (l + r) / 2;
            while (l != m)
            {
                if (arr[m] > el)
                    r = m - 1;
                else
                    l = m;
                m = (l + r) / 2;
            }
            if (arr[l] < el)
                if (el > arr[r])
                    l = r + 1;
                else
                    l = r;

            for (int j = i; j >= l; --j)
                arr[j] = arr[j - 1];

            arr[l] = temp;
        }
}

// Сортировка вставками
void insertionSort(int *arr,int n)
{
    for (int i = 1; i < n; ++i)
    {
        int temp = arr[i];
        int j;
        for (j = i - 1; j >= 0 && arr[j] > temp; --j)
            arr[j + 1] = arr[j];

        arr[j+1] = temp;
    }
}

// Обычный пузырек
void bubbleSort(int *arr, int n)
{
    for(int i=0;i<n-1;++i)
        for(int j=0;j<n-1;++j)
            if(arr[j]>arr[j+1])
            {
                int t=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=t;
            }
}

// Пузырек с первым условием Айверсона
void bubbleSort1(int *arr, int n)
{
    //Отсортирован ли массив?
    bool exit = true;
    while (exit)
    {
        exit = false;
        for (int i = 0; i < (n - 1); ++i)
            if (arr[i] > arr[i + 1])
            {
                int t=arr[i];
                arr[i]=arr[i+1];
                arr[i+1]=t;
                // Замена элементов была, значит массив не факт что отсортирован
                exit = true;
            }

    }
}

// Пузырек со вторым условием Айверсона
void bubbleSort2(int* arr, int n)
{
    int region=n-1;
    while(region != 0)
    {
        int last_swap=0;
        for (int j = 0; j < region; ++j)
            if (arr[j] > arr[j + 1])
            {
                int t=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=t;
                last_swap = j;
            }

        region = last_swap;
    }
}

void generateTests(int *arr07,int *arr0max,int *arrNearlySorted,int *arrReverseSorted)
{
    // Генерация элементов массива
    for(int i=0;i<N;++i)
    {
        arr07[i]=rand()%7;
        arr0max[i]=rand();
        arrNearlySorted[i]=rand()%N;
        arrReverseSorted[i]=rand()%N;
    }
    // "Почти" отсортированный массив
    bubbleSort2forTest(arrNearlySorted,N);
    int t=arrNearlySorted[500];
    arrNearlySorted[500]=arrNearlySorted[501];
    arrNearlySorted[501]=t;
    t=arrNearlySorted[1100];
    arrNearlySorted[1100]=arrNearlySorted[1560];
    arrNearlySorted[1560]=t;
    t=arrNearlySorted[2890];
    arrNearlySorted[2890]=arrNearlySorted[2995];
    arrNearlySorted[2995]=t;
    t=arrNearlySorted[5000];
    arrNearlySorted[5000]=arrNearlySorted[5010];
    arrNearlySorted[5010]=t;
    t=arrNearlySorted[8362];
    arrNearlySorted[8362]=arrNearlySorted[8675];
    arrNearlySorted[8675]=t;
    //Массив отсортированный в обратном порядке
    bubbleSort2forTest(arrReverseSorted,N);
    reverseArray(arrReverseSorted,N);

}

// Копирование массива
int* arrayCopy(int *arr,int n)
{
    int *res = new int[n];
    for (int i = 0; i < n; ++i)
        res[i] = arr[i];
    return res;
}

// Реверс массива
void reverseArray(int *arr,int n)
{
    for(int i=0;i<n/2;++i)
    {
        int t=arr[i];
        arr[i]=arr[n-i-1];
        arr[n-i-1]=t;
    }
}
// Пузырек со вторым условием Айверсона
void bubbleSort2forTest(int* arr, int n)
{
    int region=n-1;
    while(region != 0)
    {
        int last_swap=0;
        for (int j = 0; j < region; ++j)
            if (arr[j] > arr[j + 1])
            {
                int t=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=t;
                last_swap = j;
            }

        region = last_swap;
    }
}




