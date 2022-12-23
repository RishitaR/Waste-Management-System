#include <iostream>
#include <sstream>
#include <conio.h>
#include <string.h>
#include <mysql.h>
#include "alluser.cpp"
#include "allstartpoint.cpp"
#include "allmark.cpp"
#include <bits/stdc++.h>
#include <map>
#include <fstream>

using namespace std;

// Global variable for login check
bool login;

// Structure for performing tasks on the database
struct dbHelper
{
    // Attributes or properties of structure
    const char *server, *user, *password, *database;
    
    // To set attributes of a structure as per selected database
    void setDetails(const char *db)
    {
        server = "localhost";
        user = "root";
        password = "rish14";	
        database = db;
    }
    // To run queries on the database
    MYSQL_RES* mysqlExecuteQuery(MYSQL *connection, const char *sqlQuery)
    {
        if(mysql_query(connection, sqlQuery))
        {
            cout<<"MySQL Query Error: "<<mysql_error(connection)<<endl;
            exit(1);
        }
        return mysql_use_result(connection);
    }
    // To register the new user
    void registerUser(MYSQL *con)
    {
        struct user_details user;
        user.getDetails();
        user.capitalize(user.first_name);
        user.capitalize(user.last_name);
        if(uniquePass(con, user.password))
        {
            stringstream ss;
            string str;
            ss<<"insert into user_details(first_name, last_name, gender, address, username, password) values('"<<user.first_name<<"','"<<user.last_name<<"','"<<user.gender<<"','"<<user.address<<"','"<<user.username<<"','"<<user.password<<"');";
            str = ss.str();
            system("cls");
            if(!(mysql_query(con, str.c_str())))
            {
                cout<<"User Registeration Successful."<<endl;
            }
            else
            {
                cout<<"User Registeration Failed."<<endl;
                cout<<"con err: "<<mysql_errno(con)<<endl;
            }
        }
        else
        {
            cout<<"Enter a unique password!!!"<<endl;
        }
    }
    // To login the existing user
    struct user_details loginUser(MYSQL *con)
    {
        struct user_details user;
        string username, password;
        cout<<"Enter Username: ";
        cin>>username;
        cout<<"Enter Password: ";
        cin>>password;
        stringstream ss;
        ss<<"select * from user_details where username = '"<<username<<"' and password = '"<<password<<"';";
        string str = ss.str();
        MYSQL_RES *res;
        MYSQL_ROW row;
        res = mysqlExecuteQuery(con, str.c_str());
        if((row = mysql_fetch_row(res)) != NULL)
        {
            user.setDetails(row[0], row[1], row[2], atoi(row[3]), row[4], row[5], row[6]);
            cout<<"Verification Successful..."<<endl;
            login = true;
        }
        else
        {
            cout<<"Verification Failed..."<<endl;
            cout<<"con err: "<<mysql_errno(con)<<endl;
        }
        mysql_free_result(res);
        return user;
    }
    
    // To get record of a particular user
    void getRecord(MYSQL *con, string username, string password)
    {
        struct user_details user;
        stringstream ss;
        ss<<"select * from user_details where username = '"<<username<<"' and password = '"<<password<<"';";
        string str = ss.str();
        MYSQL_RES *res;
        MYSQL_ROW row;
        res = mysqlExecuteQuery(con, str.c_str());
        while((row = mysql_fetch_row(res)) != NULL)
        {
            user.setDetails(row[0], row[1], row[2], atoi(row[3]), row[4], row[5], row[6]);
            user.printDetails();
        }
        mysql_free_result(res);
    }
    // To change user's password
    void changePassword(MYSQL *con, struct user_details user)
    {
        string newpass;
        cout<<"Enter new password: ";
        cin>>newpass;
        stringstream ss;
        ss<<"update user_details set password = '"<<newpass<<"' where username = '"<<user.username<<"' and password = '"<<user.password<<"';";
        string str = ss.str();
        if(!(mysql_query(con, str.c_str())))
        {
            cout<<"Password Change Successful."<<endl;
        }
        else
        {
            cout<<"Password Change Failed."<<endl;
            cout<<"con err: "<<mysql_errno(con)<<endl;
        }
    }
    // To check if the password is unique
    bool uniquePass(MYSQL *con, string pass)
    {
        stringstream ss;
        ss<<"select * from user_details where password = '"<<pass<<"';";
        string str = ss.str();
        MYSQL_RES *res;
        MYSQL_ROW row;
        bool unique = true;
        res = mysqlExecuteQuery(con, str.c_str());
        if((row = mysql_fetch_row(res)) != NULL)
        {
            unique = false;
        }
        return unique;
    }
    // To check if the user exists in the database or not
    bool userExists(MYSQL *con, int acc)
    {
        stringstream ss;
        ss<<"select * from user_details where account_no = "<<acc<<";";
        string str = ss.str();
        MYSQL_RES *res;
        MYSQL_ROW row;
        bool exists = false;
        res = mysqlExecuteQuery(con, str.c_str());
        if((row = mysql_fetch_row(res)) != NULL)
        {
            exists = true;
        }
        mysql_free_result(res);
        return exists;
    }
};
    // To clear console screen
