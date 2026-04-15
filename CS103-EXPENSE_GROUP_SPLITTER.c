#include<stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#define MAX 50

//structure for Quick Expense Calculation
struct quick_expenseStr
{
    float amount;
    char description[MAX];
    int no_members;//number of members to split by  event like dinner 3 friends needs to split haha

};typedef struct quick_expenseStr quick_expense;

//structure for Expense Management
struct manage_groupStr
{
    char group_name[MAX];
    char members[MAX][MAX];
    int memberCount;
    float member_share[MAX];
    float outstanding_bal[MAX];
    float payment[MAX];
    float totalExpenses;
    float amount[MAX];
    char description[MAX][MAX];
    int no_expenses;
    char status[20][20];

}; typedef struct manage_groupStr manage;

//function for splitting and merging of expenses
float compute_expense(quick_expense q_expense_array[], int expense, int n, int choice);
float getTotalExpenseAmount(manage *expense_array, int index);
void storeGroupName(manage manage_group[], int* groupCount, int* index);
void storeExpenses(manage expense_array[], int index);
void printGroupChoices(manage manage_group[]);
void printMemberNames(manage manage_group[], int group_choice);
void removeMemberFromGroup(manage manage_group[], int group_choice, int member_choice, int* memberCount);
void deleteGroup(manage manage_group[],  manage total_expense[], int group_choice, int* groupCount);

