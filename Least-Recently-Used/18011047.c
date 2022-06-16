#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct primes{
	int flag;
};

struct personList{
	char id[10]; //tc kimlik numarasýný saklayacak olan karakter dizisi
	int birthDate; //doðum tarihini saklayacak olan integer
	char fullName[20]; //kiþinin isim-soyismini saklayacak olan karakter dizisi
	char city[10]; //kiþinin þehrini saklayacak olan karakter dizisi
	struct personList* next; //linki listenin sonraki elemanýný ifade eden pointer
};

struct hash{
	char id[10]; //tc kimlik numarasýný saklayacak olan karakter dizisi
	int order; //kiþinin cache bellekte hangi sýrada olduðunu ifade eden integer deðer
	int flag; //hash tablosundaki gözün dolu, boþ veya silinmiþ olma durumunu ifade eden deðer
};

int calculatePrimeValue(int n); //cache boyutundan büyük en küçük asal sayýyý hesaplar, 
unsigned long keyValue(char* id); //Horner metodu kullanarak TC kimlik numarasýnýn key deðerini hesaplayan fonksiyon
int calculateHashIndex(int i, int key, int tableSize); //Horner metodundan gelen key deðeri ile double hashing kullanarak index hesaplayan fonksiyon
void insertToHashTable(struct hash *hashTable, char id[10], int tableSize); //hash tablosuna TC kimlik numarasý verilen kiþiyi kaydeder
struct personList *deleteFromCache(struct personList* persons, struct hash* hashTable, int tableSize); //cache uzunluðu aþýldýðýnda sondaki elemaný silen fonksiyon
struct personList *insertToCache(struct personList* persons, char id[10], int bDate, char firstName[10], char lastName[10], char city[10]); //kontroller sonunda cache'e eklenmesi kararlaþtýrýlan kiþiyi cache'in baþýna ekler
int search(char id[10], struct hash* hashTable, int tableSize);  //hash tablosunda horner metodu ile TC kimlik numarasý hesaplanmýþ kiþiyi aratýr. bulunamazsa -1, bulunursa index numarasý döndürülür
struct personList *updateCache(struct personList* persons, int order); //o anki eleman daha evvel eklenmiþse elemaný en baþa getiren, cache'i güncelleyen fonksiyon

