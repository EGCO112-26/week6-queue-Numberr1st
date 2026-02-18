#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue_res.h"

#define MAX_MENU_ITEMS 20

// Forward declarations
void processCashier(int order_id, int amount_due);
void addMenuItem(int id, char* name, int price);

typedef struct {
    int id;
    char name[100];
    int price;
} MenuItem;

typedef struct {
    MenuItem items[MAX_MENU_ITEMS];
    int count;
} Menu;

Menu menu;
Queue* orderQueue;
int order_counter = 0;
int customer_counter = 0;

// Initialize default menu
void initMenu() {
    menu.count = 0;
    addMenuItem(1, "Fried Chicken", 35);
    addMenuItem(2, "WingZaab", 20);
    addMenuItem(3, "French Fries", 40);
    addMenuItem(4, "Mashed Potato", 30);
}

// Add menu item
void addMenuItem(int id, char* name, int price) {
    if (menu.count < MAX_MENU_ITEMS) {
        menu.items[menu.count].id = id;
        strcpy(menu.items[menu.count].name, name);
        menu.items[menu.count].price = price;
        menu.count++;
    }
}

// Display menu
void displayMenu() {
    printf("\n========== RESTAURANT MENU ==========\n");
    printf("ID | Name              | Price\n");
    printf("---|-------------------|-------\n");
    for (int i = 0; i < menu.count; i++) {
        printf("%2d | %-17s | %d Baht\n", menu.items[i].id, menu.items[i].name, menu.items[i].price);
    }
    printf("=====================================\n");
}

// Get menu item by ID
MenuItem* getMenuItemById(int id) {
    for (int i = 0; i < menu.count; i++) {
        if (menu.items[i].id == id) {
            return &menu.items[i];
        }
    }
    return NULL;
}

// Get user input for menu
int getUserMenuChoice() {
    int choice;
    displayMenu();
    printf("Enter menu item number (0 to skip): ");
    scanf("%d", &choice);
    
    if (choice > 0 && choice <= menu.count) {
        return choice - 1;
    }
    return -1;
}

// Place order - Simplified UX for customer
void placeOrder(int menu_id, int amount) {
    MenuItem* item = getMenuItemById(menu_id);
    
    if (!item) {
        printf("❌ Item #%d not found in menu!\n", menu_id);
        return;
    }
    
    int total_price = item->price * amount;
    order_counter++;  // Increment for sequential order numbering
    
    enqueue(orderQueue, order_counter, item->name, total_price, menu_id);
    printf("✓ Order placed: %s x%d = %d Baht\n", item->name, amount, total_price);
}

// Cashier payment system
void processCashier(int order_id, int amount_due) {
    int cash_received = 0;
    int remaining_payment = amount_due;
    
    printf("Order #%d\n", order_id);
    printf("Amount due: %d Baht\n", amount_due);
    printf("=====================================\n");
    
    while (remaining_payment > 0) {
        printf("\nRemaining payment: %d Baht\n", remaining_payment);
        printf("Enter cash amount: ");
        scanf("%d", &cash_received);
        
        if (cash_received >= remaining_payment) {
            int change = cash_received - remaining_payment;
            printf("\n✓ Payment accepted!\n");
            printf("Cash received: %d Baht\n", cash_received);
            printf("Thank you for your order!\n");
            if (change > 0) {
                printf("Change: %d Baht\n", change);
            }
            remaining_payment = 0;
        } else {
            printf("❌ Insufficient payment. Need %d more Baht\n", remaining_payment - cash_received);
            remaining_payment -= cash_received;
        }
    }
    printf("=====================================\n");
}

// Process order (dequeue and handle payment)
void processOrder() {
    if (getQueueSize(orderQueue) == 0) {
        printf("\n❌ No orders in queue!\n");
        return;
    }
    
    order* current_order = dequeue(orderQueue);
    if (current_order) {
        printf("\n========== ORDER PREPARED ==========\n");
        printf("Order #%d\n", current_order->order_number);
        printf("Item: %s\n", current_order->item);
        printf("Total: %d Baht\n", current_order->price);
        printf("✓ Order completed and ready for pickup!\n");
        printf("=====================================\n");
        free(current_order);
    }
}

// Display current queue status
void showQueueStatus() {
    int size = getQueueSize(orderQueue);
    printf("\n========== QUEUE STATUS ==========\n");
    printf("Current queue size: %d\n", size);
    if (size > 0) {
        printf("There are %d ppl left in the queue\n", size);
        displayQueue(orderQueue);
    } else {
        printf("Queue is empty!\n");
    }
    printf("===================================\n");
}

