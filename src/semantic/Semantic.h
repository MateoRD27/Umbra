#pragma once

#include <memory>
#include "../ast/Nodes.h"
#include <string>
#include <unordered_set>
#include <unordered_map>

namespace umbra{

enum class RvalExpressionType {
    INTEGER,
    FLOAT,
    STRING,
    BOOLEAN,
    CHAR,
    VAR_NAME
};

class Symbol;
class SymbolTable;

typedef std::unordered_map<const std::string*, std::unique_ptr<Symbol>> _SymbolMap;

class StringInterner {
    public:
        StringInterner();
        const std::string& intern(const std::string& str);
        const std::string* get(const std::string& str) const;

    private:
        std::unordered_set<std::string> internedStrings;
};

class Symbol {
public:
    enum class SymbolKind {
        VARIABLE,
        FUNCTION,
        TYPE
    };

    Symbol(std::string name, SymbolKind kind, std::unique_ptr<Type> type, StringInterner &interner);
    virtual ~Symbol() = default;

    const std::string& name;
    SymbolKind kind;
    std::unique_ptr<Type> type;

};

class ScopeManager {
public:

    ScopeManager();
    void enterScope(_SymbolMap sym);
    void exitScope(SymbolTable &symbolTable);
    _SymbolMap& currentScope() {
        return scopes.back();
    }

private:
    std::vector<_SymbolMap> scopes;
};

class SymbolTable {
public:
    SymbolTable(StringInterner &interner);
    bool addSymbol(std::unique_ptr<Symbol> symbol);
    Symbol* getSymbol(const std::string& name) const;

    // Agrega removeSymbol para eliminar un símbolo por su key
    bool removeSymbol(const std::string* key);

    void printAllSymbols() const {
        std::cout << "All symbols in the symbol table:" << std::endl;
        for (const auto& pair : symbols) {
            std::cout << "Symbol: " << *pair.first << ", Type: " << pair.second->type->baseType << std::endl;
        }
    }

private:
    StringInterner &stringInterner;
    _SymbolMap symbols;
};

class TypeCompatibility {
    public:
        // Verificar si el tipo de expresión es compatible con el tipo declarado
        static bool areTypesCompatible(const Type& declType, RvalExpressionType exprType) {
                // Verificación directa de tipos
            if (declType.baseType == "int" && exprType == RvalExpressionType::INTEGER) return true;
            if (declType.baseType == "float" && exprType == RvalExpressionType::FLOAT) return true;
            if (declType.baseType == "string" && exprType == RvalExpressionType::STRING) return true;
            if (declType.baseType == "bool" && exprType == RvalExpressionType::BOOLEAN) return true;
            if (declType.baseType == "char" && exprType == RvalExpressionType::CHAR) return true;
                
            // Conversiones implícitas permitidas
            if (declType.baseType == "float" && exprType == RvalExpressionType::INTEGER) return true;
            
            return false;
    }
};



}