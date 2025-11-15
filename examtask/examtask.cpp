#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
class Supplier {
private:
	string name;
	string contact;
	int rating;
public:
	Supplier(string n, string c, int r) : name(n), contact(c), rating(r) {};
	string getName() const {
		return name;
	}
	string getContact() const {
		return contact;
	}
	int getRating() const {
		return rating;
	}
};
enum class Category {
	Fruit,
	Vegetable,
	Dairy,
	Meat,
	Other
};
string cts(Category c) {
	switch (c) {
	case Category::Fruit: return "Fruit";
	case Category::Vegetable: return "Vegetable";
	case Category::Dairy: return "Dairy";
	case Category::Meat: return "Meat";
	case Category::Other: return "Other";
	default: return "Unknown";
	}
}
Category stc(const string& s) {
	if (s == "Fruit") return Category::Fruit;
	if (s == "Vegetable") return Category::Vegetable;
	if (s == "Dairy") return Category::Dairy;
	if (s == "Meat") return Category::Meat;
	return Category::Other;
}
class Product {
private:
	string name;
	int count;
	double price;
	bool available;
	Category category;
	vector<double> PriceHistory;
	Supplier* supplier;
public:
	Product(Product&&) = default;
	Product(string n, int c, double p, bool a, Category cc, Supplier* s) : name(n), available(a), category(cc), supplier(s) {
		if (n.empty()) throw invalid_argument("Product name cannot be empty");
		if (c < 0) {
			cout << "Warning: negative count for product " << n << ", automatically set to 0." << endl;
			count = 0;
		}
		else {
			count = c;
		}
		if (p < 0) {
			cout << "Warning: negative price for product " << n << ", automatically set to 0." << endl;
			price = 0;
		}
		else {
			price = p;
		}
	};
	explicit Product(string n, Category c) : name(n), count(0), price(0.0), available(false), category(c), supplier(nullptr) {};
	Product(const Product& product) {
		this->name = product.name;
		this->count= product.count;
		this->price = product.price;
		this->available = product.available;
		this->category = product.category;
		this->PriceHistory = product.PriceHistory;
		this->supplier = product.supplier;
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
	Category getCategory() const {
		return category;
	}
	virtual void getInfo() const {
		cout << "Name: " << name << endl;
		cout << "Category: " << cts(category) << endl;
		cout << "Count: " << count << endl;
		cout << "Price: " << price << endl;
		cout << boolalpha << "Available: " << available << endl;
		cout << "Supplier name: " << supplier->getName() << endl;
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
	void setPrice(double p) {
		price = p;
		PriceHistory.push_back(p);
	}
	void printPriceHistory() {
		cout << "Price history: " << endl;
		for (int i = 0; i < PriceHistory.size(); i++) {
			cout << "#" << i << " " << PriceHistory[i] << endl;
		}
	}
	vector<double> getPriceHistory() {
		return PriceHistory;
	}
	Supplier* getSupplier() {
		return supplier;
	}
	string getSupplierName() const{
		return supplier->getName();
	}
	Product& operator=(const Product&) = default;
	Product& operator=(Product&&) = default;
	virtual char get() {
		return 'N';
	}
	virtual bool getInsurance() {
		return false;
	}
	virtual string getBrand() {
		return "";
	}
};
class LuxuryProduct : public Product {
private:
	string brand;
	bool insurance;
public:
	LuxuryProduct(string n, int c, double p, bool a, Category cc, Supplier* s, string b, bool i) : Product(n, c, p, a, cc, s), brand(b), insurance(i) {};
	void getInfo() const override {
		cout << "Name: " << getName() << endl;
		cout << "Category: " << cts(getCategory()) << endl;
		cout << "Count: " << getCount() << endl;
		cout << "Price: " << getPrice() << endl;
		cout << boolalpha << "Available: " << getAvailable() << endl;
		cout << "Supplier name: " << getSupplierName();
		cout << "Brand: " << brand << endl;
		if (insurance) {
			cout << "Has insurance!" << endl;
		}
		else {
			cout << "No insurance!" << endl;
		}
	}
	string getBrand() override {
		return brand;
	}
	bool getInsurance() override {
		return insurance;
	}
	char get() override{
		return 'L';
	}
};
class Store {
private:
	vector<Product*> products;
	vector<Supplier*> suppliers;
public:
	Store(Store&&) = default;
	Store(vector<Product*> p) : products(p) {};
	Store(const Store& store) : products(store.products) {};
	void addProduct(Product* p) {
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
			bool nameMatch = (n == "" || products[i]->getName() == n);
			bool priceMatch = (p < 0 || products[i]->getPrice() == p);
			bool countMatch = (c < 0 || products[i]->getCount() == c);

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
			sum += products[i]->getPrice() * products[i]->getCount();
			count += products[i]->getCount();
		}
		return sum / count;
	}
	void printAll() {
		for (int i = 0; i < products.size(); i++) {
			cout << "Products #" << i << endl;
			products[i]->getInfo();
			cout << endl;
		}
	}
	void sortByName() {
		for (int i = 0; i < products.size() - 1; i++) {
			for (int j = i + 1; j < products.size(); j++) {
				if (products[i]->getName() > products[j]->getName()) {
					Product* temp = products[i];
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
					Product* temp = products[i];
					products[i] = products[j];
					products[j] = temp;
				}
			}
		}
	}
	void sortByCount() {
		for (int i = 0; i < products.size() - 1; i++) {
			for (int j = i + 1; j < products.size(); j++) {
				if (products[i]->getCount() < products[j]->getCount()) {
					Product* temp = products[i];
					products[i] = products[j];
					products[j] = temp;
				}
			}
		}
	}
	Supplier* findSupplier(const string& name) {
		for (int i = 0; i < suppliers.size(); i++) {
			if (suppliers[i]->getName() == name)
				return suppliers[i];
		}
		return nullptr;
	}
	void saveProducts(string fname = "products.txt") {
		ofstream file(fname);
		if (!file.is_open()) {
			cout << "Error while opening the file\n";
			return;
		}
		products.clear();
		for (int i = 0; i < products.size(); i++) {
			Product* p = products[i];
			char type = p->get();
			file << type << "|"	<< p->getName() << "|" << p->getCount() << "|" << p->getPrice() << "|" << p->getAvailable() << "|" << cts(p->getCategory()) << "|"	<< p->getSupplierName() << "|" << p->getSupplier()->getContact() << "|"	<< p->getSupplier()->getRating() << "|" << p->getBrand() << "|" << p->getInsurance() << "|" << endl;
		}
		file.close();
		cout << "Products saved!\n";
	}
	void loadProducts(string fname = "products.txt") {
		ifstream file(fname);
		if (!file.is_open()) {
			cout << "Error while opening the file\n";
			return;
		}
		string line;
		while (getline(file, line)) {
			string f[11];
			int idx = 0;
			for (int i = 0; i < line.size(); i++) {
				if (line[i] == '|') {
					idx++;
				}
				else {
					f[idx] += line[i];
				}
			}
			char type = f[0][0];
			string name = f[1];
			int count = stoi(f[2]);
			double price = stod(f[3]);
			bool available = false;
			if (f[4] == "1") {
				available = true;
			}
			Category cat = stc(f[5]);
			string supName = f[6];
			string supContact = f[7];
			int supRate = stoi(f[8]);
			Supplier* s = findSupplier(supName);
			if (s == nullptr) {
				s = new Supplier(supName, supContact, supRate);
				suppliers.push_back(s);
			}
			if (type == 'N') {
				products.push_back(new Product(name, count, price, available, cat, s));
			}
			else if (type == 'L') {
				string brand = f[9];
				bool ins = false;
				if (f[10] == "1") {
					ins = true;
				}
				products.push_back(new LuxuryProduct(name, count, price, available, cat, s, brand, ins));
			}
		}
		file.close();
		cout << "Products loaded!\n";
	}
};
int main()
{
	Supplier s1("FreshLine Supply", "+380671234567", 5);
	Supplier s2("PremiumFoods UA", "premiumfoods.ua@gmail.com", 5);
	Supplier s3("GreenHarvest Co.", "+380509876543", 3);
	Supplier s4("AgroTrade Global", "support@agrotrade.com", 4);
	Product p1("Apple", 10, 15, true, stc("Fruit"), &s1);
	Product p2("Banana", 20, 10, true, stc("Fruit"), &s2);
	Product p3("Pineapple", 5, 20, false, stc("Fruit"), &s3);
	Product p4("Potato", 25, 5, true, stc("Vegetable"), &s4);
	LuxuryProduct p5("Dark Belgium Chocolate", 12, 50, true, stc("Other"), &s2, "Godiva", true);
	Store s({ &p2, &p3, &p4});
	s.addProduct(&p5);
	s.addProduct(&p1);
	//s.sortByName();
	/*s.printAll();*/
	/*s.sortByPrice();
	s.printAll();*/
	/*s.sortByCount();
	s.printAll();*/
	//s.saveToFile();
	/*s.removeProduct(0);
	s.removeProduct(1);*/
	//cout << endl << "Print after removing" << endl;
	//s.printAll();
	//s.loadFromFile();
	cout << "Print after loading" << endl << endl;
	s.printAll();

	/*cout << endl << endl << endl;
	p1.setPrice(10);
	p1.setPrice(20);
	p1.setPrice(15);
	p1.printPriceHistory();*/
}