void clearScreen()
    {
        cout<<"\nPress any key to continue..."<<endl;
        getch();
        system("cls");
    }
MYSQL* setupConnection(struct dbHelper details)
{
    MYSQL *con = mysql_init(0);
    con = mysql_real_connect(con, details.server, details.user, details.password, details.database, 0, NULL, 0);
    if(!con)
    {
        cout<<"Connection Error Number: "<<mysql_errno(con)<<endl;
        exit(1);
    }
    return con;
}

// waste collection
class wastec
{
    protected:
        int total_waste,non_recyc_wgt,plastic_wt,metal_wt,glass_wt,rubber_wt,paper_wt;
        int total_recy_wt,cust_pay;
    public:
        wastec()
        {
        		cout<<"\nEnter total amount of non-recyclable waste in kgs: ";
                cin>>non_recyc_wgt;
                cout<<"*** Enter recyclable waste data of different categories: ***"<<endl;
                cout<<"Enter total amount of plastic waste in kgs: ";
                cin>>plastic_wt;
                cout<<"Enter total amount of metal waste in kgs: ";
                cin>>metal_wt;
                cout<<"Enter total amount of glass waste in kgs: ";
                cin>>glass_wt;
                cout<<"Enter total amount of rubber waste in kgs: ";
                cin>>rubber_wt;
                cout<<"Enter total amount of paper waste in kgs: ";
                cin>>paper_wt;
		}
        int cal_tot_rec()
        {
        	total_recy_wt = plastic_wt + metal_wt + glass_wt + rubber_wt + paper_wt;
            return total_recy_wt;
       
		}
        int cal_tot_wt()
        {
            total_waste = cal_tot_rec() + non_recyc_wgt;
            return total_waste;
        }
        
};

class ind:public wastec
{
    protected:
        short int recy_rev, nonrecy_rev, total_rev;
    public:
        int rec_rev()		// recyclable waste revenue 
        {
            recy_rev = 7*cal_tot_rec();   // Rs.7 per kg will be collected from the industry
            return recy_rev;
        }
        int nonrec_rev()       // non-recyclable waste revenue
        {
            nonrecy_rev = 5*non_recyc_wgt;		// Rs.5 per kg will be collected from the industry
            return nonrecy_rev;
        }
        int tot_rev()
        {
            total_rev = recy_rev + nonrecy_rev;
            return total_rev;
        }
        int user_payment()
        {
            cout<<"\nYou will only be given money for recyclable waste."<<endl;
            cust_pay = 3*cal_tot_rec();		// Rs.3 will be given to customer for 1 kg recyclable waste
            return cust_pay;
        }
        void display1()
        {   
        	cout<<"\nTotal weight of your waste in kgs is:"<<cal_tot_wt()<<endl;
            cout<<"Total weight of your recyclable waste in kgs is:"<<cal_tot_rec()<<endl;
            cout<<"Total weight of your non-recyclable waste in kgs is:"<<non_recyc_wgt<<endl;
		}
        void display()
        {
            cout<<"\nThe non-recyclable waste has been sent to biogas plant"<<endl;
            cout<<"The recyclable waste has been sent to plastic,paper,metal,rubber,glass industries repectively for making new products."<<endl;
            cout<<"\nYou recieved total amount for the given waste: "<<user_payment()<<endl;
            cout<<"\nREVENUE COLLECTED FROM 2 SECTORS IS:"<<endl;
            cout<<"\nFrom recyclable waste: "<<rec_rev();
            cout<<"\nFrom non-recyclable waste: "<<nonrec_rev();
            cout<<"\nTotal revenue from biogas plant and all other industries: "<<tot_rev();
        }
};

