void mdelay(int d){
	
	for(volatile int i = 0 ; i < d; i++)
	for(volatile int j = 0; j < 3500; j++);
	
}

void udelay(int d){ //udelay rounds the number to 10us
	
	d = d / 10 ;
	for (volatile int i = 0; i < d; i++)
		for (volatile int j = 0; j < 35 ; j ++);
	
}
