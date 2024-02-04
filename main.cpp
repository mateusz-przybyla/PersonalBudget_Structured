#include <iostream>
#include <ctime>
#include <sstream>
#include <windows.h>
#include <vector>
#include <iomanip>
#include <math.h>
#include <algorithm>

#include "Markup.h"

using namespace std;

string usersFilename = "users.xml";
string incomesFilename = "incomes.xml";
string expensesFilename = "expenses.xml";

struct User
{
    int userId = 0;
    string login = "";
    string password = "";
    string name = "";
    string surname = "";
};

struct Transaction
{
    int transactionId = 0;
    int userId = 0;
    int date = 0;
    string item = "";
    double amount = 0;
};

//reading inputs functions
string readLine();
int readInteger();
char readChar();
double readDouble();

//conversion-related functions
string convertIntToString(int number);
int convertStringToInt(string number);
double convertStringToDouble(string number);

//auxiliary functions
double roundDouble(double number);
string replaceCommaWithDot(string text);
string readNumber(string text, int signPosition);

//time-related functions
string readTodaysDate();
string readDay(string date);
string readMonth(string date);
string readYear(string date);
int mergeDateWithoutDashes(string date);
string splitDateByDashes(int date);
bool checkNumberOfDaysInMonth(int day, int month, int year);
bool checkIfEnteredDateIsCorrect(string date);
int readSelectedTransactionDate();
string readPreviousMonth(string date);
string checkPreviousYear(string date);
string calculateHowManyDaysPerPreviousMonth(string date);
string readStartDate(char choice);
string readEndDate(char choice);
void checkOrderOfEnteredDates(string &startDate, string &endDate);

//transaction-related functions
int loadIncomesFromFile(vector <Transaction> &incomes, int idOfLoggedInUser);
int loadExpensesFromFile(vector <Transaction> &expenses, int idOfLoggedInUser);
Transaction enterNewIncomeData(int idOfLastIncome, int idOfLoggedInUser);
char showTypesOfExpenses();
Transaction enterNewExpenseData(int idOfLastExpense, int idOfLoggedInUser);
bool writeNewIncomeInFile(Transaction income);
bool writeNewExpenseInFile(Transaction expense);
int addIncome(vector <Transaction> &incomes, int idOfLastIncome, int idOfLoggedInUser);
int addExpense(vector <Transaction> &expenses, int idOfLastExpense, int idOfLoggedInUser);
double sumTransactions(int startDate, int endDate, vector <Transaction> transactions);
void showTransactionData(Transaction transaction);
bool sortCondition(Transaction transaction1, Transaction transaction2);
void selectSortedTransactions(int startDate, int endDate, vector <Transaction> transactions);
void showFinanseBalance(vector <Transaction> incomes, vector <Transaction> expenses, char choice);

//user-related functions
void writeNewUserInFile(User user);
void loadUsersFromFile(vector <User> &users);
bool checkUsername(vector <User> users, string loginSuggestion);
int readNewUserId(vector <User> users);
User enterNewUserData(vector <User> users);
void registerUser(vector <User> &users);
int loginUser(vector <User> users);
void updatePasswordInFile(User user, int idOfLoggedInUser);
void changeLoggedInUserPassword(vector <User> &users, int idOfLoggedInUser);

//menus
char selectAnOptionFromMainMenu();
char selectAnOptionFromUserMenu();

