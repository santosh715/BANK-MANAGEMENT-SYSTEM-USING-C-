#include<iostream>
#include<fstream>
#include<cctype>
#include<iomanip>
using namespace std;

class account{
	int acno;
	char name[100];
	char type;
	int deposit;
	public:
		void create_account();
		void show_account() const;
		void modify();
		void dep(int);
		void draw(int);
		void report() const;
		int retacno() const;
		int retdeposit() const;
		char rettype() const;
};

void account::create_account()
{
	cout<<"Enter the Account No.:"<<endl;
	cin>>acno;
	cout<<"Enter the name of the Account Holder:"<<endl;
	cin.ignore();
	cin.getline(name,100);
	cout<<"Enter the Type of Account (C/S): "<<endl;
	cin>>type;
	type=toupper(type);
	cout<<"Enter initial amount(<=500 for Saving and <=1000 for Current):"<<endl;
	cin>>deposit;
	cout<<"Account Created!"<<endl;
}

void account::show_account() const
{
	cout<<"Account Number:"<<acno<<endl;
	cout<<"Name of the Account Holder:"<<name<<endl;
	cout<<"Type of Account" <<type<<endl;
	cout<<"Balance Amount:"<<deposit<<endl;
}

void account::modify()
{
	cout<<"Account Number:"<<acno<<endl;
	cout<<"Modify Account Holder Name:"<<endl;
	cin.ignore();
	cin.getline(name,100);
	cout<<"Modify Type of Account: "<<endl;
	cin>>type;
	type=toupper(type);
	cout<<"Modify Balance Amount:"<<endl;
	cin>>deposit;
}

void account::dep(int x)
{
	deposit+=x;
}

void account::draw(int x)
{
	deposit-=x;
}

void account::report() const
{
	cout<<acno<<setw(10)<<" "<<name<<setw(10)<<" "<<type<<setw(6)<<deposit<<endl;
}

int account::retacno() const
{
	return acno;
}

int account::retdeposit() const
{
	return deposit;
}

char account::rettype() const
{
	return type;
}

void write_account();
void display_sp(int);
void modify_account(int);
void delete_account(int);
void display_all();
void deposit_withdraw(int,int);
void intro();

void write_account()
{
	account ac;
	ofstream outfile;
	outfile.open("account.dat",ios::binary|ios::app);
	ac.create_account();
	outfile.write(reinterpret_cast<char *> (&ac),sizeof(account));
	outfile.close();
}

