#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void defineAst(std::ofstream& file, std::string baseName, std::vector<std::string> types);
void defineType(std::ofstream& file, std::string baseName, std::string className,
                std::string fieldList);

void trim(std::string& s);
std::string toLowercase(const std::string& s);

int main(int argc, char const* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <output-directory>\n" << std::endl;
        return 1;
    }

    std::string command = argv[0];
    if (command != "./generateAst") {
        std::cerr << "Usage: ./generateAst <output-directory>\n" << std::endl;
        return 1;
    }

    std::filesystem::path outputFile = std::filesystem::current_path() / argv[1] / "expr.cpp";

    std::ofstream file(outputFile);
    if (!file) {
        std::cerr << "Could not open file: " << outputFile << "\n";
        return 1;
    }

    defineAst(file, "Expr",
              std::vector<std::string>{"Binary:Expr left, TokenType operation, Expr right",
                                       "Grouping:Expr expression", "Literal:std::string value",
                                       "Unary:TokenType operation, Expr right"});

    return 0;
}

void defineAst(std::ofstream& file, std::string baseName, std::vector<std::string> types) {
    // Headers
    file << "#include <string>" << std::endl;
    file << "#include <memory>" << std::endl;
    file << std::endl;
    file << "#include \"../src/token.h\"" << std::endl;
    file << std::endl;

    // Forward declarations
    for (const std::string& type : types) {
        std::string className = type.substr(0, type.find(':'));
        file << "class " << className << ";" << std::endl;
    }
    file << std::endl;

    // ExprVisitor definition
    file << "class " << baseName << "Visitor {" << std::endl;
    file << "public:" << std::endl;
    for (const std::string& type : types) {
        std::string className = type.substr(0, type.find(':'));
        file << "    virtual void visit" << className << "(const " << className << "& "
             << toLowercase(className) << ") = 0;" << std::endl;
    }
    file << "    virtual ~" << baseName << "Visitor() = default;" << std::endl;
    file << "};" << std::endl;
    file << std::endl;

    // Base class definition
    file << "class " << baseName << " {" << std::endl;
    file << "public:" << std::endl;
    file << "    virtual void accept(" << baseName << "Visitor& visitor) const = 0;" << std::endl;
    file << "    virtual ~" << baseName << "() = default;" << std::endl;
    file << "};" << std::endl;
    file << std::endl;

    for (const std::string& type : types) {
        std::string className = type.substr(0, type.find(':'));
        std::string fields = type.substr(type.find(':') + 1, type.size());

        trim(className);
        trim(fields);

        defineType(file, baseName, className, fields);
    }
}

void defineType(std::ofstream& file, std::string baseName, std::string className,
                std::string fieldList) {
    // Extracting the variables
    std::vector<std::string> fields;
    size_t index = fieldList.find(", ");
    while (index != std::string::npos) {
        fields.push_back(fieldList.substr(0, index));
        fieldList.erase(0, index + 2);
        index = fieldList.find(", ");
    }
    fields.push_back(fieldList);

    std::vector<std::string> variableTypes;
    std::vector<std::string> variableNames;
    for (const std::string& f : fields) {
        std::string type = f.substr(0, f.find(' '));
        std::string name = f.substr(f.find(' ') + 1, f.size());
        variableTypes.push_back(type);
        variableNames.push_back(name);
    }

    // Class definition
    file << "class " << className << ": public " << baseName << " {" << std::endl;
    file << "public:" << std::endl;

    // Members
    for (size_t i = 0; i < variableTypes.size(); i++) {
        std::string type = variableTypes[i];
        std::string name = variableNames[i];
        if (type == baseName) {
            file << "    std::unique_ptr<" << type << "> " << name << ";" << std::endl;
        } else {
            file << "    " << type << " " << name << ";" << std::endl;
        }
    }
    file << std::endl;

    // Constructor
    file << "    " << className << "(";
    for (size_t i = 0; i < variableTypes.size(); i++) {
        std::string type = variableTypes[i];
        std::string name = variableNames[i];
        if (type == baseName) {
            file << "std::unique_ptr<" << type << "> " << name;
        } else {
            file << "" << type << " " << name;
        }

        if (i < variableTypes.size() - 1) file << ", ";
    }
    file << ") : ";
    for (size_t i = 0; i < variableTypes.size(); i++) {
        std::string type = variableTypes[i];
        std::string name = variableNames[i];
        if (type == baseName) {
            file << name << "(std::move(" << name << "))";
        } else {
            file << name << "(" << name << ")";
        }

        if (i < variableTypes.size() - 1) file << ", ";
    }
    file << "{}" << std::endl;

    // accept override
    file << "    void accept(" << baseName << "Visitor& visitor) const override {" << std::endl;
    file << "        visitor.visit" << className << "(*this);" << std::endl;
    file << "    }" << std::endl;

    file << "};" << std::endl;
    file << std::endl;
}

void trim(std::string& s) {
    // left trim
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isspace(c); }));
    // right trim
    s.erase(
        std::find_if(s.rbegin(), s.rend(), [](unsigned char c) { return !std::isspace(c); }).base(),
        s.end());
}

std::string toLowercase(const std::string& s) {
    std::string copy;
    copy.reserve(s.size());

    std::transform(s.begin(), s.end(), std::back_inserter(copy),
                   [](unsigned char c) { return std::tolower(c); });

    return copy;
}