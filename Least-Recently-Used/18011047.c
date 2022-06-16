#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct primes{
	int flag;
};

struct personList{
	char id[10]; //tc kimlik numaras�n� saklayacak olan karakter dizisi
	int birthDate; //do�um tarihini saklayacak olan integer
	char fullName[20]; //ki�inin isim-soyismini saklayacak olan karakter dizisi
	char city[10]; //ki�inin �ehrini saklayacak olan karakter dizisi
	struct personList* next; //linki listenin sonraki eleman�n� ifade eden pointer
};

struct hash{
	char id[10]; //tc kimlik numaras�n� saklayacak olan karakter dizisi
	int order; //ki�inin cache bellekte hangi s�rada oldu�unu ifade eden integer de�er
	int flag; //hash tablosundaki g�z�n dolu, bo� veya silinmi� olma durumunu ifade eden de�er
};

int calculatePrimeValue(int n); //cache boyutundan b�y�k en k���k asal say�y� hesaplar, 
unsigned long keyValue(char* id); //Horner metodu kullanarak TC kimlik numaras�n�n key de�erini hesaplayan fonksiyon
int calculateHashIndex(int i, int key, int tableSize); //Horner metodundan gelen key de�eri ile double hashing kullanarak index hesaplayan fonksiyon
void insertToHashTable(struct hash *hashTable, char id[10], int tableSize); //hash tablosuna TC kimlik numaras� verilen ki�iyi kaydeder
struct personList *deleteFromCache(struct personList* persons, struct hash* hashTable, int tableSize); //cache uzunlu�u a��ld���nda sondaki eleman� silen fonksiyon
struct personList *insertToCache(struct personList* persons, char id[10], int bDate, char firstName[10], char lastName[10], char city[10]); //kontroller sonunda cache'e eklenmesi kararla�t�r�lan ki�iyi cache'in ba��na ekler
int search(char id[10], struct hash* hashTable, int tableSize);  //hash tablosunda horner metodu ile TC kimlik numaras� hesaplanm�� ki�iyi arat�r. bulunamazsa -1, bulunursa index numaras� d�nd�r�l�r
struct personList *updateCache(struct personList* persons, int order); //o anki eleman daha evvel eklenmi�se eleman� en ba�a getiren, cache'i g�ncelleyen fonksiyon

int main(){
	int i=0, j, k = 2, cacheSize, tableSize, key, index, searchIndex, bDate; //i, j, k indis de�erleri, cache boyutu i�in cacheSize, tablo boyutu i�in tableSize, key ve index hesaplamalar� i�in key ve index de�i�kenleri, search fonksiyonundan d�necek olan sonu� i�in searchIndex de�i�keni, dosyadan okuma s�ras�nda buffer olarak kullan�lacak bDate de�i�keni
	FILE *inputFile; //okunacak olan test.txt dosyas�. 52. sat�rda a��l�yor
	char personId[10], firstName[10], lastName[10], city[10], idNumber[10]; //dosyadaki elemanlar� alarak ilgili fonksiyonlara yollayacak olan buffer olarak kullan�lan de�i�kenler
    struct personList *cache = NULL, *iter; //cache'i i�erisinde tutacak olan cache linkli listesi ve listeyi gezme durumlar�nda kullan�lacak olan iter de�i�keni
	struct hash* hashTable; //hash struct dizisinden kullan�lacak olan hash tablosu
	
	printf("Please enter the cache size: ");
	scanf("%d", &cacheSize);
	if(cacheSize == 1){
		printf("Please enter cache size bigger than 1.");
		return 0;
	}
    tableSize = calculatePrimeValue(cacheSize);
    printf("\nTable size will be: %d\n\n", tableSize);
	hashTable = (struct hash*)calloc(tableSize, sizeof(struct hash));
	for(i=0; i<tableSize; i++){
		strcpy(hashTable[i].id, "empty");
		hashTable[i].order = 0;
		hashTable[i].flag = -1;
	}
	
	if ((inputFile = fopen ("test.txt", "r+")) == NULL){	
        printf("Can't open the file");						
        return 0;											
    }
    i = 0;
    iter = cache;
    //g�z doluysa 1, g�z bo�sa -1, g�z dolu ama silindiyse 0 flag de�eri yerle�tirilir.
	while(!feof(inputFile)){
		k=0;
		fscanf(inputFile,"%s %s %s %d %s\n", personId, firstName, lastName, &bDate, city);
		printf("Person information: %s, %s %s, %d, %s\n\n", personId, firstName, lastName, &bDate, city);
		key = keyValue(personId);
		index = search(personId, hashTable, tableSize);
		if(index == -1){//eleman�n bulunamamas� durumunda bu if �al���r
			index = calculateHashIndex(i, key, tableSize);
			cache = insertToCache(cache, personId, bDate, firstName, lastName, city);
			insertToHashTable(hashTable, personId, tableSize);
			iter = cache->next;
			while(iter != NULL){ //hash tablosu d�zenlenir e�er cache doluysa son eleman silinir.
				searchIndex = search(iter->id, hashTable, tableSize);
				hashTable[searchIndex].order++;
				iter = iter->next;
				if(isCacheFull(cache, cacheSize)){
					deleteFromCache(cache, hashTable, tableSize);
				}
			}
		}
		else{ //eleman bulundu, cache i�erisinde bu eleman�n olma durumu
		    cache = updateCache(cache, hashTable[index].order);
			hashTable[index].order = 1;
			iter = cache;
			int k = 1;
			while(iter != NULL){//hash tablosu d�zenlenir cache bu durumda dolu olamaz
				searchIndex = search(iter->id, hashTable, tableSize);
				hashTable[searchIndex].order = k;
				k++;
				iter = iter->next;
			}
		}
		iter = cache;
		printf("Elements in the cache:\n");
		while(iter != NULL){
			printf("%s, %s, %d, %s\n", iter->id, iter->fullName, iter->birthDate, iter->city);
			iter = iter->next;
		}
		printf("\n");
		printf("Elements in the hash table:\n");
		printf("  TCKN, Order, flag\n");
		for(j=0; j<tableSize; j++){
			printf("%d--%s, %d, %d\n",j, hashTable[j].id, hashTable[j].order, hashTable[j].flag);
		}
		printf("\n");
		i++;
		printf("\n---------------------------\n");
	}
	return 0;
}

