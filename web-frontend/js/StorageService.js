export const StorageService = {
    // We are migrating to a ledger system. Let's use new keys to avoid conflicts 
    // with the old data structure, or you could write a migrator.
    // For this fresh architecture, we'll use 'ledger_' prefix.

    getItems: () => JSON.parse(localStorage.getItem('ledger_items')) || [],
    saveItems: (items) => localStorage.setItem('ledger_items', JSON.stringify(items)),
    
    getTransactions: () => JSON.parse(localStorage.getItem('ledger_transactions')) || [],
    saveTransactions: (txs) => localStorage.setItem('ledger_transactions', JSON.stringify(txs)),
    
    getNextId: () => parseInt(localStorage.getItem('ledger_next_id')) || 1,
    saveNextId: (id) => localStorage.setItem('ledger_next_id', id.toString())
};
