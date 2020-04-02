#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

char pokemonsNormal[5][100];
char pokemonsRare[5][100];
char pokemonsLegendary[5][100];

typedef struct pokemon{
    bool isFound;
    bool shiny;
    char name[50];
    int rarity;
    int price;
    int captureRate;
    int escapeRate;
}pokemon;

typedef struct shop{
	int pokeball;
	int pokeballPrice;
	int lullabyPowder;
	int lullabyPowderPrice;
	int berry;
	int berryPrice;
}shop;

pthread_t tid1,tid2;
bool done=false;

pokemon *sharePokemon;
shop *shareShop;

void *preparePokemon();
void shutdownGame();
void *pokeshop();

int main(){
    strcpy(pokemonsNormal[0] , "Bulbasaur");
    strcpy(pokemonsNormal[1] , "Charmander");
    strcpy(pokemonsNormal[2] , "Squirtle");
    strcpy(pokemonsNormal[3] , "Rattata");
    strcpy(pokemonsNormal[4] , "Caterpie");

    strcpy(pokemonsRare[0] , "Pikachu");
    strcpy(pokemonsRare[1] , "Eevee");
    strcpy(pokemonsRare[2] , "Jigglypuff");
    strcpy(pokemonsRare[3] , "Snorlax");
    strcpy(pokemonsRare[4] , "Dragonite");

    strcpy(pokemonsLegendary[0] , "Mew");
    strcpy(pokemonsLegendary[1] , "Mewtwo");
    strcpy(pokemonsLegendary[2] , "Moltres");
    strcpy(pokemonsLegendary[3] , "Zapdos");
    strcpy(pokemonsLegendary[4] , "Articuno");
    
    key_t key = 1234;
    int shmid = shmget(key,sizeof(pokemon),IPC_CREAT | 0666);
    sharePokemon = shmat(shmid, NULL, 0);
	memset(sharePokemon, 0, sizeof(sharePokemon));
    
    key_t key1 = 1235;
    int shmid2 = shmget(key1,sizeof(shop),IPC_CREAT | 0666);
    shareShop = shmat(shmid2, NULL, 0);
	memset(shareShop, 0, sizeof(shareShop));
    
    pthread_create(&tid1,NULL,preparePokemon,NULL);
    pthread_create(&tid2,NULL,pokeshop,NULL);

    printf("POKEZONE------------------\n");
    printf("1. exit\n----------------------\n");
    int choice;
    scanf("%d",&choice);
    if(choice==1)
        shutdownGame();
    else printf("Input tidak valid\n");

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    shmdt(sharePokemon);
    shmctl(shmid,IPC_RMID, NULL);

    shmdt(shareShop);
    shmctl(shmid2,IPC_RMID, NULL);

    return 0;
}

void *preparePokemon(void *arg){
    while(!done){
        if(sharePokemon->isFound) continue;

        int encounterRate = rand()%100;
        if(encounterRate<=80){
            //normal
            int randomPokemons = rand()%5 ;
            strcpy(sharePokemon->name,pokemonsNormal[randomPokemons]);
            sharePokemon->rarity = 0;
            sharePokemon->price = 80;
            sharePokemon->escapeRate = 5;
            sharePokemon->captureRate = 70;
            int isShiny = rand()%8000;
            if(isShiny == 1) sharePokemon->shiny=true;
        }
        else if(encounterRate>80 && encounterRate<=95){
            //rare
            int randomPokemons = rand()%5;
            strcpy(sharePokemon->name,pokemonsRare[randomPokemons]);
            sharePokemon->rarity = 1;
            sharePokemon->price = 100;
            sharePokemon->escapeRate = 10;
            sharePokemon->captureRate = 50;
            int isShiny = rand()%8000;
            if(isShiny == 1) sharePokemon->shiny=true;
        }
        else {
            //legendary
            int randomPokemons = rand()%5;
            strcpy(sharePokemon->name,pokemonsLegendary[randomPokemons]);
            sharePokemon->rarity = 2;
            sharePokemon->price = 200;
            sharePokemon->escapeRate = 20;
            sharePokemon->captureRate = 30;
            int isShiny = rand()%8000;
            if(isShiny == 1) sharePokemon->shiny=true;
        }
        sleep(1);
    }

}

void *pokeshop(void * arg){
	shareShop->lullabyPowder = 100;
	shareShop->lullabyPowderPrice = 60;
	shareShop->berry = 100;
	shareShop->berryPrice = 5;
	shareShop->pokeball = 100;
	shareShop->pokeballPrice = 15;
	while(!done){
		sleep(10);
		shareShop->pokeball += 10;
		if(shareShop->pokeball > 200)shareShop->pokeball = 200;
		shareShop->berry += 10;
		if(shareShop->berry > 200)shareShop->berry = 200;
		shareShop->lullabyPowder += 10;
		if(shareShop->lullabyPowder > 200)shareShop->lullabyPowder = 200;
	}
}

void shutdownGame(){
    pid_t p = getpid();
	if(fork()==0){
		char pid[10];
		snprintf(pid, sizeof(pid), "%d", p);
		char * argv[] = {"kill", "-9", pid, NULL};
		execv("/bin/kill", argv);
	}
}
