#include <iostream>
#include <string>
#include <vector>
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
	bool operator<(const Product& p) const{
		return getPrice() < p.getPrice();
	}
	bool operator>(const Product& p) const {
		return getPrice() > p.getPrice();
	}
	bool operator==(const Product& p) const {
		return getPrice() == p.getPrice() && getName() == p.getName() && getCount() == p.getCount() && getAvailable() == p.getAvailable();
	}
	bool NametoName(const Product& p) const {
		return getName() == p.getName();
	}
	bool PricetoPrice(const Product& p) const {
		return getPrice() == p.getPrice();
	}
	bool CounttoCountless(const Product& p) const {
		return getCount() < p.getCount();
	}
	bool CounttoCountmore(const Product& p) const {
		return getCount() > p.getCount();
	}
	bool CounttoCountequal(const Product& p) const {
		return getCount() == p.getCount();
	}
};

class Store {
private:
	vector<Product> products;
public:
	Store(vector<Product> p) : products(p) {};
	Store(const Store& store) : products(store.products) {};
	void addProduct(const Product& p) {
		products.push_back(p);
	}
	void removeProduct(int index) {
		if (index >= 0 && index < products.size()) {
			products.erase(products.begin() + index);
		}
		else {
			cout << "Invalid index!" << endl;
		}
	}
	int searchProduct(string n="", double p=-1, int c=-1) {
		if (n != "") {
			for (int i = 0; i < products.size(); i++) {
				if (products[i].getName() == n) {
					return i;
				}
			}
		}
		else if(p != -1) {
			for (int i = 0; i < products.size(); i++) {
				if (products[i].getPrice() == p) {
					return i;
				}
			}
		}
		else if (c != -1) {
			for (int i = 0; i < products.size(); i++) {
				if (products[i].getCount() == c) {
					return i;
				}
			}
		}
		else{
			return -1;
		}
	}
	double averagePrice() const {
		if (products.empty()) {
			return 0;
		}
		double sum=0;
		double count=0;
		for (int i = 0; i < products.size(); i++) {
			sum += products[i].getPrice() * products[i].getCount();
			count += products[i].getCount();
		}
		return sum / count;
	}
};
int main()
{
	Product p1("Apple", 10, 15, true);
	Product p2("Banana", 20, 10, true);
	Store s({ p1, p2 });

	cout << "Search Banana: " << s.searchProduct("Banana") << endl;

	cout << "Average price: " << s.averagePrice();
}
