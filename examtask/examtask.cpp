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
	string category;
public:
	Product(string n, int c, double p, bool a, string cc) : name(n), available(a), category(cc) {
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
	explicit Product(string n) : name(n), count(0), price(0.0), available(false), category("") {};
	Product(const Product& product) {
		this->name = product.name;
		this->count= product.count;
		this->price = product.price;
		this->available = product.available;
		this->category = product.category;
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
	string getCategory() const {
		return category;
	}
	void getInfo() const {
		cout << "Name: " << name << endl;
		cout << "Category: " << category << endl;
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
	int searchProduct(string n = "", double p = -1, int c = -1) {
		if (n != "") {
			for (int i = 0; i < products.size(); i++) {
				if (products[i].getName() == n) {
					return i;
				}
			}
		}
		else if (p != -1) {
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
		else {
			return -1;
		}
	}
	double averagePrice() const {
		if (products.empty()) {
			return 0;
		}
		double sum = 0;
		double count = 0;
		for (int i = 0; i < products.size(); i++) {
			sum += products[i].getPrice() * products[i].getCount();
			count += products[i].getCount();
		}
		return sum / count;
	}
	void printAll() {
		for (int i = 0; i < products.size(); i++) {
			cout << "Products #" << i << endl;
			products[i].getInfo();
			cout << endl;
		}
	}
	void sortByName() {
		for (int i = 0; i < products.size() - 1; i++) {
			for (int j = i + 1; j < products.size(); j++) {
				if (products[i].getName() > products[j].getName()) {
					Product temp = products[i];
					products[i] = products[j];
					products[j] = temp;
				}
			}
		}
	}
	void sortByPrice() {
		for (int i = 0; i < products.size() - 1; i++) {
			for (int j = i + 1; j < products.size(); j++) {
				if (products[i] > products[j]) {
					Product temp = products[i];
					products[i] = products[j];
					products[j] = temp;
				}
			}
		}
	}
	void sortByCount() {
		for (int i = 0; i < products.size() - 1; i++) {
			for (int j = i + 1; j < products.size(); j++) {
				if (products[i].getCount() < products[j].getCount()) {
					Product temp = products[i];
					products[i] = products[j];
					products[j] = temp;
				}
			}
		}
	}
};
int main()
{
	Product p1("Apple", 10, 15, true, "Fruit");
	Product p2("Banana", 20, 10, true, "Fruit");
	Product p3("Pineapple", 5, 20, false, "Fruit");
	Product p4("Potato", 25, 5, true, "Vegetable");
	Store s({ p2, p3, p1, p4 });

	/*s.sortByName();
	s.printAll();*/
	/*s.sortByPrice();
	s.printAll();*/
	/*s.sortByCount();
	s.printAll();*/
}
