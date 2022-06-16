#include <stdio.h>
#include <stdlib.h>

struct ad{
	int startTime; // reklamýn baþlangýç zamaný
	int duration; // reklamýn süresi
	int value; // reklamýn deðeri, ücreti
	int finish; // reklamýn bitiþ zamaný, duration + startTime olarak hesaplanýr
	int index; // dizinin sýrasýz halindeki indisini tutacak indis elemaný
};

void quickSort(struct ad *ads,int first,int last); //reklamlarýn sýrasýz listesini, baþlangýç ve bitiþ indislerini alarak listeyi sýralar.

int main(){
	int i; //indisleme için kullanýlýr, kod içerisinde farklý amaçlarla kullanýlmýþtýr.
	int j; //indisleme için kullanýlýr, kod içerisinde farklý amaçlarla kullanýlmýþtýr.
	int n; //reklamlarýn dizi uzunluðunu ifade eder.
	int tempProfit; //karþýlarþtýrmalarýn yapýlabilmesi için geçici bir profit deðerlerini tutacak deðiþken.
	struct ad *ads; //reklam listesini içerecek olan struct dizisi
	int *profit; //kâr deðerlerini içerisinde tutacak olan reklam sayýsý uzunluklu kâr dizisi
	FILE *fp; //dosyadan deðer okumak için kullanýlacak file pointer.
	
	if ((fp = fopen("Sample.txt", "r+")) == NULL){	
        printf("can't open the file");						
        return 0;											
    }
    
    i = 1;
	
	ads = (struct ad*)malloc(sizeof(struct ad) * 1);

	ads[0].startTime = 0;
    ads[0].duration = 0;
    ads[0].value = 0;
	
	while(!feof(fp)){  // dosyanýn sonuna kadar realloc kullanarak elemanlar alýnýr, yerleþtirilir.
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
	for(i=1; i<n+1; i++){ //sýralý hale getirilen dizi baþtan sona gezilecektir. 
		j = i-1;
		while(ads[j].finish > ads[i].startTime && j>0) // çakýþma durumu bu döngü ve kontrol içerisinde gerçekleþtirilecektir
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
	while(i>0){ //dizi sondan baþa olarak gezilerek indis deðerleri tespit edilecek.
		if(profit[i-1] == profit[i]){ //deðiþim olmadýkça geri gidilecek
			i--;
		}
		else{ // deðiþim varsa geri gidilerek son çakýþmayan eleman bulunur
			j = i;
			while(j>0 && ads[i].startTime < ads[j].finish){	 // çakýþmayan elemana kadar geri gidilir
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

void quickSort(struct ad *ads, int first, int last){ //reklamlarýn sýrasýz listesini, baþlangýç ve bitiþ indislerini alarak listeyi finish deðerine göre sýralar.
	int i; //indisleme için kullanýlacaktýr.
	int j; //indisleme için kullanýlacaktýr.
	int pivot; //seçilecek pivot deðerinin(dizinin ilk elemaný) tutacak olan deðiþken.
	struct ad temp; //swap iþleminde kullanýlacak olan geçici deðiþken.

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