int main()
{
    vector <User> users;
    vector <Transaction> incomes;
    vector <Transaction> expenses;

    char choice;
    int idOfLoggedInUser = 0, idOfLastIncome = 0, idOfLastExpense = 0;

    loadUsersFromFile(users);

    while (true)
    {
        if (idOfLoggedInUser == 0)
        {
            choice = selectAnOptionFromMainMenu();

            switch (choice)
            {
            case '1': registerUser(users); break;
            case '2': idOfLoggedInUser = loginUser(users); break;
            case '9': exit(0); break;
            default:
                cout << endl << "No such an option in the menu." << endl << endl;
                system("pause");
                break;
            }
        }
        else
        {
            if (incomes.empty())
                idOfLastIncome = loadIncomesFromFile(incomes, idOfLoggedInUser);
            if (expenses.empty())
                idOfLastExpense = loadExpensesFromFile(expenses, idOfLoggedInUser);

            choice = selectAnOptionFromUserMenu();

            switch (choice)
            {
                case '1': idOfLastIncome = addIncome(incomes, idOfLastIncome, idOfLoggedInUser); break;
                case '2': idOfLastExpense = addExpense(expenses, idOfLastExpense, idOfLoggedInUser); break;
                case '3': showFinanseBalance(incomes, expenses, '1'); break;
                case '4': showFinanseBalance(incomes, expenses, '2'); break;
                case '5': showFinanseBalance(incomes, expenses, '3'); break;
                case '6': changeLoggedInUserPassword(users, idOfLoggedInUser); break;
                case '7':
                    idOfLoggedInUser = 0;
                    incomes.clear();
                    expenses.clear();
                    break;
            }
        }
    }
    return 0;
}

string readLine()
{
    string input = "";
    getline(cin, input);
    return input;
}

int readInteger()
{
    string input = "";
    int number = 0;

    while (true)
    {
        getline(cin, input);

        stringstream myStream(input);
        if (myStream >> number)
            break;
        cout << "It is not a number. Type again. " << endl;
    }
    return number;
}

char readChar()
{
    string input = "";
    char character  = {0};

    while (true)
    {
        getline(cin, input);

        if (input.length() == 1)
        {
            character = input[0];
            break;
        }
        cout << "This is not a single character. Type again: " << endl;
    }
    return character;
}

double readDouble()
{
    string input = "";
    double number = 0;

    while (true)
    {
        getline(cin, input);

        input = replaceCommaWithDot(input);

        stringstream myStream(input);
        if (myStream >> number)
            break;
        cout << "It is not a number. Type again. " << endl;
    }
    return roundDouble(number);
}

double roundDouble(double number)
{
    return round(number * 100) / 100;
}

string readNumber(string text, int signPosition)
{
    string number = "";
    while (isdigit(text[signPosition]))
    {
        number += text[signPosition];
        signPosition ++;
    }
    return number;
}

string replaceCommaWithDot(string text)
{
    if (!text.empty())
    {
        for (size_t i = 0; i < text.length(); i++)
        {
            if (text[i] == ',')
                text.replace(i, 1, ".");
        }
    }
    return text;
}

string convertIntToString(int number)
{
    ostringstream ss;
    ss << number;
    string str = ss.str();
    return str;
}

int convertStringToInt(string number)
{
    int numberInt;
    istringstream iss(number);
    iss >> numberInt;

    return numberInt;
}

double convertStringToDouble(string number)
{
    double numberDouble;
    istringstream iss(number);
    iss >> numberDouble;

    return numberDouble;
}

string readTodaysDate()
{
    time_t now = time(0);
    tm *localTime = localtime(&now);

    ostringstream ss;
    ss << put_time(localTime, "%Y-%m-%d");

    return ss.str();
}

string readDay(string date)
{
    int position = date.find_last_of('-') + 1;
    string day = readNumber(date, position);

    return day;
}

string readMonth(string date)
{
    int position = date.find_first_of('-') + 1;
    string month = readNumber(date, position);

    return month;
}

string readYear(string date)
{
    int position = 0;
    string year = readNumber(date, position);

    return year;
}

int mergeDateWithoutDashes(string date)
{
    string mergedDate = "";

    mergedDate = readYear(date) + readMonth(date) + readDay(date);

    return convertStringToInt(mergedDate);
}

string splitDateByDashes(int date)
{
    string mergedDate = convertIntToString(date);
    string splitedDate = "";

    for (size_t i = 0; i < mergedDate.length(); i++)
    {
        splitedDate += mergedDate[i];

        if ((i == 3) || (i == 5))
            splitedDate += '-';
    }
    return splitedDate;
}

