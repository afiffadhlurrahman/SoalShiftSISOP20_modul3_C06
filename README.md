# SoalShiftSISOP20_modul3_C06
thread, ipc


## Soal 1 Pokemon

terdapat 2 script

### Soal1_Pokezone
membuat struct untuk share memory yang berisi pokemon dan shop
```
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

```
deklarasi pokemon pada awal int main
```
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
```
pembuatan share memory untuk struct pokemon dan shop
```
    key_t key = 1234;
    int shmid = shmget(key,sizeof(pokemon),IPC_CREAT | 0666);
    sharePokemon = shmat(shmid, NULL, 0);
	memset(sharePokemon, 0, sizeof(sharePokemon));
    
    key_t key1 = 1235;
    int shmid2 = shmget(key1,sizeof(shop),IPC_CREAT | 0666);
    shareShop = shmat(shmid2, NULL, 0);
	memset(shareShop, 0, sizeof(shareShop));
```
pembuatan thread agar bisa berjalan bersamaan dan selalu update
``` 
    pthread_create(&tid1,NULL,preparePokemon,NULL);
    pthread_create(&tid2,NULL,pokeshop,NULL);
```
menu pada pokezone
```
printf("POKEZONE------------------\n");
    printf("1. exit\n----------------------\n");
    int choice;
    scanf("%d",&choice);
    if(choice==1)
        shutdownGame();
    else printf("Input tidak valid\n");
```
thread share pokemon yang selalu berjalan dan update setiap 1 detik
```
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
        sleep(1); //berjalan setiap 1 detik
    }

}
```
thread shop
```
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
```
fungsi untuk shutdown game dengan fork exec
```
void shutdownGame(){
    pid_t p = getpid();
	if(fork()==0){
		char pid[10];
		snprintf(pid, sizeof(pid), "%d", p);
		char * argv[] = {"kill", "-9", pid, NULL};
		execv("/bin/kill", argv);
	}
}
```
### Soal1_Traizone

