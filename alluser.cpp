#include<iostream>

using namespace std;

// A blueprint structure of a Consumer
struct user_details
{
    // Attributes or properties of a user
    string first_name;
    string last_name;
    string gender;
    int account_no;
    string address;
    string username;
    string password;
    
    // To get details of a user object as input from the console
    void getDetails()
    {
        cout<<"Enter First Name: ";
        cin>>first_name;
        cout<<"Enter Last Name: ";
        cin>>last_name;
        cout<<"Enter Gender(Male, Female, Others): ";
        cin>>gender;
        cout<<"Enter your Address: ";
        cin>>address;
        cout<<"Enter Username: ";
        cin>>username;
        cout<<"Enter Password: ";
        cin>>password;
    }
    
    // To set details of a user object with reference to the database
    void setDetails(string first, string last, string gen, int acc, string addr, string user, string pass)
    {
        first_name = first;
        last_name = last;
        gender = gen;
        account_no = acc;
        address = addr;
        username = user;
        password = pass;
    }
    
    // To print details of a particular user
    void printDetails()
    {
        cout<<"First Name = "<<first_name<<endl;
        cout<<"Last Name = "<<last_name<<endl;
        cout<<"Gender = "<<gender<<endl;
        cout<<"Account Number = "<<account_no<<endl;
        cout<<"Address = "<<address<<endl;
        cout<<"Username = "<<username<<endl;
        cout<<"Password = "<<password<<endl;
        cout<<endl;
    }

    void capitalize(string &str)
    {
        if (str.length() == 0) 
		{
            return;
        }
        str[0] = std::toupper(str[0]);
    }
};