bool checkNumberOfDaysInMonth(int day, int month, int year)
{
    if ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12))
    {
        if ((day >= 1) && (day <= 31))
            return true;
    }
    else if ((month == 4) || (month == 6) || (month == 9) || (month == 11))
    {
        if ((day >= 1) && (day <= 30))
            return true;
    }
    else if (month == 2)
    {
        if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
        {
            if ((day >= 1) && (day <= 29))
                return true;
        }
        else
        {
            if ((day >= 1) && (day <= 28))
                return true;
        }
    }
    return false;
}

bool checkIfEnteredDateIsCorrect(string date)
{
    int enteredDay = 0, enteredMonth = 0, enteredYear = 0;

    enteredDay = convertStringToInt(readDay(date));
    enteredMonth = convertStringToInt(readMonth(date));
    enteredYear = convertStringToInt(readYear(date));

    int enteredDate = 0;
    enteredDate = mergeDateWithoutDashes(date);

    int currentDate = 0;
    currentDate = mergeDateWithoutDashes(readTodaysDate());

    if ((checkNumberOfDaysInMonth(enteredDay, enteredMonth, enteredYear)) && (enteredDate <= currentDate) && (enteredDate >= 20000101))
        return true;
    else
    {
        cout << "Incorrect date. Try again." << endl;
        return false;
    }
}

int readSelectedTransactionDate()
{
    char choice;
    string date = "";

    cout << "Select date: " << endl;
    cout << "-> 1. today's date" << endl;
    cout << "-> 2. other date" << endl << endl;
    cout << "Your choice: ";
    choice = readChar();

    switch(choice)
    {
    case '1':
        date = readTodaysDate();
        break;
    case '2':
        do
        {
            cout << "Enter date in YYYY-MM-DD format: ";
            date = readLine();
        } while (!checkIfEnteredDateIsCorrect(date));
        break;
    default:
        cout << "No such an option. Today's date was selected by default." << endl;
        date = readTodaysDate();
        system("pause");
        break;
    }
    return mergeDateWithoutDashes(date);
}

string readPreviousMonth(string date)
{
    string month = "";
    string monthsTable[12] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};

    month = readMonth(date);

    for (int i = 0; i < 12; i++)
    {
        if (month == monthsTable[0])
        {
            month = monthsTable[11];
            break;
        }
        else if (month == monthsTable[i])
        {
            month =  monthsTable[i - 1];
            break;
        }
    }
    return month;
}

string checkPreviousYear(string date)
{
    int month = 0, year = 0;
    month = convertStringToInt(readMonth(date));
    year = convertStringToInt(readYear(date));

    if (month == 1)
        year -= 1;

    return convertIntToString(year);
}

string calculateHowManyDaysPerPreviousMonth(string date)
{
    int days = 0, month = 0, year = 0;
    month = convertStringToInt(readPreviousMonth(date));
    year = convertStringToInt(readYear(date));

    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
        days = 31;
    else if (month == 4 || month == 6 || month == 9 || month == 11)
        days = 30;
    else if (month == 2)
    {
        if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
            days = 29;
        else
            days = 28;
    }
    return convertIntToString(days);
}

string readStartDate(char choice)
{
    string currentDate = "", startDate = "";
    currentDate = readTodaysDate();

    switch (choice)
    {
    case '1':
        startDate = currentDate;
        startDate.replace(8, 2, "01");
        break;
    case '2':
        startDate = currentDate;
        startDate.replace(8, 2, "01");
        startDate.replace(5, 2, readPreviousMonth(currentDate));
        startDate.replace(0, 4, checkPreviousYear(currentDate));
        break;
    case '3':
        do
        {
            cout << endl << "Enter start date in YYYY-MM-DD format: ";
            startDate = readLine();
        } while (!checkIfEnteredDateIsCorrect(startDate));
        break;
    }
    return startDate;
}

