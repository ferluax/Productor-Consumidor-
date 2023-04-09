#include <pthread.h> //hilos y mutex
#include <semaphore.h> //semaforos
#include <iostream>

#define ART_PRODUCTOR 5 //maximo de productos
#define T_BUFFER 5 //tamaño del buffer

//se definen los semaforos
sem_t emptys; 
sem_t full;

int in = 0; //indica quien coloca el siguiente producto
int out = 0; //indica quien consume el siguiente producto
int buffer[T_BUFFER]; //arreglo con el tamaño de buffer
pthread_mutex_t mtx; //declara mutex

void *productor(void *pNumero);
void *consumidor(void *cNumero);

using namespace std;

int main()
{
    int i;
    pthread_t pro[5], con[5]; //arreglo de productores y consumidores
    
    //inicia los semaforos y mutex
    pthread_mutex_init(&mtx, NULL); 
    sem_init(&emptys, 0, T_BUFFER);
    sem_init(&full, 0, 0);

    int a[5] = {1,2,3,4,5};//enumera a los productores y consumidores

    //se crean los hilos de productores y consumidores
    for (i = 0; i < 5; i++)
    {
        pthread_create(&pro[i], NULL, productor, (void *)&a[i]);
    }

    for (i = 0; i < 5; i++)
    {
        pthread_create(&con[i], NULL, consumidor, (void *)&a[i]);
    }
    
    //pone a trabajar a las dos funciones
    for (i = 0; i < 5; i++)
    {
        pthread_join(pro[i], NULL);
    }

    for (i = 0; i < 5; i++)
    {
        pthread_join(con[i], NULL);
    }
    
    //se destruyen los semaforos y mutex
    pthread_mutex_destroy(&mtx);
    sem_destroy(&emptys);
    sem_destroy(&full);

    return 0;
}

void *productor(void *pNumero)
{
    int i, item;

    for (i = 0; i < ART_PRODUCTOR; i++) //se estara repitiendo hasta que sea menor al maximo de productos
    {
        item = rand(); //genera numero random
        sem_wait(&emptys);// se indica que la ranura del buffer esta vacia y el productor puede agregar algo
        pthread_mutex_lock(&mtx);
        buffer[in] = item;

        cout<< "Productor no."<<*((int *)pNumero)<<" agrego el producto "<<buffer[in]<<" en "<<in<<"\n";
        in = (in + 1)%T_BUFFER;
        pthread_mutex_unlock(&mtx); //indicamos que el buffer esta lleno
        sem_post(&full);
    }
    
}

void *consumidor(void *cNumero)
{
    int i, item;

    for (i = 0; i < ART_PRODUCTOR; i++)
    {
        sem_wait(&full);//se indica que el buffer esta lleno de productos que consumidor pueda consumir
        pthread_mutex_lock(&mtx);
        item = buffer[out];

        cout<< "Consumidor no."<< *((int *)cNumero)<< " quito el producto "<<item<< " de "<<out<<"\n";
        out = (out + 1)%T_BUFFER;
        pthread_mutex_unlock(&mtx);//indicamos que ya no hay mas productos en el buffer
        sem_post(&emptys);
    }
    
}