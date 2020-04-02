#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

bool isNormalMode = true, isCaptureMode = false, isSearching = false, doneFinding = false, isLullabyEffect = false;
pthread_t tid1,tid2,tid3,tid4[7];

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

typedef struct tamedPokemon{
    bool shiny;
    char name[50];
    int AP;
    int rarity;
    int price;
}tamedPokemon;

typedef struct pokedexList{
    tamedPokemon* tamed_pokemon[7];
    int totalPokemon;
}pokedexList;

typedef struct inventory{
    int pokeball;
	int berry;
	int pokedollar;
	int lullabyPowder;
}inventory;

pokemon* sharePokemon ;
shop* shareShop ;
tamedPokemon* tamedNewPokemon ;
pokedexList* pokedex_List ;
inventory* inven;

void normalMode();
void captureMode();
void *cariPokemon();
void pokedex();
void pokeshop();

int main(){

    key_t key = 1234;
    int shmid = shmget(key,sizeof(pokemon),IPC_CREAT | 0666);
    sharePokemon = shmat(shmid, NULL, 0);

    key_t key1 = 1235;
    int shmid2 = shmget(key1,sizeof(shop),IPC_CREAT | 0666);
    shareShop = shmat(shmid2, NULL, 0);

    tamedNewPokemon = malloc(sizeof(tamedPokemon));
    pokedex_List = malloc(sizeof(pokedexList));
    inven = malloc(sizeof(inventory));
	memset(sharePokemon, 0, sizeof(sharePokemon));
    memset(tamedNewPokemon,0,sizeof(tamedNewPokemon));
	memset(pokedex_List, 0, sizeof(pokedex_List));
    memset(inven,0,sizeof(inven));
    
    inven->pokeball = 10;

    while(1){ //gameloop
        if(isNormalMode){
            normalMode();
        }
        else if(isCaptureMode){
            captureMode();
        }
    }

    return 0;
}

void normalMode(){
    char curStat1[100] = {"Sedang mencari.."}, curStat2[100] = {"stand by.."}, gantiOption1[100]={"Berhenti Mencari"}, gantiOption2[100]={"Cari Pokemon"};
    printf("NORMAL MODE---------------------------\n");
    if(isSearching){
        printf("Status : %s\n", curStat1);
        printf("1. %s", gantiOption1);
    } 
    else {
        printf("Status : %s\n", curStat2);
        printf("1. %s", gantiOption2);
    }
    printf("\n2. Pokedex\n3. Shop\n" );
    printf("--------------------------------------\n");

    
    int input=0;
    scanf("%d",&input);
    if(input==1){ //cari pokemon
        if(!isSearching){
            doneFinding=false;
            pthread_create(&tid1,NULL,cariPokemon,NULL);

            isSearching=true;
        }
        else{
            doneFinding=true;
            pthread_cancel(tid1);
            isSearching=false;
        }
    }
    else if(input==2){ //pokedex
        pokedex();
    }
    else if(input==3){ //shop
        pokeshop();
    }
    else{
        printf("input salah\n");
    }
}

void *escapePokemon(void *arg){
    while(1){
        sleep(20);
        int randEscape = rand()%100;
        if(isLullabyEffect) continue;
        int shiny_eff=0;
        if(sharePokemon->shiny)shiny_eff=5; 
        if(randEscape <= sharePokemon->escapeRate+shiny_eff){
            //kabur
            sharePokemon->isFound=false;
            break;
        }
    }
}

void *lullabyEffect(void *arg){
    isLullabyEffect=true;
    sleep(10);
    isLullabyEffect=false;
}

void *turuninAP(void *arg){
    int x = pokedex_List->totalPokemon-1;
    while(1){
        sleep(10);
        pokedex_List->tamed_pokemon[x]->AP -= 10;

        if(pokedex_List->tamed_pokemon[x]->AP <=0){
            int chance = rand()%100;
            if(chance<=10){
                pokedex_List->tamed_pokemon[x]->AP = 50;
            }
            else{
                int index=x;
                if(index == pokedex_List->totalPokemon || pokedex_List->totalPokemon==1){
                    pokedex_List->totalPokemon--;
                }
                else{
                    for(int i=index-1;i<pokedex_List->totalPokemon;i++){
                        if(i+1==pokedex_List->totalPokemon-1){
                            pokedex_List->tamed_pokemon[i] = pokedex_List->tamed_pokemon[i+1]; 
                            pokedex_List->tamed_pokemon[i+1] = NULL;
                            pokedex_List->totalPokemon--;
                            break;
                        }
                    pokedex_List->tamed_pokemon[i] = pokedex_List->tamed_pokemon[i+1]; 
                    }
                }
            }
        }
    }
}

