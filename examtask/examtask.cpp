#include <iostream>
#include <fstream>
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
	Product(Product&&) = default;
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
	Store(Store&&) = default;
	Store(vector<Product> p) : products(p) {};
	Store(const Store& store) : products(store.products) {};
	void addProduct(const Product& p) {
		products.push_back(p);
	}
	bool removeProduct(int index) {
		if (!products.empty()) {
			if (index >= 0 && index < products.size()) {
				products.erase(products.begin() + index);
				return true;
			}
			else {
				cout << "Invalid index!" << endl;
				return false;
			}
		}
		return false;
	}
	int searchProduct(string n = "", double p = -1, int c = -1) {
		for (int i = 0; i < products.size(); i++) {
			bool nameMatch = (n == "" || products[i].getName() == n);
			bool priceMatch = (p < 0 || products[i].getPrice() == p);
			bool countMatch = (c < 0 || products[i].getCount() == c);

			if (nameMatch && priceMatch && countMatch) {
				return i;
			}
		}
		return -1;
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

	void saveToFile(string fname = "data.txt") {
		ofstream file(fname);
		if (!file.is_open()) {
			cout << "Error while opening the file " << endl;
			return;
		}
		for (int i = 0; i < products.size(); i++) {
			file << products[i].getName() << "|" << products[i].getCount() << "|" << products[i].getPrice() << "|" << products[i].getAvailable() << "|" << products[i].getCategory() << "|" << endl;
		}
		file.close();
		cout << "Saved! " << endl;
	}
	void loadFromFile(string fname = "data.txt") {
		ifstream file(fname);
		if (!file.is_open()) {
			cout << "Error while opening the file " << endl;
			return;
		}
		string line;
		while (getline(file, line)) {
			string fields[5];
			int index = 0;

			for (char c : line) {
				if (c == '|') {
					index++;
					if (index >= 5) break;
				}
				else {
					fields[index] += c;
				}
			}

			if (index < 4) continue;

			string name = fields[0];
			int count;
			double price;
			bool available;
			string category = fields[4];

			try {
				count = stoi(fields[1]);
				price = stod(fields[2]);
			}
			catch (...) {
				continue;
			}

			if (fields[3] == "1" || fields[3] == "true") {
				available = true;
			}
			else if (fields[3] == "0" || fields[3] == "false") {
				available = false;
			}
			else {
				continue;
			}

			if (searchProduct(name, price, count) == -1) {
				products.push_back(Product(name, count, price, available, category));
			}
		}
		file.close();
		cout << "Loaded! " << endl;
	}
};
int main()
{
	Product p1("Apple", 10, 15, true, "Fruit");
	Product p2("Banana", 20, 10, true, "Fruit");
	Product p3("Pineapple", 5, 20, false, "Fruit");
	Product p4("Potato", 25, 5, true, "Vegetable");
	Store s({ p2, p3, p1, p4 });

	s.sortByName();
	/*s.printAll();*/
	/*s.sortByPrice();
	s.printAll();*/
	/*s.sortByCount();
	s.printAll();*/
	//s.saveToFile();
	s.removeProduct(0);
	s.removeProduct(1);
	//cout << endl << "Print after removing" << endl;
	//s.printAll();
	s.loadFromFile();
	cout << "Print after loading" << endl << endl;
	s.printAll();
}
