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
	FILE *inputFile;	//de�erlerin okunaca�� dosya
	int n=0;			//noktalardan olu�acak olan struct dizisinin boyutu
	int temp;			//struct dizisinin boyutunu bulmak i�in gerekli ilk gezintide sat�rlar� saklamak i�in kullan�lacak temp de�er
	int i=0;			//s�ral� listeyi ekrana yazd�racak for d�ng�s�
	struct point *points;	//point struct yap�lar�ndan olu�acak struct dizisi
	
	
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
	
	fclose(inputFile);				//dosya pointer�n�n s�f�rlanmas� i�in kapat�l�yor, tekrar a��lacak.
	
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
	minVal = findDistance(points[0], points[1]);	//en k���k de�eri initialize etmek i�in 0 ile 1. noktalar aras�ndaki fark hesaplan�yor.
	point1 = points[0];
	point2 = points[1];	
	splitAndCalculate(points, n);							
	printf("Minimum distance is between pairs (%d, %d) and (%d, %d). Minimum distance is: %f\n",point1.x, point1.y, point2.x, point2.y, minVal);
	
	return 0;
}

void bubbleSort(struct point *points, int n) {
	int i; //diziyi ba�tan sona gezecek indis
	int j; //dizinin i+1. eleman�ndan sonras�n� gezecek olan indis
	struct point temp; //swap i�in gerekli ge�ici de�i�ken
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

float findDistance(struct point p1, struct point p2){ // iki nokta aras�ndaki euclidean distance'� bulacak olan fonksiyon. �ki noktay� parametre olarak al�r farklar�n� float de�er olarak geri d�nd�r�r.
	printf("calculating distance between:\nx1: (%d,%d)\nx2: (%d,%d)\n\n", p1.x, p1.y, p2.x, p2.y);
	return sqrt((p1.x  - p2.x)*(p1.x  - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

float findMedian(struct point *points, int n){ //verilen listeyi ortadan ikiye b�lmek i�in gerekecek medyan de�erini bulur. Diziyi ve dizinin uzunlu�unu parametre olarak al�r. Geriye medyan de�erini float olarak d�nd�r�r
	if(n%2 == 1){
		return points[n/2].x;
	}
	else{
		float fl = (n-1)/2;
		float cl = (n/2);
		return (float)(points[(int)fl].x + points[(int)cl].x)/2;
	}
}

void splitAndCalculate(struct point *points, int n){ // diziyi ikiye b�le b�le rek�rsif olarak yar�layacak, de�er 3'den k���kse brute force metodunu �a��racak, de�ilse tekrar ikiye b�lecek olan fonksiyon. Noktalar dizisini ve dizinin uzunlu�unu al�r. D�n�� de�eri yoktur.
	int i;	//B�l�necek olan sol ve sa� dizilere elemanlar� yerle�tirmek i�in gerekli olan indis.
	int leftLength=0;	//sol dizinin b�y�kl���
	int rightLength=0;	//sa� dizinin b�y�kl���
	float median;		//parametre olarak verilen dizinin medyan de�eri.
	struct point *leftPoints;	//soldaki b�l�nm�� dizinin elemanlar�n� tutacak olan struct dizisi
	struct point *rightPoints;	//sa�daki b�l�nm�� dizinin elemanlar�n� tutacak olan struct dizisi

	if(n%2 == 1){ //dizi tek say�da veya �ift say�da elemandan olu�mas�na g�re iki farkl� y�ntemde olu�turulabilir.
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

void bruteForce(struct point *points, int n){ //2 veya 3 elemanl� dizideki elemanlar�n aralar�ndaki farklar� brute force olarak hesaplayacak fonksiyon. noktalar dizisini ve dizinin b�y�kl���n� parametre olarak al�r. D�n�� de�eri yoktur, global de�i�kenleri g�nceller
	float dist1; //dizi 3 elemanl�ysa 0. ve 1. elemanlar aras�ndaki fark�, iki elemanl�ysa iki eleman aras�ndaki fark�n de�erini tutacak olan de�i�ken
	float dist2; //dizi 3 elemanl�ysa 0. ve 2. elemanlar aras�ndaki fark� tutacak olan de�i�ken
	float dist3; //dizi 3 elemanl�ysa 1. ve 2. elemanlar aras�ndaki fark� tutacak olan de�i�ken
	struct point temp1; //hesaplanan uzakl�k de�erinin en k���k uzakl�ktan k���k olup olmamas�na g�re noktalar de�i�ecektir. Bu s�re i�erisinde 1. noktay� tutacak olan temp de�eri
	struct point temp2; //hesaplanan uzakl�k de�erinin en k���k uzakl�ktan k���k olup olmamas�na g�re noktalar de�i�ecektir. Bu s�re i�erisinde 2. noktay� tutacak olan temp de�eri
	
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
	if(dist1 < minVal){ //bulunan en k���k mesafe daha �nce bulunan mesafeden k���kse minVal global de�i�keni , point1 ve point2 de�i�kenleri g�ncellenir.
		point1 = temp1;
		point2 = temp2;
		minVal = dist1;
		printf("minimum distance is updated!\n");
	}
}

void comparePairs(struct point *leftPoints, struct point *rightPoints, int leftLength, int rightLength, float median){ //medyan�n sa� ve solundaki en fazla minVal uzakl�ktaki de�erleri birbirleriyle kar��la�t�racak, e�er minVal'den k���k de�er varsa minVal, point1, point2 de�erlerini g�ncelleyecek olan fonksiyon.
																														// Parametre olarak sol dizideki struct de�erlerini, sa� dizideki struct de�erlerini, sol dizinin uzunlu�unu, sa� dizinin uzunlu�unu ve dizinin medyan de�erini al�r. D�n�� de�eri yoktur. global de�i�kenleri g�nceller.
	int i = leftLength-1; //soldaki dizinin elemanlar�n� mesafe hesab� i�in tek tek gezecek olan indis
	int j = 0; //sa�daki dizinin elemanlar�n� mesafe hesab� i�in tek tek gezecek olan indis
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