void captureMode(){
    pthread_create(&tid3,NULL,escapePokemon,NULL);
    int input=0;

    while(sharePokemon->isFound){
        printf("CAPTURE MODE---------------------------\n");
        printf("nama Pokemon   : %s\n",sharePokemon->name);
        printf("rarity         : %d\n",sharePokemon->rarity);
        printf("escape Rate    : %d\n",sharePokemon->escapeRate);
        printf("capture Rate   : %d\n",sharePokemon->captureRate);
        printf("Action : \n");
        printf("1. tangkap\n2. item\n3. keluar\n");
        printf("---------------------------------------\n");
        scanf("%d",&input);
        if(!sharePokemon->isFound){
            printf("pokemon telah kabur :(");
            fflush(stdout);
            break;
        }
        if(input==1){ //tangkap
            if(inven->pokeball <= 0){
                printf("pokeball habis!\n");
                break;
            }
            else{
                inven->pokeball--;
                int randPokemonCaptureRate = rand()%100;
                int lullaby_eff =0, shiny_eff=0;
                if(lullabyEffect) lullaby_eff=20;
                if(sharePokemon->shiny) shiny_eff = -20;
                if(randPokemonCaptureRate <= sharePokemon->captureRate + lullaby_eff + shiny_eff){
                    if(pokedex_List->totalPokemon >=7){
                        inven->pokedollar += sharePokemon->price;
                        printf("pokedex sudah penuh, gabisa nangkep sorry bro\n");
                        break;
                    }
                    tamedNewPokemon = malloc(sizeof(tamedPokemon));
                    pthread_cancel(tid3);
                    strcpy(tamedNewPokemon->name, sharePokemon->name);
                    tamedNewPokemon->rarity = sharePokemon->rarity;
                    tamedNewPokemon->price = sharePokemon->price;
                    tamedNewPokemon->AP = 100;
                    tamedNewPokemon->shiny = sharePokemon->shiny;
                    pokedex_List->tamed_pokemon[pokedex_List->totalPokemon] = tamedNewPokemon;
                    pokedex_List->totalPokemon++;

                    pthread_create(&tid4[pokedex_List->totalPokemon-1],NULL,turuninAP,NULL);

                    printf("tertangkap nihhhh yeee, total pokemon : %d\n",pokedex_List->totalPokemon);
                    sharePokemon->isFound=false;
                    break;
                }
                else{
                    printf("tidak tertangkap duh\n");
                }
            }
        }
        else if(input==2){//item
            if(inven->lullabyPowder<=0){
                printf("kamu tidak punya lullaby powder\n");
            }
            else{
                inven->lullabyPowder--;
                pthread_create(&tid2,NULL,lullabyEffect,NULL);
            }
        }
        else if(input==3){
            isNormalMode = true;
            isCaptureMode = false;
        }
        else printf("input salah\n");
    }
    isNormalMode = true;
    isCaptureMode = false;
}

void *cariPokemon(void *arg){
    while(!doneFinding){
        sleep(5);
        int chance = rand()%100;
        if(chance <= 60) { //ketemu pokemon
            sharePokemon->isFound = true;
            printf("Pokemon ditemukan!\n");
            printf("nama : %s\n",sharePokemon->name);
            fflush(stdout);
            isNormalMode = false;
            isCaptureMode = true;
    
            printf("input 1 untuk battle!\n");
            pthread_cancel(tid1);
        }
        else{
            printf("tidak menemukan pokemon :(\n");
        }
    }
}

