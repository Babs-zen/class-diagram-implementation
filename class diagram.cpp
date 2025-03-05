#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
using namespace std;

// Product class
class Product {
public:
    int pID;
    string pName;
    double pPrice;
    int pQuantity;

    Product(int id, string name, double price, int quantity)
        : pID(id), pName(name), pPrice(price), pQuantity(quantity) {}
};

// Order class
class Order {
public:
    static int lastOrderId; // Static variable to keep track of the last order ID
    int orderId;
    vector<Product> orderedItems;
    double totalAmount;
    string status;

    Order(vector<Product> items, double total, string orderStatus)
        : orderedItems(items), totalAmount(total), status(orderStatus) {
        orderId = ++lastOrderId; // Increment and assign the new order ID
    }
};

// Initialize the static variable
int Order::lastOrderId = 0; // Start from 0 or 1 based on your preference

vector<Order> orderHistory;

// Function to validate character input
char getValidChar() {
    char value;
    bool validInput = false;
    while (!validInput) {
        cin >> value;
        if (cin.fail() || (value != 'Y' && value != 'y' && value != 'N' && value != 'n') || cin.peek() != '\n') {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter 'Y' or 'N' only: ";
        } else {
            validInput = true;
        }
    }
    return toupper(value);
}

// Function to validate integer input
int getValidInteger() {
    int value;
    bool validInput = false;
    while (!validInput) {
        if (cin >> value && value > 0) {
            validInput = true;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a whole number: ";
        }
    }
    return value;
}

// Shopping Cart class
class ShoppingCart {
private:
    vector<Product> cartItems;
    double totalPrice = 0;
    int totalQuantity = 0;

public:
    void addItem(Product product, int quantity) {
        product.pQuantity = quantity;
        cartItems.push_back(product);
        totalPrice += product.pPrice * quantity;
        totalQuantity += quantity;
        cout << "Product added successfully!\n";
    }

    void removeItem(int productId) {
        for (auto it = cartItems.begin(); it != cartItems.end(); ++it) {
            if (it->pID == productId) {
                totalPrice -= it->pPrice * it->pQuantity; // Update total price
                totalQuantity -= it->pQuantity; // Update total quantity
                cartItems.erase(it); // Remove the item from the cart
                cout << "Product removed successfully!\n";
                return;
            }
        }
        cout << "Product ID not found in the cart.\n";
    }

    void viewCart() {
        if (cartItems.empty()) {
            cout << "Shopping cart is empty.\n";
            return;
        }

        cout << "\nShopping Cart:\n";
        cout << "--------------------------------------------------\n";
        cout << "| ID  | Name           | Price      | Quantity  |\n";
        cout << "--------------------------------------------------\n";
        for (const auto& item : cartItems) {
            cout << "| " << setw(3) << item.pID << " | "
                 << setw(13) << left << item.pName << " | "
                 << setw(10) << fixed << setprecision(2) << item.pPrice << " | "
                 << setw(9) << item.pQuantity << " |\n";
        }
        cout << "--------------------------------------------------\n";
        cout << "Total Price: " << fixed << setprecision(2) << totalPrice << "\n";
        cout << "Total Quantity: " << totalQuantity << "\n";

        cout << "\nDo you want to checkout? (Y/N): ";
        char choice = getValidChar();
        if (choice == 'Y') {
            cout << "Checking out...\n";
            Order newOrder(cartItems, totalPrice, "Completed"); // Create new order without ID
            orderHistory.push_back(newOrder);
            clearCart();
            cout << "Order placed successfully! Order ID: " << newOrder.orderId << "\n"; // Display the order ID
        } else {
            cout << "\nWould you like to remove a product from your cart? (Y/N): ";
            char removeChoice = getValidChar();
            if (removeChoice == 'Y') {
                cout << "Enter the product ID to remove: ";
                int productId = getValidInteger();
                removeItem(productId);
            }
        }
    }
    
    void clearCart() {
        cartItems.clear();
        totalPrice = 0;
        totalQuantity = 0;
    }

    bool isEmpty() {
        return cartItems.empty();
    }
};

// Function to display order history
void viewOrders() {
    if (orderHistory.empty()) {
        cout << "No orders have been placed yet.\n";
        return;
    }

    cout << "\nOrder History:\n";
    cout << "------------------------------------------------------\n";
    for (const auto& order : orderHistory) {
        cout << "Order ID: " << order.orderId << "\n";
        cout << "Items: ";
        for (const auto& item : order.orderedItems) {
            cout << item.pName << " (x" << item.pQuantity << "), ";
        }
        cout << "\nTotal Amount: " << fixed << setprecision(2) << order.totalAmount << "\n";
        cout << "Status: " << order.status << "\n";
        cout << "------------------------------------------------------\n";
    }
}

// Function to display available products and add to cart
void viewProducts(vector<Product>& productList, ShoppingCart& cart) { // Pass productList by reference
    char addMore = 'Y';
    while (addMore == 'Y') {
        cout << "\nAvailable Products:\n";
        cout << "--------------------------------------------------\n";
        cout << "| ID  | Name           | Price      | Stock     |\n";
        cout << "--------------------------------------------------\n";
        for (const auto& product : productList) {
            cout << "| " << setw(3) << product.pID << " | "
                 << setw(13) << left << product.pName << " | "
                 << setw(10) << fixed << setprecision(2) << product.pPrice << " | "
                 << setw(9) << product.pQuantity << " |\n";
        }
        cout << "--------------------------------------------------\n";

        cout << "\nWould you like to add a product to your cart? (Y/N): ";
        char choice = getValidChar();
        if (choice == 'N') return;

        cout << "Enter the product ID to add to cart: ";
        int productId = getValidInteger();
        
        for (auto& product : productList) {  // Use reference (&) to modify stock
            if (product.pID == productId) {
                cout << "Enter quantity: ";
                int quantity = getValidInteger();

                // Check if enough stock is available
                if (quantity > product.pQuantity) {
                    cout << "Not enough stock available! Only " << product.pQuantity << " left.\n";
                    return;
                }

                cart.addItem(product, quantity);  // Add to cart
                product.pQuantity -= quantity;    // Reduce stock

                cout << "Updated stock for " << product.pName << ": " << product.pQuantity << " left.\n";
                break; // Exit the loop after adding the product
            }
        }

        // Ask if the user wants to add another product
        cout << "\nWould you like to add another product? (Y/N): ";
        addMore = getValidChar();
    }
}

// Main program
int main() {
    vector<Product> productList = {
        {1, "iPhone 13", 26890, 54},
        {2, "iPhone 14", 42790, 32},
        {3, "iPhone 15", 52090, 61},
        {4, "iPhone 16", 59990, 23},
        {5, "iPad 10th Gen", 33590, 9},
        {6, "iPad 9th Gen", 25490, 6},
        {7, "iPad Pro 2022", 75580, 19}
    };

    ShoppingCart cart;
    int choice;

    do {
        cout << "\nMenu:\n";
        cout << "1. View Products\n";
        cout << "2. View Shopping Cart\n";
        cout << "3. View Orders\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        choice = getValidInteger();

        switch (choice) {
            case 1:
                viewProducts(productList, cart);
                break;
            case 2:
                cart.viewCart();
                break;
            case 3:
                viewOrders();
                break;
            case 4:
                cout << "Exiting program. Thank you!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}