struct personList *updateCache(struct personList* persons, int order){ //o anki eleman daha evvel eklenmi�se eleman� en ba�a getiren, cache'i g�ncelleyen fonksiyon
	int i; //i indeks de�eri
	struct personList *temp = persons, *tempNode = NULL; //linkli listede yap�lacak ayr�m ve birle�tirmeler i�in kullan�lacak olan personList pointerlar
	
	if(order == 1){
		return persons;
	}
	
	for(i=0; i<order-2; i++){
		printf("%s, %d\n", temp->id, i);
		temp = temp->next;
	}

	tempNode = temp->next;
	temp->next = temp->next->next;
	tempNode->next = persons;
	
	return tempNode;
}

//g�z doluysa 1, g�z bo�sa -1, g�z dolu ama silindiyse 0
int search(char id[10], struct hash* hashTable, int tableSize){ //hash tablosunda TC kimlik numaras� ile ki�iyi arat�r. bulunamazsa -1, bulunursa index numaras� d�nd�r�l�r
	int i = 0; //index de�i�keni
	int key, index; //arama i�in kullan�lacak olan horner metodu ile belirlenecek olan key ve index de�erleri
	key = keyValue(id);
	index = calculateHashIndex(i, key, tableSize);
	
	if(hashTable[index].flag == 1 && !strcmp(hashTable[index].id, id)){
		return index;
	}
	else if(hashTable[index].flag != -1){ //orada eleman var veya silinmi� ama arad���m�z yok, aramaya devam.
		while(i < tableSize && strcmp(hashTable[index].id, id)){
			i++;
			key = keyValue(id);
			index = calculateHashIndex(i, key, tableSize);
		}
		if(i == tableSize){
			return -1;
		}
		else{
			return index;
		}
	}
	else{
		return -1;
	}
}

struct personList *insertToCache(struct personList* persons, char id[10], int bDate, char firstName[10], char lastName[10], char city[10]){ //kontroller sonunda cache'e eklenmesi kararla�t�r�lan ki�iyi cache'in ba��na ekler yenl listeyi d�nd�r�r. yeni liste main i�erisinde cache yerine kullan�l�r.
	char fullName[20] = ""; //isim ve soyisim de�i�kenlerini birle�tirecek olan yap�
	struct personList* person = (struct personList*) malloc(sizeof(struct personList)); //cache'in ba��na eklemek i�in kullan�lacak olan yeni liste
	person->birthDate = bDate;
	strcpy(person->city, city);
	strcat(fullName, firstName);
	strcat(fullName, " ");
	strcat(fullName, lastName);
	strcpy(person->fullName, fullName);
	strcpy(person->id, id);
	person->next = persons;
	
	return person;
}

