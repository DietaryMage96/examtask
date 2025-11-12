#include <iostream>
#include <string>
using namespace std;

class Product {
private:
	string name;
	int count;
	double price;
	bool available;
public:
	Product(string n, int c, double p, bool a) : name(n), count(c), price(p), available(a) {};
	explicit Product(string n) : name(n), count(0), price(0.0), available(false) {};
	Product(Product& product) {
		this->name = product.name;
		this->count= product.count;
		this->price = product.price;
		this->available = product.available;
	};
};
int main()
{
}
