#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct point {
    int x, y;
};

struct point point1;
struct point point2;
float minVal;

float findMedian(struct point *points, int n);
float findDistance(struct point p1, struct point p2);
void bubbleSort(struct point *points, int n);
void bruteForce(struct point *points, int n);
void splitAndCalculate(struct point *points, int n);
void comparePairs(struct point *leftPoints, struct point *rightPoints, int leftLength, int rightLength, float median);

int main(){
	FILE *inputFile;	//deðerlerin okunacaðý dosya
	int n=0;			//noktalardan oluþacak olan struct dizisinin boyutu
	int temp;			//struct dizisinin boyutunu bulmak için gerekli ilk gezintide satýrlarý saklamak için kullanýlacak temp deðer
	int i=0;			//sýralý listeyi ekrana yazdýracak for döngüsü
	struct point *points;	//point struct yapýlarýndan oluþacak struct dizisi
	
	
	if ((inputFile = fopen ("sample.txt", "r+")) == NULL){	
        printf("can't open the file");						
        return 0;											
    }
    
	while(!feof(inputFile)){								
    	fscanf(inputFile,"%d %d\n", &temp, &temp);			
    	n++;												
	}
	printf("number of lines in file: %d\n", n);				
		
	points = (struct point *)malloc(sizeof(struct point) * n);	
	
	fclose(inputFile);				//dosya pointerýnýn sýfýrlanmasý için kapatýlýyor, tekrar açýlacak.
	
	if ((inputFile = fopen ("sample.txt", "r+")) == NULL){
        printf("can't open the file");						
        return 0;											
    }
    
	for(i=0; i<n; i++){										
    	fscanf(inputFile,"%d %d\n", &points[i].x, &points[i].y);	
	}
	fclose(inputFile);								
	printf("Unsorted arrays of (x,y):\n");							
	for(i=0; i<n; i++){										
		printf("(%d,%d)\n", points[i].x, points[i].y);		
	}
	
    bubbleSort(points, n);
								
    printf("Sorted arrays of (x,y):\n");							
	for(i=0; i<n; i++){										
		printf("(%d,%d)\n", points[i].x, points[i].y);		
	}
	printf("first minimum distance is initializing...\n");
	minVal = findDistance(points[0], points[1]);	//en küçük deðeri initialize etmek için 0 ile 1. noktalar arasýndaki fark hesaplanýyor.
	point1 = points[0];
	point2 = points[1];	
	splitAndCalculate(points, n);							
	printf("Minimum distance is between pairs (%d, %d) and (%d, %d). Minimum distance is: %f\n",point1.x, point1.y, point2.x, point2.y, minVal);
	
	return 0;
}

void bubbleSort(struct point *points, int n) {
	int i; //diziyi baþtan sona gezecek indis
	int j; //dizinin i+1. elemanýndan sonrasýný gezecek olan indis
	struct point temp; //swap için gerekli geçici deðiþken
	for (i = 0; i < n - 1; ++i) {
    	for (j = i; j < n-i-1; ++j) {
      		if(points[i].x > points[i+1].x) {
        		temp = points[i];
        		points[i] = points[i + 1];
        		points[i + 1] = temp;
      		}
    	}
  	}
}

