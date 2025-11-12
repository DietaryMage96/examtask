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
	Product(string n, int c, double p, bool a) : name(n), available(a) {
		if (c < 0) {
			count = 0;
		}
		else {
			count = c;
		}
		if (p < 0) {
			price = 0;
		}
		else {
			price = p;
		}
	};
	explicit Product(string n) : name(n), count(0), price(0.0), available(false) {};
	Product(const Product& product) {
		this->name = product.name;
		this->count= product.count;
		this->price = product.price;
		this->available = product.available;
	};
	string getName() const {
		return name;
	}
	int getCount() const {
		return count;
	}
	double getPrice() const {
		return price;
	}
	bool getAvailable() const {
		return available;
	}
	void getInfo() const {
		cout << "Name: " << name << endl;
		cout << "Count: " << count << endl;
		cout << "Price: " << price << endl;
		cout << boolalpha << "Available: " << available << endl;
	}
	Product& operator+=(int c) {
		count += c;
		return *this;
	}

	Product& operator-=(int c) {
		count -= c;
		if (count < 0) count = 0;
		return *this;
	}
};
int main()
{
	Product p("Apple", 10, 15, true);
	p.getInfo();
	p += 10;
	cout << p.getCount();
}
