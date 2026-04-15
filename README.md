# Expense Group Splitter (CLI)

**Language:** C  
**File:** `CS103-EXPENSE_GROUP_SPLITTER.c`  

Expense Group Splitter is a C-based command-line tool for managing and splitting expenses within groups. It supports both quick, one-off calculations and full group management with member shares, payments, and summaries.

---

## Features

### Main Menu

- **Expense Management**  
  Full group-based expense tracking with member names, shares, payments, and summaries.

- **Quick Expense Calculation**  
  Fast, one-time expense splits (by event or merged total).

- **Exit**  
  Close the program.

---

### Quick Expense Calculation

For simple, ad‑hoc splits.

- Choose input method:
  - **Scan file** – read expenses from a text file
  - **User input** – enter expenses manually
- Enter multiple expenses:
  - Amount
  - Description
- Choose what to do:
  - **Split Expenses by Event**  
    For each expense:
    - Enter number of members to split by
    - Program prints per-member share for that expense
  - **Merge and Split Expenses**  
    - Shows all expenses with amounts and descriptions  
    - Merges all amounts into a single total  
    - You enter the number of members  
    - Program prints equal share per member for the merged total
  - **Back** – return to main menu

---

### Expense Management

For named groups that you want to track over time.

#### Manage Group Expenses

- **Create New Group**
  - Input method:
    - **Scan file** – load groups, members, and expenses from a text file
    - **User input** – create one group interactively
  - When using **user input**:
    - Enter a group name (e.g., `mine`)
    - Add members by name (`jan`, `nay`, `lao`, …, `done`)
    - Enter how many expenses to input
    - For each expense:
      - Amount spent
      - Brief description
  - Total group expenses are computed automatically.

- **Edit Existing Group**
  - **Rename Group**  
    - Choose a group and enter a new name.
  - **Add Member**  
    - Choose a group and add additional members.
  - **Remove Member**  
    - Choose a group and remove a member by selection.
  - **Delete Group**  
    - Choose a group to remove entirely (shifts remaining groups down).
  - **Back** – return to previous menu.

- **Track Payments**
  - Select a group to track.
  - Options:
    - **Manage Member Share**  
      - Enter each member’s share of the total expenses.  
      - Program checks that the sum of shares equals the total group expense.  
      - If not equal, it asks you to re-enter the shares.  
      - When valid, it:
        - Calculates each member’s outstanding balance vs. an equal share
        - Prints who owes whom and how much (e.g., `-nay owes Php 2917.00 to jan`)
    - **Payment Record**  
      - Choose a member.
      - Enter payment amount.
      - Program records payment and updates remaining outstanding balances.
    - **Back** – return to previous menu.

#### View Summary

- Select a group to view a full summary:
  - **Expenses**
    - Numbered list of each expense:
      - Description
      - Amount
  - **Total expenses**
    - Sum of all expenses in the group
  - **Outstanding Balances**
    - Who still owes whom and how much, based on shares and payments
  - **Payment record table**
    - For each member:
      - Name  
      - Amount paid  
      - Outstanding balance  
      - Status:
        - `Paid`
        - `Overshared/Overpaid`
        - `No transactions`
        - `Unpaid`

#### Save Summary

- Select a group:
  - Enter a filename (e.g., `summary.txt`).
  - Program writes:
    - Group name
    - Expenses list
    - Total expenses
    - Outstanding balances
    - Payment record table  
  - You can choose to save another group summary or return to the main menu.

---

## Building

Requirements:

- C compiler (e.g., `gcc`, `clang`)
- Standard C library, `math.h`, `string.h`, `unistd.h`

Compile:

```bash
git clone https://github.com/jannamorcozo/EXPENSE-SPLITTER-CLI-.git
cd EXPENSE-SPLITTER-CLI-

gcc -Wall -Wextra -O2 -o expense-splitter CS103-EXPENSE_GROUP_SPLITTER.c -lm
```

(`-lm` links the math library for `fabs`.)

Run:

```bash
./expense-splitter
```

---

## File-Based Input (Quick Expense / Group Creation)

The code supports reading from text files for:

- Loading existing groups, members, and expenses (for Expense Management → Create New Group → Scan file)
- Loading expenses for Quick Expense Calculation → Scan file

You can document the exact file format you use. For example, for quick expenses, something like:

```text
2
100 ew
213 sa
```

And for groups, something like:

```text
1
mine
3
jan
nay
lao
2
86 sjd
8923 dh
```

Adjust this section based on the exact format you’re using when you test.

---

## Limitations / Notes

- Maximum number of groups, members, and expenses is bounded by `MAX` (currently 50).
- Input validation is basic; incorrect input types may cause undefined behavior.
- All data is stored in memory; closing the program loses any unsaved changes.
- Negative or zero amounts are not explicitly validated.

---

## Possible Future Improvements

- Stronger input validation and error handling
- Configurable currency or localization
- Support for unequal splits in Quick Expense mode
- JSON/CSV import and export
- Automated tests for core splitting and settlement logic

---
