#include<iostream>
#include<list>
using namespace std;

class Interface
{
	private:
    	list<string> Waste_M_Sys = {"hc","Worker"};
    	list<string> WMS_address = {"Head Office, Star House, C-5, G-Block, Bandra Kurla Complex, Bandra (East), Mumbai-400 051"};
	public:
    	void displayFIRST_PAGE()
    	{
        	cout<<"Enter 1 for CUSTOMER LOGIN."<<endl;
        	cout<<"Enter 0 to EXIT."<<endl<<endl;
        	cout<<"Enter option: ";
    	}	
    	void displaySecond_page()
    	{
    		cout<<"Enter 1 to  Waste management system."<<endl;
        	cout<<"Enter 2 to Register New User."<<endl;
        	cout<<"Enter 3 for WMS WORKER."<<endl;
        	cout<<"Enter 4 to Recycle_Market."<<endl;
        	cout<<"Enter 0 to Move Back"<<endl<<endl;
        	cout<<"Enter option: ";
    	}
    	void displayFunctions()
    	{
        	cout<<"Enter 1 to Call for waste collection."<<endl;
        	cout<<"Enter 2 to Change Password."<<endl;
        	cout<<"Enter 3 to Display WMS_Company Details."<<endl;
        	cout<<"Enter 0 to Move Back."<<endl<<endl;
        	cout<<"Enter option: ";
    	}
    	list<string> get_WMS_Company()
    	{
        	return Waste_M_Sys;
    	}
    
    	list<string> get_WMS_Address()
    	{
        	return WMS_address;
    	}
    
    	const char* getConstValue(list<string> l, int index)
    	{
        	list<string>::iterator itr = l.begin();
        	advance(itr, index-1);
        	string s = *itr;
        	return s.c_str();
    	}
    
    	string getValue(list<string> l, int index)
    	{
        	list<string>::iterator itr = l.begin();
        	advance(itr, index-1);
        	return *itr;
    	}
};
