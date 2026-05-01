import { inventoryManager } from './InventoryManager.js';

export const UIController = {
    currentSort: 'id', // Default sorting

    init() {
        this.cacheDOM();
        this.bindEvents();
        this.render();
    },

    cacheDOM() {
        // Forms
        this.form = document.getElementById('item-form');
        this.editForm = document.getElementById('edit-form');
        
        // Table & Controls
        this.tbody = document.getElementById('inventoryBody');
        this.emptyState = document.getElementById('emptyState');
        this.tableContainer = document.getElementById('tableContainer');
        this.sortByIdBtn = document.getElementById('sortById');
        this.sortByNameBtn = document.getElementById('sortByName');
        
        // Dashboard
        this.dashTotalValue = document.getElementById('dashTotalValue');
        this.dashItemsSold = document.getElementById('dashItemsSold');
        this.dashAlerts = document.getElementById('dashAlerts');
        
        // Modals & Toasts
        this.modal = document.getElementById('editModal');
        this.cancelEditBtn = document.getElementById('cancelEdit');
        this.toastContainer = document.getElementById('toastContainer');
    },

    bindEvents() {
        this.form.addEventListener('submit', (e) => this.handleAddItem(e));
        this.editForm.addEventListener('submit', (e) => this.handleEditItem(e));
        this.cancelEditBtn.addEventListener('click', () => this.closeModal());
        
        // Sorting Controls
        this.sortByIdBtn.addEventListener('click', () => {
            this.currentSort = 'id';
            this.updateSortButtons();
            this.renderTable();
        });
        
        this.sortByNameBtn.addEventListener('click', () => {
            this.currentSort = 'name';
            this.updateSortButtons();
            this.renderTable();
        });

        // Event Bus
        document.addEventListener('inventoryUpdated', () => this.render());
    },

    triggerUpdate() {
        document.dispatchEvent(new Event('inventoryUpdated'));
    },

    showToast(message, type = 'success') {
        const toast = document.createElement('div');
        toast.className = `toast ${type}`;
        
        const icon = type === 'urgent' ? '⚠️' : '✅';
        toast.innerHTML = `<span>${icon}</span> <span>${message}</span>`;
        
        this.toastContainer.appendChild(toast);
        
        setTimeout(() => {
            toast.style.animation = 'fadeOut 0.3s ease forwards';
            setTimeout(() => toast.remove(), 300);
        }, 3500);
    },

    updateSortButtons() {
        this.sortByIdBtn.classList.toggle('active', this.currentSort === 'id');
        this.sortByNameBtn.classList.toggle('active', this.currentSort === 'name');
    },

    handleAddItem(e) {
        e.preventDefault();
        const name = document.getElementById('itemName').value.trim();
        const quantity = parseInt(document.getElementById('itemQty').value);
        const price = parseFloat(document.getElementById('itemPrice').value);

        if (name && quantity >= 0 && price >= 0) {
            inventoryManager.addItem(name, price, quantity);
            this.form.reset();
            document.getElementById('itemName').focus();
            this.triggerUpdate();
            this.showToast(`Added ${name} to inventory`, 'success');
        }
    },

    handleEditItem(e) {
        e.preventDefault();
        const id = parseInt(document.getElementById('editId').value);
        const name = document.getElementById('editName').value.trim();
        const quantity = parseInt(document.getElementById('editQty').value);
        const price = parseFloat(document.getElementById('editPrice').value);

        if (name && quantity >= 0 && price >= 0) {
            inventoryManager.editItem(id, name, price, quantity);
            this.closeModal();
            this.triggerUpdate();
            this.showToast(`Updated ${name}`, 'success');
        }
    },

    sellItem(id) {
        const item = inventoryManager.getAllActiveItems().find(i => i.id === id);
        
        if (inventoryManager.sellItem(id)) {
            this.triggerUpdate();
            this.showToast(`Sold 1x ${item.name}`, 'success');
            
            const newStock = inventoryManager.getStock(id);
            if (newStock === 0) {
                this.showToast(`URGENT: ${item.name} is completely out of stock!`, 'urgent');
            } else if (newStock <= 5) {
                this.showToast(`Low Stock: Only ${newStock} ${item.name} left.`, 'urgent');
            }
        }
    },

    deleteItem(id) {
        if (confirm("Are you sure you want to delete this item?")) {
            inventoryManager.deleteItem(id);
            this.triggerUpdate();
            this.showToast(`Item deleted`, 'success');
        }
    },

    openEditModal(id) {
        const item = inventoryManager.getAllActiveItems().find(i => i.id === id);
        if (!item) return;
        
        document.getElementById('editId').value = item.id;
        document.getElementById('editName').value = item.name;
        document.getElementById('editQty').value = 0;
        document.getElementById('editPrice').value = item.price;
        
        this.modal.classList.add('show');
    },

    closeModal() {
        this.modal.classList.remove('show');
    },

    render() {
        this.renderDashboard();
        this.renderTable();
    },

    renderDashboard() {
        const stats = inventoryManager.getDashboardStats();
        const formatter = new Intl.NumberFormat('en-US', { style: 'currency', currency: 'USD' });
        
        this.dashTotalValue.textContent = formatter.format(stats.totalValue);
        
        // Pluralization fix
        const unitLabel = stats.totalItemsSold === 1 ? 'unit' : 'units';
        this.dashItemsSold.textContent = `${stats.totalItemsSold} ${unitLabel} (30d)`;
        
        if (stats.lowStockItems.length > 0) {
            this.dashAlerts.innerHTML = stats.lowStockItems.map(i => 
                `<div class="alert-item">⚠️ ${i.name} (Only ${i.stock} left)</div>`
            ).join('');
        } else {
            this.dashAlerts.innerHTML = `<div class="alert-item success">✅ All stock levels healthy</div>`;
        }
    },

    renderTable() {
        let items = inventoryManager.getAllActiveItems();
        
        // Apply sorting
        items.sort((a, b) => {
            if (this.currentSort === 'id') return a.id - b.id;
            if (this.currentSort === 'name') return a.name.localeCompare(b.name);
            return 0;
        });

        const formatter = new Intl.NumberFormat('en-US', { style: 'currency', currency: 'USD' });

        if (items.length === 0) {
            this.tableContainer.style.display = 'none';
            this.emptyState.style.display = 'block';
        } else {
            this.tableContainer.style.display = 'block';
            this.emptyState.style.display = 'none';
        }

        this.tbody.innerHTML = '';
        items.forEach((item) => {
            const tr = document.createElement('tr');
            
            let statusHtml = '';
            if (item.quantity === 0) {
                statusHtml = '<span class="badge out-stock">Out of Stock</span>';
            } else if (item.quantity <= 5) {
                statusHtml = '<span class="badge low-stock">Low Stock</span>';
            } else {
                statusHtml = '<span class="badge in-stock">In Stock</span>';
            }
            
            const isOut = item.quantity === 0;

            tr.innerHTML = `
                <td>#${item.id}</td>
                <td><strong>${item.name}</strong></td>
                <td>${item.quantity}</td>
                <td>${item.sold}</td>
                <td>${statusHtml}</td>
                <td>${formatter.format(item.price)}</td>
                <td class="action-btns">
                    <button class="sell-btn" data-action="sell" data-id="${item.id}" ${isOut ? 'disabled' : ''}>Sell 1</button>
                    <button class="edit-btn" data-action="edit" data-id="${item.id}">Edit</button>
                    <button class="delete-btn" data-action="delete" data-id="${item.id}">Del</button>
                </td>
            `;
            this.tbody.appendChild(tr);
        });

        // Delegate events
        this.tbody.querySelectorAll('button').forEach(btn => {
            btn.addEventListener('click', (e) => {
                const action = e.target.getAttribute('data-action');
                const id = parseInt(e.target.getAttribute('data-id'));
                if (action === 'sell') this.sellItem(id);
                if (action === 'edit') this.openEditModal(id);
                if (action === 'delete') this.deleteItem(id);
            });
        });
    }
};