int main() {

    //declaration of array of structure for quick expense
    quick_expense q_expense_array[MAX];

    //for expense management
    manage manage_group[MAX];
    manage total_expense[MAX], share[MAX], pay[MAX], balance[MAX], remaining_balance[MAX];

    int n, i, j, no_expense, no_group, memberCount = 0, groupCount = 0, index = 0, count, group_choice, member_choice;
    float merge_total, split_expense, amount, contribution, min_debt, total, member_outstanding_bal[MAX],
    total_memberShare = 0;

    char file[MAX], scan[MAX], member[MAX], buffer[MAX];

    //variable declaration for choices for controlling the loop
    int choice, first_choice, second_choice, third_choice, fourth_choice;

    printf("\n|===========================================================================================|\n");
    printf("|-------------------------------------------------------------------------------------------|\n");
    printf("|                               EXPENSE GROUP SPLITTER                                      |\n");
    printf("|-------------------------------------------------------------------------------------------|\n");
    printf("|===========================================================================================|\n");
    printf("\tWelcome to Expense Group Splitter! Please choose one of the following options:\n");

    //utilizing while loop to create an infinite loop until the condition is true
    while (1) {

        printf("\n");
        printf("1. Expense Management \n"
               "2. Quick Expense Calculation \n"
               "3. Exit\n");
        printf("\n");

        printf("Please enter your choice: ");
        scanf("%d", &choice);

        //for exit
        if (choice == 3) {
            printf("\n");
            printf("Exiting Expense Group Splitter. Thank you for using!\n");
            break;
        }

        //for expense management
        if (choice == 1) {

            while (1) {

                printf("\n***********************************\n");
                printf("*        Expense Management       *\n");
                printf("***********************************\n");

                printf("What would you like to do next?\n");
                printf("    1. Manage Group Expenses\n"
                       "    2. View Summary\n"
                       "    3. Save Summary\n"
                       "    4. Exit\n");

                printf("\nPlease enter your choice: ");
                scanf("%d", &second_choice);

                //for exit
                if (second_choice == 4) {
                    printf("\nRedirecting to previous menu...\n");
                    printf("\nPlease choose one of the following options:\n");
                    break;
                }

                //manage group expenses
                if (second_choice == 1) {

                    while (1) {

                        printf("\n==========================================\n");
                        printf("           Manage Group Expenses \n");
                        printf("==========================================\n");
                        printf("Please choose one of the following options:\n");
                        printf("    1. Create New Group\n"
                               "    2. Edit Existing Group\n"
                               "    3. Track Payments\n"
                               "    4. Back\n");

                        printf("\nPlease enter your choice: ");
                        scanf("%d", &third_choice);

                        //create a new group
                        if (third_choice == 1) {

                            printf("\n=================================================\n");
                            printf("                Create New Group \n");
                            printf("=================================================\n");
                            printf("Would you prefer to input the data by scanning a\n"
                                   "text file or manually inputting it one by one?\n");
                            printf("\tINPUT 1 - Scan file\n");
                            printf("\tINPUT 2 - User input\n");

                            printf("\nPlease enter your choice: ");
                            scanf("%d", &first_choice);

                            //input data via file
                            if (first_choice == 1) {

                                FILE *file_ptr; //declare file pointer

                                //store file name in a separate variable
                                printf("\nInput the name of the text file (e.g. name.txt) : ");
                                scanf("%s", file);

                                file_ptr = fopen(file, "rt"); //open the file

                                if (file_ptr == NULL) {
                                    printf("Error opening input file.\n");
                                    return 1;
                                }

                                //read the number of groups from the file
                                fscanf(file_ptr, "%d", &no_group);

                                //read group details from the file
                                for ( i = 0; i < no_group; i++, groupCount++) {

                                    fscanf(file_ptr, "%s", manage_group[groupCount].group_name);
                                    fscanf(file_ptr, "%d", &manage_group[groupCount].memberCount);

                                    //scanning members
                                    for ( j = 0; j < manage_group[groupCount].memberCount; j++) {
                                        fscanf(file_ptr, "%s", manage_group[groupCount].members[j]);
                                    }

                                    //for checking
                                    if (fscanf(file_ptr, "%d", &manage_group[groupCount].no_expenses) != 1) {
                                        printf("Error reading expenses for group %d\n", i + 1);
                                        return 1;
                                    }

                                    //scanning the expenses and its description per group
                                    for ( j = 0; j < manage_group[groupCount].no_expenses; ++j) {
                                        fscanf(file_ptr, "%f%c", &manage_group[groupCount].amount[j], &buffer[j]);
                                        fscanf(file_ptr, "%s", manage_group[groupCount].description[j]);
                                    }

                                    index++;
                                    total_expense[groupCount].totalExpenses = getTotalExpenseAmount(manage_group, groupCount);

                                }

                                fclose(file_ptr); //close the file

                                printf("\nGroup data loaded successfully from the file.\n");

                            }

                            //input data via user input
                            if (first_choice == 2) {

                                //function call to store group names
                                storeGroupName(manage_group, &groupCount, &index);

                                while (1) {

                                    memberCount = 0; //reset member count

                                    //add members to the group
                                    printf("\nAdd members to the group by entering their names (type 'done' when finished):\n");

                                    while (1) {
                                        printf("- ");
                                        fgets(member, MAX, stdin);
                                        member[strcspn(member, "\n")] = '\0'; //remove newline character

                                        if (strcmp(member, "done") == 0) {
                                            break;
                                        }

                                        strcpy(manage_group[index].members[memberCount], member);
                                        memberCount++; //increment member count
                                    }

                                    manage_group[index].memberCount = memberCount; //update member count

                                    printf("\nMembers added successfully!\n\n");

                                    printf("Now, let's input expenses.\n");
                                    printf("How many expenses would you like to input? ");
                                    scanf("%d", &manage_group[index].no_expenses);

                                    no_expense = manage_group[index].no_expenses;

                                    //function to store expenses
                                    storeExpenses( manage_group, index);

                                    printf("\nExpenses added successfully!\n");
                                    printf("\nWhat would like to do next?\n");

                                    total_expense[index].totalExpenses = getTotalExpenseAmount(manage_group, index);

                                    break;

                                }

                            }

                        }

                        //edit existing group
                        //options: rename, add member, remove, delete group, group
                        if (third_choice == 2) {

                            while (1) {

                                printf("\n=================================================\n");
                                printf("                Edit Existing Group\n");
                                printf("=================================================\n");
                                printf("Please choose one of the following options:\n");
                                printf("    1. Rename Group\n"
                                       "    2. Add Member\n"
                                       "    3. Remove Member\n"
                                       "    4. Delete Group\n"
                                       "    5. Back\n");

                                printf("\nPlease enter your choice: ");
                                scanf("%d", &fourth_choice);

                                //rename group
                                if (fourth_choice == 1) {

                                    printf("\nSelect the group you want to edit:\n");

                                    //function to print group choices
                                    printGroupChoices(manage_group);

                                    printf("\nPlease enter your choice: ");
                                    scanf("%d", &group_choice);

                                    printf("\nEnter the new name for group '%s': ", manage_group[group_choice - 1].group_name);

                                    while (getchar() != '\n'); //clear input buffer
                                    fgets(manage_group[group_choice - 1].group_name, MAX, stdin);
                                    manage_group[group_choice - 1].group_name[strcspn(manage_group[group_choice - 1].group_name, "\n")] = '\0';

                                    printf("Group '%s' renamed successfully!\n", manage_group[group_choice - 1].group_name);

                                    printf("\nWhat would like to do next?\n");

                                }

                                //add member to group
                                if (fourth_choice == 2) {

                                    printf("\nSelect the group you want to add a member to:\n");

                                    //function to print group choices
                                    printGroupChoices(manage_group);

                                    printf("\nPlease enter your choice: ");
                                    scanf("%d", &group_choice);

                                    printf("\nAdd members to the group by entering their names (type 'done' when finished):\n");
                                    while (getchar() != '\n'); //clear input buffer

                                    while (1) {
                                        memberCount++;
                                        printf("- ");
                                        fgets(member, MAX, stdin);
                                        member[strcspn(member, "\n")] = '\0'; //remove newline character

                                        if (strcmp(member, "done") == 0) {
                                            break;
                                        }

                                        //adds members to group
                                        strcpy(manage_group[group_choice - 1].members[manage_group[group_choice - 1].memberCount], member);
                                        manage_group[group_choice - 1].memberCount++; //increment member count
                                    }

                                    printf("\nMember(s) added successfully.\n");

                                }

                                //remove member
                                if (fourth_choice == 3) {

                                    printf("\nSelect the group from which you want to remove a member:\n");

                                    //function to print group choices
                                    printGroupChoices(manage_group);

                                    printf("\nPlease enter your choice: ");
                                    scanf("%d", &group_choice);
                                    printf("\n");

                                    printf("\nSelect the member you want to remove:\n");

                                    //function print member names for a group
                                    printMemberNames(manage_group, group_choice);

                                    printf("\nPlease enter your choice: ");
                                    scanf("%d", &member_choice);
                                    printf("\n");

                                    //function to remove a member from a group
                                    removeMemberFromGroup(manage_group, group_choice, member_choice, &memberCount);

                                    printf("Member removed from the group successfully!\n");

                                }

                                //delete group
                                if (fourth_choice == 4) {

                                    index--; //decrease index

                                    printf("\nSelect the group you want to delete:\n");

                                    //function to print group choices
                                    printGroupChoices(manage_group);

                                    printf("\nPlease enter your choice: ");
                                    scanf("%d", &group_choice);
                                    printf("\n");

                                    //function to delete a group
                                    deleteGroup(manage_group, total_expense, group_choice, &groupCount);

                                    printf("Group deleted successfully!\n");
                                    printf("\nWhat would like to do next?\n");

                                }

                                //back option
                                if (fourth_choice == 5) {
                                    printf("\nReturning to previous menu...\n");
                                    break;
                                }
                            }
                        }

                        //for track payments
                        if (third_choice == 3) {

                            printf("\n=================================================\n");
                            printf("                 Track Payments\n");
                            printf("=================================================\n");
                            printf("Select the group you want to track:\n");

                            //function to print group choices
                            printGroupChoices(manage_group);

                            while (1) {
                                printf("\nPlease enter your choice: ");
                                if (scanf("%d", &group_choice) != 1 || group_choice < 1 || group_choice > MAX || strlen(manage_group[group_choice - 1].group_name) == 0) {
                                    printf("Invalid group choice. Please choose again.\n\n");

                                    while (getchar() != '\n'); //clear input buffer
                                    continue;
                                }
                                break;
                            }

                            while (1) {

                                printf("\n[  Tracking payments for '%s'  ]\n",  manage_group[group_choice-1].group_name);
                                printf("Please choose one of the following option:\n"
                                       "    1. Manage Member Share\n"
                                       "    2. Payment Record\n"
                                       "    3. Back\n");

                                printf("\nPlease enter your choice: ");
                                scanf("%d", &fourth_choice);

                                //manage member share
                                if (fourth_choice == 1) {

                                    while (1) {

                                        total_memberShare = 0; //reset total member share

                                        printf("\nEnter member share:\n");
                                        for (i = 0; i < manage_group[group_choice - 1].memberCount; i++) {
                                            printf("\tEnter share for %s: ", manage_group[group_choice - 1].members[i]);
                                            scanf("%f", &share[group_choice - 1].member_share[i]);
                                            total_memberShare += share[group_choice - 1].member_share[i];
                                        }
                                        printf("\n");

                                        total = total_expense[group_choice - 1].totalExpenses;
                                        count = manage_group[group_choice - 1].memberCount;
                                        contribution = total / (float) count;

                                        //condition to check
                                        if (total_memberShare == total) {

                                            //calculate outstanding balances and break out of the loop if no errors
                                            for (i = 0; i < count; i++) {
                                                balance[group_choice - 1].outstanding_bal[i] = contribution - share[group_choice - 1].member_share[i];
                                                member_outstanding_bal[i] = balance[group_choice - 1].outstanding_bal[i];
                                                remaining_balance[group_choice - 1].outstanding_bal[i] = balance[group_choice - 1].outstanding_bal[i];
                                            }

                                            //print debts
                                            printf("\nOutstanding Balances:\n");
                                            for (i = 0; i < count; i++) {
                                                for (j = i + 1; j < count; j++) {
                                                    if (member_outstanding_bal[i] > 0 && member_outstanding_bal[j] < 0) {
                                                        min_debt = fabs(member_outstanding_bal[i]);
                                                        printf("	-%s owes Php %.2f to %s\n", manage_group[group_choice - 1].members[i], min_debt, manage_group[group_choice - 1].members[j]);
                                                        member_outstanding_bal[i] -= min_debt;
                                                        member_outstanding_bal[j] += min_debt;
                                                    } else if (member_outstanding_bal[i] < 0 && member_outstanding_bal[j] > 0) {
                                                        min_debt = fabs(member_outstanding_bal[j]);
                                                        printf("	-%s owes Php %.2f to %s\n",manage_group[group_choice - 1].members[j], min_debt, manage_group[group_choice - 1].members[i]);
                                                        member_outstanding_bal[j] += min_debt;
                                                        member_outstanding_bal[i] -= min_debt;
                                                    }
                                                }
                                            }

                                            printf("\nMember share recorded successfully!\n\n");
                                            printf("What would you like to do next?\n");

                                            break; //exit the loop if no errors

                                        } else {

                                            printf("Your member share(s) is not equal to your inputted expenses\n");
                                            printf("Please retype your member share...\n");

                                        }

                                    }

                                    total_memberShare = 0; //reset total member share

                                }

                                //payment record
                                if (fourth_choice == 2) {

                                    printf("\nSelect the member from the list:\n");

                                    //function print member names for a group
                                    printMemberNames(manage_group, group_choice);

                                    printf("\nPlease enter your choice: ");
                                    scanf("%d", &member_choice);

                                    if (member_choice < 0 || member_choice > manage_group[group_choice - 1].memberCount) {
                                        printf("Invalid member choice. Please choose again.\n\n");
                                        continue;
                                    }

                                    printf("\nEnter amount of payment: ");
                                    scanf("%f", &amount);

                                    pay[group_choice - 1].payment[member_choice - 1] = amount;
                                    remaining_balance[group_choice - 1].outstanding_bal[member_choice - 1] -= amount;

                                    while (amount > 0) {
                                        int payment_distributed = 0; //flag to track if payment has been distributed in this iteration
                                        for ( j = 0; j < manage_group[group_choice - 1].memberCount; j++) {
                                            if (remaining_balance[group_choice - 1].outstanding_bal[j] < 0 && amount > 0) {
                                                remaining_balance[group_choice - 1].outstanding_bal[j] += 1;
                                                amount -= 1;
                                                payment_distributed = 1; //set the flag to indicate payment distribution
                                            }
                                        }

                                        //check if payment has been distributed in this iteration
                                        if (!payment_distributed) {
                                            break; //if payment hasn't been distributed in this iteration, break the loop
                                        }
                                    }

                                    printf("Payment recorded successfully!\n\n");

                                    printf("What would you like to do next?\n");
                                }

                                if (fourth_choice == 3) {
                                    printf("\nRedirecting to previous menu...\n");
                                    break;
                                }

                            }

                        }

                        if (third_choice == 4) {
                            printf("\nRedirecting to previous menu...\n\n");
                            break;
                        }

                    }
                }

                //to view expense summary
                if (second_choice == 2) {

                    printf("\n=============================================\n");
                    printf("               View Summary\n");
                    printf("=============================================\n");

                    while (1) {

                        printf("Select the group to view expense summary:\n");

                        //function to print group choices
                        printGroupChoices(manage_group);

                        printf("\nPlease enter your choice: ");
                        scanf("%d", &group_choice);

                        //for status

                        for (j = 0; j < manage_group[group_choice - 1].memberCount; j++) {
                            if (pay[group_choice - 1].payment[j] > 0 && remaining_balance[group_choice - 1].outstanding_bal[j]==0) {
                                strcpy(manage_group[group_choice-1].status[j], "Paid");
                            } else if (remaining_balance[group_choice - 1].outstanding_bal[j] < 0) {
                                strcpy(manage_group[group_choice-1].status[j], "Overshared/Overpaid");
                            } else if (remaining_balance[group_choice - 1].outstanding_bal[j] == 0) {
                                strcpy(manage_group[group_choice-1].status[j], "No transactions");
                            } else {
                                strcpy(manage_group[group_choice-1].status[j], "Unpaid");
                            }
                        }

                        //group choice must be in range of group count to avoid index out of bound
                        if (group_choice < 0 || group_choice > groupCount) {
                            printf("\nIndex out of bound");
                            return 1;
                        }

                        printf("\nExpense summary for %s:\n", manage_group[group_choice - 1].group_name);

                        printf("Expenses: \n");
                        for ( i = 0; i < manage_group[group_choice-1].no_expenses; i++) {
                            printf("\t%d. %s: Php %.2f\n", i + 1, manage_group[group_choice-1].description[i], manage_group[group_choice-1].amount[i]);
                        }

                        printf("\nTotal expenses: Php %.2f\n", total_expense[group_choice - 1].totalExpenses);

                        //member shares
                        for (i = 0; i < count; i++) {
                            member_outstanding_bal[i] = balance[group_choice - 1].outstanding_bal[i];
                        }

                        printf("\nOutstanding Balances:\n");
                        for (i = 0; i < count; i++) {
                            for (j = i + 1; j < count; j++) {
                                if (member_outstanding_bal[i] > 0 && member_outstanding_bal[j] < 0) {
                                    min_debt = fabs(member_outstanding_bal[i]);
                                    printf("	-%s owes Php %.2f to %s\n", manage_group[group_choice - 1].members[i], min_debt, manage_group[group_choice - 1].members[j]);
                                    member_outstanding_bal[i] -= min_debt;
                                    member_outstanding_bal[j] += min_debt;
                                } else if (member_outstanding_bal[i] < 0 && member_outstanding_bal[j] > 0) {
                                    min_debt = fabs(member_outstanding_bal[j]);
                                    printf("	-%s owes Php %.2f to %s\n",manage_group[group_choice - 1].members[j], min_debt, manage_group[group_choice - 1].members[i]);
                                    member_outstanding_bal[j] += min_debt;
                                    member_outstanding_bal[i] -= min_debt;
                                }
                            }
                        }

                        //payment record
                        printf("\nPayment record:\n");
                        printf("--------------------------------------------------------------------------------------------\n");
                        printf("| %-15s | %-17s | %-22s | %-24s |\n", "      Name   ", "   Amount Paid  ", "  Outstanding Balance  ", "         Status  ");
                        printf("--------------------------------------------------------------------------------------------\n");
                        for (i = 0; i < manage_group[group_choice - 1].memberCount; i++) {
                            printf("| %-15s | Php %-13.2f | Php %-19.2f | %-24s | \n", manage_group[group_choice - 1].members[i], pay[group_choice - 1].payment[i], remaining_balance[group_choice - 1].outstanding_bal[i], manage_group[group_choice-1].status[i]);
                        }
                        printf("--------------------------------------------------------------------------------------------\n");

                        printf("\nPlease select an option:\n");
                        printf("1. View expense summary for another group\n");
                        printf("2. Back to the main menu\n");

                        printf("\nPlease enter your choice: ");
                        scanf(" %d", &third_choice);
                        printf("\n");

                        if (third_choice == 1) {
                            continue;
                        }

                        if (third_choice == 2) {
                            break;
                        }

                    }

                }

                //to save summary (file)
                if (second_choice == 3) {

                    printf("\n=============================================\n");
                    printf("               Save Summary\n");
                    printf("=============================================\n");

                    //assuming manage_group, expense_array, total_expense, remaining_balance, and pay are defined elsewhere
		            while (1) {

                        char filePath[MAX] = "";
		                FILE *filePtr;

		                printf("Select the group to view expense summary:\n");

		                //function to print group choices
		                printGroupChoices(manage_group);

		                printf("\nPlease enter your choice: ");
                        scanf("%d", &group_choice);

		                //group choice must be in range of group count to avoid index out of bound
		                if (group_choice < 0 || group_choice > groupCount) {
		                    printf("\nIndex out of bound");
		                    return 1;
		                }

		                printf("\nEnter the filename to save the expense summary (e.g. name.txt) : ");
		                scanf("%s", filePath);

		                filePtr = fopen(filePath, "w"); //open the file in write mode

		                //check if the file is opened successfully
		                if (filePtr == NULL) {
		                    printf("Error opening file!\n");
		                    return 1;
		                }

                        fprintf(filePtr, "\nExpense summary for %s:\n", manage_group[group_choice - 1].group_name);

                        fprintf(filePtr, "Expenses: \n");
                        for ( i = 0; i < manage_group[group_choice-1].no_expenses; i++) {
                            fprintf(filePtr,  "\t%d. %s: Php %.2f\n", i + 1, manage_group[group_choice-1].description[i], manage_group[group_choice-1].amount[i]);
                        }

                        fprintf(filePtr, "\nTotal expenses: Php %.2f\n", total_expense[group_choice - 1].totalExpenses);

                        //member shares
                        for (i = 0; i < count; i++) {
                            member_outstanding_bal[i] = balance[group_choice - 1].outstanding_bal[i];
                        }

                        fprintf(filePtr, "\nOutstanding Balances:\n");
                        for (i = 0; i < count; i++) {
                            for (j = i + 1; j < count; j++) {
                                if (member_outstanding_bal[i] > 0 && member_outstanding_bal[j] < 0) {
                                    min_debt = fabs(member_outstanding_bal[i]);
                                    fprintf(filePtr, " %s owes Php %.2f to %s\n", manage_group[group_choice - 1].members[i], min_debt, manage_group[group_choice - 1].members[j]);
                                    member_outstanding_bal[i] -= min_debt;
                                    member_outstanding_bal[j] += min_debt;
                                } else if (member_outstanding_bal[i] < 0 && member_outstanding_bal[j] > 0) {
                                    min_debt = fabs(member_outstanding_bal[j]);
                                    fprintf(filePtr, " %s owes Php %.2f to %s\n",manage_group[group_choice - 1].members[j], min_debt, manage_group[group_choice - 1].members[i]);
                                    member_outstanding_bal[j] += min_debt;
                                    member_outstanding_bal[i] -= min_debt;
                                }
                            }
                        }

                        //payment record
                        fprintf(filePtr, "\nPayment record:\n");
                        fprintf(filePtr, "--------------------------------------------------------------------------------------------\n");
                        fprintf(filePtr, "| %-15s | %-17s | %-22s | %-24s |\n", "      Name   ", "   Amount Paid  ", "  Outstanding Balance  ", "         Status  ");
                        fprintf(filePtr, "--------------------------------------------------------------------------------------------\n");
                        for (i = 0; i < manage_group[group_choice - 1].memberCount; i++) {
                            fprintf(filePtr, "| %-15s | Php %-13.2f | Php %-19.2f | %-24s | \n", manage_group[group_choice - 1].members[i], pay[group_choice - 1].payment[i], remaining_balance[group_choice - 1].outstanding_bal[i], manage_group[group_choice-1].status[i]);
                        }
                        fprintf(filePtr, "--------------------------------------------------------------------------------------------\n");

			            fclose(filePtr);

			            printf("\nPlease select an option:\n");
                        printf("1. Save expense summary for another group\n");
                        printf("2. Back to the main menu\n");

                        printf("\nPlease enter your choice: ");
                        scanf(" %d", &third_choice);

                        if (third_choice == 1) {
                            continue;
                        }

                        if (third_choice == 2) {
                            break;
                        }

                    }

                }

            }

        }

        //quick expense calculation
        if (choice == 2) {

            printf("\n******************************************\n");
            printf("*        Quick Expense Calculation       *\n");
            printf("******************************************\n");
            printf("Would you prefer to input the data by scanning a\n"
                   "text file or manually inputting it one by one?\n");
            printf("\tINPUT 1 - Scan file\n");
            printf("\tINPUT 2 - User input\n");

            printf("\nPlease enter your choice:");
            scanf("%d", &second_choice);
            printf("\n");

            if (second_choice == 1 || second_choice == 2) {

                //function scanning file
                if (second_choice == 1) {

                    FILE *file_ptr; //declare file pointer

                    file_ptr = fopen(file, "rt"); //open file before the choice menu

                    printf("Input the name of the text file: ");
                    scanf("%s", file); //store file name in a separate variable

                    file_ptr = fopen(file, "rt"); //open the file with the new file name

                    if (file_ptr == NULL) {
                        printf("Error opening input file\n");
                        return 1;
                    }

                    fscanf(file_ptr, "%d", &no_expense);

                    for (i = 0; i < no_expense; ++i) {
                        fscanf(file_ptr, "%f", &(q_expense_array[i].amount));
                        fgetc(file_ptr);

                        fgets(q_expense_array[i].description, MAX, file_ptr);
                        q_expense_array[i].description[strcspn(q_expense_array[i].description, "\n")] = '\0';
                    }

                    fclose(file_ptr);
                    printf("\nData loaded successfully from the file.\n");

                }

                if (second_choice == 2) {

                    printf("Enter the number of expenses: ");
                    scanf("%d", &no_expense);

                    for (i = 0; i < no_expense; ++i) {

                        printf("\nExpense %d:\n", i + 1);
                        printf("\tAmount: ");
                        scanf("%f", &(q_expense_array[i].amount));

                        getchar(); //consume the newline character left in the input buffer

                        printf("\tDescription: ");
                        fgets(q_expense_array[i].description, MAX, stdin);

                        q_expense_array[i].description[strcspn(q_expense_array[i].description, "\n")] = '\0'; //remove the newline character from fgets

                    }

                    printf("\nExpense input complete...\n");

                }

                printf("\nDo you want to:\n");
                printf("\tINPUT 1 - Split Expenses by Event\n");
                printf("\tINPUT 2 - Merge and Split Expenses\n");
                printf("\tINPUT 3 - Back\n");

                while (1) {

                    printf("\nPlease enter your choice: ");
                    scanf("%d", &third_choice);

                    if (third_choice == 3) {
                        printf("\n");
                        printf("Redirecting to main menu...\n");
                        break;
                    }

                    //for split expenses by event
                    if (third_choice == 1) {

                        for (i = 0; i < no_expense; i++) {
                            printf("\nEnter the members to split by for '%s': ", q_expense_array[i].description);
                            scanf("%d", &(q_expense_array[i].no_members));

                            printf("\nExpenses split for '%s' among %d members:\n", q_expense_array[i].description, q_expense_array[i].no_members);

                            printf("Total Amount: Php %.2f\n", q_expense_array[i].amount);
                            for (j = 0; j < q_expense_array[i].no_members ; ++j) {
                                split_expense = q_expense_array[i].amount / (float) q_expense_array[i].no_members;

                                printf("\t-Member %d: Php %.2f\n", j + 1, split_expense);
                            }
                        }

                        printf("\nRedirecting to main menu...\n");
                        printf("\nWhat would like to do next?");
                        break;

                    }

                    //for merge and split expenses
                    if (third_choice == 2) {

                        for (i = 0; i < no_expense; ++i) {
                            printf("\t%d. Php %.2f for %s\n", i + 1, q_expense_array[i].amount, q_expense_array[i].description);
                        }
                        printf("\n");

                        //function call for the total
                        merge_total = compute_expense(q_expense_array, no_expense, n, 3);//3 kasi meron doon if else na 1 nd 2 para doon siya sa isa mag bagsak

                        //merge total
                        printf("Merge total: Php %.2f\n", merge_total);
                        printf("\n");

                        printf("Enter the number of members to split by: ");
                        scanf("%d", &n);
                        printf("\n");

                        //function call for splitting
                        split_expense = compute_expense(q_expense_array, no_expense, n, 1);

                        printf("Expenses split equally among %d members:\n", n);

                        printf("Total Amount: Php %.2f\n", merge_total);
                        for (i = 0; i < n; i++) {
                            printf("\t-Member %d: Php %.2f\n", i + 1, split_expense);
                        }

                        //function to print choices
                        printf("\nRedirecting to main menu...\n");
                        printf("\nWhat would like to do next?");

                        break;

                    }

                    printf("\n");

                }
            }

        }

    }

    return 0;

}


