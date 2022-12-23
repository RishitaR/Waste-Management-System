#include<iostream>
#include<sstream>
#include<mysql.h>
#include"allinterface.cpp"

using namespace std;

// A blueprint class for Waste management system
class WMS:public Interface
{
    // Attributes or properties of a WMS
    private:
    string name, address;
    
    // Member functions of a WMS
    public:
        // To set attributes of WMS
        void setDetails(string name, string address)
        {
            WMS::name = name;
            WMS::address = address;
        }
        
        // To print details of the WMS
        void getDetails()
        {
            cout<<"WMS Name = "<<WMS::name<<endl;
            cout<<"COMPANY ADDRESS = "<<WMS::address<<endl;
	        cout<<endl;
    	}
    	// To get name of the WMS
    	string getName()
    	{
    	    return WMS::name;
    	}
    
    	// To get address of the WMS
    	string getAddress()
    	{
    		return WMS::address;
    	}
};      
