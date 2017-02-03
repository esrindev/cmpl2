#include <iostream>
  
  int main (){
  
	double x,y,z;
	
	std::cout<< "Zehmet olmasa ededi daxil edin\n";
	std::cin>>x;
	
	std::cout<< "Zehmet olmasa faizi daxil edin\n";
	std::cin>>y;
	
	//x -in y faizini z deyishenine menimsedek
	//riyaziyyatdan bilirik ki, faizi hesabalamaq ucun
	//ededi faiz gosteren reqeme vurub 100-e bolmeliyik
	z = (x*y)/100;
	
	std::cout<< x << " -in "<< y << " faizi = " << z <<"\n";
	
}