float findDistance(struct point p1, struct point p2){ // iki nokta arasýndaki euclidean distance'ý bulacak olan fonksiyon. Ýki noktayý parametre olarak alýr farklarýný float deðer olarak geri döndürür.
	printf("calculating distance between:\nx1: (%d,%d)\nx2: (%d,%d)\n\n", p1.x, p1.y, p2.x, p2.y);
	return sqrt((p1.x  - p2.x)*(p1.x  - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

float findMedian(struct point *points, int n){ //verilen listeyi ortadan ikiye bölmek için gerekecek medyan deðerini bulur. Diziyi ve dizinin uzunluðunu parametre olarak alýr. Geriye medyan deðerini float olarak döndürür
	if(n%2 == 1){
		return points[n/2].x;
	}
	else{
		float fl = (n-1)/2;
		float cl = (n/2);
		return (float)(points[(int)fl].x + points[(int)cl].x)/2;
	}
}

void splitAndCalculate(struct point *points, int n){ // diziyi ikiye böle böle rekürsif olarak yarýlayacak, deðer 3'den küçükse brute force metodunu çaðýracak, deðilse tekrar ikiye bölecek olan fonksiyon. Noktalar dizisini ve dizinin uzunluðunu alýr. Dönüþ deðeri yoktur.
	int i;	//Bölünecek olan sol ve sað dizilere elemanlarý yerleþtirmek için gerekli olan indis.
	int leftLength=0;	//sol dizinin büyüklüðü
	int rightLength=0;	//sað dizinin büyüklüðü
	float median;		//parametre olarak verilen dizinin medyan deðeri.
	struct point *leftPoints;	//soldaki bölünmüþ dizinin elemanlarýný tutacak olan struct dizisi
	struct point *rightPoints;	//saðdaki bölünmüþ dizinin elemanlarýný tutacak olan struct dizisi

	if(n%2 == 1){ //dizi tek sayýda veya çift sayýda elemandan oluþmasýna göre iki farklý yöntemde oluþturulabilir.
		leftPoints = (struct point *) malloc(sizeof(struct point) * (n/2));
		leftLength = n/2;
		rightPoints = (struct point *) malloc(sizeof(struct point) * (n+1)/2);
		rightLength = (n+1)/2;
	}
	else{
		leftPoints = (struct point *) malloc(sizeof(struct point) * (n/2));
		leftLength = n/2;
		rightPoints = (struct point *) malloc(sizeof(struct point) * (n/2));
		rightLength = (n+1)/2;
	}
	if(n<=3){
		bruteForce(points, n);
	}
	else{
		median = findMedian(points, n);
		for(i=0; i<leftLength; i++){
			leftPoints[i] = points[i];
			printf("leftPoints[%d]: (%d,%d)\n",i, leftPoints[i].x,leftPoints[i].y);	
		}
		for(i=0; i<rightLength; i++){
			rightPoints[i] = points[i+leftLength];
			printf("rightPoints[%d]: (%d,%d)\n",i, rightPoints[i].x,rightPoints[i].y);	
		}
		printf("left pair length: %d\n", leftLength);
		printf("right pair length: %d\n", rightLength);
		printf("(Goes to the left pair)\n");
		splitAndCalculate(leftPoints, leftLength);
		printf("(Goes to the right pair)\n");
		splitAndCalculate(rightPoints, rightLength);
		comparePairs(leftPoints, rightPoints, leftLength, rightLength, median);
	}
}

void bruteForce(struct point *points, int n){ //2 veya 3 elemanlý dizideki elemanlarýn aralarýndaki farklarý brute force olarak hesaplayacak fonksiyon. noktalar dizisini ve dizinin büyüklüðünü parametre olarak alýr. Dönüþ deðeri yoktur, global deðiþkenleri günceller
	float dist1; //dizi 3 elemanlýysa 0. ve 1. elemanlar arasýndaki farký, iki elemanlýysa iki eleman arasýndaki farkýn deðerini tutacak olan deðiþken
	float dist2; //dizi 3 elemanlýysa 0. ve 2. elemanlar arasýndaki farký tutacak olan deðiþken
	float dist3; //dizi 3 elemanlýysa 1. ve 2. elemanlar arasýndaki farký tutacak olan deðiþken
	struct point temp1; //hesaplanan uzaklýk deðerinin en küçük uzaklýktan küçük olup olmamasýna göre noktalar deðiþecektir. Bu süre içerisinde 1. noktayý tutacak olan temp deðeri
	struct point temp2; //hesaplanan uzaklýk deðerinin en küçük uzaklýktan küçük olup olmamasýna göre noktalar deðiþecektir. Bu süre içerisinde 2. noktayý tutacak olan temp deðeri
	
	if(n == 3){
		printf("comparing (%d, %d),(%d, %d),(%d, %d)\n", points[0].x, points[0].y,  points[1].x, points[1].y, points[2].x, points[2].y);
		dist1 = findDistance(points[0], points[1]);
		printf("distance between (%d, %d) and (%d, %d) = %f\n", points[0].x, points[0].y,  points[1].x, points[1].y, dist1);
		dist2 = findDistance(points[0], points[2]);
		printf("distance between (%d, %d) and (%d, %d) = %f\n", points[0].x, points[0].y,  points[2].x, points[2].y, dist2);
		dist3 = findDistance(points[1], points[2]);
		printf("distance between (%d, %d) and (%d, %d) = %f\n", points[1].x, points[1].y,  points[2].x, points[2].y, dist3);
		if(dist2 < dist1){
			if(dist2 < dist3){
				temp1 = points[0];
				temp2 = points[2];
				dist1 = dist2;
			}
			else{
				temp1 = points[1];
				temp2 = points[2];
				dist1 = dist3;
			}
		}
		else if(dist3 < dist1){
			temp1 = points[1];
			temp2 = points[2];
			dist1 = dist3;
		}
		else{
			temp1 = points[0];
			temp2 = points[1];
		}
	}
	else{ 
		printf("\n\ncomparing (%d, %d), (%d, %d)\n", points[0].x, points[0].y,  points[1].x, points[1].y);
		temp1 = points[0];
		temp2 = points[1];
		dist1 = findDistance(points[0], points[1]);
		printf("distance between (%d, %d) and (%d, %d) = %f\n", points[0].x, points[0].y,  points[1].x, points[1].y, dist1);
	}
	if(dist1 < minVal){ //bulunan en küçük mesafe daha önce bulunan mesafeden küçükse minVal global deðiþkeni , point1 ve point2 deðiþkenleri güncellenir.
		point1 = temp1;
		point2 = temp2;
		minVal = dist1;
		printf("minimum distance is updated!\n");
	}
}

void comparePairs(struct point *leftPoints, struct point *rightPoints, int leftLength, int rightLength, float median){ //medyanýn sað ve solundaki en fazla minVal uzaklýktaki deðerleri birbirleriyle karþýlaþtýracak, eðer minVal'den küçük deðer varsa minVal, point1, point2 deðerlerini güncelleyecek olan fonksiyon.
																														// Parametre olarak sol dizideki struct deðerlerini, sað dizideki struct deðerlerini, sol dizinin uzunluðunu, sað dizinin uzunluðunu ve dizinin medyan deðerini alýr. Dönüþ deðeri yoktur. global deðiþkenleri günceller.
	int i = leftLength-1; //soldaki dizinin elemanlarýný mesafe hesabý için tek tek gezecek olan indis
	int j = 0; //saðdaki dizinin elemanlarýný mesafe hesabý için tek tek gezecek olan indis
	while(leftPoints[i].x > (int)(median - minVal)){
		while(rightPoints[j].x < (int)(median + minVal)){
			if(findDistance(leftPoints[i], rightPoints[j]) < minVal){
				printf("comparing (%d, %d), (%d, %d)\n", leftPoints[i].x, leftPoints[i].y, rightPoints[j].x, rightPoints[j].y);
				minVal = findDistance(leftPoints[i], rightPoints[j]);
				point1 = leftPoints[i];
				point2 = rightPoints[j];
				printf("minimum distance is updated!\n");
			}
			j++;
		}
		i--;
	}
}