//Market
class market
{
	public:
		char c1, confirm_quantity;
		float quantity;
		int selectedNum;
		double total_amount = 0;
		void display();

		int flag = 0;
	// Stores items with their corresponding price
	map<string, double> items = {
	// plastic products and their cost
	{ "Plastic Bag", 5 },{ "Plastic Bottle", 12 },{ "Platic Box", 20 },{"Bucket",150},{"Containers",250},
	// glass products and their cost
	{ "Glass Bottle", 100 },{ "Jar", 80 },{ "Cosmetics", 349 },{ "Mug", 60 },{ "Glass plate", 55 },
	// rubber products and their cost
	{ "Rubber Band", 3 },{ "Tubes", 100 },{ "Mouldings", 50 },{ "Washers", 140 },{ "Gloves", 30 },
	// glass products and their cost
	{ "Metal Box", 150 },{ "Wire", 180 },{ "Cable", 150 },{ "Cans", 20 },{ "Metal Plate", 50 },
	// glass products and their cost
	{ "Paper Plate", 15 },{"Newspaper",13},{"Cardboard",25},{"Magazines",55},{"Envelopes",20}
	};

// Stores the selected items with their quantity
	map<string, int> selected_items;
	void printBill(map<string, double> items,map<string, int> selected_items,float total_amount);
	void customerDetails();
	void showMenu();
	void showPlasticMenu();
	void showGlassMenu();
	void showRubberMenu();
	void showMetalMenu();
	void showPaperMenu();

// Function to print the bill after shopping is completed prints the items, quantity, their cost along with total amount


// Function to display the plastic category
	void selectedPlastic()
	{
		cout << "Do you wish to continue? (for yes press (Y/y), if no press other letter ): ";
		cin >> c1;

		if (c1 == 'Y' || c1 == 'y') 
		{
			cout << "Enter respective number: ";
			cin >> selectedNum;

			if (selectedNum == 1 || selectedNum == 2 || selectedNum == 3 || selectedNum == 4 || selectedNum == 5) 
        	{
			// Selected Plastic Bag
				if (selectedNum == 1) 
            	{
					cout << "selected Plastic Bag\n";
					do {
						cout << "Quantity: ";
						cin >> quantity;
						cout << "You have selected Plastic Bag - "<< quantity << endl;
						cout << "Are you sure? (for yes press (Y/y), if no press other letter): ";
						cin >> confirm_quantity;

					} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity) != floor(quantity)));

					if (confirm_quantity == 'y'|| confirm_quantity == 'Y')
                	{
						total_amount += quantity* items["Plastic Bag"];
						selected_items["Plastic Bag"] = quantity;
						cout << "amount = "<< total_amount << endl;
					}	
				}
			// Selected Plastic Bottle
				if (selectedNum == 2) 
            	{
					cout << "selected Plastic Bottle\n";
					do {
						cout << "Quantity: ";
						cin >> quantity;
						cout << "You have selected plastic bottle - "<< quantity << endl;
						cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
						cin >> confirm_quantity;
					} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

					if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                	{
						total_amount += quantity * items["Plastic Bottle"];
						selected_items["Plastic Bottle"] = quantity;
						cout << "amount = "<< total_amount << endl;
					}
				}
				// Selected Platic Box
				if (selectedNum == 3)
            	{
					cout << "You have selected Platic Box\n";
					do {
						cout << "Quantity: ";
						cin >> quantity;
						cout << "You have selected Platic Box - "<< quantity<< endl;
						cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
						cin >> confirm_quantity;
					} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

					if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                	{
						total_amount += quantity * items["Platic Box"];
						selected_items["Platic Box"] = quantity;
						cout << "amount = "<< total_amount<< endl;
					}	
				}
		