float compute_expense(quick_expense q_expense_array[], int expense, int n, int choice) {
    float total = 0, computation = 0;
    for (int i = 0; i < expense; ++i) {
        total += q_expense_array[i].amount;
    }

    //when choice is it's for splitting expenses
    if (choice == 1) {
        computation = total / (float) n;
        return computation;
    }

    return total;
}

float getTotalExpenseAmount(manage *expense_array, int index) {
    float total = 0;
    for (int i = 0; i < expense_array[index].no_expenses; i++) {
        total += expense_array[index].amount[i];
    }
    return total;
}

void storeGroupName(manage manage_group[], int* groupCount, int* index) {
    //find first available slot to add new group
    while (*index < MAX && strlen(manage_group[*index].group_name) > 0) {
        (*index)++;
    }

    //add new group if there is space
    printf("\nEnter the name of the new group: ");
    while (getchar() != '\n'); //clear input buffer

    //store new group name
    fgets(manage_group[*index].group_name, MAX, stdin);
    manage_group[*index].group_name[strcspn(manage_group[*index].group_name, "\n")] = '\0';

    (*groupCount)++; //increase group count since user created a new group

    printf("\nGroup '%s' created successfully!\n", manage_group[*index].group_name);
}

void storeExpenses(manage expense_array[], int index) {
    for (int i = 0; i < expense_array[index].no_expenses; ++i) {
        printf("\n");
        printf("	Enter the amount spent: ");
        scanf("%f", &expense_array[index].amount[i]);
        printf("	Enter a brief description: ");
        while (getchar() != '\n'); //clear input buffer
        fgets(expense_array[index].description[i], MAX, stdin);
        expense_array[index].description[i][strcspn(expense_array[index].description[i], "\n")] = '\0';
    }
}

