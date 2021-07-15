#include "Tree.h"
#include "Format.h"
#include "Consistency.h"


int main()
{
	Tree xml("D:/3rd CSE/Out_XML.txt");  //create Tree Object

	xml.insert("D:/3rd CSE/Json_Minify.txt"); //Insert file.xml into xml object

	xml.print_minify(xml.get_root()); //Minifying file.xml

	format("D:/3rd CSE/informat.txt", "D:/3rd CSE/Out_XML.txt");  //Prettifying file.xml

	//check <file.xml> consistency 
	consistency("D:/3rd CSE/inconsistent.txt", "D:/3rd CSE/Out_Consistent.txt", "D:/3rd CSE/Out_Inconsistent.txt");

	xml.print_Json(xml.get_root()); //converting <file.xml> to <file.json>

	return 0;
}