// Display main menu
void displayMainMenu() {
    printf("\n╔════════════════════════════╗\n");
    printf("║   RESTAURANT MANAGEMENT   ║\n");
    printf("╠════════════════════════════╣\n");
    printf("║ 1. Customer Order          ║\n");
    printf("║ 2. Prepare Order (Ready)   ║\n");
    printf("║ 3. View Queue Status       ║\n");
    printf("║ 4. Customize Menu          ║\n");
    printf("║ 5. Show Menu               ║\n");
    printf("║ 6. Close Restaurant        ║\n");
    printf("╚════════════════════════════╝\n");
}

// Customize menu
void customizeMenu() {
    int choice;
    printf("\n========== CUSTOMIZE MENU ==========\n");
    printf("1. Add new item\n");
    printf("2. View current menu\n");
    printf("3. Back to main menu\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        char item_name[100];
        int item_price;
        printf("Enter item name: ");
        getchar();  // consume newline
        fgets(item_name, sizeof(item_name), stdin);
        item_name[strcspn(item_name, "\n")] = 0;  // remove newline
        
        printf("Enter price: ");
        scanf("%d", &item_price);
        
        int next_id = menu.count + 1;
        addMenuItem(next_id, item_name, item_price);
        printf("✓ Item added successfully! (ID: %d)\n", next_id);
    } else if (choice == 2) {
        displayMenu();
    }
}

// Process command line arguments
void processArguments(int argc, char* argv[]) {
    printf("\n========== LOADING MENU ==========\n");
    
    // Default menu if no arguments
    if (argc < 2) {
        printf("No menu items provided. Using default menu.\n");
    } else {
        // Parse arguments: name:price name:price ...
        for (int i = 1; i < argc; i++) {
            char* item_info = argv[i];
            char* colon = strchr(item_info, ':');
            
            if (colon) {
                int name_len = colon - item_info;
                char name[100];
                int price;
                int id = menu.count + 1;
                
                strncpy(name, item_info, name_len);
                name[name_len] = '\0';
                price = atoi(colon + 1);
                
                addMenuItem(id, name, price);
            }
        }
    }
    
    printf("✓ Menu loaded with %d items\n", menu.count);
    printf("===================================\n");
}

int main(int argc, char* argv[]) {
    orderQueue = initQueue();
    
    if (!orderQueue) {
        printf("Failed to initialize queue!\n");
        return 1;
    }
    
    // Initialize menu with default items
    initMenu();
    
    printf("\n================================\n");
    printf("   RESTAURANT ORDERING SYSTEM  \n");
    printf("================================\n\n");
    
    // Display menu
    displayMenu();
    
    // Process command line arguments: order_id amount order_id amount ... x ...
    if (argc < 2) {
        printf("Usage: %s <order_id> <amount> [<order_id> <amount>] ... [x] ...\n", argv[0]);
        printf("Example: %s 1 100 2 150 x 3 200 x\n", argv[0]);
        freeQueue(orderQueue);
        return 1;
    }
    
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "x") == 0) {
            // Process checkout
            if (getQueueSize(orderQueue) > 0) {
                order* current_order = dequeue(orderQueue);
                if (current_order) {
                    printf("\n========== PLACE ORDER ==========\n");
                    printf("Order #%d\n", current_order->order_number);
                    printf("Item: %s\n", current_order->item);
                    printf("Total: %d Baht\n", current_order->price);
                    printf("\n========== CHECKOUT ==========\n");
                    // Cashier payment
                    processCashier(current_order->order_number, current_order->price);
                    
                    free(current_order);
                }
            } else {
                printf("\n⚠ No orders to checkout!\n");
            }
            i++;
        } else {
            // Parse order_id and amount
            if (i + 1 < argc) {
                int menu_id = atoi(argv[i]);
                int amount = atoi(argv[i + 1]);
                
                placeOrder(menu_id, amount);
                
                i += 2;
            } else {
                printf("❌ Error: Missing amount for order ID %s\n", argv[i]);
                i++;
            }
        }
    }
    
    // Show final queue count and cleanup
    int remaining = getQueueSize(orderQueue);
    if (remaining > 0) {
        printf("\n⚠ %d people left in queue\n", remaining);
    } else {
        printf("\n✓ All orders have been processed\n");
    }
    
    printf("Cleaning up all queue...\n");
    clearQueue(orderQueue);
    printf("✓ Goodbye!\n");
    
    freeQueue(orderQueue);
    return 0;
}