void printGroupChoices(manage manage_group[]) {
    for (int i = 0; i < MAX; i++) {
        if (strlen(manage_group[i].group_name) > 0) {
            printf("   %d. %s\n", i + 1, manage_group[i].group_name);
        }
    }
}

void printMemberNames(manage manage_group[], int group_choice) {
    for (int i = 0; i < manage_group[group_choice - 1].memberCount; i++) {
        printf("    %d. %s\n", i + 1, manage_group[group_choice - 1].members[i]);
    }
}

void removeMemberFromGroup(manage manage_group[], int group_choice, int member_choice, int* memberCount) {
    //remove member from the group by shifting all elements to the left to overwrite the member to be deleted
    for (int i = member_choice - 1; i < manage_group[group_choice - 1].memberCount - 1; i++) {
        strcpy(manage_group[group_choice - 1].members[i], manage_group[group_choice - 1].members[i + 1]);
    }

    manage_group[group_choice - 1].memberCount--; //decrease member count

    (*memberCount)--; //decrease member count
}

void deleteGroup(manage manage_group[], manage total_expense[], int group_choice, int* groupCount) {
    //shift all elements to the left to overwrite the group to be deleted
    for (int i = group_choice - 1; i < MAX - 1; i++) {
        strcpy(manage_group[i].group_name, manage_group[i + 1].group_name);
        manage_group[i].memberCount = manage_group[i + 1].memberCount;

        for (int j = 0; j < manage_group[i + 1].memberCount; j++) {
            strcpy(manage_group[i].members[j], manage_group[i + 1].members[j]);
            strcpy(manage_group[i].status[j], manage_group[i+1].status[j]);
        }

        total_expense[i].totalExpenses = total_expense[i + 1].totalExpenses;

        manage_group[i].no_expenses = manage_group[i + 1].no_expenses;
        for (int j = 0; j < manage_group[group_choice-1].no_expenses; j++) {
            manage_group[i].amount[j] = manage_group[i + 1].amount[j];
            strcpy(manage_group[i].description[j], manage_group[i + 1].description[j]);
        }
    }

    //clear the last element
    strcpy(manage_group[MAX - 1].group_name, "");
    manage_group[MAX - 1].memberCount = 0;

    --(*groupCount); //decrease group count
}
