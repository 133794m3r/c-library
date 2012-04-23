#include <math.h>
#include <limits.h>
#include <sys/types.h>
#include <regex.h>
#include <time.h>
#include <stdlib.h>
// notes to self.
// reverse the range so that it starts small and gets big.
// after that, do the multiplication the same as you were.
// this will make it so that you don't have to perform
// a mergesort.
/*
s=sum
n=number of items
x=sides
(s-n)/x
 SUM  (-1)^k * C(n,k) * C(s-x*k-1,n-1)
 k=0
*/
#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
float chance_calc_single(float min, float max, float amount, float desired_result){
	printf("%f\n",desired_result);	
    float range=(max-min)+1;
    float series=ceil((desired_result-amount)/range);
	float i;
	--amount;
	long double chances=0.0;
	printf("range=%f",range);
	for(i=0;i<=series;++i){
		chances=pow((-1),i)*m_product_cf(amount,i)*m_product_cf(desired_result-(range*i)-1,amount)+chances;
		printf("\nm_product(desired_result)=%E\n",m_product_cf(desired_result-(range*i)-1,amount));
	}
	return chances;
}



unsigned long microtime(void){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_usec;
}

int match(const char *string, char *pattern) {
  int status;
  regex_t re;
  if(regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
    return 0;
  }
  status = regexec(&re, string, (size_t)0, NULL, 0);
  regfree(&re);
  if(status != 0) {
    return 0; 
   }

  return 1;
} 
