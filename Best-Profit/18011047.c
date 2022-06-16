#include <stdio.h>
#include <stdlib.h>

struct ad{
	int startTime; // reklam�n ba�lang�� zaman�
	int duration; // reklam�n s�resi
	int value; // reklam�n de�eri, �creti
	int finish; // reklam�n biti� zaman�, duration + startTime olarak hesaplan�r
	int index; // dizinin s�ras�z halindeki indisini tutacak indis eleman�
};

void quickSort(struct ad *ads,int first,int last); //reklamlar�n s�ras�z listesini, ba�lang�� ve biti� indislerini alarak listeyi s�ralar.

int main(){
	int i; //indisleme i�in kullan�l�r, kod i�erisinde farkl� ama�larla kullan�lm��t�r.
	int j; //indisleme i�in kullan�l�r, kod i�erisinde farkl� ama�larla kullan�lm��t�r.
	int n; //reklamlar�n dizi uzunlu�unu ifade eder.
	int tempProfit; //kar��lar�t�rmalar�n yap�labilmesi i�in ge�ici bir profit de�erlerini tutacak de�i�ken.
	struct ad *ads; //reklam listesini i�erecek olan struct dizisi
	int *profit; //k�r de�erlerini i�erisinde tutacak olan reklam say�s� uzunluklu k�r dizisi
	FILE *fp; //dosyadan de�er okumak i�in kullan�lacak file pointer.
	
	if ((fp = fopen("Sample.txt", "r+")) == NULL){	
        printf("can't open the file");						
        return 0;											
    }
    
    i = 1;
	
	ads = (struct ad*)malloc(sizeof(struct ad) * 1);

	ads[0].startTime = 0;
    ads[0].duration = 0;
    ads[0].value = 0;
	
	while(!feof(fp)){  // dosyan�n sonuna kadar realloc kullanarak elemanlar al�n�r, yerle�tirilir.
		ads = (struct ad*) realloc(ads, sizeof(struct ad)*(i+1));
		fscanf(fp,"%d %d %d\n", &ads[i].startTime, &ads[i].duration, &ads[i].value);
		ads[i].finish = ads[i].startTime + ads[i].duration;
		ads[i].index = i;
		i++;
	}
	
	n=i-1;
	profit = (int *)malloc(sizeof(int) * i);
	
	printf("Unsorted: \n");
	printf("Start Time\tDuration\tValue\t\tFinish Time\n");
	for(i=1; i<n+1; i++){
		printf("%d- %d\t\t%d\t\t%d\t\t%d\n", i, ads[i].startTime, ads[i].duration, ads[i].value, ads[i].finish);
	}
	
	quickSort(ads, 0, n);
	
	printf("\n\n");
	printf("Sorted: \n");
	printf("Start Time\tDuration\tValue\t\tFinish Time\n");
	for(i=1; i<n+1; i++){
		printf("%d- %d\t\t%d\t\t%d\t\t%d\n", i, ads[i].startTime, ads[i].duration, ads[i].value, ads[i].finish);
	}
	
	profit[0] = 0;
	profit[1] = ads[1].value;
	tempProfit = 0;
	
	printf("\n");
	for(i=1; i<n+1; i++){ //s�ral� hale getirilen dizi ba�tan sona gezilecektir. 
		j = i-1;
		while(ads[j].finish > ads[i].startTime && j>0) // �ak��ma durumu bu d�ng� ve kontrol i�erisinde ger�ekle�tirilecektir
			j--;
		if(j != 0)
			printf("The last ad where the %d don't overlap is the %d. ad", i, j);
		else
			printf("There are no ads before %d and not overlapping", i);
		tempProfit = ads[i].value + profit[j];
			
		if(tempProfit > profit[i-1])
			profit[i] = tempProfit;
		else
			profit[i] = profit[i-1];
		printf("; Max Profit so far: %d\n", profit[i]);
	}
	printf("\nList of profits: ");
	for(i=0; i<n+1; i++){
		printf("%d ", profit[i]);
	}
	
	printf("\nMax Profit: %d\n", profit[n]);
	
	i=n;
	printf("Selected items are(Unsorted list's elements): \n");
	while(i>0){ //dizi sondan ba�a olarak gezilerek indis de�erleri tespit edilecek.
		if(profit[i-1] == profit[i]){ //de�i�im olmad�k�a geri gidilecek
			i--;
		}
		else{ // de�i�im varsa geri gidilerek son �ak��mayan eleman bulunur
			j = i;
			while(j>0 && ads[i].startTime < ads[j].finish){	 // �ak��mayan elemana kadar geri gidilir
                j--;
            }
			printf("%d\t", ads[i].index);
            i=j;
		}
	}
	free(profit);
	free(ads);
	return 0;
}

void quickSort(struct ad *ads, int first, int last){ //reklamlar�n s�ras�z listesini, ba�lang�� ve biti� indislerini alarak listeyi finish de�erine g�re s�ralar.
	int i; //indisleme i�in kullan�lacakt�r.
	int j; //indisleme i�in kullan�lacakt�r.
	int pivot; //se�ilecek pivot de�erinin(dizinin ilk eleman�) tutacak olan de�i�ken.
	struct ad temp; //swap i�leminde kullan�lacak olan ge�ici de�i�ken.

	if(first < last){
      pivot = first;
      i = first;
      j = last;

    	while(i<j){
        	while(ads[i].finish<=ads[pivot].finish && i<last)
            	i++;
        	while(ads[j].finish > ads[pivot].finish)
            	j--;
        	if(i<j){
            	temp = ads[i];
            	ads[i] = ads[j];
            	ads[j] = temp;
         	}
      }

      temp = ads[pivot];
      ads[pivot] = ads[j];
      ads[j] = temp;
      
      quickSort(ads,first,j-1);
      quickSort(ads,j+1,last);

   }
}
