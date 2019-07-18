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

static unsigned int initial_seeds[500];
static unsigned int main_seeds[4096];
void ur_gen_seeds(unsigned int total_seeds){
	while(total_seeds>=0){
		initial_seeds[0]=initial_seeds[1];
		initial_seeds[1]=initial_seeds[2];
		initial_seeds[2]=initial_seeds[3];
		initial_seeds[3]=initial_seeds[4];
		initial_seeds[4]=initial_seeds[5];
		initial_seeds[5]=(initial_seeds[5]^(initial_seeds[5]<<6))^(initial_seeds[1]^(initial_seeds[1]<<13));
		main_seeds[total_seeds]=(initial_seeds[1]+initial_seeds[2])*(initial_seeds[5]+initial_seeds[0]);
		--total_seeds;
	}
}

int unsigned mcwm4096(unsigned int c){
	int i=4095;
	char loops=5;
	unsigned long long t, a=1872LL;
	unsigned long x,r=0xfffffffe;
	while(loops!=0){
		i=(i+1)&4095;
		t=a*main_seeds[i]+c;
		c=(t>>32);
		x=t+c;
		if(x<c){
			x++;
			c++;
		}
		main_seeds[i]=r-x;
		--loops;
	}
	return main_seeds[i];
}