string readEndDate(char choice)
{
    string currentDate = "", endDate = "";
    currentDate = readTodaysDate();

    switch (choice)
    {
    case '1':
        endDate = currentDate;
        break;
    case '2':
        endDate = currentDate;
        endDate.replace(8, 2, calculateHowManyDaysPerPreviousMonth(currentDate));
        endDate.replace(5, 2, readPreviousMonth(currentDate));
        endDate.replace(0, 4, checkPreviousYear(currentDate));
        break;
    case '3':
        do
        {
            cout << "Enter end date in YYYY-MM-DD format: ";
            endDate = readLine();
        } while (!checkIfEnteredDateIsCorrect(endDate));
        break;
    }
    return endDate;
}

void checkOrderOfEnteredDates(string &startDate, string &endDate)
{
    int startDateInt = 0, endDateInt = 0;
    string helper = "";

    startDateInt = mergeDateWithoutDashes(startDate);
    endDateInt = mergeDateWithoutDashes(endDate);

    if (startDateInt > endDateInt)
    {
        helper = startDate;
        startDate = endDate;
        endDate = helper;
    }
}

int loadIncomesFromFile(vector <Transaction> &incomes, int idOfLoggedInUser)
{
    CMarkup xml;
    int idOfLastIncome = 0;
    bool fileExist = xml.Load(incomesFilename);

    if (fileExist)
    {
        while (xml.FindElem("incomes"))
        {
            xml.IntoElem();
            while (xml.FindElem("income"))
            {
                xml.IntoElem();
                xml.FindElem("userId");
                if (convertStringToInt(xml.GetData()) == idOfLoggedInUser)
                {
                    Transaction income;

                    xml.ResetMainPos();
                    xml.FindElem("incomeId");
                    income.transactionId = convertStringToInt(xml.GetData());
                    xml.FindElem("userId");
                    income.userId = convertStringToInt(xml.GetData());
                    xml.FindElem("date");
                    income.date = convertStringToInt(xml.GetData());
                    xml.FindElem("item");
                    income.item = xml.GetData();
                    xml.FindElem("amount");
                    income.amount = convertStringToDouble(xml.GetData()) / 100;

                    incomes.push_back(income);
                    idOfLastIncome = income.transactionId;
                }
                xml.OutOfElem();
            }
        }
    }
    return idOfLastIncome;
}

int loadExpensesFromFile(vector <Transaction> &expenses, int idOfLoggedInUser)
{
    CMarkup xml;
    int idOfLastExpense = 0;
    bool fileExist = xml.Load(expensesFilename);

    if (fileExist)
    {
        while (xml.FindElem("expenses"))
        {
            xml.IntoElem();
            while (xml.FindElem("expense"))
            {
                xml.IntoElem();
                xml.FindElem("userId");
                if (convertStringToInt(xml.GetData()) == idOfLoggedInUser)
                {
                    Transaction expense;

                    xml.ResetMainPos();
                    xml.FindElem("expenseId");
                    expense.transactionId = convertStringToInt(xml.GetData());
                    xml.FindElem("userId");
                    expense.userId = convertStringToInt(xml.GetData());
                    xml.FindElem("date");
                    expense.date = convertStringToInt(xml.GetData());
                    xml.FindElem("item");
                    expense.item = xml.GetData();
                    xml.FindElem("amount");
                    expense.amount = convertStringToDouble(xml.GetData()) / 100;

                    expenses.push_back(expense);
                    idOfLastExpense = expense.transactionId;
                }
                xml.OutOfElem();
            }
        }
    }
    return idOfLastExpense;
}

Transaction enterNewIncomeData(int idOfLastIncome, int idOfLoggedInUser)
{
    Transaction income;

    income.transactionId = ++idOfLastIncome;
    income.userId = idOfLoggedInUser;
    income.date = readSelectedTransactionDate();

    cout << "Enter purpose: ";
    income.item = readLine();
    cout << "Enter amount: ";
    income.amount = readDouble();

    return income;
}

char showTypesOfExpenses()
{
    char choice;

    system("cls");
    cout << "Select an expense category: " << endl;
    cout << "-> 1. food" << endl;
    cout << "-> 2. vehicles" << endl;
    cout << "-> 3. home" << endl;
    cout << "-> 4. health" << endl;
    cout << "-> 5. events" << endl;
    cout << "-> 6. holiday/journeys" << endl;
    cout << "-> 7. other" << endl << endl;
    cout << "Your choice: ";
    choice = readChar();

    return choice;
}

