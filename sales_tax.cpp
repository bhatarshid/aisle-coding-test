#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

struct InputItem {
    int quantity;
    std::string name;
    bool isImported;
    double price;
    bool book;
    bool food;
    bool medicince;
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
    if (item.food || item.book || item.medicince) return true;
    return false;
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

int main() {
    std::vector<InputItem> input1 = {
        {1, "book", false, 12.49, true, false, false},
        {1, "music CD", false, 14.99, false, false, false},
        {1, "chocolate bar", false, 0.85, false, true, true}
    };

    std::vector<InputItem> input2 = {
        {1, "box of chocolates", true, 10.00, false, true, false},
        {1, "bottle of perfume", true, 47.50, false, false, false}
    };

    std::vector<InputItem> input3 = {
        {1, "bottle of perfume", true, 27.99, false, false, false},
        {1, "bottle of perfume", false, 18.99, false, false, false},
        {1, "packet of headache pills", false, 9.75, false, false, true},
        {1, "box of chocolates", true, 11.25, false, true, false}
    };


    cout << "Output 1:" << endl;
    runInput(input1);

    cout << "Output 2:" << endl;
    runInput(input2);

    cout << "Output 3:" << endl;
    runInput(input3);

    return 0;
}
