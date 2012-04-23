/*!
*    Set of functions to calculate the probabilty of n number of items adding up to s
*    with sides x. The question that this program relates to can be found at the url of
*    http://stackoverflow.com/questions/6394120/
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
* The following functions calculates the probability of n items with x sides
* that add up to a value of s. The formula for this is included below.
*
* The formula comes from. http://mathforum.org/library/drmath/view/52207.html
*
*s=sum
*n=number of items
*x=sides
*(s-n)/x
* SUM  (-1)^k * C(n,k) * C(s-x*k-1,n-1)
* k=0
*/

float chance_calc_single(float min, float max, float amount, float desired_result){
    float range=(max-min)+1;
    float series=ceil((desired_result-amount)/range);
    float i;
    --amount;
    float chances=0.0;
    for(i=0;i<=series;++i){
        chances=pow((-1),i)*m_product_cf(amount,i)*m_product_cf(desired_result-(range*i)-1,amount)+chances;
    }
    return chances;
}

/*
* The following functions implement failing factorials so that we can
* do binomial coeffecients more quickly.
* Via the following formula.
*
*    K
*  PROD    (n-(k-i))/i
*    i=1;
*/

//unsigned int return
unsigned int m_product_c( int k,  int n){
    int i=1;
    float result=1;
    for(i=1;i<=k;++i){
    	result=((n-(k-i))/i)*result;
    }
    return result;
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