Transaction enterNewExpenseData(int idOfLastExpense, int idOfLoggedInUser)
{
    Transaction expense;

    expense.transactionId = ++idOfLastExpense;
    expense.userId = idOfLoggedInUser;
    expense.date = readSelectedTransactionDate();

    while (expense.item.empty())
    {
        char choice;
        choice = showTypesOfExpenses();

        switch (choice)
        {
            case '1': expense.item = "food"; break;
            case '2': expense.item = "vehicles"; break;
            case '3': expense.item = "home"; break;
            case '4': expense.item = "health"; break;
            case '5': expense.item = "events"; break;
            case '6': expense.item = "holiday/journeys"; break;
            case '7': expense.item = "other"; break;
            default:
                cout << endl << "There is no such option! Try again." << endl << endl;
                system("pause");
                break;
        }
    }
    cout << "Enter amount: ";
    expense.amount = readDouble();

    return expense;
}

bool writeNewIncomeInFile(Transaction income)
{
    CMarkup xml;
    bool fileExist = xml.Load(incomesFilename);

    if (!fileExist)
    {
        xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
        xml.AddElem("incomes");
    }
    xml.FindElem();
    xml.IntoElem();
    xml.AddElem("income");
    xml.IntoElem();
    xml.AddElem("incomeId", income.transactionId);
    xml.AddElem("userId", income.userId);
    xml.AddElem("date", income.date);
    xml.AddElem("item", income.item);
    xml.AddElem("amount", income.amount * 100);

    if (xml.Save(incomesFilename))
        return true;
    else
        return false;
}

bool writeNewExpenseInFile(Transaction expense)
{
    CMarkup xml;
    bool fileExist = xml.Load(expensesFilename);

    if (!fileExist)
    {
        xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
        xml.AddElem("expenses");
    }
    xml.FindElem();
    xml.IntoElem();
    xml.AddElem("expense");
    xml.IntoElem();
    xml.AddElem("expenseId", expense.transactionId);
    xml.AddElem("userId", expense.userId);
    xml.AddElem("date", expense.date);
    xml.AddElem("item", expense.item);
    xml.AddElem("amount", expense.amount * 100);

    if (xml.Save(expensesFilename))
        return true;
    else
        return false;
}

int addIncome(vector <Transaction> &incomes, int idOfLastIncome, int idOfLoggedInUser)
{
    Transaction income;

    system("cls");
    cout << " >>> ADDING NEW INCOME <<<" << endl << endl;
    income = enterNewIncomeData(idOfLastIncome, idOfLoggedInUser);

    incomes.push_back(income);

    if (writeNewIncomeInFile(income))
        cout << endl << "New income was added." << endl << endl;
    else
       cout << "Error. Failed to add new income to file." << endl;
    system("pause");

    return ++idOfLastIncome;
}

int addExpense(vector <Transaction> &expenses, int idOfLastExpense, int idOfLoggedInUser)
{
    Transaction expense;

    system("cls");
    cout << " >>> ADDING NEW EXPENSE <<<" << endl << endl;
    expense = enterNewExpenseData(idOfLastExpense, idOfLoggedInUser);

    expenses.push_back(expense);

    if (writeNewExpenseInFile(expense))
        cout << endl << "New expense was added." << endl << endl;
    else
       cout << "Error. Failed to add new expense to file." << endl;
    system("pause");

    return ++idOfLastExpense;
}

double sumTransactions(int startDate, int endDate, vector <Transaction> transactions)
{
    double sum = 0;

    for (vector <Transaction>::iterator itr = transactions.begin(); itr != transactions.end(); itr++)
    {
        if (((itr -> date) >= startDate) && ((itr -> date) <= endDate))
            sum += itr -> amount;
    }
    return sum;
}

void showTransactionData(Transaction transaction)
{
    cout << endl;
    cout << transaction.transactionId << "|";
    cout << transaction.userId << "|";
    cout << splitDateByDashes(transaction.date) << "|";
    cout << transaction.item << "|";
    cout << transaction.amount << "|";
}