int main(){
	int i=0, j, k = 2, cacheSize, tableSize, key, index, searchIndex, bDate; //i, j, k indis deðerleri, cache boyutu için cacheSize, tablo boyutu için tableSize, key ve index hesaplamalarý için key ve index deðiþkenleri, search fonksiyonundan dönecek olan sonuç için searchIndex deðiþkeni, dosyadan okuma sýrasýnda buffer olarak kullanýlacak bDate deðiþkeni
	FILE *inputFile; //okunacak olan test.txt dosyasý. 52. satýrda açýlýyor
	char personId[10], firstName[10], lastName[10], city[10], idNumber[10]; //dosyadaki elemanlarý alarak ilgili fonksiyonlara yollayacak olan buffer olarak kullanýlan deðiþkenler
    struct personList *cache = NULL, *iter; //cache'i içerisinde tutacak olan cache linkli listesi ve listeyi gezme durumlarýnda kullanýlacak olan iter deðiþkeni
	struct hash* hashTable; //hash struct dizisinden kullanýlacak olan hash tablosu
	
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
    //göz doluysa 1, göz boþsa -1, göz dolu ama silindiyse 0 flag deðeri yerleþtirilir.
	while(!feof(inputFile)){
		k=0;
		fscanf(inputFile,"%s %s %s %d %s\n", personId, firstName, lastName, &bDate, city);
		printf("Person information: %s, %s %s, %d, %s\n\n", personId, firstName, lastName, &bDate, city);
		key = keyValue(personId);
		index = search(personId, hashTable, tableSize);
		if(index == -1){//elemanýn bulunamamasý durumunda bu if çalýþýr
			index = calculateHashIndex(i, key, tableSize);
			cache = insertToCache(cache, personId, bDate, firstName, lastName, city);
			insertToHashTable(hashTable, personId, tableSize);
			iter = cache->next;
			while(iter != NULL){ //hash tablosu düzenlenir eðer cache doluysa son eleman silinir.
				searchIndex = search(iter->id, hashTable, tableSize);
				hashTable[searchIndex].order++;
				iter = iter->next;
				if(isCacheFull(cache, cacheSize)){
					deleteFromCache(cache, hashTable, tableSize);
				}
			}
		}
		else{ //eleman bulundu, cache içerisinde bu elemanýn olma durumu
		    cache = updateCache(cache, hashTable[index].order);
			hashTable[index].order = 1;
			iter = cache;
			int k = 1;
			while(iter != NULL){//hash tablosu düzenlenir cache bu durumda dolu olamaz
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

struct personList *updateCache(struct personList* persons, int order){ //o anki eleman daha evvel eklenmiþse elemaný en baþa getiren, cache'i güncelleyen fonksiyon
	int i; //i indeks deðeri
	struct personList *temp = persons, *tempNode = NULL; //linkli listede yapýlacak ayrým ve birleþtirmeler için kullanýlacak olan personList pointerlar
	
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

//göz doluysa 1, göz boþsa -1, göz dolu ama silindiyse 0
int search(char id[10], struct hash* hashTable, int tableSize){ //hash tablosunda TC kimlik numarasý ile kiþiyi aratýr. bulunamazsa -1, bulunursa index numarasý döndürülür
	int i = 0; //index deðiþkeni
	int key, index; //arama için kullanýlacak olan horner metodu ile belirlenecek olan key ve index deðerleri
	key = keyValue(id);
	index = calculateHashIndex(i, key, tableSize);
	
	if(hashTable[index].flag == 1 && !strcmp(hashTable[index].id, id)){
		return index;
	}
	else if(hashTable[index].flag != -1){ //orada eleman var veya silinmiþ ama aradýðýmýz yok, aramaya devam.
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

struct personList *insertToCache(struct personList* persons, char id[10], int bDate, char firstName[10], char lastName[10], char city[10]){ //kontroller sonunda cache'e eklenmesi kararlaþtýrýlan kiþiyi cache'in baþýna ekler yenl listeyi döndürür. yeni liste main içerisinde cache yerine kullanýlýr.
	char fullName[20] = ""; //isim ve soyisim deðiþkenlerini birleþtirecek olan yapý
	struct personList* person = (struct personList*) malloc(sizeof(struct personList)); //cache'in baþýna eklemek için kullanýlacak olan yeni liste
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

struct personList *deleteFromCache(struct personList* persons, struct hash* hashTable, int tableSize){ //cache uzunluðu aþýldýðýnda sondaki elemaný silen fonksiyon
	int i = 0, index; //indis için kullanýlacak olan i ve hash tablosundaki yeri ifade edecek olan index deðeri
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

int calculateHashIndex(int i, int key, int tableSize){ //Horner metodundan gelen key deðeri ile double hashing kullanarak index hesaplayan fonksiyon
	int h1 = key%tableSize; //double hashing için kullanýlacak olan h1 fonksiyonu
	int h2 = 1 + (key % (tableSize-1)); //double hashing için kullanýlacak olan h2 fonksiyonu
	int index = (h1 + i * h2) % tableSize; //double hashing için kullanýlacak olan hash tablosundaki yeri ifade eden indis deðerini döndürecek olan deðer
	
	return index;
}
//göz doluysa 1, göz boþsa -1, göz dolu ama silindiyse 0
void insertToHashTable(struct hash *hashTable, char id[10], int tableSize){ //hash tablosuna TC kimlik numarasý verilen kiþiyi kaydeder
	int i = 0, key, index; //indis, key ve hash tablosundaki index deðerlerini tutacak olan deðiþkenler
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
	struct personList* iter; //cache içerisini gezecek olan personList pointerý
	int i=1; //listeyi gezerken kaç eleman gezildiðini kontrol edecek olan indis deðiþkeni. buna göre de 1 veya 0 döndürülme þartý kontrol edilecek
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

unsigned long keyValue(char* id){ //Horner metodu kullanarak TC kimlik numarasýnýn key deðerini hesaplayan fonksiyon
    unsigned long sum = 0; //büyük ölçekli pozitif sayýlar tutacak olan unsigned long deðeri. sum deðeri dönecek olan deðerin toplanmasý içindir.
    int i, j = 0, k=0, prime = 31; //i,j,k indis deðerleri ve horner için gerekli olan asal sayý; 31 olarak alýnmýþtýr
    for(i=strlen(id); i>0; i--){
        sum += pow(prime, k)*(id[j]-48);
        j++;
        k++;
    }
    return sum;
}

int calculatePrimeValue(int n){ //n(cache size) / loadFactor'dan büyük en küçük asal sayýyý döndürecek olan fonksiyon
	float loadFactor = 0.60, nFloat; //0,60 olarak alýnan loadFactor ve n deðerinin float halini tutacak olan nFloat deðeri
	nFloat = (float)n;
	float m = nFloat/loadFactor; //tablo büyüklüðünüm olabilecek min deðeri. asal olmayacaðý varsayýmýyla bu sayýdan büyük en küçük asal sayý tanýmlanýr
	int i, j, k; //i, j, k indis deðerleri
	struct primes *primesArray; //prime sayýlarý ifade edeceðimiz 2*min tablo büyüklüðü deðeri
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