void display_sp(int n)
{
	account ac;
	bool flag=false;
	ifstream infile;
	infile.open("account.dat",ios::binary);
	if(!infile)
	{
		cout<<"File could not be open!! Press any key..."<<endl;
		return;
		
	}
	
	cout<<"BALANCE DETAILS:"<<endl;
	while(infile.read(reinterpret_cast<char *> (&ac),sizeof(account)))
	{
		if(ac.retacno()==n)
		{
			ac.show_account();
			flag=true;
		}
	}
	infile.close();
	if(flag==false)
	{
		cout<<"Acoount number does not exist:"<<endl;
	}
}
	
	void modify_account(int n)
	{
		bool found=false;
		account ac;
		fstream file;
		file.open("account.dat",ios::binary|ios::in|ios::out);
		if(!file)
		{
			cout<<"File could not be open!! Press any key...."<<endl;
			return;
		}
		while(!file.eof() && found==false)
		{
			file.read(reinterpret_cast<char *> (&ac),sizeof(account));
			if(ac.retacno()==n)
			{
				ac.show_account();
				cout<<"Enter the New Details of account:"<<endl;
				ac.modify();
				int pos=(-1)*static_cast<int>(sizeof(account));
				file.seekp(pos,ios::cur);
				file.write(reinterpret_cast<char *> (&ac),sizeof(account));
				cout<<"Record Updated!!"<<endl;
				found=true;
				
			}
		}
		file.close();
		if(found==false)
		{
			cout<<"Record Not Found!!"<<endl;
		}
	}
	
	void delete_account(int n)
	{
		account ac;
		ifstream infile;
		ofstream outfile;
		infile.open("account.dat",ios::binary);
		if(!infile)
		{
			cout<<"File could not be open!! Press any key....."<<endl;
			return;
		}
		outfile.open("Temp.dat",ios::binary);
		infile.seekg(0,ios::beg);
		while(infile.read(reinterpret_cast<char *> (&ac),sizeof(account)));
		{
			if(ac.retacno()==n)
			{
				outfile.write(reinterpret_cast<char *> (&ac),sizeof(account));
			}
		}
		infile.close();
		outfile.close();
		remove("account.dat");
		rename("account.dat","Temp.dat");
		cout<<"Record Deleted!!"<<endl;
	}
	
	void display_all()
	{
		account ac;
		ifstream infile;
		infile.open("account.dat",ios::binary);
		if(!infile)
		{
			cout<<"File could not be open!! Press any key....."<<endl;
			return;
	    }
			cout<<"Account Holder List:"<<endl;
			cout<<"=========================================================================="<<endl;
			cout<<"A/c No.     Name      type     balance  "<<endl;
			cout<<"=========================================================================="<<endl;
			while(infile.read(reinterpret_cast<char *> (&ac),sizeof(account)))
			{
				ac.report();
			}
			infile.close();
	}
	
	void deposit_withdraw(int n, int option)
	{
		int amt;
		bool found=false;
		account ac;
		fstream file;
		file.open("account.dat",ios::binary|ios::in|ios::out);
		if(!file)
		{
			cout<<"File could not be open!! Press any key....."<<endl;
			return;
		}
		while(!file.eof() && found==false)
		{
			file.read(reinterpret_cast<char *> (&ac),sizeof(account));
			if(ac.retacno()==n)
			{
				ac.show_account();
				if(option==1)
				{
					cout<<"TO DEPOSIT AMOUNT"<<endl;
					cout<<"Enter the amount to be deposited:"<<endl;
					cin>>amt;
					ac.dep(amt);
				}
				if(option==2)
				{
					cout<<"TO WITHDRAW AMOUNT"<<endl;
					cout<<"Enter the amount to be Withdraw:"<<endl;
					cin>>amt;
					int bal=ac.retdeposit()-amt;
					if((bal<500 && ac.rettype()=='S') || (bal<1000 && ac.rettype()=='C'))
						cout<<"Insufficient Balance!!"<<endl;
					else
						ac.draw(amt);
				}
				int pos=(-1)*static_cast<int>(sizeof(account));
				file.seekp(pos,ios::cur);
				file.write(reinterpret_cast<char *> (&ac),sizeof(account));
				cout<<"Record Updated!!"<<endl;
				found=true;
			}
		}
		file.close();
		if(found==false)
		{
			cout<<"Record Not Found!!"<<endl;
		}
	}
	
	void intro()
	{
		cout<<"\n\n\n\t BANK";
		cout<<"\n\n\t MANAGEMENT";
		cout<<"\n\n\t SYSTEM";
		cout<<"\n\n\n\n MADE BY : GUPTA SANTOSH MAHENDRA";
		cout<<"\n\n COLLEGE : K.P.B.HINDUJA COLLEGE";
		cin.get();
	}



int main()
{
	account ac;
	char ch;
	int num;
	intro();
	cout<<endl;
	do{
	       cout<<"MAIN MENU:"<<endl;
	       cout<<"01-> NEW ACCOUNT:"<<endl;
		   cout<<"02-> DEPOSIT ACCOUNT"<<endl;
		   cout<<"03-> WITHDRAW ACCOUNT:"<<endl;
	       cout<<"04-> BALANCE ENQUIRY:"<<endl;
		   cout<<"05-> ALL ACCOUNT LIST:"<<endl;
		   cout<<"06-> CLOSE AN ACCOUNT:"<<endl;
		   cout<<"07-> MODIFY AN ACCOUNT:"<<endl;
	       cout<<"08-> EXIT:"<<endl;
	       cout<<"Select your option "<<endl;
	       cin>>ch;
	       switch(ch)
	       {
	       	case '1':
	       		    write_account();
	       		    break;
	       		    
	       	case '2':
			        cout<<"Enter the account Number:"<<endl;
					cin>>num;
					deposit_withdraw(num,1);
					break;
			
			case '3':
				    cout<<"Enter the account Number:"<<endl;
					cin>>num;
					deposit_withdraw(num,2);
					break;
					
			case '4':
			       	cout<<"Enter the account Number:"<<endl;
					cin>>num;
					display_sp(num);
					break;
					
			case '5':
			        display_all();
					break;
					
			case '6':
			        cout<<"Enter the account Number:"<<endl;
					cin>>num;
					delete_account(num);
					break;
					
			case '7':
			        cout<<"Enter the account Number:"<<endl;
					cin>>num;
				    modify_account(num);
					break;
					
			case '8':
				    cout<<"Thanks for Using Bank Management System.."<<endl;
				    break;
				    
			default:
     		       cout<<"\a"<<endl;
		}
		cin.ignore();
		cin.get();
	}while(ch!=8);
	return 0;
	}