bool sortCondition(Transaction transaction1, Transaction transaction2)
{
    return (transaction1.date < transaction2.date);
}

void selectSortedTransactions(int startDate, int endDate, vector <Transaction> transactions)
{
    sort(transactions.begin(), transactions.end(), sortCondition);

    for (vector <Transaction>::iterator itr = transactions.begin(); itr != transactions.end(); itr++)
    {
        if (((itr -> date) >= startDate) && ((itr -> date) <= endDate))
            showTransactionData(*itr);
    }
}

void showFinanseBalance(vector <Transaction> incomes, vector <Transaction> expenses, char choice)
{
    string startDate = "", endDate = "";
    double sumOfIncomes = 0, sumOfExpenses = 0;

    startDate = readStartDate(choice);
    endDate = readEndDate(choice);
    checkOrderOfEnteredDates(startDate, endDate);

    system("cls");
    if ((!incomes.empty()) || (!expenses.empty()))
    {
        cout << ">>> TRANSACTIONS SEARCHING <<<" << endl;
        cout << "------------------------------" << endl;
        cout << "From " << startDate << " to " << endDate << " found: ";
        cout << endl << endl << "-> incomes: " << endl;
        selectSortedTransactions(mergeDateWithoutDashes(startDate), mergeDateWithoutDashes(endDate), incomes);
        sumOfIncomes = sumTransactions(mergeDateWithoutDashes(startDate), mergeDateWithoutDashes(endDate), incomes);
        cout << endl << "------------------------------" << endl;
        cout << "Sum of incomes: " << sumOfIncomes << " zl" << endl;
        cout << endl << endl << "-> expenses: " << endl;
        selectSortedTransactions(mergeDateWithoutDashes(startDate), mergeDateWithoutDashes(endDate), expenses);
        sumOfExpenses = sumTransactions(mergeDateWithoutDashes(startDate), mergeDateWithoutDashes(endDate), expenses);
        cout << endl << "------------------------------" << endl;
        cout << "Sum of expenses: " << sumOfExpenses << " zl" << endl;
        cout << endl << "==============================" << endl;
        cout << "Balance (Incomes - Expenses): " << sumOfIncomes - sumOfExpenses << " zl" << endl;
    }
    else
        cout << endl << "No transactions on the list." << endl << endl;

    cout << endl << endl;
    system("pause");
}

void writeNewUserInFile(User user)
{
    CMarkup xml;
    bool fileExist = xml.Load(usersFilename);

    if (!fileExist)
    {
        xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
        xml.AddElem("users");
    }
    xml.FindElem();
    xml.IntoElem();
    xml.AddElem("user");
    xml.IntoElem();
    xml.AddElem("userId", user.userId);
    xml.AddElem("login", user.login);
    xml.AddElem("password", user.password);
    xml.AddElem("name", user.name);
    xml.AddElem("surname", user.surname);

    xml.Save(usersFilename);
}

void loadUsersFromFile(vector <User> &users)
{
    CMarkup xml;
    bool fileExist = xml.Load(usersFilename);

    if (fileExist)
    {
        while (xml.FindElem("users"))
        {
            User user;
            xml.IntoElem();

            while (xml.FindElem("user"))
            {
                xml.IntoElem();
                xml.FindElem("userId");
                user.userId = convertStringToInt(xml.GetData());
                xml.FindElem("login");
                user.login = xml.GetData();
                xml.FindElem("password");
                user.password = xml.GetData();
                xml.FindElem("name");
                user.name = xml.GetData();
                xml.FindElem("surname");
                user.surname = xml.GetData();

                users.push_back(user);
                xml.OutOfElem();
            }
        }
    }
}

bool checkUsername(vector <User> users, string loginSuggestion)
{
    vector <User>::iterator itr = users.begin();

    while (itr != users.end())
    {
        if (itr -> login == loginSuggestion)
        {
            cout << endl << "Such a username exists." << endl;
            return true;
        }
        else
            itr++;
    }
    return false;
}