pembuatan struct awal
```
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
```
share memory yang berhubungan dengan pokezone
```
key_t key = 1234;
    int shmid = shmget(key,sizeof(pokemon),IPC_CREAT | 0666);
    sharePokemon = shmat(shmid, NULL, 0);

    key_t key1 = 1235;
    int shmid2 = shmget(key1,sizeof(shop),IPC_CREAT | 0666);
    shareShop = shmat(shmid2, NULL, 0);

```
game loop yang terdiri dari normal mode dan capture mode
```
while(1){ //gameloop
        if(isNormalMode){
            normalMode();
        }
        else if(isCaptureMode){
            captureMode();
        }
    }
```
fungsi normal mode
```
void normalMode(){
    char curStat1[100] = {"Sedang mencari.."}, curStat2[100] = {"stand by.."}, 
gantiOption1[100]={"Berhenti Mencari"}, gantiOption2[100]={"Cari Pokemon"};
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
```
fungsi cari pokemon
```
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
```
fungsi capture mode
```
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

```
thread lullaby effect, escapePokemon, turunin AP
```
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
```
pokedex
```
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
```
Shop
```
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
```
## Soal 2
## Soal 3
mengelompokan ekstensi file kedalam foldernya masing masing dengan nama ekstensi tersebut
terdapat 3 perintah berbeda yaitu -f, -d , dan *
```
pthread_t tid[1000];

char* nowDir, *tempDir;

void *moveFile();
void *moveFile2();
void *moveFileBintang();

int main(int argc, char **argv){
    char tes[1000];
    nowDir = getcwd(tes,sizeof(tes));
    printf("dir : %s\n", nowDir);
    if(!strcmp(argv[1],"-f")){
        printf("1\n");
        for(int i=0;i<argc-2;i++){
            pthread_create(&tid[i],NULL,moveFile,(void *)argv[i+2]);
        }
        for(int i=0;i<argc-2;i++){
            pthread_join(tid[i],NULL);
        }
    }
    else if(!strcmp(argv[1],"-d")){
        printf("2\n");
        int index=0;
        chdir(argv[2]); //ganti directory
        tempDir = getcwd(tes,sizeof(tes));
        struct dirent* dp;
        struct stat file;
        DIR* fd = opendir(".");
        if(fd){
            while((dp = readdir(fd)) != NULL){
                if (stat(dp->d_name, &file) < 0);
                else if (!S_ISDIR(file.st_mode))
                {
                    pthread_create(&tid[index],NULL,moveFile2,(void *)dp->d_name);
                }
                index++;
            }
            for(int j=0;j<(index-2);j++){
                pthread_join(tid[j],NULL);
            }
        }

    }
    else if(!strcmp(argv[1],"*")){
        printf("3\n");
        int index=0;
        struct dirent* dp;
        struct stat file;
        DIR* fd = opendir(".");
        if(fd){
            while((dp = readdir(fd)) != NULL){
                if (stat(dp->d_name, &file) < 0);
                else if (!S_ISDIR(file.st_mode))
                {
                    pthread_create(&tid[index],NULL,moveFileBintang,(void *)dp->d_name);
                }
                index++;
            }
            for(int j=0;j<(index-2);j++){
                pthread_join(tid[j],NULL);
            }
        }
    }

    return 0;
}

char* getFileName(char name[]){
    char* fileName, *final;
    fileName = strchr(name,'/');
    if(fileName == NULL)return name;
    else{
        while (fileName != NULL) {
            final = fileName+1;
            fileName = strchr(fileName+1,'/');
        }
    }
    return final;
}
```
mendapatkan extensi file dengan getExtentionFile, menggunakan strchr
```
char *getExtentionFile(char name[]){
    char *ex = strchr(getFileName(name),'.'); 
    if(ex==NULL) return NULL;
    else{
        return ex+1;
    }
}
```
memindahkan file dengan fopen
```
void moveFileNow(char s[],char d[]){ //mindahin file
    int c;
    FILE *f1, *f2;
    f1 = fopen(s,"r");
    f2 = fopen(d,"w");
    if (!f1) {
        printf("Unable to open source file to read!\n");
        fclose(f2);
        return ;
    }
    if (!f2) {
        printf("Unable to open target file to write!\n");
        return ;
    }
    while ((c = fgetc(f1)) != EOF)fputc(c, f2);
    fclose(f1);
    fclose(f2);
    remove(s);
}
```
memindahkan file dengan perintah -f (memindahkan file yang kita input di argumen
```
void *moveFile(void *arg){
    char* extention ;
    extention= getExtentionFile((char *)arg);
    char destinationFolder[1000];
    if(extention==NULL) strcpy(destinationFolder,"Unknown");
    else{
        strcpy(destinationFolder,extention);
        //typecast huruf besar ke huruf kecil
        for(int i=0;i<strlen(destinationFolder);i++){
            if(destinationFolder[i]>=65 && destinationFolder[i]<=98){
                destinationFolder[i]+=32;
            }
        }
    }
    mkdir(destinationFolder,0777);
    char destinationPath[1000];
    snprintf(destinationPath, sizeof(destinationPath), "%s/%s/%s", nowDir,destinationFolder,getFileName((char*)arg));
    moveFileNow((char *)arg, destinationPath);
}
```
memindahkan file dengan perintah -d (memindahkan dengan input directory)
```
void *moveFile2(void *arg){
    char *extention;
    extention = getExtentionFile((char *)arg);
    char destinationFolder[1000];
    if(extention==NULL) strcpy(destinationFolder,"Unknown");
    else{
        strcpy(destinationFolder, extention);
        //typecast huruf besar ke huruf kecil
        for(int i=0;i<strlen(destinationFolder);i++){
            if(destinationFolder[i]>=65 && destinationFolder[i]<=98){
                destinationFolder[i]+=32;
            }
        }
    }
    mkdir(destinationFolder,0777);
    char destinationPath[1000],sourcePath[1000];
    snprintf(sourcePath, sizeof(sourcePath), "%s/%s", tempDir, (char *)arg);
    snprintf(destinationPath, sizeof(destinationPath), "%s/%s/%s", nowDir, destinationFolder, getFileName((char *)arg));
    moveFileNow(sourcePath, destinationPath);
}
```
memindahkan file dengan perintah * (memindahkan semua file di directory sekarang)
```
void *moveFileBintang(void *arg){
    char *extention ;
    extention = getExtentionFile((char *)arg);
    char destinationFolder[1000];
    if(extention==NULL) strcpy(destinationFolder,"Unknown");
    else{
        strcpy(destinationFolder, extention);
        //typecast huruf besar ke huruf kecil
        for(int i=0;i<strlen(destinationFolder);i++){
            if(destinationFolder[i]>=65 && destinationFolder[i]<=98){
                destinationFolder[i]+=32;
            }
        }
    }
    mkdir(destinationFolder,0777);
    char destinationPath[1000],sourcePath[1000];
    snprintf(sourcePath, sizeof(sourcePath), "%s/%s", nowDir, (char *)arg);
    snprintf(destinationPath, sizeof(destinationPath), "%s/%s/%s", nowDir, destinationFolder, getFileName((char *)arg));
    moveFileNow(sourcePath, destinationPath);

}

```
## Soal 4
