#include <iostream>
#include <vector>
#include <fstream>

#define SPACE ' '

/**
 * @brief to capitalize the string 'str'
 * 
 * @param str string that needs to be capitalized
 * @returns the capitalized version of string 'str'
 */
std::string toCapitalize(std::string str)
{
    for (std::size_t i = 0; i < str.length(); ++i)
    {
        if (i == 0 || (i > 0 && str[i - 1] == SPACE))
        {
            str[i] = std::toupper(str[i]);
        }
    }
    return str;
}

/**
 * @brief read class name and attributes of a class from the file  'inputFile'
 * 
 * @param className name of the class 
 * @param attributes list of attributes of class 'className'
 * @param inputFile input file that contains class & attributes information
 * @return true if input file exists
 * @return false if input file does not exist
*/
bool readAttributes(std::string &className, std::vector<std::pair<std::string, std::string> > &attributes, const char *inputFile)
{
    std::ifstream fin(inputFile);

    if (fin.fail())
    {
        perror(inputFile);
        return false;
    }

    // read class name
    fin >> className;

    fin.ignore(100000, '\n');

    // read data type and name of each attribute
    for (std::string line; getline(fin, line);)
    {
        if (line.find_first_not_of(' ') != std::string::npos)
        {
            std::size_t index = line.find_last_of(SPACE);

            // extract data type and name of attribute
            std::string dataType = line.substr(0, index);
            std::string name = line.substr(index + 1);

            attributes.push_back(std::make_pair(dataType, name));
        }
    }

    fin.close();
    return true;
}

/**
 * @brief generate source file i.e .cpp file, for class className
 * 
 * @param className name of the class for which source file needs to be generated
 * @param attributes list of attributes of class 'className'
 */
void generateSourceFile(std::string className, const std::vector<std::pair<std::string, std::string> > &attributes)
{

    std::ofstream fout(className + ".cpp");

    if (fout.fail())
    {
        perror(NULL);
    }
    else
    {
        // include header file
        fout << "#include \"./" + className + ".h\"\n\n";

        for (std::size_t i = 0; i < attributes.size(); ++i)
        {

            std::pair<std::string, std::string> attribute = attributes.at(i);
            std::string dataType = attribute.first;
            std::string name = attribute.second;

            // setter implementation with comment
            std::string setter =
                "// setter to set " + name +
                "\nvoid " + className + "::set" + toCapitalize(name) + "(const " + dataType + "& " + name + ") {\n\tthis->" + name + " = " + name + ";\n}";

            // getter implementation with cm
            std::string getter =
                "// getter to get " + name + "\n" +
                dataType + " " + className + "::get" + toCapitalize(name) + "() const {\n\treturn this->" + name + ";\n}";

            fout << setter << "\n\n";
            fout << getter << "\n\n\n";
        }
        fout.close();
    }
}

/**
 * @brief generate header file i.e .h file, for class className
 * 
 * @param className name of the class for which header file needs to be generated
 * @param attributes list of attributes of class 'className'
*/
void generateHeaderFile(std::string className, const std::vector<std::pair<std::string, std::string> > &attributes)
{
    std::ofstream fout(className + ".h");

    if (fout.fail())
    {
        perror(NULL);
    }
    else
    {
        // parameterized constructor prototype
        fout << "class " + className + " {\npublic:\n";

        // setters and getters prototype
        for (std::size_t i = 0; i < attributes.size(); ++i)
        {

            std::pair<std::string, std::string> attribute = attributes.at(i);
            std::string dataType = attribute.first;
            std::string name = attribute.second;

            // setter prototype with documentation
            std::string setter =
                "\t/**\n"
                "\t * to set " +
                name + "\n" +
                "\t *\n" +
                "\t * @param " + name + " the " + name + " to set\n" +
                "\t*/\n";

            setter += "\tvoid set" + toCapitalize(name) + "(const " + dataType + "& " + name + ");";

            // getter prototype with documentation
            std::string getter =
                "\t/**\n"
                "\t * to get " +
                name + "\n" +
                "\t *\n" +
                "\t * @returns the " + name + " to get\n" +
                "\t*/\n";

            getter += "\t" + dataType + " get" + toCapitalize(name) + "() const;";

            fout << setter << "\n\n";
            fout << getter << "\n\n\n";
        }

        // write attributes

        fout << "private:\n";
        for (std::size_t i = 0; i < attributes.size(); ++i)
        {

            std::pair<std::string, std::string> attribute = attributes.at(i);
            std::string dataType = attribute.first;
            std::string name = attribute.second;

            fout << "\t" << dataType << " " << name << ";\n";
        }

        fout << "};\n";

        fout.close();
    }
}

/**
 * @brief this function generates both header and source file with class name & attributes in file 'inputFile'
 * 
 * @param inputFile contains class name & list of attributes of the class
 * @returns true if files are generated successfully
 * @returns false if files are not generated
 */
bool generateClassBoilerplate(const char *inputFile = "inputFile")
{
    // to save class name
    std::string className;

    // to save pair of data type & name of list of attributes
    std::vector<std::pair<std::string, std::string> > attributes;

    // set className & attributes
    if (readAttributes(className, attributes, inputFile))
    {
        generateHeaderFile(className, attributes);
        generateSourceFile(className, attributes);

        return true;
    }

    return false;
}

int main()
{
    // generate .h & .cpp files
    std::cout << generateClassBoilerplate("exampleInput");

    return 0;
}