struct personList *deleteFromCache(struct personList* persons, struct hash* hashTable, int tableSize){ //cache uzunlu�u a��ld���nda sondaki eleman� silen fonksiyon
	int i = 0, index; //indis i�in kullan�lacak olan i ve hash tablosundaki yeri ifade edecek olan index de�eri
	struct personList *iter = persons; //cache listesini gezecek olan iter
	while(iter->next->next != NULL){ 
		iter = iter->next;
	}
	index = search(iter->next->id, hashTable, tableSize);
	strcpy(hashTable[index].id, "deleted");
	hashTable[index].flag = 0;
	hashTable[index].order = 0;
	
	free(iter->next);
	iter->next = NULL;
	
	
	return persons;
}

int calculateHashIndex(int i, int key, int tableSize){ //Horner metodundan gelen key de�eri ile double hashing kullanarak index hesaplayan fonksiyon
	int h1 = key%tableSize; //double hashing i�in kullan�lacak olan h1 fonksiyonu
	int h2 = 1 + (key % (tableSize-1)); //double hashing i�in kullan�lacak olan h2 fonksiyonu
	int index = (h1 + i * h2) % tableSize; //double hashing i�in kullan�lacak olan hash tablosundaki yeri ifade eden indis de�erini d�nd�recek olan de�er
	
	return index;
}
//g�z doluysa 1, g�z bo�sa -1, g�z dolu ama silindiyse 0
void insertToHashTable(struct hash *hashTable, char id[10], int tableSize){ //hash tablosuna TC kimlik numaras� verilen ki�iyi kaydeder
	int i = 0, key, index; //indis, key ve hash tablosundaki index de�erlerini tutacak olan de�i�kenler
	key = keyValue(id);
	index = calculateHashIndex(i, key, tableSize);
	
	if(hashTable[index].flag == -1 || hashTable[index].flag == 0){
		printf("%s, inserted to the index: %d\n", id, index);
		strcpy(hashTable[index].id, id);
		hashTable[index].flag = 1;
		hashTable[index].order = 1;
	}
	else{
		while(hashTable[index].flag != -1 && i<tableSize){
			i++;
			index = calculateHashIndex(i, key, tableSize);
		}
		if(i<tableSize){
			printf("inserted to the index: %d\n", index);
			strcpy(hashTable[index].id, id);
			hashTable[index].flag = 1;
			hashTable[index].order = 1;
		}
		else
			printf("!!!\nTable is full!\n!!!");
		}
}

int isCacheFull(struct personList* cache, int cacheSize){
	struct personList* iter; //cache i�erisini gezecek olan personList pointer�
	int i=1; //listeyi gezerken ka� eleman gezildi�ini kontrol edecek olan indis de�i�keni. buna g�re de 1 veya 0 d�nd�r�lme �art� kontrol edilecek
	iter = cache;
	
	while(iter!=NULL && i<=cacheSize){
		iter=iter->next;
		i++;
	}
	if(iter == NULL)
		return 0;
	else
		return 1;
}

unsigned long keyValue(char* id){ //Horner metodu kullanarak TC kimlik numaras�n�n key de�erini hesaplayan fonksiyon
    unsigned long sum = 0; //b�y�k �l�ekli pozitif say�lar tutacak olan unsigned long de�eri. sum de�eri d�necek olan de�erin toplanmas� i�indir.
    int i, j = 0, k=0, prime = 31; //i,j,k indis de�erleri ve horner i�in gerekli olan asal say�; 31 olarak al�nm��t�r
    for(i=strlen(id); i>0; i--){
        sum += pow(prime, k)*(id[j]-48);
        j++;
        k++;
    }
    return sum;
}

int calculatePrimeValue(int n){ //n(cache size) / loadFactor'dan b�y�k en k���k asal say�y� d�nd�recek olan fonksiyon
	float loadFactor = 0.60, nFloat; //0,60 olarak al�nan loadFactor ve n de�erinin float halini tutacak olan nFloat de�eri
	nFloat = (float)n;
	float m = nFloat/loadFactor; //tablo b�y�kl���n�m olabilecek min de�eri. asal olmayaca�� varsay�m�yla bu say�dan b�y�k en k���k asal say� tan�mlan�r
	int i, j, k; //i, j, k indis de�erleri
	struct primes *primesArray; //prime say�lar� ifade edece�imiz 2*min tablo b�y�kl��� de�eri
	primesArray = (struct primes*) malloc(sizeof(struct primes) * (m*2));
	
	for(i=1; i<2*m; i++){
		primesArray[i].flag = 0;
	}
	
	for(i=1; i<=m/2; i++){
		j=i+1;
		k = 3;
		while(k<2*m){
			if(k % j == 0){
				primesArray[k].flag=1;
			}
			k++;
		}
	}
	
	for(i=m+1; i<2*m; i++){
		if(primesArray[i].flag == 0){
			return i;
		}
	}
}
