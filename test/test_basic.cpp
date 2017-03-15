#include<iostream>
#include<vector>
#include<memory>


void test_inter(int num,char ch){
	if(num){
		std::cout << ch << " : "  << num << std::endl;
	}

	if(!num){
		std::cout << "!" <<  ch << " : "  << num << std::endl;
	}



}

class Demo{
public:
	Demo() { std::cout << "Demo init" << std::endl; }
	~Demo() { std::cout << "Demo des" << std::endl;}

public:
	int a;
};

int main(){
	/*
	int a = 0;
	int b = 1;
	int c = -1;

	test_inter(a,'a');
	test_inter(b,'b');
	test_inter(c,'c');
	*/
	std::vector<std::shared_ptr<Demo>> vec;
	{
		auto p_a = std::make_shared<Demo>();
		p_a->a = 100;
		vec.push_back(p_a);
	}

	std::cout << vec[0]->a <<std::endl;

	return 0;
}
