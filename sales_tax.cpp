#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

struct InputItem {
    int quantity;
    string name;
    bool isImported;
    double price;
    bool book;
    bool food;
    bool medicine;
};

class Product {
public:
    Product(string name, double price, bool isImported, bool isExempt)
        : name(name), price(price), isImported(isImported), isExempt(isExempt) {}

    double getPrice() const {
        return price;
    }

    string getDisplayName() const {
        return (isImported ? "imported " : "") + name;
    }

    double calculateTax() const {
        double tax = 0.0;
        if (!isExempt) {
            tax += 0.10 * price;
        }
        if (isImported) {
            tax += 0.05 * price;
        }
        return roundTax(tax);
    }

private:
    string name;
    double price;
    bool isImported;
    bool isExempt;

    double roundTax(double value) const {
        return ceil(value * 20.0) / 20.0;
    }
};

class LineItem {
public:
    LineItem(const Product& product, int quantity)
        : product(product), quantity(quantity) {
        salesTax = product.calculateTax() * quantity;
        totalPrice = (product.getPrice() + product.calculateTax()) * quantity;
    }

    void print() const {
        cout << quantity << " " << product.getDisplayName() << ": "
             << fixed << setprecision(2) << totalPrice << endl;
    }

    double getSalesTax() const {
        return salesTax;
    }

    double getTotalPrice() const {
        return totalPrice;
    }

private:
    Product product;
    int quantity;
    double salesTax;
    double totalPrice;
};

class Receipt {
public:
    void addItem(const LineItem& item) {
        items.push_back(item);
        totalSalesTax += item.getSalesTax();
        totalAmount += item.getTotalPrice();
    }

    void print() const {
        for (const auto& item : items) {
            item.print();
        }
        cout << "Sales Taxes: " << fixed << setprecision(2) << totalSalesTax << endl;
        cout << "Total: " << fixed << setprecision(2) << totalAmount << endl;
    }

private:
    vector<LineItem> items;
    double totalSalesTax = 0.0;
    double totalAmount = 0.0;
};

bool isExemptProduct(const InputItem& item) {
    return item.book || item.food || item.medicine;
}

class ShoppingBasket {
public:
    ShoppingBasket(const vector<InputItem>& inputItems) {
        for (const auto& item : inputItems) {
            bool exempt = isExemptProduct(item);
            Product product(item.name, item.price, item.isImported, exempt);
            LineItem lineItem(product, item.quantity);
            items.push_back(lineItem);
        }
    }

    Receipt getReceipt() const {
        Receipt receipt;
        for (const auto& item : items) {
            receipt.addItem(item);
        }
        return receipt;
    }

private:
    vector<LineItem> items;
};

void runInput(const vector<InputItem>& items) {
    ShoppingBasket basket(items);
    Receipt receipt = basket.getReceipt();
    receipt.print();
    cout << endl;
}

bool getYesNo(const string& prompt) {
    string response;
    while (true) {
        cout << prompt << " (y/n): ";
        cin >> response;
        if (response == "y" || response == "Y") return true;
        if (response == "n" || response == "N") return false;
        cout << "Invalid input. Please enter 'y' or 'n'." << endl;
    }
}

int main() {
    int itemCount;
    cout << "Enter number of items: ";
    cin >> itemCount;
    cin.ignore(); 

    vector<InputItem> items;

    for (int i = 0; i < itemCount; ++i) {
        InputItem item;
        cout << "\nItem #" << (i + 1) << endl;

        cout << "Enter product name: ";
        getline(cin, item.name);

        cout << "Enter quantity: ";
        cin >> item.quantity;

        cout << "Enter price: ";
        cin >> item.price;

        item.isImported = getYesNo("Is the item imported?");
        item.book = getYesNo("Is the item a book?");
        if (item.book == true) {
            item.food = false;
            item.medicine = false;
            
            cin.ignore();
            items.push_back(item);
            continue;
        }
        item.food = getYesNo("Is the item a food?");
        if (item.food == true) {
            item.medicine = false;
            cin.ignore();
            items.push_back(item);
            continue;
        }
        item.medicine = getYesNo("Is the item a medicine?");
        
        cin.ignore(); 
        items.push_back(item);
    }

    cout << "\n===== RECEIPT =====" << endl;
    runInput(items);

    return 0;
}