void pokedex(){
    printf("POKEDEX---------------------------\n");
    if(pokedex_List->totalPokemon<=0){
        printf("kamu tidak punya pokemon :(\n");
    }
    else{
        for(int i=0;i<pokedex_List->totalPokemon;i++){
            printf("%d.\n",i+1);
            printf("Name    : %s\n",pokedex_List->tamed_pokemon[i]->name);
            printf("AP      : %d\n",pokedex_List->tamed_pokemon[i]->AP);
            printf("Rarity  : %d ",pokedex_List->tamed_pokemon[i]->rarity);
            if(pokedex_List->tamed_pokemon[i]->shiny) printf("(Shiny)\n");
            else printf("\n");
            printf("\n");
        }
    }
    printf("Berry   : %d\n",inven->berry);
    printf("1. Beri Berry\n2. Lepas Pokemon\n3. Keluar\n");
    int input;
    scanf("%d",&input);
    if(input==1){
        if(inven->berry <=0 ){
            printf("berry tidak cukup\n");
        }
        else{
            if(pokedex_List->totalPokemon<=0){
                printf("kamu tidak punya pokemon untuk diberi Berry :(\n");
            }
            else{
                inven->berry--;
                for(int i = 0; i < pokedex_List->totalPokemon; i++){
                    pokedex_List->tamed_pokemon[i]->AP +=10;
                    if(pokedex_List->tamed_pokemon[i]->AP >= 100) pokedex_List->tamed_pokemon[i]->AP=100;
				}
                printf("Berry telah diberikan \n");
            }
        }
    }
    else if(input==2){
        if(pokedex_List->totalPokemon<=0){
            printf("kamu tidak punya pokemon untuk dilepas\n");
        }
        else{
            int index;
            printf("pilih pokemon yang mau dilepas\n");
            scanf("%d",&index);
            if(index == pokedex_List->totalPokemon || pokedex_List->totalPokemon==1){
                pokedex_List->totalPokemon--;
            }
            else{
                for(int i=index-1;i<pokedex_List->totalPokemon;i++){
                    if(i+1==pokedex_List->totalPokemon-1){
                        pokedex_List->tamed_pokemon[i] = pokedex_List->tamed_pokemon[i+1]; 
                        pokedex_List->tamed_pokemon[i+1] = NULL;
                        pokedex_List->totalPokemon--;
                        break;
                    }
                    pokedex_List->tamed_pokemon[i] = pokedex_List->tamed_pokemon[i+1]; 
                }
            }
            if(pokedex_List->tamed_pokemon[index-1]->shiny)
                inven->pokedollar += pokedex_List->tamed_pokemon[index-1]->price + 5000;
            else
                inven->pokedollar += pokedex_List->tamed_pokemon[index-1]->price;
            printf("pokemon telah dilepas\n");
        }
    }
    else if(input==3){
        isNormalMode = true;
        isCaptureMode = false;
    }
    else{
        printf("input tidak valid");
    }
}

void pokeshop(){
    printf("POKESHOP---------------------------\n");
    printf("1.Pokeball\nStock   : %d\nPrice   : %d\n",shareShop->pokeball,shareShop->pokeballPrice);
    printf("2.Berry\nStock   : %d\nPrice   : %d\n",shareShop->berry,shareShop->berryPrice);
    printf("3.Lullaby Powder\nStock   : %d\nPrice   : %d\n",shareShop->lullabyPowder,shareShop->lullabyPowderPrice);

    printf("\nYour Item : \n");
    printf("Pokeball        : %d\n",inven->pokeball);
    printf("Berry           : %d\n",inven->berry);
    printf("Lullaby Powder  : %d\n",inven->lullabyPowder);
    printf("\nYour Pokedollar : %d\n",inven->pokedollar);
    int input;
    printf("input 1,2,3 untuk membeli. 4 untuk keluar\n");
    scanf("%d",&input);
    if(input==1){ //beli pokeball
        if(inven->pokedollar < shareShop->pokeballPrice){
			printf("Pokedollar kurang :(\n");
		}else{
			if(shareShop->pokeball > 0){
				printf("Pokeball berhasil dibeli sejumlah 1\n");
				inven->pokeball++;
				inven->pokedollar-=shareShop->pokeballPrice;
			}else{
				printf("Stock habis\n");
			}
		}
    }
    else if(input==2){//beli berry
        if(inven->pokedollar < shareShop->berryPrice){
			printf("Pokedollar kurang :(\n");
		}else{
			if(shareShop->berry > 0){
				printf("Berry berhasil dibeli sejumlah 1\n");
				inven->berry++;
				inven->pokedollar-=shareShop->berryPrice;
			}else{
				printf("Stock habis\n");
			}
		}
    }
    else if(input==3){//beli lullaby powder
        if(inven->pokedollar < shareShop->lullabyPowderPrice){
			printf("Pokedollar kurang :(\n");
		}else{
			if(shareShop->lullabyPowder > 0){
				printf("Lullaby Powder berhasil dibeli sejumlah 1\n");
				inven->lullabyPowder++;
				inven->pokedollar-=shareShop->lullabyPowderPrice;
			}else{
				printf("Stock habis\n");
			}
		}
    }
    else{
        printf("invalid input\n");
    }

}