int readNewUserId(vector <User> users)
{
    if (users.empty())
        return 1;
    else
        return users.back().userId + 1;
}

User enterNewUserData(vector <User> users)
{
    User user;

    user.userId = readNewUserId(users);

    do
    {
        cout << "Enter username: ";
        user.login = readLine();
    } while (checkUsername(users, user.login));

    cout << "Enter password: ";
    user.password = readLine();
    cout << "Enter name: ";
    user.name = readLine();
    cout << "Enter surname: ";
    user.surname = readLine();

    return user;
}

void registerUser(vector <User> &users)
{
    User user = enterNewUserData(users);

    users.push_back(user);
    writeNewUserInFile(user);

    cout << endl << "Account created" << endl << endl;
    system("pause");
}

int loginUser(vector <User> users)
{
    User user;
    string enteredLogin = "", enteredPassword = "";

    cout << endl << "Enter username: ";
    enteredLogin = readLine();

    vector <User>::iterator itr = users.begin();
    while (itr != users.end())
    {
        if (itr -> login == enteredLogin)
        {
            for (int attempt = 3; attempt > 0; attempt--)
            {
                cout << "Enter password. Attempts left: " << attempt << ": ";
                enteredPassword = readLine();

                if (itr -> password == enteredPassword)
                {
                    cout << endl << "You logged in." << endl << endl;
                    system("pause");
                    return itr -> userId;
                }
            }
            cout << "You have entered the wrong password 3 times." << endl;
            system("pause");
            return 0;
        }
        itr++;
    }
    cout << "There is no user with that username." << endl << endl;
    system("pause");
    return 0;
}

void updatePasswordInFile(User user, int idOfLoggedInUser)
{
    CMarkup xml;
    bool fileExist = xml.Load(usersFilename);

    if (fileExist)
    {
        while (xml.FindElem("users"))
        {
            xml.IntoElem();

            while (xml.FindElem("user"))
            {
                xml.IntoElem();
                xml.FindElem("userId");
                if (convertStringToInt(xml.GetData()) == idOfLoggedInUser)
                {
                    xml.FindElem("password");
                    xml.RemoveElem();
                    xml.AddElem("password", user.password);
                    xml.Save(usersFilename);
                }
                xml.OutOfElem();
            }
        }
    }
}

void changeLoggedInUserPassword(vector <User> &users, int idOfLoggedInUser)
{
    string newPassword = "";
    cout << endl << "Enter new password: ";
    newPassword = readLine();

    for (vector <User>::iterator itr = users.begin(); itr != users.end(); itr++)
    {
        if (itr -> userId == idOfLoggedInUser)
        {
            itr -> password = newPassword;
            updatePasswordInFile(*itr, idOfLoggedInUser);
            cout << "Password was changed." << endl << endl;
            system("pause");
        }
    }
}

char selectAnOptionFromMainMenu()
{
    char choice;

    system("cls");
    cout << "     >>> MAIN MENU <<<" << endl;
    cout << "---------------------------" << endl;
    cout << "-> 1. Create an account" << endl;
    cout << "-> 2. Sign in" << endl;
    cout << "-> 9. Close app" << endl;
    cout << "---------------------------" << endl;
    cout << "Your choice: ";
    choice = readChar();

    return choice;
}

char selectAnOptionFromUserMenu()
{
    char choice;

    system("cls");
    cout << "======================================" << endl;
    cout << ">>>>>>>>>>>>>> USER MENU <<<<<<<<<<<<<" << endl;
    cout << "======================================" << endl << endl;
    cout << "-> 1. Add new income" << endl;
    cout << "-> 2. Add new expense" << endl;
    cout << "-> 3. Show current month's balance" << endl;
    cout << "-> 4. Show previous month's balance" << endl;
    cout << "-> 5. Show selected period balance" << endl;
    cout << "======================================" << endl;
    cout << "-> 6. Change password" << endl;
    cout << "-> 7. Sign out" << endl;
    cout << "======================================" << endl;
    cout << "Your choice: ";
    choice = readChar();

    return choice;
}
