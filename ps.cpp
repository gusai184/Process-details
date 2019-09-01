// only file name of digits are processes in proc

#include<unistd.h>
#include<string.h>
#include<fstream>
#include<dirent.h>
#include<iostream>
using namespace std;

bool isValidProcess(string s)
{
	for(int i=0;i<s.length();i++)
	{
		if(isdigit(s[i])==0)
			return false;
	}
	return true;
}

int main()
{
	DIR * dir_ptr,*fb_dir_ptr;
	struct dirent * dirent_ptr,*fb_dirent_ptr;
	string line;
	
	dir_ptr = opendir("/proc");
	if(dir_ptr==NULL)
	{
		cout<<"Error 0: Could not open proc file"<<endl;
		return 0;
	}
	
	dirent_ptr = readdir(dir_ptr);
	while(dirent_ptr != NULL)
	{
		string process_id = dirent_ptr->d_name;

		if(isValidProcess(process_id)==false)
		{
			dirent_ptr = readdir(dir_ptr);
			continue;	
		}


		//process found-----------------------------
		if(dirent_ptr->d_type == DT_DIR)
		{	cout<<"Process id : "<<process_id<<endl;
			
			//Open Status file --------------------------------------
			ifstream file ("/proc/"+process_id+"/status");
			if(file.is_open())
			{
				int line_no = 0;
				while(getline(file,line))
				{
					line_no++;
					if(line_no==3 || line_no==7 || line_no==1)
					cout<<line<<endl;
					if(line_no == 7)
						break;					
				}
				
			}
			else
			{
				cout<<"Error 1: Could not open status file"<<endl;
			}
			file.close();
			//----------------------------------------------------------

			//Open enviorn file-----------------------------------------
			cout<<"Enviornment Variable : "<<endl;
			file.open("/proc/"+process_id+"/environ");
			if(file.is_open())
			{
				while(getline(file,line,'\0'))
				{
					if(line!="")
						cout<<"\t"<<line<<endl;	
				}
				//cout<<endl;
			}
			else
			{
				cout<<"Error 1: Could not open enviorn file"<<endl;
			}
			file.close();
			
			//-----------------------------------------------------------


			//Open sessionid file----------------------------------------
			file.open("/proc/"+process_id+"/sessionid");
			if(file.is_open())
			{
				while(getline(file,line))
				{
					cout<<"Session id : "<<line<<endl;
				}
			}
			else
			{
				cout<<"Error : Could not open enviorn file"<<endl;
			}
			file.close();
			//--------------------------------------------------------------
			
			//exe file--------------------------------------------------------

		/*	char buffer[100];
			string path = "/proc/"+process_id+"/exe";
			readlink(path.c_str(),buffer,100);
			cout<<"Process Root is "<<buffer<<endl;
			//----------------------------------------------------------------
*/
			//Process root folder --------------------------------------------
			char buffer1[400];
			string path1 = "/proc/"+process_id+"/root";
			readlink(path1.c_str(),buffer1,400);
			cout<<"Process Root folder is "<<buffer1<<endl;
			//----------------------------------------------------------------
			
		

			//Open fd directory ------------------------------------------------
			cout<<"File Discriptors : "<<endl;
			string fb_path = "/proc/"+process_id+"/fd";
			fb_dir_ptr = opendir(fb_path.c_str());

			if(fb_dir_ptr!=NULL)
			{
				fb_dirent_ptr = readdir(fb_dir_ptr);
				while(fb_dirent_ptr != NULL/* && fb_dirent_ptr->d_type == DT_LNK*/)
				{
					char buffer[256];
					//string buffer;
					cout<<'\t'<<fb_dirent_ptr->d_name<<" ";
					string path = "/proc/"+process_id+"/fd/"+fb_dirent_ptr->d_name;
					int x = readlink(path.c_str(),buffer,256);
					buffer[x] = '\0';
 					cout<<buffer<<endl;
					fb_dirent_ptr = readdir(fb_dir_ptr);			
				}
				
			}
			else
			{
				cout<<"Error : Could not open fb directory"<<endl;			
			}
			closedir(fb_dir_ptr);
			cout<<endl;
			cout<<"----------------------------------------------------------------------------------------------------"<<endl;
			cout<<endl;	

			
		}

		
		
		dirent_ptr = readdir(dir_ptr);
	}

	//CPU file--------------------------------------------------------	
	cout<<"_________________________________________"<<endl;
	ifstream file;
	file.open("/proc/stat");
	if(file.is_open())
	{
		while(getline(file,line))
		{
			//cout<<(line.substr(0,13))<<endl;
			if(line.substr(0,13)=="procs_running" || line.substr(0,13)=="procs_blocked")
				cout<<line<<endl;
			if( line.substr(0,4)=="ctxt")
				cout<<"Total no of context switches : "<<line.substr(4)<<endl;

		}	
		cout<<endl;
	}
	else
	{
		cout<<"Error 1: Could not open enviorn file"<<endl;
	}
	file.close();
	//------------------------------------------------------------------
	
	closedir(dir_ptr);

	return 1;
}