				if (selectedNum == 4)
            	{
					cout << "You have selected Bucket\n";
					do {
						cout << "Quantity: ";
						cin >> quantity;
						cout << "You have selected Bucket - "<< quantity<< endl;
						cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
						cin >> confirm_quantity;
					} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

					if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                	{
						total_amount += quantity * items["Bucket"];
						selected_items["Bucket"] = quantity;
						cout << "amount = "<< total_amount<< endl;
					}	
				}
				if (selectedNum == 5)
            	{
					cout << "You have selected Containers\n";
					do {
						cout << "Quantity: ";
						cin >> quantity;
						cout << "You have selected Containers - "<< quantity<< endl;
						cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
						cin >> confirm_quantity;
					} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

					if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                	{
						total_amount += quantity * items["Containers"];
						selected_items["Containers"] = quantity;
						cout << "amount = "<< total_amount<< endl;
					}
				}
			}
			else 
			{
				flag = 1;
			}
		}
		else 
		{
			flag = 1;
		}
}

// If Glass category is selected
void selectedGlass()
{
	cout << "Do you wish to continue? (for yes press (Y/y), if no press other letter): ";
	cin >> c1;
	if (c1 == 'Y' || c1 == 'y') 
    {
		cout << "Enter respective number: ";
		cin >> selectedNum;
		if (selectedNum == 1 || selectedNum == 2 || selectedNum == 3 || selectedNum == 4 || selectedNum == 5) 
        {
			// selected Glass Bottle
			if (selectedNum == 1) 
            {
				cout << "selected Glass Bottle\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Glass Bottle - "<< quantity << endl;
					cout << "Are you sure? (for yes press (Y/y ), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Glass Bottle"];
					selected_items["Glass Bottle"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
			// selected Glass Plate
			if (selectedNum == 2) 
            {
				cout << "selected Jar\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Jar - "<< quantity << endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity!= 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Jar"];
					selected_items["Jar"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
			// selected Glass Vase
			if (selectedNum == 3) 
            {
				cout << "selected Cosmetics\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Cosmetics - "<< quantity << endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y' || confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Cosmetics"];
					selected_items["Cosmetics"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
			if (selectedNum == 4)
            {
				cout << "You have selected Mug\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Mug - "<< quantity<< endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Mug"];
					selected_items["Mug"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
			if (selectedNum == 5)
            {
				cout << "You have selected Glass plate\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Glass plate - "<< quantity<< endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Glass Plate"];
					selected_items["Glass Plate"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
		}
		else 
		{
			flag = 1;
		}
	}
	else 
	{
		flag = 1;
	}
}

// If Rubber category is selected
void selectedRubber()
{
	cout << "Do you wish to continue? (for yes press (Y/y), if no press other letter): ";
	cin >> c1;
	if (c1 == 'Y' || c1 == 'y') 
    {
		cout << "Enter respective number: ";
		cin >> selectedNum;
		if (selectedNum == 1 || selectedNum == 2 || selectedNum == 3 || selectedNum == 4 || selectedNum == 5) 
		{
			// selected Rubber Band
			if (selectedNum == 1) 
            {
				cout << "selected Rubber Band\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Rubber Band - "<< quantity << endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Rubber Band"];
					selected_items["Rubber Band"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}

			// selected Tube
			if (selectedNum == 2) 
            {
				cout << "selected Tube\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Tube- "<< quantity << endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity	!= 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y' || confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Tube"];
					selected_items["Tube"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}

			if (selectedNum == 3) 
            {
				cout << "selected Mouldings\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Rubber Grip - "<< quantity << endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                {
					total_amount += quantity* items["Mouldings"];
					selected_items["Mouldings"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
			if (selectedNum == 4)
            {
				cout << "You have selected Washers\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Washers - "<< quantity<< endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Washers"];
					selected_items["Washers"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
			if (selectedNum == 5)
            {
				cout << "You have selected Gloves\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Gloves - "<< quantity<< endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Gloves"];
					selected_items["Gloves"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
		}
		else 
		{
			flag = 1;
		}
	}
	else 
	{
		flag = 1;
	}
}

// Function to display the Metal category
void selectedMetal()
{
	cout << "Do you wish to continue? (for yes press (Y/y), if no press other letter ): ";
	cin >> c1;

	if (c1 == 'Y' || c1 == 'y') {
		cout << "Enter respective number: ";
		cin >> selectedNum;

		if (selectedNum == 1 || selectedNum == 2 || selectedNum == 3 || selectedNum == 4 || selectedNum == 5) 
		{
			// Selected Metal Box
			if (selectedNum == 1) 
			{
				cout << "selected Metal Box\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Metal Box - "<< quantity << endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter): ";

					cin >> confirm_quantity;

				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity) != floor(quantity)));

				if (confirm_quantity == 'y'	|| confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Metal Box"];
					selected_items["Metal Box"] = quantity;
					cout << "amount = "<< total_amount << endl;
				}
			}

			// Selected wire
			if (selectedNum == 2) 
            {
				cout << "selected Wire\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selectedted Metal Chain - "<< quantity << endl;
					cout << "Are you sure?(for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Wire"];
					selected_items["Wire"] = quantity;
					cout << "amount = "<< total_amount << endl;
				}
			}

			// Selected Cable
			if (selectedNum == 3) {
				cout << "You have selected Metal Handle\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Cable - "<< quantity<< endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y' || confirm_quantity == 'Y') 
                    {
					total_amount += quantity * items["Cable"];
					selected_items["Cable"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
			if (selectedNum == 4)
            {
				cout << "You have selected Cans\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Cans - "<< quantity<< endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Cans"];
					selected_items["Cans"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
			if (selectedNum == 5)
            {
				cout << "You have selected Metal Plate\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Metal Plate - "<< quantity<< endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y'|| confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Metal Plate"];
					selected_items["Metal Plate"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
		}
		else 
		{
			flag = 1;
		}
	}
	else 
	{
		flag = 1;
	}
}


// If Paper category is selected
void selectedPaper()
{
	cout << "Do you wish to continue?(for yes press (Y/y),if no press other letter): ";
	cin >> c1;
	if (c1 == 'Y' || c1 == 'y') 
    {
		cout << "Enter respective number: ";
		cin >> selectedNum;
		if (selectedNum == 1 || selectedNum == 2 || selectedNum == 3 || selectedNum == 4 || selectedNum == 5) 
        {
			// selected Paper Plate
			if (selectedNum == 1) 
            {
				cout << "selected Paper Plate\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Paper Plate - "<< quantity << endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y' || confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Paper Plate"];
					selected_items["Paper Plate"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}

			// selected Newspaper
			if (selectedNum == 2) 
            {
				cout << "selected Newspaper\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Newspaper - "<< quantity << endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity	!= 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y' || confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Newspaper"];
					selected_items["Newspaper"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}

			// selected Cardboard
			if (selectedNum == 3) 
            {
				cout << "selected Cardboard\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Cardboard - "<< quantity << endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y' || confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Cardboard"];
					selected_items["Cardboard"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
			// selected Magazines
			if (selectedNum == 4) 
            {
				cout << "selected Magazines\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Magazines - "<< quantity << endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y' || confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Magazines"];
					selected_items["Magazines"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
			// selected Envelopes
			if (selectedNum == 5) 
            {
				cout << "selected Envelopes\n";
				do {
					cout << "Quantity: ";
					cin >> quantity;
					cout << "You have selected Envelopes - "<< quantity << endl;
					cout << "Are you sure? (for yes press (Y/y), if no press other letter ): ";
					cin >> confirm_quantity;
				} while ((confirm_quantity != 'y'&& confirm_quantity != 'Y')|| (quantity < 0)|| (ceil(quantity)	!= floor(quantity)));

				if (confirm_quantity == 'y' || confirm_quantity == 'Y') 
                {
					total_amount += quantity * items["Envelopes"];
					selected_items["Envelopes"] = quantity;
					cout << "amount = "<< total_amount<< endl;
				}
			}
		}
		else 
		{
			flag = 1;
		}
	}
	else 
	{
		flag = 1;
	}
}
};

void market::printBill(map<string, double> items,map<string, int> selected_items,float total_amount)
{
	cout << "Item	 "
		<< "Quantity	 "
		<< "Cost\n";

	for (auto j = selected_items.begin();j != selected_items.end(); j++) 
	{
		cout << j->first << "	 ";
		cout << j->second << "		 ";
		cout << (selected_items[j->first])* (items[j->first])<< endl;
	}
	cout << "------------------------------------\n";
	cout << "Total amount:			 "<< total_amount << endl;
	cout << "------------------------------------\n";
	cout << "**THANK YOU && HAPPY ONLINE SHOPPING**";
}

// Function to ask the basic details of any customer
void market::customerDetails()
{
	cout << "Enter your name: ";
	string customer_name;
	getline(cin, customer_name);
	cout << "WELCOME ";
	for (int i = 0;i < customer_name.length();i++) 
    {
		cout << char(toupper(customer_name[i]));
	}
	cout << "\n";
}

// showMenu() is to print the menu to the user
void market::showMenu()
{
	cout << "Menu\n";
	cout << "= = = = = = = = = = = = = \n";
	cout << "1.Plastic\n2.Glass\n3."<< "Rubber\n4.Metal\n5.Paper\n";
	cout << "= = = = = = = = "<< " = = = = = \n";
}

// Function to display the plastic products
void market::showPlasticMenu()
{
	cout << "- - - - - - - - - - - - -\nItem	 Cost\n";
	cout << "1.Plastic Bag       Rs.5/-\n";
	cout << "2.Plastic Bottle    Rs.12/-\n";
	cout << "3.Platic Box        Rs.20/-\n";
	cout << "4.Bucket    	     Rs.150/-\n";
	cout << "5.Containers        Rs.250/-\n";
	cout << "- - - - - - - - - - - - -\n";
}

// Function to display Glass products
void market::showGlassMenu()
{
	cout << "- - - - - - - - - - - - -\nItem	 Cost\n";
	cout << "1.Glass Bottle  			 Rs.100/-\n";
	cout << "2.Jar	 		 			 Rs.80/-\n";
	cout << "3.Cosmetics     			 Rs.349/-\n";
	cout << "4.Mug  		 			 Rs.60/-\n";
	cout << "5.Glass Plate	 			 Rs.55/-\n";
	cout << "- - - - - - - - - - - - -\n";
}

// Function to display Rubber products
void market::showRubberMenu()
{
	cout << "- - - - - - - - - - - -\nItem	 Cost\n";
	cout << "1.Rubber Band			 Rs.3/-\n";
	cout << "2.Tubes	 			 Rs.100/-\n";
	cout << "3.Mouldings			 Rs.50/-\n";
	cout << "4.Washers			 	 Rs.140/-\n";
	cout << "5.Gloves			 	 Rs.30/-\n";
	cout << "- - - - - - - - - - - - -\n";
}

// Function to display Metal products
void market::showMetalMenu()
{
	cout << "- - - - - - - - - - "<< " - -\nItem	 Cost\n";
	cout << "1.Metal Box			 	Rs.150/-\n";
	cout << "2.Wire					 	Rs.180/-\n";
	cout << "3.Cable	 			 	Rs.150/-\n";
	cout << "4.Cans	 			 		Rs.20/-\n";
	cout << "5.Metal Plate				Rs.50/-\n";
	cout << "- - - - - - - - - - - - -\n";
}

// Function to display Paper products
void market::showPaperMenu()
{
	cout << "- - - - - - - - - - "<< " - -\nItem	 Cost\n";
	cout << "1.Paper Plate		 Rs.15/-\n";
	cout << "2.Newspaper		 Rs.13/-\n";
	cout << "3.Cardboard		 Rs.25/-\n";
	cout << "4.Magazines		 Rs.55/-\n";
	cout << "5.Envelopes		 Rs.20/-\n";
	cout << "- - - - - - - - - - - - -\n";
}

int main()
{
    MYSQL *con;
    WMS w;
//    ind i;
    int flag=0;
	market m;
	int n;
    int ch;
    string plastic[20];
    fstream FileName;						
    struct user_details user;
    struct dbHelper mysqlD, db;
    int option,dOption;
    bool loginScreen;
    
    
    while(1)
    {
        w.displayFIRST_PAGE();
        cin>>option;
        if(option == 0)
        {
            system("cls");
            cout<<"Program terminated.....";
            exit(0);
        }
        if(option > 2)
        {
            system("cls");
            cout<<"Invalid option!!!"<<endl;
            clearScreen();
            continue;
        }
        system("cls");
        loginScreen = true;
        mysqlD.setDetails(w.getConstValue(w.get_WMS_Company(), option));
        w.setDetails(w.getValue(w.get_WMS_Company(), option), w.getValue(w. get_WMS_Address(), option));
        while(loginScreen)
        {
            w.displaySecond_page();
            cin>>option;
            system("cls");
            con = setupConnection(mysqlD);
            switch(option)
            {
                case 1:
                {
                    user = mysqlD.loginUser(con);
                    clearScreen();
                    while(login)
                    {
                        w.displayFunctions();
                        cin>>option;
                        system("cls");
                        switch(option)
                        {
                            case 1:
                            {
                                //call for waste collection
                                ind i;
								i.cal_tot_rec();
    							i.cal_tot_wt();
    							i.display1();
    							i.rec_rev();
    							i.nonrec_rev();
    							i.tot_rev();
    							i.user_payment();
    							i.display();  
								    
								clearScreen();
							    break;
                            }
                            case 2:
                            {
                                mysqlD.changePassword(con, user);
                                clearScreen();
                                break;
                            }
                            case 3:
                            {
                                w.getDetails();
                                clearScreen();
                                break;
                            }
                            case 0:
                            {
                                cout<<"Moving to main screen."<<endl;
                                clearScreen();
                                login = false;
                                break;
                            }
                             default:
                    		{
                       			cout<<"Invalid option!!!"<<endl;
                        		clearScreen();
                       			break;
							}
                       	}
					}
					break;
				}   		
                case 2:
                {
                    mysqlD.registerUser(con);
					clearScreen();
					break;
                }
                case 3:
                {
                    // worker function
					do{
					    cout<<"\n\n1. For Entering data in file.";
					    cout<<"\n2. For Displaying data.";
						cout<<"\nEnter your choice: ";
					    cin>>ch;
					    switch (ch)
					    {
							case 1:    
							{          
							    FileName.open("FileName.txt", ios::out);                
							    if (!FileName) {                            
							        cout<<" Error while creating the file ";          
							    }
							    else {
							        cout<<"File created and data got written to file";  
							        cout<<"\nEnter no. of plastic products: ";
							        cin>>n;
							        cout<<"Enter plastic products: ";
							        for (int i = 0; i < n; i++)
							        {
							            cin>>plastic[i];
							            FileName<<plastic[i];  
							        }
							        FileName.close();                   
							    }
							        break;
							}
							case 2:
							{	
							    FileName.open("FileName.txt", ios::in);         
							    if (!FileName) 
								{                        
							        cout<<"File doesn't exist.";          
						        }
						        else 
								{
							        cout<<("\nDisplaying all the file data.\n");
						            char x;                     
						            while (1) 
									{         
							            FileName>>x;              
							            if(FileName.eof())          
						                break;              
						                cout<<x;                  
						            }
						        }
						        break;
						    }
							default:
							{
							    cout<<"Invalid option. Try again.";
						     	break;
							}
						}					   
					}while(ch!=2);
					clearScreen();
					break;						
				}
                case 4:
                {
                    // function call
					m.customerDetails();
					do {
						showMenu();
						cout << "Do you wish to continue?"<< "(for yes press (Y/y ), "<< " if no press other letter ): ";
						char c;
						cin >> c;
						if (c == 'Y' || c == 'y') 
						{
							cout << "Enter respective number: ";
							int num;
							cin >> num;
							if (num == 1 || num == 2|| num == 3 || num == 4 || num == 5 ) 
							{
								switch (num) 
								{
									case 1:
									{
										// For Plastic
										showPlasticMenu();
										selectedPlastic();
										break;
									}
									case 2:
									{
										// For Glass
										showGlassMenu();
										selectedGlass();
										break;
									}
									case 3:
									{
										// For Rubber
										showRubberMenu();
										selectedRubber();
										break;
									}
									case 4:
									{
										// For Metal
										showMetalMenu();
										selectedMetal();
										break;
									}
									case 5:
									{
										// For Paper
										showPaperMenu();
										selectedPaper();
										break;
									}
								}
							}
							else 
							{
								flag = 1;
							}
						}
						else 
						{
							flag = 1;
						}
					} while (flag == 0);
					// print bill
					printBill(items, selected_items,total_amount);              
                    clearScreen();
                  	break;
                }
                case 0:
                {
                    cout<<"Moving to First screen."<<endl;
                    clearScreen();
                    loginScreen = false;
                    break;
                }
                default:
                {
                    cout<<"Invalid option!!!"<<endl;
                    clearScreen();
                    break;
               }
           }
       }
   }
    mysql_close(con);
 	return 0;
}
