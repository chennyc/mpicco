public int K= 50; // length of array 

public int main() {
   public int i, j;
   private int A[K];
   private int temp; 

   smcinput(A, 1, K);
   
   for (i = K - 1; i > 0; i--) { 
      for (j = 0; j < i; j++) {
	if(A[j] > A[j+1])
	{
 		temp = A[j]; 
 		A[j] = A[j+1]; 
		A[j+1] = temp;
	} 
      }
   }

   smcoutput(A, 1, K);
   return 0;
}

