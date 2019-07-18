/*!
*    Various functions that I have written or have came across to make my life easier.
*    
*     Copyright 2011, Macarthur Inbody
*    
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the Lesser GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the Lesser GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/lgpl-3.0.html>.
*     
*   2011-06-20 06:03:57 PM -0400
*    
*   These functions work by any input that is provided. For a function demonstrating it.
*    Please look at the second source file at the post of the question on stack overflow.
*    It also includes an answer for implenting it using recursion if that is your favored
*    way of doing it. I personally do not feel comfortable working with recursion so that is
*    why I went with the implementation that I have included.
*/

/*
C(3,3)

C(n,k)

k
prod n-(k-i)/i
i=1

k-i
k-1
(3-(3-1))/1 * (3-(3-2))/2 * (3-(3-3))/3
k=n


*/

/*
* The following functions implement failing factorials so that we can
* do binomial coeffecients more quickly.
* Via the following formula.
*
*	K
*  PROD	(n-(k-i))/i
*	i=1;
*/
#include "functions.h"
//unsigned int return
unsigned int m_product_c( int k,  int n){
	int i=1;
	float result=1;
	for(i=1;i<=k;++i){
		result=((n-(k-i))/i)*result;
	}
	return (int) result;
}

//float return
float m_product_cf(float n, float k){
	int i=1;
	float result=1;
	for(i=1;i<=k;++i){
		result=((n-(k-i))/i)*result;
	}
	return result;
}

float mean(long long *array,long long array_length){
        long long i=0LL;
        float array_total=0;
        float array_mean=0;
        for(i=0;i<=array_length;++i){
                array_total=array_total+array[i];
        }
        printf("array_total:%f\n",array_total);
        array_mean=array_total/array_length;
        return array_mean;        
}

float standard_deviation(long long *array, float mean,long long array_length){
        long long i=0LL;
        unsigned long long tmp=0LL;
        long long array_total=0LL;
        for(i=0;i<=array_length;++i){
                tmp=tmp+pow((array[i]-mean),2);
        }
        array_total=(tmp/(array_length));
        tmp=sqrt(array_total);
        return tmp;
}
