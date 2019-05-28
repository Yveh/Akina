#include <iostream>
#include "Command.hpp"
#include "UserMananger.hpp"
int main()
{
	bool Exit=true;
	UserManager MainUser;
//	std::fstream In,Out;
//	In.open("6.in",std::ios::in);
//	Out.open("x.out",std::ios::out|std::ios::app);
	while (Exit)
	{
		Main_Command(std::cin,std::cout,MainUser,Exit);
		//Main_Command(In,Out,MainUser,Exit);
	}
//	In.close();
//	Out.close();
	return 0;
}