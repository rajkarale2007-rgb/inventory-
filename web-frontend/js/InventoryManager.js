import { StorageService } from './StorageService.js';

class InventoryManager {
    constructor() {
        this.items = StorageService.getItems();
        this.transactions = StorageService.getTransactions();
        this.nextId = StorageService.getNextId();
    }

    _save() {
        StorageService.saveItems(this.items);
        StorageService.saveTransactions(this.transactions);
        StorageService.saveNextId(this.nextId);
    }

    // Ledger approach: Add an item, then record an 'ADD' transaction for initial stock
    addItem(name, price, initialQuantity) {
        const item = { id: this.nextId++, name, price };
        this.items.push(item);
        
        if (initialQuantity > 0) {
            this.recordTransaction(item.id, 'ADD', initialQuantity);
        } else {
            this._save();
        }
        
        return item;
    }

    // The core Ledger mechanism: every stock movement is an immutable event
    recordTransaction(itemId, type, amount) {
        const tx = {
            id: Date.now().toString() + Math.random().toString(36).substring(2, 7),
            itemId,
            type, // 'ADD' or 'REMOVE'
            amount: parseInt(amount),
            date: new Date().toISOString()
        };
        this.transactions.push(tx);
        this._save();
    }

    editItem(id, name, price, additionalQuantity) {
        const item = this.items.find(i => i.id === id);
        if (item) {
            item.name = name;
            item.price = price;
            
            // In a ledger system, editing quantity usually means adding stock (restocking)
            if (additionalQuantity > 0) {
                 this.recordTransaction(id, 'ADD', additionalQuantity);
            } else {
                 this._save();
            }
        }
    }

    deleteItem(id) {
        this.items = this.items.filter(i => i.id !== id);
        // We keep the transactions in the ledger for historical financial data, 
        // but the item itself is removed from the active view.
        this._save();
    }

    sellItem(id, amount = 1) {
        const currentStock = this.getStock(id);
        if (currentStock >= amount) {
            this.recordTransaction(id, 'REMOVE', amount);
            return true;
        }
        return false;
    }

    // Helper: Replay the transactions to calculate current stock dynamically
    getStock(itemId) {
        return this.transactions
            .filter(tx => tx.itemId === itemId)
            .reduce((total, tx) => {
                if (tx.type === 'ADD') return total + tx.amount;
                if (tx.type === 'REMOVE') return total - tx.amount;
                return total;
            }, 0);
    }

    // Helper: Replay transactions to calculate total items sold
    getSoldAmount(itemId) {
        return this.transactions
            .filter(tx => tx.itemId === itemId && tx.type === 'REMOVE')
            .reduce((total, tx) => total + tx.amount, 0);
    }

    // Aggregate Data for the Dashboard Summary View
    getDashboardStats() {
        let totalValue = 0;
        let lowStockItems = [];
        let totalItemsSold = 0;

        this.items.forEach(item => {
            const stock = this.getStock(item.id);
            totalValue += stock * item.price;
            
            if (stock <= 5) {
                lowStockItems.push({ ...item, stock });
            }
        });

        // Calculate trends: Items sold in the last 30 days
        const thirtyDaysAgo = new Date();
        thirtyDaysAgo.setDate(thirtyDaysAgo.getDate() - 30);
        
        totalItemsSold = this.transactions
            .filter(tx => tx.type === 'REMOVE' && new Date(tx.date) >= thirtyDaysAgo)
            .reduce((total, tx) => total + tx.amount, 0);

        return {
            totalValue,
            lowStockItems,
            totalItemsSold
        };
    }
    
    // Get enriched items for the UI table
    getAllActiveItems() {
        return this.items.map(item => ({
            ...item,
            quantity: this.getStock(item.id),
            sold: this.getSoldAmount(item.id)
        }));
    }
}

// Export a single instance to act as a singleton service
export const inventoryManager = new InventoryManager();
