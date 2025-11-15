#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
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
		cout << "Supplier name: " << getSupplierName() << endl;
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
		if (!products.empty()) {
			for (int i = 0; i < products.size() - 1; i++) {
				for (int j = i + 1; j < products.size(); j++) {
					if (products[i]->getName() > products[j]->getName()) {
						Product* temp = products[i];
						products[i] = products[j];
						products[j] = temp;
					}
				}
			}
			cout << "Sorted by name." << endl;
		}
	}
	void sortByPrice() {
		if (!products.empty()) {
			for (int i = 0; i < products.size() - 1; i++) {
				for (int j = i + 1; j < products.size(); j++) {
					if (products[i] > products[j]) {
						Product* temp = products[i];
						products[i] = products[j];
						products[j] = temp;
					}
				}
			}
			cout << "Sorted by price." << endl;
		}
	}
	void sortByCount() {
		if (!products.empty()) {
			for (int i = 0; i < products.size() - 1; i++) {
				for (int j = i + 1; j < products.size(); j++) {
					if (products[i]->getCount() < products[j]->getCount()) {
						Product* temp = products[i];
						products[i] = products[j];
						products[j] = temp;
					}
				}
			}
			cout << "Sorted by count." << endl;
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
			cout << "Error while opening the file" << endl;
			return;
		}		
		for (int i = 0; i < products.size(); i++) {
			Product* p = products[i];
			char type = p->get();
			file << type << "|"	<< p->getName() << "|" << p->getCount() << "|" << p->getPrice() << "|" << p->getAvailable() << "|" << cts(p->getCategory()) << "|"	<< p->getSupplierName() << "|" << p->getSupplier()->getContact() << "|"	<< p->getSupplier()->getRating() << "|" << p->getBrand() << "|" << p->getInsurance() << "|" << endl;
		}
		file.close();
		cout << "Products saved!" << endl;
	}
	void loadProducts(string fname = "products.txt") {
		ifstream file(fname);
		if (!file.is_open()) {
			cout << "Error while opening the file" << endl;
			return;
		}
		products.clear();
		string line;
		while (getline(file, line)) {
			string f[12];
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
		cout << "Products loaded!" << endl;
	}
	void printP(int i) {
		products[i]->getInfo();
	}
	void pushSup(Supplier* s) {
		if(findSupplier(s->getName())==nullptr)
			suppliers.push_back(s);
	}
	~Store() {
		for (int i = 0; i < products.size(); i++) {
			delete products[i];
		}
		for (int i = 0; i < suppliers.size(); i++) {
			delete suppliers[i];
		}
	}
};
void autoSave(Store* store) {
	store->saveProducts();
}
void Menu() {
	int counter = 0;
	Store store({});
	int choice = -1;
	while (choice != 0) {
		cout << endl << "=== STORE MENU ===" << endl;
		cout << "1. Show all products" << endl;
		cout << "2. Add product" << endl;
		cout << "3. Remove product" << endl;
		cout << "4. Search product" << endl;
		cout << "5. Sort products by name" << endl;
		cout << "6. Sort products by price" << endl;
		cout << "7. Sort products by count" << endl;
		cout << "8. Save products to file" << endl;
		cout << "9. Load products from file" << endl;
		cout << "0. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			store.printAll();
			counter++;
			break;
		case 2: {
			string name, catStr, supplierName, supplierContact;
			string countStr, priceStr, availStr, supplierRatingStr;
			try {
				cout << "Product name: ";
				cin.ignore();
				getline(cin, name);

				cout << "Count: ";
				cin >> countStr;
				int count = stoi(countStr);
				if (count < 0)
					throw invalid_argument("Count cannot be negative");

				cout << "Price: ";
				cin >> priceStr;
				double price = stod(priceStr);
				if (price < 0) 
					throw invalid_argument("Price cannot be negative");

				cout << "Available (1/0): ";
				cin >> availStr;
				bool avail;
				if (availStr == "1") 
					avail = true;
				else if (availStr == "0")
					avail = false;
				else 
					throw invalid_argument("Available must be 1 or 0");

				cout << "Category (Fruit, Vegetable, Dairy, Meat, Other): ";
				cin >> catStr;
				Category cat = stc(catStr);

				cout << "Supplier name: ";
				cin >> supplierName;
				cout << "Supplier contact: ";
				cin >> supplierContact;

				cout << "Supplier rating: ";
				cin >> supplierRatingStr;
				int supplierRating = stoi(supplierRatingStr);
				if (supplierRating < 0) 
					throw invalid_argument("Rating cannot be negative");

				Supplier* s = store.findSupplier(supplierName);
				if (!s) {
					s = new Supplier(supplierName, supplierContact, supplierRating);
					store.pushSup(s);
				}
				char type;
				cout << "Normal or Luxury (N/L): ";
				cin >> type;

				if (type == 'L' || type == 'l') {
					string brand, insStr;
					cout << "Brand: ";
					cin >> brand;

					cout << "Has insurance (1/0): ";
					cin >> insStr;
					bool ins;
					if (insStr == "1")
						ins = true;
					else if (insStr == "0")
						ins = false;
					else
						throw invalid_argument("Insurance must be 1 or 0");

					store.addProduct(new LuxuryProduct(name, count, price, avail, cat, s, brand, ins));
				}
				else {
					store.addProduct(new Product(name, count, price, avail, cat, s));
				}

				cout << "Product added!" << endl;
			}
			catch (const exception& e) {
				cout << "Error adding product: " << e.what() << endl;
				break;
			}
			counter++;
			break;
		}
		case 3: {
			try {
				int idx;
				cout << "Enter product index to remove: "; cin >> idx;
				if (store.removeProduct(idx)) {
					cout << "Removed!" << endl;
				}
			}
			catch (const exception& e) {
				cout << "Error: " << e.what() << endl;
			}
			counter++;
			break;
		}
		case 4: {
			try {
				string name;
				double price;
				int count;
				cout << "Enter name to search (Enter to skip): ";
				cin.ignore();
				getline(cin, name);
				cout << "Enter price to search (-1 to skip): ";
				cin >> price;
				cout << "Enter count to search (-1 to skip): ";
				cin >> count;
				int idx = store.searchProduct(name, price, count);
				if (idx != -1) {
					cout << "Found product at index " << idx << ":" << endl;
					store.printP(idx);
				}
				else {
					cout << "Not found!" << endl;
				}
			}
			catch (const exception& e) {
				cout << "Error: " << e.what() << endl;
			}
			counter++;
			break;
		}
		case 5:
			try {
				store.sortByName();
			}
			catch (const exception& e) {
				cout << "Error: " << e.what() << endl;
			}
			counter++;
			break;
		case 6:
			try {
				store.sortByPrice();
			}
			catch (const exception& e) {
				cout << "Error: " << e.what() << endl;
			}
			counter++;
			break;
		case 7:
			try {
				store.sortByCount();
			}
			catch (const exception& e) {
				cout << "Error: " << e.what() << endl;
			}
			counter++;
			break;
		case 8:
			try { store.saveProducts(); }
			catch (const exception& e) { cout << "Error: " << e.what() << endl; }
			counter++;
			break;
		case 9:
			try { store.loadProducts(); }
			catch (const exception& e) { cout << "Error: " << e.what() << endl; }
			counter++;
			break;
		case 0:
			cout << "Exiting..." << endl;
			break;
		default:
			cout << "Invalid choice!" << endl;
			break;
		}
		if (counter >= 5) {
			thread saver(autoSave, &store);
			saver.detach();
			counter = 0;
		}
	}
}
int main()
{
	Menu();
}