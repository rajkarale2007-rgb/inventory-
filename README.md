# Inventory Management Console Application

A modular console application with a C data layer for binary file I/O and a C++ frontend for application logic, memory management, and menu handling.

## Architecture
- **Backend (C)**: Manages low-level binary file operations (`inventory.dat`). Supports soft deletes.
- **Frontend (C++)**: Wraps the C API using an `InventoryManager` class. Manages input validation and sorts lists using STL.

## Build and Run Instructions

### Using CMake (Recommended for Windows/Cross-Platform)
1. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```
2. Generate build files:
   ```bash
   cmake ..
   ```
3. Compile the application:
   ```bash
   cmake --build .
   ```
4. Run:
   - Windows: `Debug\inventory_app.exe` or `inventory_app.exe`
   - Linux/macOS: `./inventory_app`

### Using Makefile (Linux / WSL / MinGW)
1. In the project root, run:
   ```bash
   make
   ```
2. Run:
   ```bash
   ./inventory_app
   ```

## Test Cases

1. **Test Case 1: Add a Valid Item**
   - **Action**: Add Item (ID: 1, Name: "Laptop", Qty: 10, Price: 999.99)
   - **Expected**: "Item added successfully."

2. **Test Case 2: Reject Duplicate ID**
   - **Action**: Add Item (ID: 1, Name: "Mouse", Qty: 50, Price: 19.99)
   - **Expected**: "Failed to add item." (Shows constraints check failed).

3. **Test Case 3: Reject Invalid Inputs**
   - **Action**: Add Item (ID: -5, Name: "Keyboard", Qty: -1, Price: -10)
   - **Expected**: "Failed to add item."

4. **Test Case 4: Update Existing Item**
   - **Action**: Update Item (ID: 1, New Name: "Gaming Laptop", New Qty: 8, New Price: 1200.00)
   - **Expected**: "Item updated successfully." Get Item with ID 1 should reflect the new data.

5. **Test Case 5: Soft Delete and Persistence**
   - **Action**: Delete Item (ID: 1).
   - **Expected**: "Item deleted successfully." Listing items should not show ID 1. Restarting the application and listing items should still not show ID 1, proving binary persistence of the soft delete.
