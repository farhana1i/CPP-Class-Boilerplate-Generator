#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <string>

#define SPACE ' '
#define POINTER '*'

/**
 * @brief to capitalize the string 'str'
 *
 * @param str string that needs to be capitalized
 * @returns the capitalized version of string 'str'
 */
std::string toCapitalize(const std::string &str)
{
    std::string retStr;

    // if empty returns the same string
    if (str.empty())
        return retStr;

    // change first letter case to uppper case
    retStr.push_back(std::toupper(str[0]));

    // for each letter l
    // if there is a space ' ' exactly one index before l, replace l with L i.e change to upper case
    // otherwise, copy l in retStr
    for (std::size_t i = 1; i < str.length(); ++i)
    {
        if (str[i - 1] == SPACE)
        {
            retStr.push_back(std::toupper(str[i]));
        }
        else
        {
            retStr.push_back(str[i]);
        }
    }
    return retStr;
}

/**
 * @brief this function converts the camel case string 'str'
 *          to lower case
 *
 * @param str string whose case needs to be converted
 * @return lower case version of camel case string 'str'
 */
std::string camelTolowerCase(const std::string &str)
{
    std::string retStr;

    // if empty returns the same string
    if (str.empty())
        return retStr;

    // change first letter case to lower case
    retStr.push_back(std::tolower(str[0]));

    // for each letter l
    // if l is in Capital Case i.e L, replace L with a space ' ' and lower case of L i.e l
    // otherwise, copy l in retStr
    for (std::size_t i = 1; i < str.length(); ++i)
    {
        if (std::isupper(str[i]))
        {
            retStr.push_back(SPACE);
            retStr.push_back(std::tolower(str[i]));
        }
        else
        {
            retStr.push_back(str[i]);
        }
    }
    return retStr;
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
bool readAttributes(std::string &className, std::vector<std::pair<std::string, std::string>> &attributes, const std::string &inputFile)
{
    std::ifstream fin(inputFile);

    if (fin.fail())
    {
        perror(inputFile.c_str());
        return false;
    }

    // read class name
    fin >> className;

    // ignore characters till new line charatcer i.e '\n'
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
 * this function returns parameter based on the data type i.e an object or a pointer
 * 
 * @param dataType data type of the parameter
 * @param name name of the parameter
 * @return parameter 
 */
std::string getParameter(const std::string &dataType, const std::string name)
{
    return (dataType.find_last_of(POINTER) == std::string::npos)
               ? "const " + dataType + "& " + name
               : dataType + " " + name;
}

/**
 * @brief generate source file i.e .cpp file, for class className
 *
 * @param className name of the class for which source file needs to be generated
 * @param attributes list of attributes of class 'className'
 */
void generateSourceFile(std::string className, const std::vector<std::pair<std::string, std::string>> &attributes)
{

    std::ofstream fout("../Output/" + className + ".cpp");

    if (fout.fail())
    {
        perror(NULL);
    }
    else
    {
        // include header file
        fout << "#include \"./" + className + ".h\"\n\n";

        for (auto attribute : attributes)
        {
            std::string dataType = attribute.first;
            std::string name = attribute.second;

            // parameter for setter
            std::string parameter = getParameter(dataType, name);

            // setter implementation with comment
            std::string setter =
                "// setter to set " + camelTolowerCase(name) +
                "\nvoid " + className + "::set" + toCapitalize(name) + "("+ parameter +") {\n\tthis->" + name + " = " + name + ";\n}";

            // getter implementation with cm
            std::string getter =
                "// getter to get " + camelTolowerCase(name) + "\n" +
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
void generateHeaderFile(std::string className, const std::vector<std::pair<std::string, std::string>> &attributes)
{
    std::ofstream fout("../Output/" + className + ".h");

    if (fout.fail())
    {
        perror(NULL);
    }
    else
    {
        // parameterized constructor prototype
        fout << "class " + className + " {\npublic:\n";

        // setters and getters prototype
        for (auto attribute : attributes)
        {
            std::string dataType = attribute.first;
            std::string name = attribute.second;

            // parameter for setter
            std::string parameter = getParameter(dataType, name);


            // setter prototype with documentation
            std::string setter =
                "\t/**\n"
                "\t * to set " +
                camelTolowerCase(name) + "\n" +
                "\t *\n" +
                "\t * @param " + name + " the " + camelTolowerCase(name) + " to set\n" +
                "\t*/\n";

            setter += "\tvoid set" + toCapitalize(name) + "(" + parameter + ");";

            // getter prototype with documentation
            std::string getter =
                "\t/**\n"
                "\t * to get " +
                camelTolowerCase(name) + "\n" +
                "\t *\n" +
                "\t * @returns the " + camelTolowerCase(name) + " to get\n" +
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
bool generateClassBoilerplate(const std::string &inputFile)
{
    // to save class name
    std::string className;

    // to save pair of data type & name of list of attributes
    std::vector<std::pair<std::string, std::string>> attributes;

    // set className & attributes
    if (readAttributes(className, attributes, inputFile))
    {

        // create 'Headers' directory
        const char directoryPath[] = "../Output/";
        std::filesystem::create_directory(directoryPath);

        generateHeaderFile(className, attributes);
        generateSourceFile(className, attributes);

        return true;
    }

    return false;
}

/**
 * @brief this function generates both header and source files for a list of classes.
 *
 * @param inputFiles contains list of names of files.
 *         Each file contains class name & list of attributes of the class
 * @returns true if all files are generated successfully
 * @returns false if any file did not generate successfully
 */
bool generateClassBoilerplate(std::vector<std::string> inputFiles)
{

    for (auto filename : inputFiles)
    {
        if (!generateClassBoilerplate(filename))
        {
            return false;
        }
    }

    return true;
}

int main()
{
    // generate .h & .cpp files for a single class
    std::string inputFile = "../examples/inputFormat";
    std::cout << (generateClassBoilerplate(inputFile) ? "Files Generated Successfully!" : "Failed To Generate Files") << std::endl;

    // generate .h & .cpp files for multiple classes
    std::vector<std::string> inputFiles = {
        "../examples/exampleInput1",
        "../examples/exampleInput2",
        "../examples/exampleInput3"};
    std::cout << (generateClassBoilerplate(inputFiles) ? "Files Generated Successfully!" : "Failed To Generate Files") << std::endl;

    return 0;
}
